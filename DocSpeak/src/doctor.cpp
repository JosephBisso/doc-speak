#include "doctor.h"

using namespace docspeak;

std::shared_ptr<Doctor> docspeak::DOCTOR (const std::string& first_name, const std::string& last_name, char sex, const std::string& doctor_number) {
    std::shared_ptr<Doctor> doctor (new Doctor(first_name, last_name, sex, doctor_number));

    auto book = Book<Doctor>::get_book();

    if (auto book_observer = book.lock()){
        book_observer -> add(doctor);

        return doctor;
    } else {
        auto msg = std::format("The book cannot be accessed");
        throw std::invalid_argument(msg);
    }
}


void Doctor::_save() {
    std::cout << "Saving Doctor";
}

void Doctor::_load() {
    std::cout << "Loading Doctor";
}

template<>
void Book<Doctor>::add(std::shared_ptr<Doctor> element) {
    for (auto p : m_elements) {
        if (p->get_first_name() == element->get_first_name() && p->get_last_name() == element->get_last_name()) {
            auto msg = std::format("{} {} {} already added.", p->get_type_string(), p->get_first_name(), p->get_last_name());
            throw std::invalid_argument (msg.c_str()) ;
        }
    }

    m_elements.push_back(std::move(element));
}