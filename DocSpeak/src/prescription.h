#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include "protocol.h"
#include "serializable.h"

namespace docspeak
{ 
    class Prescription: public Protocol<Prescription>, public Item<Prescription>
    {
    public:
        enum ChargeType {
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

    private:
        std::array<std::string, 3> m_medications;
        std::array<bool, 3> m_med_auf_idem;
        std::string m_cost_bearer;
        size_t m_num_of_medications = 0;
        ChargeType m_charge_type = ChargeType::Apply;
        Accident m_accident;

    public:
        explicit Prescription(const std::string& medication);
        ~Prescription();

        void add_medication(const std::string& medication);
        inline std::array<std::string,3>& get_medications() {return m_medications;}
        inline size_t get_num_of_medications() {return m_num_of_medications;}

        inline void set_cost_bearer(const std::string& cost_bearer) {m_cost_bearer = cost_bearer;}
        inline std::string get_cost_bearer() {return m_cost_bearer;}

        inline void set_auf_idem(size_t position) {m_med_auf_idem[position - 1] = true;}

        inline ChargeType& charge_type() {return m_charge_type;}
        inline Accident& accident() {return m_accident;}

        bool contains(const std::string& medication) const;
        
        Printer::StatusInfo print();
        std::string to_string();

        virtual bool is_like (const Prescription& prescription) const override;

    };

    typedef Book<Prescription> PrescriptionBook;

} // namespace docspeak



inline docspeak::Prescription& operator<< (docspeak::Prescription& stream, const std::string& medication) {
    stream.add_medication(medication);
    return stream;
}

#endif