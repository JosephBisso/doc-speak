#include "prescription.h"

using namespace docspeak;

Prescription::Prescription(const std::string& medication)
{
   m_medications[m_num_of_medications++] = medication;
}

Prescription::~Prescription()
{
}

void Prescription::add_medication(const std::string& medication) {
    if (m_num_of_medications < 3) {
        m_medications[m_num_of_medications++] = medication;
        return;
    }

    auto msg = std::format("Cannot add more than 3 medications in a prescription.");
    throw std::invalid_argument (msg.c_str()) ;
}


void Prescription::print() {

}

std::string Prescription::to_string() {
    std::string result;
    std::for_each(m_medications.begin(), m_medications.end(), [&result](auto medication) {
        result += medication + ";";
    });

    return result;
}

bool Prescription::is_like (const Prescription& prescription) const {
    return std::any_of(prescription.m_medications.begin(), prescription.m_medications.end(), [&](auto m) {
        return this -> contains (m);
    });
}

bool Prescription::contains(const std::string& medication) const{
    return std::any_of(m_medications.begin(), m_medications.end(), [&medication](auto m) {
        return !m.empty() && m == medication;
    });
}


template<>
void Book<Prescription>::add(std::shared_ptr<Prescription> element) {
    if (!s_instance)
        s_instance = std::shared_ptr<Book>(new PrescriptionBook);

    if (element -> get_medications().size() == 0) {
        auto msg = std::format("A Prescription must contain at least one medication");
        throw std::invalid_argument (msg.c_str()) ;
    }

    s_instance -> m_elements.push_back(std::move(element));
}
