#include "record.h"

using namespace docspeak;

Record::Record(std::time_t timestamp) : m_timestamp(timestamp)
{
}

Record::~Record()
{
}

void Record::save(const std::filesystem::path& path) {
    std::cout << "Saving Record";
}

void Record::load(const std::filesystem::path& path) {
    std::cout << "Loading Record";
}
