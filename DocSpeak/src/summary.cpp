#include "summary.h"

using namespace docspeak;

Summary::Summary(): Protocol()
{
}

Summary::~Summary()
{
}

Printer::StatusInfo Summary::print() {
    auto& printer = *get_printer();
    auto file_name = std::format("report_{}_Patient{}.pdf", m_date, m_patient_number);
    auto output_set = printer.set_output_path(get_output_folder()/file_name);
    if (!output_set.success) {
        return Printer::StatusInfo(false, output_set.error_message);
    }


    auto current_template = Summary::get_current_template();

    for (auto [name, element] : current_template) {
        switch (element.element_id)
        {
        case SummaryElement::REASON_OF_CONSULTATION:
            printer << Printer::Text (element.x, element.y, m_reason_of_consultation, element.width_or_size, element.color); 
            break;
        case SummaryElement::DIAGNOSIS:
            printer << Printer::Text (element.x, element.y, m_diagnosis, element.width_or_size, element.color); 
            break;
        case SummaryElement::FINDINGS:
            printer << Printer::Text (element.x, element.y, m_findings, element.width_or_size, element.color); 
            break;
        case SummaryElement::PATIENT_NUMBER:
            printer << Printer::Text (element.x, element.y, m_patient_number, element.width_or_size, element.color); 
            break;
        case SummaryElement::DOCTOR_NUMBER:
            printer << Printer::Text (element.x, element.y, m_doctor_number, element.width_or_size, element.color); 
            break;
        case SummaryElement::DATE:
            printer << Printer::Text (element.x, element.y, m_date, element.width_or_size, element.color); 
            break;
        case SummaryElement::STAMP:
            if (std::filesystem::exists(Summary::s_assets_path / "stamp.jfif"))
                printer << Printer::Image(element.x, element.y, (Summary::s_assets_path / "stamp.jfif").string(), element.width_or_size, element.heigth); 
            break;
        case SummaryElement::DOCTOR_SIGNATURE:
            if (std::filesystem::exists(Summary::s_assets_path / std::format("{}_signature.jfif", m_doctor_number)))
                printer << Printer::Image (element.x, element.y, std::format("{}_signature.jfif", m_doctor_number), element.width_or_size, element.heigth); 
            break;
        
        default:
            if (element.type == TemplateElement::Text) 
                printer << Printer::Text (element.x, element.y, element.text_or_path, element.width_or_size, element.color);
            else if (element.type == TemplateElement::Image) 
                printer << Printer::Image (element.x, element.y, element.text_or_path, element.width_or_size, element.heigth);
            break;
        }
    }

    return printer.print();

}

std::string Summary::to_string() {
    return "";
}

template<> 
std::vector<std::map<std::string, TemplateElement>> Protocol<Summary>::s_templates = {
    {
        {"reason_of_consultation", TemplateElement(Summary::SummaryElement::REASON_OF_CONSULTATION, 400/16 + 10 + 75, 280 - 125, "reason_of_consultation", 8)},
        {"diagnosis", TemplateElement(Summary::SummaryElement::DIAGNOSIS, 400/16 + 10 + 75, 280 - 125, "diagnosis", 8)},
        {"findings", TemplateElement(Summary::SummaryElement::FINDINGS, 400/16 + 10 + 75, 280 - 125, "findings", 8)},
        {"doctor_number", TemplateElement(Summary::SummaryElement::DOCTOR_NUMBER, 400/16 + 10 + 75, 280 - 125, "doctor_number", 8)},
        {"patient_number", TemplateElement(Summary::SummaryElement::PATIENT_NUMBER, 400/16 + 10 + 75, 280 - 125, "patient_number", 8)},
        {"date", TemplateElement(Summary::SummaryElement::DATE, 400/16 + 10 + 170, 280 - 125, "date", 8)},
        {"stamp", TemplateElement(Summary::SummaryElement::STAMP, (3*400)/4 + 50 , 280/2 - 5, "path", 50, TemplateElement::Type::Image, 50)},
        {"doctor_signature", TemplateElement(Summary::SummaryElement::DOCTOR_SIGNATURE, (3*400)/4 + 50 , 280/2 - 50, "path", 200, TemplateElement::Type::Image, 10)}
    }
};
