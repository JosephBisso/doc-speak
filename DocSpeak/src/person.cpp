#include "person.h"

using namespace docspeak;

Person::Person(const std::string& first_name,const std::string& last_name, const char& sex) : m_first_name (first_name), m_last_name(last_name), m_sex(sex)
{
}

Person::~Person()
{
}

void Person::save() {
    std::cout << "Saving Person";
}

void Person::load() {
    std::cout << "Loading Person";
}

PersonBook* PersonBook::s_instance = nullptr;

PersonBook* PersonBook::initPersonBook() {
    if (!s_instance) 
        s_instance = new PersonBook();
    
    return s_instance;
}

PersonBook::~PersonBook() {
    delete s_instance;
    s_instance = nullptr;
}
