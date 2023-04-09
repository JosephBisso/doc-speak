#include "patient.h"

using namespace docspeak;

Patient::Patient(const std::string& first_name,const std::string& last_name, const char& sex,  const Insurance& health_insurance, const std::string& insurance_number) : Person(first_name, last_name, sex), m_health_insurance(health_insurance), m_insurance_number(insurance_number)
{
}

Patient::~Patient()
{
}

void Patient::save() {
    std::cout << "Saving Patient";
}

void Patient::load() {
    std::cout << "Loading Patient";
}
