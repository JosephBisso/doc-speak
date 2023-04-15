#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include "protocol.h"

namespace docspeak
{
    class Prescription: public Protocol
    {
    private:
        std::set<std::string> m_medications;

    public:
        Prescription(/* args */);
        ~Prescription();

        inline void add_medication(const std::string& medication) {m_medications.insert(medication);}
        inline std::set<std::string> get_medications() {return m_medications;}

        inline bool contains(std::string medication) {return m_medications.contains(medication);}
        
        void print() override;
        std::string to_string() override;
    };
        
} // namespace docspeak


#endif