#include "serializable.h"

using namespace docspeak;

int Serializable::number_of_elements = 0;

Serializable::Serializable()
{
    m_id = ++number_of_elements;
    s_serializables[m_id] = this;
}

Serializable::~Serializable()
{
    std::erase_if(s_serializables, [&](auto serializable) {
        return serializable.first == this->m_id;
    });   
}

void Serializable::save_all(const std::filesystem::path& path) {
    std::for_each(
        s_serializables.cbegin(), s_serializables.cend(), [&path](auto serializable) {
            serializable.second -> save(path);
        }
    );
}




