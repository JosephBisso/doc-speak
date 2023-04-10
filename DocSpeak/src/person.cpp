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

    auto book = Book<Person>::get_book();

    if (auto book_observer = book.lock()){
        book_observer -> add(std::shared_ptr<Person>(this));
    } else {
        auto msg = std::format("The book cannot be accessed");
        throw std::invalid_argument(msg);
    }

}

Person::~Person()
{
}

template<>
void Book<Person>::add(std::shared_ptr<Person> element) {
    for (auto p : m_elements) {
        if (p->get_first_name() == element->get_first_name() && p->get_last_name() == element->get_last_name()) {
            auto msg = std::format("{} {} {} already added.", p->get_type_string(), p->get_first_name(), p->get_last_name());
            throw std::invalid_argument (msg.c_str()) ;
        }
    }

    m_elements.push_back(element);
}
