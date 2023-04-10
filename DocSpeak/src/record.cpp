#include "record.h"

using namespace docspeak;

Record::Record(std::time_t timestamp) : m_timestamp(timestamp)
{
    auto book = Book<Record>::get_book();

    if (auto book_observer = book.lock()){
        book_observer -> add(std::shared_ptr<Record>(this));
    } else {
        auto msg = std::format("The book cannot be accessed");
        throw std::invalid_argument(msg);
    }
}

Record::~Record()
{
}

template<>
void Book<Record>::add(std::shared_ptr<Record> element) {
    for (auto el : m_elements) {
        if (auto tp = el->get_timestamp(); tp == element->get_timestamp()) {
            auto msg = std::format("Record with identical time point already added.");
            throw std::invalid_argument (msg.c_str()) ;
        }
    }

    m_elements.push_back(element);
}

