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
        explicit Record(std::time_t timestamp);
        ~Record();

        void save(const std::filesystem::path& path) override;
        void load(const std::filesystem::path& path) override;

        inline void set_doctor (std::shared_ptr<Doctor> doctor) {m_doctor = doctor;}
        inline std::shared_ptr<Doctor> get_doctor () {return m_doctor;}

        inline void set_prescription (std::shared_ptr<Prescription> prescription) {m_prescription = prescription;}
        inline std::shared_ptr<Prescription> get_prescription () {return m_prescription;}

        inline void set_summary (std::shared_ptr<Summary> summary) {m_summary = summary;}
        inline std::shared_ptr<Summary> get_summary () {return m_summary;}

        inline std::time_t get_timestamp() {return m_timestamp;}

    };

}


#endif