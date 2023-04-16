#include "record.h"

using namespace docspeak;

Record::Record(std::time_t timestamp) : m_timestamp(timestamp)
{
    
}

Record::~Record()
{
}

template<>
void Book<Record>::add(std::shared_ptr<Record> element) {
    if (!s_instance)
        s_instance = std::shared_ptr<Book>(new RecordBook);

    for (auto el : s_instance -> m_elements) {
        if (auto tp = el->get_timestamp(); tp == element->get_timestamp()) {
            if (auto doctor = el -> get_doctor().lock()) {
                if (auto doctor2 = element -> get_doctor().lock()) {
                    if (*doctor == *doctor2) {
                        auto msg = std::format("Record with identical time point already added. The Doctor is {} {}", doctor -> get_first_name(), doctor -> get_last_name());
                        throw std::invalid_argument (msg.c_str()) ;
                    }
                }
            }
        }
    }

    s_instance -> m_elements.push_back(std::move(element));
}

bool Record::is_like(const Record& record) const {
    bool is_like = m_timestamp == record.m_timestamp;

    if (auto doctor = m_doctor.lock()) {
        if (auto doctor2 = record.m_doctor.lock()) {
            is_like |= doctor -> is_like(*doctor2);
        }
    }

    is_like |= m_prescription.lock() && m_prescription.lock() -> contains(record.m_prescription.lock() -> to_string());

    return is_like;
}

bool Record::equals(const Record& record) const {
    return *get_doctor().lock() == *record.get_doctor().lock() &&
            get_timestamp() == record.get_timestamp();
}

std::shared_ptr<Prescription> docspeak::PRESCRIPTION (const std::string& medication, std::shared_ptr<Record> record) {
    std::shared_ptr<Prescription> prescription (new Prescription(medication));
    
    PrescriptionBook::add(prescription);

    record -> set_prescription(prescription);

    return prescription;
}


