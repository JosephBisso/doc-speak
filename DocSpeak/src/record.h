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

    class Record: public Serializable
    {
    private:
        std::shared_ptr<Doctor> m_doctor;
        std::shared_ptr<Prescription> m_prescription;
        std::shared_ptr<Summary> m_summary;
        std::time_t m_timestamp;
    public:
        Record(/* args */);
        ~Record();

        void save() override;
        void load() override;
    };

}


#endif