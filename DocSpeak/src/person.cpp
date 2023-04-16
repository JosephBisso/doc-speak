#include "person.h"

using namespace docspeak;

Person::Person(Type type, const std::string& first_name,const std::string& last_name, const std::string& sex) : m_type(type), m_first_name (first_name), m_last_name(last_name), m_sex(sex)
{
    if (first_name.empty() && last_name.empty()) {
        auto msg = std::format("Either the first or last name should not be empty");
        throw std::invalid_argument (msg.c_str()) ;
    }

}

Person::~Person()
{
}

template<>
void PersonBook::add(std::shared_ptr<Person> element) {
    if (!s_instance)
        s_instance = std::shared_ptr<Book>(new PersonBook);

    for (auto p : s_instance->m_elements) {
        if (p->get_first_name() == element->get_first_name() && p->get_last_name() == element->get_last_name()) {
            auto msg = std::format("{} {} {} already added.", p->get_type_string(), p->get_first_name(), p->get_last_name());
            throw std::invalid_argument (msg.c_str()) ;
        }
    }

    s_instance->m_elements.push_back(std::move(element));
}

bool Person::is_like(const Person& person) const {
    return (!person.m_first_name.empty() && m_first_name == person.m_first_name )||
           ( !person.m_last_name.empty() && m_last_name == person.m_last_name )||
           ( !person.m_phone_number.empty() && m_phone_number == person.m_phone_number )||
           ( !person.m_email.empty() && m_email == person.m_email )||
           ( person.m_birth_date.ok() && m_birth_date == person.m_birth_date )||
           ( !person.m_sex.empty() && m_sex == person.m_sex )||
           ( !person.m_occupation.empty() && m_occupation == person.m_occupation );
}

bool Person::equals(const Person& person) const {
    return get_first_name() == person.get_first_name() &&
                get_last_name() == person.get_last_name() &&
                get_phone_number() == person.get_phone_number() &&
                get_email() == person.get_email() &&
                get_birth_date() == person.get_birth_date() &&
                get_sex() == person.get_sex() &&
                get_occupation() == person.get_occupation() &&
                get_type() == person.get_type();  
}