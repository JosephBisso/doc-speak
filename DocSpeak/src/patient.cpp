#include "patient.h"

using namespace docspeak;

Patient::Patient(const std::string& first_name,const std::string& last_name, const std::string& sex,  const Insurance& health_insurance, const std::string& insurance_number, const std::string status,  const std::chrono::year_month_day& birth_date) : 
    Person(Person::PATIENT, first_name, last_name, sex, birth_date), m_health_insurance(health_insurance), m_insurance_number(insurance_number), m_status(status)
{
    
}

std::shared_ptr<Patient> docspeak::PATIENT(const std::string& first_name, const std::string& last_name, const std::string& sex, const Insurance& health_insurance, const std::string& insurance_number, const std::string status, const std::chrono::year_month_day& birth_date) {
    auto patient = std::shared_ptr<Patient>(new Patient(first_name, last_name, sex, health_insurance, insurance_number, status, birth_date));

    PatientBook::add(patient);

    return patient;
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

template<>
void PatientBook::add(std::shared_ptr<Patient> element) {
    if (!s_instance)
        init_book();

    for (auto p : s_instance -> m_elements) {
        if (p->get_first_name() == element->get_first_name() && p->get_last_name() == element->get_last_name()) {
            auto msg = std::format("{} {} {} already added.", p->get_type_string(), p->get_first_name(), p->get_last_name());
            throw std::invalid_argument (msg.c_str()) ;
        }
        if (p->get_insurance_number() == element->get_insurance_number()) {
            auto msg = std::format("{} with insurance number {} already added: {} {}.",
                p->get_type_string(), p->get_insurance_number(), p->get_first_name(), p->get_last_name());
            throw std::invalid_argument (msg.c_str()) ;
        }
    }

    s_instance -> m_elements.push_back(std::move(element));
}

bool Patient::is_like(const Patient& patient) const {
    if (Person::is_like(patient))
        return true;

    return (!patient.m_insurance_number.empty() && m_insurance_number == patient.m_insurance_number) ||
            (!patient.m_health_insurance.name.empty() && m_health_insurance.name == patient.m_health_insurance.name);
}

void Patient::add_record(std::shared_ptr<Record> record) {
    m_records.push_back(std::weak_ptr(record));
}