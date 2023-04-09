#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "transcripter.h"
#include "printer.h"


namespace docspeak
{
    class Protocol
    {
    private:
        std::shared_ptr<Printer> m_printer;
        std::shared_ptr<Transcripter> m_transcripter;
        //PDFDoc Template
    public:
        Protocol(/* args */);
        ~Protocol();

        virtual void print() = 0;
        virtual std::string to_string() = 0;
    };
    
} // namespace docspeak


#endif