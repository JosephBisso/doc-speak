#ifndef SUMMARY_H
#define SUMMARY_H

#include "protocol.h"

namespace docspeak
{
    class Summary: public Protocol
    {
    private:
        std::string m_reason_of_consultation;
        std::string m_general_condition;
        std::string m_findings;
    public:
        Summary(/* args */);
        ~Summary();

        inline void add_reason_of_consultation(const std::string& reason) {m_reason_of_consultation += reason;}        
        inline std::string get_reason_of_consultation() {return m_reason_of_consultation;}

        inline void add_general_condition(const std::string& condition) {m_general_condition += condition;}
        inline std::string get_general_conditionn() {return m_general_condition;}
        
        inline void add_findings(const std::string& finding) {m_findings += finding;}        
        inline std::string get_findings() {return m_findings;}


        void print() override;
        std::string to_string() override;
    };
        
} // namespace docspeak


#endif