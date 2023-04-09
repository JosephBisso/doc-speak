#ifndef SUMMARY_H
#define SUMMARY_H

#include "protocol.h"

namespace docspeak
{
    class Summary: public Protocol
    {
    private:
    public:
        Summary(/* args */);
        ~Summary();

        void print() override;
        std::string to_string() override;
    };
        
} // namespace docspeak


#endif