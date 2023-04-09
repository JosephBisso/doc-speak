#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include "protocol.h"

namespace docspeak
{
    class Prescription: public Protocol
    {
    private:
    public:
        Prescription(/* args */);
        ~Prescription();

        void print() override;
        std::string to_string() override;
    };
        
} // namespace docspeak


#endif