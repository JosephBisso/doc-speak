#include "serializable.h"

using namespace docspeak;

int Serializable::number_of_elements = 0;

Serializable::Serializable()
{
    m_id = ++number_of_elements;
}

Serializable::~Serializable()
{
}
