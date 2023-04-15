#ifndef PATIENT_H
#define PATIENT_H

#include "person.h"
#include "record.h"

namespace docspeak {
    typedef std::vector<std::weak_ptr<Record>> Records;

    struct Insurance {
        enum Type {PRIVATE, PUBLIC};

        std::string name;
        std::string number;
        Type type;
    };

    class Patient : public Person
    {
    private:
        Insurance m_health_insurance;
        std::string m_insurance_number;
        Records m_records;
    private:
        void _save() override;
        void _load() override;

    public:
        explicit Patient(const std::string& first_name, const std::string& last_name, char sex, const Insurance& health_insurance, const std::string& insurance_number);
        ~Patient();

        inline void set_health_insurance(const Insurance& health_insurance) {m_health_insurance = health_insurance;}
        inline Insurance get_health_insurance() const {return m_health_insurance;}

        inline void set_insurance_number(const std::string& insurance_number) {m_insurance_number = insurance_number;}
        inline std::string get_insurance_number() const {return m_insurance_number;}

        inline void add_record(std::shared_ptr<Record> record) {m_records.push_back(std::weak_ptr(record));}
        inline Records& get_records() {return m_records;}
        inline size_t get_records_size() {return m_records.size();}
    };

    typedef Book<Patient> PatientBook;
    std::shared_ptr<Patient> PATIENT(const std::string& first_name, const std::string& last_name, char sex, const Insurance& health_insurance, const std::string& insurance_number);
    std::shared_ptr<Record> RECORD (std::time_t timestamp, std::shared_ptr<Patient> patient);

}
#endif