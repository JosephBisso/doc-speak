#ifndef PATIENT_H
#define PATIENT_H

#include "person.h"
#include "record.h"

namespace docspeak {

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
        std::vector<std::shared_ptr<Record>> m_records;

    public:
        explicit Patient(const std::string& first_name, const std::string& last_name, const char& sex, const Insurance& health_insurance, const std::string& insurance_number);
        ~Patient();

        void save() override;
        void load() override;

        inline void set_health_insurance(const Insurance& health_insurance) {m_health_insurance = health_insurance;}
        inline Insurance get_health_insurance() {return m_health_insurance;}

        inline void set_insurance_number(const std::string& insurance_number) {m_insurance_number = insurance_number;}
        inline std::string get_insurance_number() {return m_insurance_number;}

    };

}
#endif