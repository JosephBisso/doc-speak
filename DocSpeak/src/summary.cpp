#include "summary.h"

using namespace docspeak;

Summary::Summary(): Protocol()
{
}

Summary::~Summary()
{
}

// Printer::StatusInfo Summary::print() {

// }

std::string Summary::to_string() {
    return "";
}

template<>
std::map<std::string, Protocol<Summary>::Template> Protocol<Summary>::s_templates;