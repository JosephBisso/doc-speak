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
std::vector<std::map<std::string, TemplateElement>> Protocol<Summary>::s_templates;
