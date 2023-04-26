#ifndef DOCTOR_H
#define DOCTOR_H

#include "person.h"

namespace docspeak {
    class Doctor : public Person
    {
    private:
        std::string m_doctor_number;
    private:
        void _save() override;
        void _load() override;
    public:
        Doctor(const std::string& first_name, const std::string& last_name, const std::string& sex, const std::string& doctor_number, const std::chrono::year_month_day& birth_date = std::chrono::January / 1 / 2023) : Person(Person::DOCTOR, first_name, last_name, sex, birth_date), m_doctor_number(doctor_number) {};
        ~Doctor() {};

        inline void set_doctor_number(const std::string& doctor_number) {m_doctor_number = doctor_number;}
        inline std::string get_doctor_number() {return m_doctor_number;}
    };

    typedef Book<Doctor, Person> DoctorBook;

    std::shared_ptr<Doctor> DOCTOR (const std::string& first_name, const std::string& last_name, const std::string& sex, const std::string& doctor_number, const std::chrono::year_month_day& birth_date = std::chrono::January / 1 / 2023);
}


#endif