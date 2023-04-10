#include "patient.h"

using namespace docspeak;

Patient::Patient(const std::string& first_name,const std::string& last_name, char sex,  const Insurance& health_insurance, const std::string& insurance_number) : Person(Person::PATIENT, first_name, last_name, sex), m_health_insurance(health_insurance), m_insurance_number(insurance_number)
{
    auto book = Book<Patient>::get_book();

    if (auto book_observer = book.lock()){
        book_observer -> add(std::shared_ptr<Patient>(this));
    } else {
        auto msg = std::format("The patient book cannot be accessed");
        throw std::invalid_argument(msg);
    }
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
void Book<Patient>::add(std::shared_ptr<Patient> element) {
    for (auto p : m_elements) {
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

    m_elements.push_back(element);
}
