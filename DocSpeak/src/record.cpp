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
    for (auto el : m_elements) {
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

    m_elements.push_back(std::move(element));
}

bool Record::is_like(const Record& record) {
    bool is_like = m_timestamp == record.m_timestamp;

    if (auto doctor = m_doctor.lock()) {
        if (auto doctor2 = record.m_doctor.lock()) {
            is_like |= doctor -> is_like(*doctor2);
        }
    }

    is_like |= m_prescription && m_prescription -> contains(record.m_prescription -> to_string());

    return is_like;
}

