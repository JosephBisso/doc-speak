#include "transcripter.h"

using namespace docspeak;

std::filesystem::path Transcripter::s_scorer_path = std::filesystem::current_path() / "deepspeech-0.9.3-models.scorer";
std::filesystem::path Transcripter::s_model_path = std::filesystem::current_path() / "deepspeech-0.9.3-models.pbmm";
std::filesystem::path Transcripter::s_audio_input_directory;
std::filesystem::path Transcripter::s_audio_output_directory = std::filesystem::current_path()/"output";

Transcripter::Transcripter()
{
}

Transcripter::~Transcripter()
{
}

void Transcripter::to_text() {
    
}


bool Transcripter::set_model_path(const std::filesystem::path& model_path) {
    if (!std::filesystem::exists(model_path)) {
        PLOGW << std::format("Could not set model path because path doesn't exit: {}", model_path.string());
        return false;
    }

    PLOGV << std::format("Set model path to {}", model_path.string());
    s_model_path = model_path;
    return true;
}

bool Transcripter::set_scorer_path(const std::filesystem::path& scorer_path) {
    if (!std::filesystem::exists(scorer_path)) {
        PLOGW << std::format("Could not set scorer path because path doesn't exit: {}", scorer_path.string());
        return false;
    }

    PLOGV << std::format("Set scorer path to {}", scorer_path.string());
    s_scorer_path = scorer_path;
    return true;
}

bool Transcripter::set_audio_input_directory(const std::filesystem::path& audio_directory) {
    if (!std::filesystem::exists(audio_directory) || std::filesystem::status(audio_directory).type() != std::filesystem::file_type::directory) {
        PLOGW << std::format("Could not set input  path because path doesn't exit or not a directory: {}", audio_directory.string());
        return false;
    }

    PLOGV << std::format("Set audio input directory path to {}", audio_directory.string());
    s_audio_input_directory = audio_directory;
    return true;
}

bool Transcripter::set_audio_output_directory(const std::filesystem::path& audio_directory) {
    if (audio_directory.empty())
        return true;

    if (std::filesystem::exists(audio_directory)) {
        PLOGI << std::format("Setting audio output folder to {}", audio_directory.string());
        s_audio_output_directory = audio_directory;
        return true;
    } 

    std::error_code ec;
    auto created = std::filesystem::create_directory(audio_directory, ec);
    if (created) {
        PLOGI << std::format("Setting audio output folder to {}", audio_directory.string());
        s_audio_output_directory = audio_directory;
        return true;
    }
    else {
        PLOGW << std::format("Cannot create folder at path {}. Error: {}", audio_directory.string(), ec.message());
        return false;
    }
}

std::vector<std::filesystem::path> Transcripter::__get_audio_from_dir() {
    std::vector<std::filesystem::path> result;

    if (s_audio_input_directory.empty()) {
        PLOGW << "Cannot get audio beacause directory is empty";
        return result;
    }

    for (auto const& element : std::filesystem::recursive_directory_iterator{s_audio_input_directory}) {
        if (!element.is_regular_file())
            continue;
        
        if (auto path = element.path(); path.extension() == ".wav") {
            PLOGV << std::format("Found audio file: {}", path.string());
            result.push_back(path);
        }
    }

    return result;
}

bool Transcripter::__check_if_ready() {
  auto ready = !(s_model_path.empty() || s_scorer_path.empty());

  PLOGI << (ready ? "Transcripter ready" : "Transcripter not ready");

  m_ready = ready;
  return m_ready;
}


std::vector<std::string>Transcripter::transcript_test() {
    auto model = s_model_path.string();
    auto scorer = s_scorer_path.string();
    auto all_audios = __get_audio_from_dir();
    bool set_beamwidth = false;
    int beam_width = 0;
    bool set_alphabeta = false;
    float lm_alpha = 0.f;
    float lm_beta = 0.f;
    bool show_times = false;
    bool has_versions = false;
    bool extended_metadata = false;
    bool json_output = false;
    int json_candidate_transcripts = 3;
    int stream_size = 0;
    int extended_stream_size = 0;
    char* hot_words = NULL;

    std::vector<std::string> result;

    if (!__check_if_ready()) {
      PLOGW << "Transcripter not ready. Aborting";
      return result;
    }


    ModelState* ctx;

    int status = DS_CreateModel(model.c_str(), &ctx);
    if (status != 0) {
        char* error = DS_ErrorCodeToErrorMessage(status);
        PLOGW << std::format("Could not create model: {}", error);
        free(error);
        return result;
    }

    if (!scorer.empty()) {
        status = DS_EnableExternalScorer(ctx, scorer.c_str());
        if (status != 0) {
            PLOGW << "Could not enable external scorer";
            return result;
        }
        if (set_alphabeta) {
            status = DS_SetScorerAlphaBeta(ctx, lm_alpha, lm_beta);

        if (status != 0) {
            PLOGW << "Error setting scorer alpha and beta.";
            return result;
        }
        }
  }

//   if (hot_words) {
//     std::vector<std::string> hot_words_ = SplitStringOnDelim(hot_words, ",");
//     for ( std::string hot_word_ : hot_words_ ) {
//       std::vector<std::string> pair_ = SplitStringOnDelim(hot_word_, ":");
//       const char* word = (pair_[0]).c_str();
//       // the strtof function will return 0 in case of non numeric characters
//       // so, check the boost string before we turn it into a float
//       bool boost_is_valid = (pair_[1].find_first_not_of("-.0123456789") == std::string::npos);
//       float boost = strtof((pair_[1]).c_str(),0);
//       status = DS_AddHotWord(ctx, word, boost);
//       if (status != 0 || !boost_is_valid) {
//         fprintf(stderr, "Could not enable hot-word.\n");
//         return 1;
//       }
//     }
//   }

    std::for_each(all_audios.begin(), all_audios.end(), [&](const std::filesystem::path& audio_path) {
        auto audio = audio_path.string();

        struct stat wav_info;

        if (0 != stat(audio.c_str(), &wav_info)) {
            PLOGW << std::format("Error on stat: {}", errno);
        }

        switch (wav_info.st_mode & S_IFMT) {
        case S_IFREG: {
            auto speech = __ProcessFile(ctx, audio.c_str(), show_times);
            if(speech.empty())
                PLOGW << std::format("Empty Text for transcripted audio: {}", audio.c_str());
            else {
                result.push_back(speech);
                PLOGV << std::format("Heard: '{}'", speech);
                m_transcript.push_back(speech);
                if ( (bool)m_observer)
                  m_observer(speech);
            }
            break;
        }

        default:
            PLOGW << std::format("Cannot process given audio path {}", audio.c_str());
            break;
        }

    });

    return result;

}

std::string Transcripter::__ProcessFile(ModelState* context, const char* path, bool show_times)
{
    std::string transcripted;

    ds_audio_buffer audio = __GetAudioBuffer(path, DS_GetModelSampleRate(context));

    // Pass audio to DeepSpeech
    // We take half of buffer_size because buffer is a char* while
    // LocalDsSTT() expected a short*
    ds_result result = __LocalDsSTT(context,
                                    (const short*)audio.buffer,
                                    audio.buffer_size / 2,
                                    /*extended_metadata*/ false,
                                    /*json_output*/ false);
    free(audio.buffer);

    if (result.string) {
        transcripted = result.string;
        DS_FreeString((char*)result.string);
    }

    if (show_times) {
        PLOGD << std::format("cpu_time_overall={}",result.cpu_time_overall);
    }

    return transcripted;
}

Transcripter::ds_audio_buffer Transcripter::__GetAudioBuffer(const char* path, int desired_sample_rate)
{
  ds_audio_buffer res = {0};

  // FIXME: Hack and support only mono 16-bits PCM with standard SoX header
  FILE* wave = fopen(path, "r");

  size_t rv;

  unsigned short audio_format;
  fseek(wave, 20, SEEK_SET); rv = fread(&audio_format, 2, 1, wave);

  unsigned short num_channels;
  fseek(wave, 22, SEEK_SET); rv = fread(&num_channels, 2, 1, wave);

  unsigned int sample_rate;
  fseek(wave, 24, SEEK_SET); rv = fread(&sample_rate, 4, 1, wave);

  unsigned short bits_per_sample;
  fseek(wave, 34, SEEK_SET); rv = fread(&bits_per_sample, 2, 1, wave);

  // assert(audio_format == 1); // 1 is PCM
  // assert(num_channels == 1); // MONO
  // assert(sample_rate == desired_sample_rate); // at desired sample rate
  // assert(bits_per_sample == 16); // 16 bits per sample

  PLOGD << std::format("audio_format={}", audio_format);
  PLOGD << std::format("num_channels={}", num_channels);
  PLOGD << std::format("sample_rate={} (desired={})", sample_rate, desired_sample_rate);
  PLOGD << std::format("bits_per_sample={}", bits_per_sample);

  fseek(wave, 40, SEEK_SET); rv = fread(&res.buffer_size, 4, 1, wave);
  PLOGD << std::format("res.buffer_size={}", (long int)res.buffer_size);

  fseek(wave, 44, SEEK_SET);
  res.buffer = (char*)malloc(sizeof(char) * res.buffer_size);
  rv = fread(res.buffer, sizeof(char), res.buffer_size, wave);

  fclose(wave);

  return res;
}

Transcripter::ds_result Transcripter::__LocalDsSTT(ModelState* aCtx, const short* aBuffer, size_t aBufferSize, bool extended_output, bool json_output)
{
  ds_result res = {0};

  clock_t ds_start_time = clock();

  // sphinx-doc: c_ref_inference_start
  if (extended_output) {
    Metadata *result = DS_SpeechToTextWithMetadata(aCtx, aBuffer, aBufferSize, 1);
    res.string = __CandidateTranscriptToString(&result->transcripts[0]);
    DS_FreeMetadata(result);
  } else if (json_output) {
    Metadata *result = DS_SpeechToTextWithMetadata(aCtx, aBuffer, aBufferSize, /*json_candidate_transcripts*/ 3);
    res.string = __MetadataToJSON(result);
    DS_FreeMetadata(result);
  } else if (/*stream_size*/ 0 > 0) {
    StreamingState* ctx;
    int status = DS_CreateStream(aCtx, &ctx);
    if (status != DS_ERR_OK) {
      res.string = strdup("");
      return res;
    }
    size_t off = 0;
    const char *last = nullptr;
    const char *prev = nullptr;
    while (off < aBufferSize) {
      size_t cur = aBufferSize - off > /*stream_size*/ 0 ? /*stream_size*/ 0 : aBufferSize - off;
      DS_FeedAudioContent(ctx, aBuffer + off, cur);
      off += cur;
      prev = last;
      const char* partial = DS_IntermediateDecode(ctx);
      if (last == nullptr || strcmp(last, partial)) {
        PLOGD << std::format("{}", partial);
        last = partial;
      } else {
        DS_FreeString((char *) partial);
      }
      if (prev != nullptr && prev != last) {
        DS_FreeString((char *) prev);
      }
    }
    if (last != nullptr) {
      DS_FreeString((char *) last);
    }
    res.string = DS_FinishStream(ctx);
  } else if (/*extended_stream_size*/ 0 > 0) {
    StreamingState* ctx;
    int status = DS_CreateStream(aCtx, &ctx);
    if (status != DS_ERR_OK) {
      res.string = strdup("");
      return res;
    }
    size_t off = 0;
    const char *last = nullptr;
    const char *prev = nullptr;
    while (off < aBufferSize) {
      size_t cur = aBufferSize - off > /*extended_stream_size*/ 0 ? /*extended_stream_size*/ 0 : aBufferSize - off;
      DS_FeedAudioContent(ctx, aBuffer + off, cur);
      off += cur;
      prev = last;
      const Metadata* result = DS_IntermediateDecodeWithMetadata(ctx, 1);
      const char* partial = __CandidateTranscriptToString(&result->transcripts[0]);
      if (last == nullptr || strcmp(last, partial)) {
        PLOGD << std::format("{}", partial);
       last = partial;
      } else {
        free((char *) partial);
      }
      if (prev != nullptr && prev != last) {
        free((char *) prev);
      }
      DS_FreeMetadata((Metadata *)result);
    }
    const Metadata* result = DS_FinishStreamWithMetadata(ctx, 1);
    res.string = __CandidateTranscriptToString(&result->transcripts[0]);
    DS_FreeMetadata((Metadata *)result);
    free((char *) last);
  } else {
    res.string = DS_SpeechToText(aCtx, aBuffer, aBufferSize);
  }
  // sphinx-doc: c_ref_inference_stop

  clock_t ds_end_infer = clock();

  res.cpu_time_overall =
    ((double) (ds_end_infer - ds_start_time)) / CLOCKS_PER_SEC;

  return res;
}

char* Transcripter::__CandidateTranscriptToString(const CandidateTranscript* transcript)
{
  std::string retval = "";
  for (int i = 0; i < transcript->num_tokens; i++) {
    const TokenMetadata& token = transcript->tokens[i];
    retval += token.text;
  }
  return strdup(retval.c_str());
}

char* Transcripter::__MetadataToJSON(Metadata* result)
{
  std::ostringstream out_string;
  out_string << "{\n";

  for (int j=0; j < result->num_transcripts; ++j) {
    const CandidateTranscript *transcript = &result->transcripts[j];

    if (j == 0) {
      out_string << __CandidateTranscriptToJSON(transcript);

      if (result->num_transcripts > 1) {
        out_string << ",\n" << R"("alternatives")" << ":[\n";
      }
    } else {
      out_string << "{" << __CandidateTranscriptToJSON(transcript) << "}";

      if (j < result->num_transcripts - 1) {
        out_string << ",\n";
      } else {
        out_string << "\n]";
      }
    }
  }
  
  out_string << "\n}\n";

  return strdup(out_string.str().c_str());
}

std::string Transcripter::__CandidateTranscriptToJSON(const CandidateTranscript *transcript)
{
  std::ostringstream out_string;

  std::vector<meta_word> words = __CandidateTranscriptToWords(transcript);

  out_string << R"("metadata":{"confidence":)" << transcript->confidence << R"(},"words":[)";

  for (int i = 0; i < words.size(); i++) {
    meta_word w = words[i];
    out_string << R"({"word":")" << w.word << R"(","time":)" << w.start_time << R"(,"duration":)" << w.duration << "}";

    if (i < words.size() - 1) {
      out_string << ",";
    }
  }

  out_string << "]";

  return out_string.str();
}

std::vector<Transcripter::meta_word> Transcripter::__CandidateTranscriptToWords(const CandidateTranscript* transcript)
{
  std::vector<meta_word> word_list;

  std::string word = "";
  float word_start_time = 0;

  // Loop through each token
  for (int i = 0; i < transcript->num_tokens; i++) {
    const TokenMetadata& token = transcript->tokens[i];

    // Append token to word if it's not a space
    if (strcmp(token.text, " ") != 0) {
      // Log the start time of the new word
      if (word.length() == 0) {
        word_start_time = token.start_time;
      }
      word.append(token.text);
    }

    // Word boundary is either a space or the last token in the array
    if (strcmp(token.text, " ") == 0 || i == transcript->num_tokens-1) {
      float word_duration = token.start_time - word_start_time;

      if (word_duration < 0) {
        word_duration = 0;
      }

      meta_word w;
      w.word = word;
      w.start_time = word_start_time;
      w.duration = word_duration;

      word_list.push_back(w);

      // Reset
      word = "";
      word_start_time = 0;
    }
  }

  return word_list;
}

int Transcripter::__streamCallback(const void *input, void *output,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData )
{
  std::vector<SAMPLE>* buffer = (std::vector<SAMPLE>*) userData;
  
  const SAMPLE *in = (const SAMPLE*) input;
  
  for (int i = 0; i < framesPerBuffer; i++)
  {
    buffer->push_back(in[i]);
  }
  
  return paContinue;
}

bool Transcripter::__save_wave_to_file(const std::vector<SAMPLE>& wave, std::string file_name) {

  PLOGI << "--- Saving wav ---";
  std::cerr << "--- Saving wav ---\n";

  TinyWav tw;

  tinywav_open_write(&tw, NUM_CHANNELS, SAMPLE_RATE, TW_INT16, TW_INTERLEAVED, (s_audio_output_directory/file_name).string().c_str());

  if (!tw.f) {
    PLOGW << std::format("Error: cannot open {}", (s_audio_output_directory/file_name).string());
    return false;
  } else 
    PLOGV << std::format("Saving to: {}", (s_audio_output_directory/file_name).string());

  auto total = 0;
  auto start = wave.begin();

  while(true) {
    std::vector<SAMPLE> out_buffer;
    out_buffer.reserve(SAMPLE_PER_FRAME);

    std::copy_n(start, SAMPLE_PER_FRAME,  std::back_inserter(out_buffer));
    start += SAMPLE_PER_FRAME;

    total += out_buffer.size();

    tinywav_write_f(&tw, out_buffer.data(), SAMPLE_PER_FRAME);

    if (total >= wave.size())
      break;
  }

  tinywav_close_write(&tw);

  PLOGI << "--- End Saving wav ---";
  return true;
}


/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
int Transcripter::__recordCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    paTestData *data = (paTestData*)userData;
    const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
    SAMPLE *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    long framesToCalc;
    long i;
    int finished;
    unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

    (void) outputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;

    if( framesLeft < framesPerBuffer )
    {
        framesToCalc = framesLeft;
        finished = paComplete;
    }
    else
    {
        framesToCalc = framesPerBuffer;
        finished = paContinue;
    }

    if( inputBuffer == NULL )
    {
        for( i=0; i<framesToCalc; i++ )
        {
            *wptr++ = SAMPLE_SILENCE;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = SAMPLE_SILENCE;  /* right */
        }
    }
    else
    {
        for( i=0; i<framesToCalc; i++ )
        {
            *wptr++ = *rptr++;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
        }
    }
    data->frameIndex += framesToCalc;
    return finished;
}

/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
int Transcripter::__playCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData )
{
    paTestData *data = (paTestData*)userData;
    SAMPLE *rptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    SAMPLE *wptr = (SAMPLE*)outputBuffer;
    unsigned int i;
    int finished;
    unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

    (void) inputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    if( framesLeft < framesPerBuffer )
    {
        /* final buffer... */
        for( i=0; i<framesLeft; i++ )
        {
            *wptr++ = *rptr++;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
        }
        for( ; i<framesPerBuffer; i++ )
        {
            *wptr++ = 0;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = 0;  /* right */
        }
        data->frameIndex += framesLeft;
        finished = paComplete;
    }
    else
    {
        for( i=0; i<framesPerBuffer; i++ )
        {
            *wptr++ = *rptr++;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
        }
        data->frameIndex += framesPerBuffer;
        finished = paContinue;
    }
    return finished;
}

int Transcripter::record_test() {
  PaStreamParameters  inputParameters,
                        outputParameters;
    PaStream*           stream;
    PaError             err = paNoError;
    paTestData          data;
    int                 i;
    int                 totalFrames;
    int                 numSamples;
    int                 numBytes;
    SAMPLE              max, val;
    double              average;

    try {
      PLOGV << ("--- Starting record ---");

      data.maxFrameIndex = totalFrames = AUDIO_DURATION * SAMPLE_RATE; /* Record for a few seconds. */
      data.frameIndex = 0;
      numSamples = totalFrames * NUM_CHANNELS;
      numBytes = numSamples * sizeof(SAMPLE);
      data.recordedSamples = (SAMPLE *) malloc( numBytes ); /* From now on, recordedSamples is initialised. */
      if( data.recordedSamples == NULL )
      {
          PLOGW << ("Could not allocate record array");
          throw std::exception("Aborted record");
      }
      for( i=0; i<numSamples; i++ ) data.recordedSamples[i] = 0;

      err = Pa_Initialize();
      if( err != paNoError ) throw std::exception("Aborted record");

      inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
      if (inputParameters.device == paNoDevice) {
          PLOGW<< ("Error: No default input device.");
          throw std::exception("Aborted record");
      }
      inputParameters.channelCount = NUM_CHANNELS;
      inputParameters.sampleFormat = PA_SAMPLE_TYPE;
      inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
      inputParameters.hostApiSpecificStreamInfo = NULL;

      /* Record some audio. -------------------------------------------- */
      err = Pa_OpenStream(
                &stream,
                &inputParameters,
                NULL,                  /* &outputParameters, */
                SAMPLE_RATE,
                SAMPLE_PER_FRAME,
                paClipOff,      /* we won't output out of range samples so don't bother clipping them */
                __recordCallback,
                &data );
      if( err != paNoError ) throw std::exception("Aborted record");

      err = Pa_StartStream( stream );
      if( err != paNoError ) throw std::exception("Aborted record");
      PLOGI << "=== Now recording!! ===";
      std::cerr << "=== Now recording!! ===\n";

      while( ( err = Pa_IsStreamActive( stream ) ) == 1 )
      {
          Pa_Sleep(1000);
          PLOGV << std::format("index = {}", data.frameIndex );
          std::cerr << std::format("index = {}\n", data.frameIndex );
      }
      if( err < 0 ) throw std::exception("Aborted record");

      err = Pa_CloseStream( stream );
      if( err != paNoError ) throw std::exception("Aborted record");

      PLOGI << ("--- End record ---");
      std::cerr << ("--- End recording ---\n");

      /* Measure maximum peak amplitude. */
      max = 0;
      average = 0.0;
      for( i=0; i<numSamples; i++ )
      {
          val = data.recordedSamples[i];
          if( val < 0 ) val = -val; /* ABS */
          if( val > max )
          {
              max = val;
          }
          average += val;
      }

      average = average / (double)numSamples;

      PLOGV << std::format("sample max amplitude = {}", max );
      PLOGV << std::format("sample average = {}", average );

      /* Write recorded data to a file. */
      FILE  *fid;
      fid = fopen("recorded.raw", "wb");
      if( fid == NULL )
      {
          PLOGW << ("Could not open file.");
      }
      else
      {
          fwrite( data.recordedSamples, NUM_CHANNELS * sizeof(SAMPLE), totalFrames, fid );
          fclose( fid );
          PLOGI << ("Wrote data to 'recorded.raw'");
      }

      /* Playback recorded data.  -------------------------------------------- */
    data.frameIndex = 0;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
        PLOGW<< ("Error: No default output device.");
          throw std::exception("Aborted playback");
    }
    outputParameters.channelCount = NUM_CHANNELS;
    outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    PLOGI << ("=== Now playing back. ===");
    err = Pa_OpenStream(
              &stream,
              NULL, /* no input */
              &outputParameters,
              SAMPLE_RATE,
              SAMPLE_PER_FRAME,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              __playCallback,
              &data );
    if( err != paNoError ) 
          throw std::exception("Aborted playback");

    if( stream )
    {
        err = Pa_StartStream( stream );
        if( err != paNoError ) 
          throw std::exception("Aborted playback");


        PLOGV << ("Waiting for playback to finish.");

        while( ( err = Pa_IsStreamActive( stream ) ) == 1 ) Pa_Sleep(100);
        if( err < 0 ) 
          throw std::exception("Aborted playback");


        err = Pa_CloseStream( stream );
        if( err != paNoError ) 
          throw std::exception("Aborted playback");


        PLOGV << ("Done.");
    }

      ////////////////////////////////////////
      ////////////////Converting to wav ////////////////////////
      ////////////////////////////////////////

      auto outputPath = "recorded.wav";

      // Open the output file using tinywav
      TinyWav tw;
      tinywav_open_write(&tw, NUM_CHANNELS, SAMPLE_RATE, TW_INT16, TW_INTERLEAVED, outputPath);
      
      // Check if the output file is opened successfully
      if (!tw.f)
      {
        PLOGW << std::format("Error: cannot open {}", outputPath);
        return -1;
      }
      
      auto total = 0;
      
      for (int num_of_buffer = 0; num_of_buffer <= totalFrames / SAMPLE_PER_FRAME; num_of_buffer++) {
        std::vector<SAMPLE> buffer;
        
        if (num_of_buffer * SAMPLE_PER_FRAME + SAMPLE_PER_FRAME < totalFrames) {
          buffer.reserve(SAMPLE_PER_FRAME); 

          for (auto frames = 0; frames < SAMPLE_PER_FRAME; frames++) {
            buffer.push_back(data.recordedSamples[num_of_buffer*SAMPLE_PER_FRAME + frames]);
          }

          total += tinywav_write_f(&tw, buffer.data(), SAMPLE_PER_FRAME);
          
        } else {
          auto rest_frames = totalFrames % SAMPLE_PER_FRAME;
          buffer.reserve(rest_frames); 
          
          for (auto frames = 0; frames < rest_frames; frames++) {
            buffer.push_back(data.recordedSamples[num_of_buffer*SAMPLE_PER_FRAME + rest_frames]);
          }

          total += tinywav_write_f(&tw, buffer.data(), rest_frames);

        }
      }
      
      // Close the input and output files
      tinywav_close_write(&tw);
      PLOGV << std::format("total read {}", total );
      PLOGV << std::format("total must {}", numBytes );

    } catch (const std::exception& exc) {
        PLOGW << std::format("{}", exc.what() );
        Pa_Terminate();
        if( data.recordedSamples )       /* Sure it is NULL or valid. */
            free( data.recordedSamples );
        if( err != paNoError )
        {
            PLOGW << std::format("An error occurred while using the portaudio stream" );
            PLOGW << std::format("Error number: {}", err );
            PLOGW << std::format("Error message:{}", Pa_GetErrorText( err ) );
            err = 1;          /* Always return 0 or 1, but no other return codes. */
        }
    }    
    
    return err;
}

int Transcripter::__get_speech_from_frame (Fvad* vad, 
                    const SAMPLE* in_buffer, 
                    std::vector<SAMPLE>& out_buffer, 
                    std::deque<std::pair<std::vector<SAMPLE>, bool>>& speech_deque, 
                    size_t offset, 
                    size_t length,
                    bool& triggered)
{
    auto padding_ms = 300;
    auto speech_ratio = 0.75;
    auto silence_ratio = 0.75;
    auto frame_duration_ms = 1000 * SAMPLE_PER_FRAME / SAMPLE_RATE;
    auto num_padding_frames = padding_ms / frame_duration_ms;

    std::vector<SAMPLE> actual_buffer {
      in_buffer + offset,
      in_buffer + offset + SAMPLE_PER_FRAME
    };

    bool speech = __is_speech(vad, actual_buffer.data(), SAMPLE_PER_FRAME);

    int res = 0;

    if (!triggered) {
      speech_deque.push_back(std::make_pair(actual_buffer, speech));
      if (speech_deque.size() > num_padding_frames) 
        speech_deque.pop_front();

      auto num_voiced = std::count_if(speech_deque.begin(), speech_deque.end(), 
        [](auto feed) {return feed.second;}
      );

      if (num_voiced > (int)(speech_ratio * num_padding_frames)) {
          triggered = true;

          out_buffer.clear(); out_buffer.reserve(speech_deque.size());
          std::for_each(speech_deque.begin(), speech_deque.end(), [&out_buffer](auto speech) {
            std::copy(speech.first.begin(), speech.first.end(), std::back_inserter(out_buffer));
          });

          speech_deque.clear();
          res = 1;
      }

    } else {
        out_buffer.clear(); out_buffer.reserve(actual_buffer.size());
        std::copy(actual_buffer.begin(), actual_buffer.end(), std::back_inserter(out_buffer));
        res = 1;
        
        speech_deque.push_back(std::make_pair(actual_buffer, speech));
        if (speech_deque.size() > num_padding_frames) 
            speech_deque.pop_front();

        auto num_unvoiced = std::count_if(speech_deque.begin(), speech_deque.end(), 
          [](auto feed) {return !feed.second;}
        );

        if (num_unvoiced > (int)(silence_ratio * num_padding_frames)) {
            triggered = false;

            speech_deque.clear();
            res = 2;
        }
    }

  return res;

}


// Check if audio frame contains a speech
bool Transcripter::__is_speech(Fvad* vad, const short* frame, int length) {

  auto result = fvad_process(vad, frame, length);
  
  if (result == -1) {
    PLOGW << "invalid frame length. Frame length must be a multiple of 10 ms";
    return false;
  }

  return result == 1;
}

