#ifndef TRANSCRIPTER_H
#define TRANSCRIPTER_H

#include <DeepSpeech/deepspeech.h>

#include <memory>
#include <string>
#include <vector>
#include <array>
#include <chrono>

namespace docspeak {
    class Transcripter
    {
    private:
        
    public:
        Transcripter();
        ~Transcripter();

        void record();
        void to_text();
    };

}

#endif