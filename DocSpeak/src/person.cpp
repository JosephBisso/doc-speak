#include "person.h"

using namespace docspeak;

Person::Person(Type type, const std::string& first_name,const std::string& last_name, char sex) : m_type(type), m_first_name (first_name), m_last_name(last_name), m_sex(sex)
{
    if (sex != 'm' && sex != 'f') {
        auto msg = std::format("Sex can either be 'm' or 'f', not {}", sex);
        throw std::invalid_argument (msg.c_str()) ;
    }

    if (first_name.empty() && last_name.empty()) {
        auto msg = std::format("Either the first or last name should not be empty");
        throw std::invalid_argument (msg.c_str()) ;
    }
}

Person::~Person()
{
}


PersonBook* PersonBook::s_instance = nullptr;

PersonBook* PersonBook::getPersonBook() {
    if (!s_instance) 
        s_instance = new PersonBook();
    
    return s_instance;
}

PersonBook::~PersonBook() {
    delete s_instance;
    s_instance = nullptr;
}

void PersonBook::addPerson(std::shared_ptr<Person> person) {
    for (auto p : m_persons) {
        if (p->get_first_name() == person->get_first_name() && p->get_last_name() == person->get_last_name()) {
            auto msg = std::format("{} {} {} already added.", p->get_type_string(), p->get_first_name(), p->get_last_name());
            throw std::invalid_argument (msg.c_str()) ;
        }
    }

    m_persons.push_back(person);
}

void PersonBook::save(const std::filesystem::path&){
    
}
void PersonBook::load(const std::filesystem::path&){

}

// std::vector<std::shared_ptr<Person>> PersonBook::getPersons(const Person& filterPerson) {
//     auto filtered =  m_persons | std::views::filter([&filterPerson](std::shared_ptr<Person> person){ 
//             return person -> get_first_name() == filterPerson.get_first_name() ||
//                     person -> get_last_name() == filterPerson.get_last_name() ||
//                     person -> get_sex() == filterPerson.get_sex() ||
//                     person -> get_type() == filterPerson.get_type();
//             });
    
// }
