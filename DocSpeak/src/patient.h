#ifndef PATIENT_H
#define PATIENT_H

#include "person.h"
#include "doctor.h"

namespace docspeak {
    class Record;

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
        std::string m_status;
        Records m_records;
    private:
        void _save() override;
        void _load() override;

    public:
        explicit Patient(const std::string& first_name, const std::string& last_name, const std::string& sex, const Insurance& health_insurance, const std::string& insurance_number, const std::string status = "123", const std::chrono::year_month_day& birth_date = std::chrono::January / 1 / 2023);
        ~Patient();

        inline void set_health_insurance(const Insurance& health_insurance) {m_health_insurance = health_insurance;}
        inline Insurance get_health_insurance() const {return m_health_insurance;}

        inline void set_insurance_number(const std::string& insurance_number) {m_insurance_number = insurance_number;}
        inline std::string get_insurance_number() const {return m_insurance_number;}

        inline void set_status(const std::string& status) {m_status = status;}
        inline std::string get_status() const {return m_status;}

        void add_record(std::shared_ptr<Record> record);
        inline Records& get_records() {return m_records;}
        inline size_t get_records_size() {return m_records.size();}

        bool is_like(const Patient& patient) const;

    };

    typedef Book<Patient, Person> PatientBook;

    std::shared_ptr<Patient> PATIENT(const std::string& first_name, const std::string& last_name, const std::string& sex, const Insurance& health_insurance, const std::string& insurance_number, const std::string status = "123", const std::chrono::year_month_day& birth_date = std::chrono::January / 1 / 2023);

}
#endif