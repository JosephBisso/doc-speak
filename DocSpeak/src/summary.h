#ifndef SUMMARY_H
#define SUMMARY_H

#include "protocol.h"

namespace docspeak
{
    class Summary: public Protocol<Summary>
    {
    public:
        enum SummaryElement {
            REASON_OF_CONSULTATION = 1,
            DIAGNOSIS,
            FINDINGS,
            DOCTOR_NUMBER,
            PATIENT_NUMBER,
            DATE,
            STAMP,
            DOCTOR_SIGNATURE
        };

    private:
        std::string m_doctor_number, m_date, m_patient_number;

        std::string m_reason_of_consultation;
        std::string m_diagnosis;
        std::string m_findings;
        std::map<std::string, std::string> m_additional_info;
    public:
        Summary(/* args */);
        ~Summary();

        inline void add_reason_of_consultation(const std::string& reason) {m_reason_of_consultation += reason;}        
        inline std::string get_reason_of_consultation() {return m_reason_of_consultation;}

        inline void add_diagnosis(const std::string& diagnosis) {m_diagnosis += diagnosis;}
        inline std::string get_diagnosisn() {return m_diagnosis;}
        
        inline void add_findings(const std::string& finding) {m_findings += finding;}        
        inline std::string get_findings() {return m_findings;}

        inline void add_additional_info(const std::string& info_name, const std::string& info) {m_additional_info[info_name] += info;}        
        inline std::map<std::string, std::string>& get_additional_info() {return m_additional_info;}

        inline void set_doctor_number(std::string doctor_number) {m_doctor_number = doctor_number;}
        inline void set_patient_number(std::string patient_number) {m_patient_number = patient_number;}
        inline void set_date(std::string date) {m_date = date;}

        Printer::StatusInfo print();
        std::string to_string();
    };
        
} // namespace docspeak


#endif