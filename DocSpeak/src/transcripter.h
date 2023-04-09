#ifndef TRANSCRIPTER_H
#define TRANSCRIPTER_H

#include <deepspeech.h>

#include <memory>
#include <string>

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