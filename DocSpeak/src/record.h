#ifndef RECORD_H
#define RECORD_H

#include <ctime>

#include "serializable.h"
#include "doctor.h"
#include "prescription.h"
#include "summary.h"

namespace docspeak {
    class Doctor;
    class Prescription;
    class Summary;

    class Record
    {
    private:
        std::weak_ptr<Doctor> m_doctor;
        std::shared_ptr<Prescription> m_prescription;
        std::shared_ptr<Summary> m_summary;
        std::time_t m_timestamp;
    public:
        explicit Record(std::time_t timestamp);
        ~Record();

        inline void set_doctor (std::shared_ptr<Doctor> doctor) {m_doctor = std::weak_ptr(doctor);}
        inline std::weak_ptr<Doctor> get_doctor () const {return m_doctor;}

        inline void set_prescription (std::shared_ptr<Prescription> prescription) {m_prescription = prescription;}
        inline std::shared_ptr<Prescription> get_prescription () {return m_prescription;}

        inline void set_summary (std::shared_ptr<Summary> summary) {m_summary = summary;}
        inline std::shared_ptr<Summary> get_summary () {return m_summary;}

        inline std::time_t get_timestamp() const {return m_timestamp;}

        bool is_like (const Record& record);

    };

    typedef Book<Record> RecordBook;

}

inline bool operator==(const Record& lhs, const Record& rhs) {
    return *lhs.get_doctor().lock() == *rhs.get_doctor().lock() &&
            lhs.get_timestamp() == rhs.get_timestamp();
}

#endif