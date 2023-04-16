#include "doctor.h"

using namespace docspeak;

std::shared_ptr<Doctor> docspeak::DOCTOR (const std::string& first_name, const std::string& last_name, const std::string& sex, const std::string& doctor_number) {
    std::shared_ptr<Doctor> doctor (new Doctor(first_name, last_name, sex, doctor_number));

    DoctorBook::add(doctor);

    return doctor;
}


void Doctor::_save() {
    std::cout << "Saving Doctor";
}

void Doctor::_load() {
    std::cout << "Loading Doctor";
}

template<>
void DoctorBook::add(std::shared_ptr<Doctor> element) {
    if (!s_instance)
        s_instance = std::shared_ptr<Book>(new DoctorBook);

    for (auto p : s_instance -> m_elements) {
        if (p->get_first_name() == element->get_first_name() && p->get_last_name() == element->get_last_name()) {
            auto msg = std::format("{} {} {} already added.", p->get_type_string(), p->get_first_name(), p->get_last_name());
            throw std::invalid_argument (msg.c_str()) ;
        }
    }

    s_instance -> m_elements.push_back(std::move(element));
}