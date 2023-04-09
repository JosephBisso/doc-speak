#ifndef PERSON_H
#define PERSON_H

#include "serializable.h"

namespace docspeak {
    typedef std::map<std::string, std::string> AdditionalInfos;

    class Person : private Serializable
    {
    private:
        std::string m_first_name;
        std::string m_last_name;
        std::string m_phone_number;
        std::string m_email;
        std::chrono::year_month_day m_birth_date;
        char m_sex;
        std::string m_occupation;
        std::vector<std::string> m_allergies;
        AdditionalInfos m_additional_info;

    public:
        explicit Person(const std::string& first_name, const std::string& last_name, const char& sex);
        ~Person();

        void save() override;
        void load() override;

        inline void set_first_name(const std::string& first_name) {m_first_name = first_name;}
        inline std::string get_first_name() {return m_first_name;}
        
        inline void set_last_name(const std::string& last_name) {m_last_name = last_name;}
        inline std::string get_last_name() {return m_last_name;}

        inline void set_phone_number(const std::string& phone_number) {m_phone_number = phone_number;}
        inline std::string get_phone_number() {return m_phone_number;}

        inline void set_email(const std::string& email) {m_email = email;}
        inline std::string get_email() {return m_email;}

        inline void set_birth_date(const std::chrono::year_month_day& birth_date) {m_birth_date = birth_date;}
        inline const std::chrono::year_month_day& get_birth_date() {return m_birth_date;}

        inline void set_sex(char sex) {m_sex = sex;}
        inline char get_sex() {return m_sex;}

        inline void set_occupation(const std::string& occupation) {m_occupation = occupation;}
        inline std::string get_occupation() {return m_occupation;}

        inline void set_allergies(const std::vector<std::string>& allergies) {m_allergies = allergies;}
        inline std::vector<std::string> get_allergies() {return m_allergies;}

        inline void set_additional_info(const AdditionalInfos& additional_info) {m_additional_info = additional_info;}
        inline AdditionalInfos get_additional_info() {return m_additional_info;}

    };

    class PersonBook
    {
    private:        
        PersonBook() = default;
        ~PersonBook();

        std::vector<std::shared_ptr<Person>> m_persons;

        static PersonBook* s_instance;
    public:

        inline PersonBook* get_instance() {return s_instance;}

        PersonBook* initPersonBook();
    };
    
    
}
#endif