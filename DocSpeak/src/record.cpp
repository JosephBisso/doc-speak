#include "record.h"

using namespace docspeak;

Record::Record(std::time_t timestamp) : m_timestamp(timestamp)
{
    
}

Record::~Record()
{
}

std::string Record::s_date_format = "%d-%m-%Y";

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

std::shared_ptr<Record> docspeak::RECORD (std::time_t timestamp, std::shared_ptr<Patient> patient, std::shared_ptr<Doctor> doctor) {
    std::shared_ptr<Record> record (new Record(timestamp));

    if (doctor)
        record -> set_doctor(doctor);

    patient -> add_record(record);
    record -> set_patient(patient);
    
    RecordBook::add(record);
    return record;
}

std::string docspeak::toString(std::time_t timestamp, const char* date_format) {
    char timeString[std::size("dd-mm-yyyy")];
    std::strftime(std::data(timeString), std::size(timeString), date_format, std::gmtime(&timestamp));
    return timeString;
}
