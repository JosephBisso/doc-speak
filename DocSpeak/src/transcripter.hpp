using namespace docspeak;

template <typename T>
StatusInfo Transcripter::transcript(bool output_to_wav,
                            T* streamer,
                            std::function<void(T*, std::string)>  streaming_fn)
{
    m_transcript.clear();

  if (!__check_if_ready()) {
        PLOGW << "Transcripter not ready. Aborting";
        return StatusInfo(false, "Transcripter not ready. Aborting");
  }

  // DeepSpeech Init
  ModelState* model;
  int status = DS_CreateModel(s_model_path.string().c_str(), &model);
  if (status != 0)
  {
    char* error = DS_ErrorCodeToErrorMessage(status);
    auto msg = std::format("Could not create model: {}", error); 
    PLOGW << msg;
    free(error);
    return StatusInfo(false, msg);
  }

  status = DS_EnableExternalScorer(model, s_scorer_path.string().c_str());
  if (status != 0)
  {
    char* error = DS_ErrorCodeToErrorMessage(status);
    auto msg = std::format("Could not create model: {}", error);
    PLOGW << msg;
    free(error);
    return StatusInfo(false, msg);
  }

  StreamingState* ds_stream;
  status = DS_CreateStream(model, &ds_stream);
  if (status != 0)
  {
    char* error = DS_ErrorCodeToErrorMessage(status);
    auto msg = std::format("Could not create model: {}", error);
    PLOGW << msg;

    free(error);
    return StatusInfo(false, msg);
  }

  // VAD Init
  Fvad* vad = fvad_new();
  if (vad == NULL)
  {
    PLOGW << "WebRtcVad_Create error";
    return StatusInfo(false, "WebRtcVad_Create error");
  }

  status = fvad_set_mode(vad, 3);
  if (status != 0)
  {
    PLOGW << "set_mode error";
    return StatusInfo(false, "set_mode error");
  }

  status = fvad_set_sample_rate(vad, SAMPLE_RATE);
  if (status != 0)
  {
    PLOGW << "set_sample_rate error";
    return StatusInfo(false, "set_sample_rate error");
  }


  // PortAudio Init
  PaStreamParameters  inputParameters;
  PaStream*           pa_stream;
  PaError             err;
  std::vector<SAMPLE> buffer;
  int                 totalFrames;
  int                 numSamples;
  int                 numBytes;

  err = Pa_Initialize();
  if (err != paNoError) {
    auto msg = std::format("PortAudio error: {}", Pa_GetErrorText(err));
    PLOGW << msg;
    return StatusInfo(false, msg);
  }

  inputParameters.device = Pa_GetDefaultInputDevice();
  if (inputParameters.device == paNoDevice) {
    PLOGW << "PortAudio error: No default input device";
    return StatusInfo(false, "PortAudio error: No default input device");
  }
  inputParameters.channelCount = NUM_CHANNELS;
  inputParameters.sampleFormat = PA_SAMPLE_TYPE;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  buffer.reserve(MAX_STREAM_DURATION * SAMPLE_RATE);

  PLOGV << "--- Transcript Start ---";
  std::cerr << "--- Transcript Start ---\n";

  err = Pa_OpenStream(
            &pa_stream,
            &inputParameters,
            NULL,
            SAMPLE_RATE,
            SAMPLE_PER_FRAME,
            paClipOff,
            __streamCallback,
            &buffer);
  if (err != paNoError) {
    auto msg = std::format ("PortAudio error: {}", Pa_GetErrorText(err));
    PLOGW << msg;
    return StatusInfo(false, msg);
  }

  err = Pa_StartStream(pa_stream);
  if (err != paNoError) {
    auto msg = std::format ("PortAudio error: {}", Pa_GetErrorText(err));
    PLOGW << msg;
    return StatusInfo(false, msg);
  }

    // Loop througth frames in buffer
  totalFrames = STREAM_DURATION * SAMPLE_RATE;

  bool on_going_speech = false;
  bool on_going_silence = false;
  int actual_buffer_index = 0;
  std::deque<std::pair<std::vector<SAMPLE>, bool>> speech_deque;
  std::vector<SAMPLE> speech_frames;
  std::vector<SAMPLE> wave;
  bool triggered = false;

  auto stream_start = std::chrono::steady_clock::now();
  auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  std::chrono::milliseconds actual_silence_duration (0);

  char time[std::size("dd_mm__HH_MM_SS")];

  while ((err = Pa_IsStreamActive(pa_stream)) == 1 && 
        std::chrono::steady_clock::now() - stream_start < C_MAX_STREAM_DURATION) 
  {
      // Check if actual frame contains speech
      int speech = -1;

      if (buffer.size() > (actual_buffer_index + 1) * SAMPLE_PER_FRAME) {
        speech = __get_speech_from_frame(vad, 
                buffer.data(), 
                speech_frames,
                speech_deque, 
                actual_buffer_index++ * SAMPLE_PER_FRAME,
                SAMPLE_PER_FRAME,
                triggered);
      }

      if (speech == 0) {
        actual_silence_duration += std::chrono::milliseconds(1000 * SAMPLE_PER_FRAME / SAMPLE_RATE); //Frame duration
        if (actual_silence_duration >= m_max_silence_duration) {
          PLOGI << "Stopping Streaming because max silence duration was reached";
          std::cerr << "Stopping Streaming because max silence duration was reached\n";
          break;
        }

      } else if (speech == 1) { // If there is recognized speech in frame, feed it to the stream context
        actual_silence_duration = std::chrono::milliseconds(0);

        DS_FeedAudioContent(ds_stream, speech_frames.data(), speech_frames.size());

        if (output_to_wav)
          std::copy(speech_frames.begin(), speech_frames.end(), std::back_inserter(wave));

      } else if (speech == 2) {
        actual_silence_duration = std::chrono::milliseconds(0);

        PLOGI << std::format("transcripting ...");
        std::cerr << "transcripting ...\n";

        now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        char* result = DS_FinishStream(ds_stream);
        std::string res (result);
        std::strftime(std::data(time), std::size(time), "%d_%m__%H_%M_%S", std::gmtime(&now));

        PLOGV << std::format("{} :: Heard: '{}'", time, result);
        std::cerr << std::format("{} :: Heard: '{}'\n", time, result);

        m_transcript.push_back(result);
        if ( (bool)m_observer)
          m_observer(result);
        if (streamer && streaming_fn)
          streaming_fn(streamer, result);

        DS_FreeString(result);
        
        // create new stream context for the next transcription
        status = DS_CreateStream(model, &ds_stream);
        if (status != 0) {
          auto msg = std::format("DS_CreateStream error: ", DS_ErrorCodeToErrorMessage(status));
          PLOGW << msg;
          return StatusInfo(false, msg);
        }

        if (output_to_wav &&  wave.size() > 0) {
          auto file_name = std::format("transcript_{}__frame_{}.wav", time, actual_buffer_index);
          __save_wave_to_file(wave, file_name);
          wave.clear();
        }
      }   
  }
  
  if( err < 0 ){
    PLOGW << "Error while streaming. PA Stream not active";
    return StatusInfo(false, "Error while streaming. PA Stream not active");
  }

    // End stream and trascript last output
    char* result = DS_FinishStream(ds_stream);
    std::string res(result);
    res.append(" |end|");

    PLOGV << std::format("End of Steam. Heard: '{}'", res);
    std::cerr << std::format("End of Steam. Heard: '{}'\n", res);
    
    m_transcript.push_back(res);
    if ( (bool)m_observer)
      m_observer(res);
    if (streamer && streaming_fn)
      streaming_fn(streamer, res);

    DS_FreeString(result);

    if (output_to_wav &&  wave.size() > 0) {
      auto file_name = std::format("transcript_{}__final_frame.wav", time);
      __save_wave_to_file(wave, file_name);
      wave.clear();
    }
    
    // close stream
    err = Pa_CloseStream(pa_stream);
    if (err != paNoError)
    {
      auto msg = std::format("PortAudio error: ", Pa_GetErrorText(err));
      PLOGW << msg;
      return StatusInfo(false, msg);
    }

    err = Pa_Terminate();
    if (err != paNoError)
    {
      auto msg = std::format("PortAudio error: ", Pa_GetErrorText(err));
      PLOGW << msg;
      return StatusInfo(false, msg);
    }

    PLOGI << "--- Transcript End ---";
    std::cerr << "--- Transcript End ---\n";


    fvad_free(vad);

    DS_FreeModel(model);

    return StatusInfo(true, "Successfully transcriped");
}
