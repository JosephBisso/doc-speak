#ifndef DOCTOR_H
#define DOCTOR_H

#include "person.h"

namespace docspeak {
    class Doctor : public Person
    {
    private:
        std::string m_doctor_number;
    public:
        Doctor(const std::string& first_name, const std::string& last_name, const char& sex, const std::string& doctor_number) : Person(first_name, last_name, sex), m_doctor_number(doctor_number) {};
        ~Doctor() {};
    };
}


#endif