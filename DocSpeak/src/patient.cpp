#include "patient.h"

using namespace docspeak;

Patient::Patient(const std::string& first_name,const std::string& last_name, char sex,  const Insurance& health_insurance, const std::string& insurance_number) : Person(Person::PATIENT, first_name, last_name, sex), m_health_insurance(health_insurance), m_insurance_number(insurance_number)
{
}

Patient::~Patient()
{
}

void Patient::_save() {
    std::cout << "Saving Patient";
}

void Patient::_load() {
    std::cout << "Loading Patient";
}
