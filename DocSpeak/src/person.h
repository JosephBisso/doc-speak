#ifndef PERSON_H
#define PERSON_H

#include "serializable.h"

namespace docspeak {
    typedef std::map<std::string, std::string> AdditionalInfos;

    class Person: public Item<Person>
    {
    public:
        enum Type {DOCTOR, PATIENT};
    private:
        std::string m_first_name;
        std::string m_last_name;
        std::string m_phone_number;
        std::string m_email;
        std::chrono::year_month_day m_birth_date;
        std::string m_sex;
        std::string m_occupation;
        std::vector<std::string> m_allergies;
        AdditionalInfos m_additional_info;
        
        Type m_type;
    

    public:
        explicit Person(Type type, const std::string& first_name, const std::string& last_name, const std::string& sex, const std::chrono::year_month_day& birth_date = std::chrono::January / 1 / 2023);
        ~Person();

        virtual void _save() = 0;
        virtual void _load() = 0;

        virtual bool is_like(const Person& person) const override;
        virtual bool equals(const Person& person) const override;

        inline Type get_type() const {return m_type;}
        inline std::string get_type_string() const {return m_type == Person::DOCTOR ? "Doctor" : "Patient";}

        inline void set_first_name(const std::string& first_name) {m_first_name = first_name;}
        inline std::string get_first_name() const {return m_first_name;}
        
        inline void set_last_name(const std::string& last_name) {m_last_name = last_name;}
        inline std::string get_last_name() const {return m_last_name;}

        inline std::string get_full_name() {return std::format("{}, {}", m_first_name, m_last_name);}
        
        inline void set_phone_number(const std::string& phone_number) {m_phone_number = phone_number;}
        inline std::string get_phone_number() const {return m_phone_number;}

        inline void set_email(const std::string& email) {m_email = email;}
        inline std::string get_email() const {return m_email;}

        inline void set_birth_date(const std::chrono::year_month_day& birth_date) {m_birth_date = birth_date;}
        inline const std::chrono::year_month_day& get_birth_date() const {return m_birth_date;}

        inline void set_sex(const std::string& sex) {m_sex = sex;}
        inline std::string get_sex() const {return m_sex;}

        inline void set_occupation(const std::string& occupation) {m_occupation = occupation;}
        inline std::string get_occupation() const {return m_occupation;}

        inline void set_allergies(const std::vector<std::string>& allergies) {m_allergies = allergies;}
        inline std::vector<std::string> get_allergies() const {return m_allergies;}

        inline void set_additional_info(const AdditionalInfos& additional_info) {m_additional_info = additional_info;}
        inline AdditionalInfos get_additional_info() const {return m_additional_info;}

    };

    typedef Book<Person> PersonBook;

}

inline bool operator==(const docspeak::Person& lhs, const docspeak::Person& rhs) {
    return lhs.equals(rhs);
}

#endif