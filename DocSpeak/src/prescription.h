#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include "protocol.h"
#include "serializable.h"
#include "record.h"

namespace docspeak
{  
    class Record;
    
    class Prescription: public Protocol<Prescription>, public Item<Prescription>
    {
    public:
        enum ChargeType 
        {
            Free = 1 << 1,
            Apply = 1 << 2,
            Nigth = 1 << 3,
            Others = 1 << 4
        };

        struct Accident {
            enum AccidentType {None, Normal, Work};
            AccidentType accident_type = AccidentType::None;
            std::string employer;
            std::chrono::year_month_day accident_date;
        };

        enum PrescriptionElement {
            COST_BEARER = 1,
            PATIENT_NAME,
            PATIENT_BIRTH_DATE,
            PATIENT_NUMBER,
            PATIENT_STATUS,
            INSURANCE_NUMBER,
            HOUSE_NUMBER,
            DOCTOR_NUMBER,
            DATE,
            CHARGE_TYPE_FREE,
            CHARGE_TYPE_APPLY,
            CHARGE_TYPE_NIGTH,
            CHARGE_TYPE_OTHERS,
            ACCIDENT,
            ACCIDENT_WORK,
            ACCIDENT_EMPLOYER,
            ACCIDENT_DATE,
            MEDICATION_1,
            MEDICATION_2,
            MEDICATION_3,
            MED_AUF_IDEM_1,
            MED_AUF_IDEM_2,
            MED_AUF_IDEM_3,
            STAMP,
            DOCTOR_SIGNATURE,
        };

    private:
        std::array<std::string, 3> m_medications;
        std::array<bool, 3> m_med_auf_idem = {false, false, false};
        std::string m_cost_bearer;
        std::string m_house_number;
        size_t m_num_of_medications = 0;
        ChargeType m_charge_type = ChargeType::Apply;
        Accident m_accident;

        struct RecordInfo {
            std::string patient_name,
                        patient_number,
                        patient_birth_date,
                        patient_status,
                        insurance_number,
                        doctor_number,
                        date;
        } m_record_info;

    public:
        explicit Prescription(const std::string& medication);
        ~Prescription();

        void add_medication(const std::string& medication);
        inline std::array<std::string,3>& get_medications() {return m_medications;}
        inline size_t get_num_of_medications() {return m_num_of_medications;}

        inline void set_cost_bearer(const std::string& cost_bearer) {m_cost_bearer = cost_bearer;}
        inline std::string get_cost_bearer() {return m_cost_bearer;}

        inline void set_house_number(const std::string& house_number) {m_house_number = house_number;}
        inline std::string get_house_number() {return m_house_number;}

        inline void set_auf_idem(size_t position) {m_med_auf_idem[position - 1] = true;}

        inline ChargeType& charge_type() {return m_charge_type;}

        inline void set_accident(const Accident& accident) {m_accident = accident;}
        inline Accident& accident() {return m_accident;}

        inline void set_record_info(const RecordInfo& record_info) {m_record_info = record_info;}
        inline RecordInfo& get_record_info() {return m_record_info;}

        bool contains(const std::string& medication) const;
        
        Printer::StatusInfo print();
        std::string to_string();

        virtual bool is_like (const Prescription& prescription) const override;

    };

    typedef Book<Prescription> PrescriptionBook;

    std::shared_ptr<Prescription> PRESCRIPTION (const std::string& medication, std::shared_ptr<Record> record);


} // namespace docspeak



inline docspeak::Prescription& operator<< (docspeak::Prescription& stream, const std::string& medication) {
    stream.add_medication(medication);
    return stream;
}

inline Prescription::ChargeType operator|(Prescription::ChargeType a, Prescription::ChargeType b)
{
    return static_cast<Prescription::ChargeType>(static_cast<int>(a) | static_cast<int>(b));
}

#endif