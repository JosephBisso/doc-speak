
#ifndef TRANSCRIPTER_H
#define TRANSCRIPTER_H

#include "printer.h"

#include <DeepSpeech/deepspeech.h>
#include <plog/Log.h>
#include <portaudio.h>
#include <tinywav.h>
#include <fvad.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <chrono>
#include <functional>
#include <filesystem>
#include <utility>
#include <deque>

using namespace std::literals;
using namespace std::placeholders;

/* #define SAMPLE_RATE  (17932) // Test failure to open with this value. */
#define SAMPLE_RATE             (16000)
#define SAMPLE_PER_FRAME        (320) //50 Sample per second -> 16000 / 50 = 320.
#define AUDIO_DURATION          (5)
#define STREAM_DURATION         (20)
#define MAX_STREAM_DURATION     (300)
#define NUM_CHANNELS            (1)

#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"

namespace docspeak {

    class Transcripter
    {
    private:
        bool m_ready;
        std::vector<std::string> m_transcript;
        std::function<void(std::string)> m_observer;

        const std::chrono::seconds C_MAX_STREAM_DURATION {MAX_STREAM_DURATION};
        std::chrono::seconds m_max_silence_duration {5};

    public:
        static std::filesystem::path s_scorer_path;
        static std::filesystem::path s_model_path;
        static std::filesystem::path s_audio_input_directory;
        static std::filesystem::path s_audio_output_directory;

        Transcripter();
        ~Transcripter();

        void to_text();

        template <typename T>
        StatusInfo transcript(bool output_to_wav = true,
                            T* streamer = nullptr,
                            std::function<void(T*, std::string)> streaming_fn = nullptr);

        static int __streamCallback(const void *input, void *output,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData );

        static bool set_model_path(const std::filesystem::path& model_path);
        static bool set_scorer_path(const std::filesystem::path& scorer_path);
        static bool set_audio_input_directory(const std::filesystem::path& audio_directory);
        static bool set_audio_output_directory(const std::filesystem::path& audio_directory);

        inline void set_max_silence_duration(std::chrono::seconds max_silence_duration) {m_max_silence_duration = max_silence_duration;}
        inline std::chrono::seconds get_max_silence_duration() {return m_max_silence_duration;}

        std::vector<std::string> transcript_test();
        int record_test();

        inline std::vector<std::string>& get_transcription() {return m_transcript;}
        inline void set_observer(std::function<void(std::string)> observer) {m_observer = observer;__check_if_ready();}

    private:
        std::vector<std::filesystem::path> __get_audio_from_dir();
        
        bool __save_wave_to_file(const std::vector<SAMPLE>& wave, std::string file_name);
        
        bool __check_if_ready();

        bool __is_speech(Fvad* vad, const short* frame, int length);

        int __get_speech_from_frame(Fvad* vad, 
                    const SAMPLE* in_buffer, 
                    std::vector<SAMPLE>& out_buffer, 
                    std::deque<std::pair<std::vector<SAMPLE>, bool>>& speech_buffer, 
                    size_t offset, 
                    size_t length,
                    bool& triggered);

    
    public:
        typedef struct {
                char*  buffer;
                size_t buffer_size;
            } ds_audio_buffer;

            typedef struct {
                const char* string;
                double cpu_time_overall;
            } ds_result;

            struct meta_word {
                std::string word;
                float start_time;
                float duration;
            };

            typedef struct{
                int          frameIndex;  /* Index into sample array. */
                int          maxFrameIndex;
                SAMPLE      *recordedSamples;
            } paTestData;

            static int __recordCallback( const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData );

            static int __playCallback( const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData );
    private:
        std::string __ProcessFile(ModelState* context, const char* path, bool show_times);
        ds_audio_buffer __GetAudioBuffer(const char* path, int desired_sample_rate);
        ds_result __LocalDsSTT(ModelState* aCtx, const short* aBuffer, size_t aBufferSize, bool extended_output, bool json_output);
        char* __CandidateTranscriptToString(const CandidateTranscript* transcript);
        std::string __CandidateTranscriptToJSON(const CandidateTranscript *transcript);
        std::vector<meta_word> __CandidateTranscriptToWords(const CandidateTranscript* transcript);
        char* __MetadataToJSON(Metadata* result);
    };

}

#include "transcripter.hpp"
#endif
