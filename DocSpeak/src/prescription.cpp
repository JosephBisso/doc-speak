#include "prescription.h"

using namespace docspeak;

Prescription::Prescription(const std::string& medication): Protocol()
{
   m_medications[m_num_of_medications++] = medication;
}

Prescription::~Prescription()
{
}

void Prescription::add_medication(const std::string& medication) {
    if (m_num_of_medications < 3) {
        m_medications[m_num_of_medications++] = medication;
        return;
    }

    auto msg = std::format("Cannot add more than 3 medications in a prescription.");
    throw std::invalid_argument (msg.c_str()) ;
}


Printer::StatusInfo Prescription::print() {
    auto& printer = *get_printer();
    auto file_name = std::format("prescription_{}_{}_Patient{}.pdf", get_id(), m_record_info.date, m_record_info.patient_number);
    auto output_set = printer.set_output_path(get_output_folder()/file_name);
    if (!output_set.success) {
        return Printer::StatusInfo(false, output_set.error_message);
    }


    auto current_template = Prescription::get_current_template();
    bool work_accident_occured = m_accident.accident_type == Accident::AccidentType::Work; 

    for (auto [name, element] : current_template) {
        switch (element.element_id)
        {
        case PrescriptionElement::COST_BEARER:
            printer << Printer::Text (element.x, element.y, m_cost_bearer, element.width_or_size, element.color);
            break;
        case PrescriptionElement::PATIENT_NAME:
            printer << Printer::Text (element.x, element.y, m_record_info.patient_name, element.width_or_size, element.color); 
            break;
        case PrescriptionElement::PATIENT_BIRTH_DATE:
            printer << Printer::Text (element.x, element.y, m_record_info.patient_birth_date, element.width_or_size, element.color); 
            break;
        case PrescriptionElement::PATIENT_NUMBER:
            printer << Printer::Text (element.x, element.y, m_record_info.patient_number, element.width_or_size, element.color); 
            break;
        case PrescriptionElement::PATIENT_STATUS:
            printer << Printer::Text (element.x, element.y, m_record_info.patient_status, element.width_or_size, element.color); 
            break;
        case PrescriptionElement::INSURANCE_NUMBER:
            printer << Printer::Text (element.x, element.y, m_record_info.insurance_number, element.width_or_size, element.color); 
            break;
        case PrescriptionElement::HOUSE_NUMBER:
            printer << Printer::Text (element.x, element.y, m_house_number, element.width_or_size, element.color); 
            break;
        case PrescriptionElement::DOCTOR_NUMBER:
            printer << Printer::Text (element.x, element.y, m_record_info.doctor_number, element.width_or_size, element.color); 
            break;
        case PrescriptionElement::DATE:
            printer << Printer::Text (element.x, element.y, m_record_info.date, element.width_or_size, element.color); 
            break;
        case PrescriptionElement::CHARGE_TYPE_FREE:
            printer << Printer::Text (m_charge_type & ChargeType::Free ? element.x : -1, element.y, element.text_or_path, element.width_or_size, element.color); 
            break;
        case PrescriptionElement::CHARGE_TYPE_APPLY:
            printer << Printer::Text (m_charge_type & ChargeType::Apply ? element.x : -1, element.y, element.text_or_path, element.width_or_size, element.color); 
            break;
        case PrescriptionElement::CHARGE_TYPE_NIGTH:
            printer << Printer::Text (m_charge_type & ChargeType::Nigth ? element.x : -1, element.y, element.text_or_path, element.width_or_size, element.color); 
            break;
        case PrescriptionElement::CHARGE_TYPE_OTHERS:
            printer << Printer::Text (m_charge_type & ChargeType::Others ? element.x : -1, element.y, element.text_or_path, element.width_or_size, element.color); 
            break;
        case PrescriptionElement::ACCIDENT:
            printer << Printer::Text (m_accident.accident_type != Accident::AccidentType::None ? element.x : -1, element.y, element.text_or_path, element.width_or_size, element.color); 
            break;
        case PrescriptionElement::ACCIDENT_WORK:
            printer << Printer::Text (work_accident_occured ? element.x : -1, element.y, element.text_or_path, element.width_or_size, element.color); 
            break;
        case PrescriptionElement::ACCIDENT_EMPLOYER:
            if (work_accident_occured)
                printer << Printer::Text (element.x, element.y, m_accident.employer, element.width_or_size, element.color); 
            break;
        case PrescriptionElement::ACCIDENT_DATE:
            if (work_accident_occured)
                printer << Printer::Text (element.x, element.y, docspeak::toString(m_accident.accident_date, Record::s_date_format.c_str()), element.width_or_size, element.color); 
            break;
        case PrescriptionElement::MEDICATION_1:
            if(m_num_of_medications >= 1)
                printer << Printer::Text (element.x, element.y, m_medications[0], element.width_or_size, element.color); 
            break;
        case PrescriptionElement::MEDICATION_2:
            if(m_num_of_medications >= 2)
                printer << Printer::Text (element.x, element.y, m_medications[1], element.width_or_size, element.color); 
            break;
        case PrescriptionElement::MEDICATION_3:
            if(m_num_of_medications >= 3)
                printer << Printer::Text (element.x, element.y, m_medications[2], element.width_or_size, element.color); 
            break;
        case PrescriptionElement::MED_AUF_IDEM_1:
            if(m_med_auf_idem[0])
                printer << Printer::Text (element.x, element.y, element.text_or_path, element.width_or_size, element.color); 
            break;
        case PrescriptionElement::MED_AUF_IDEM_2:
            if(m_med_auf_idem[1])
                printer << Printer::Text (element.x, element.y, element.text_or_path, element.width_or_size, element.color); 
            break;
        case PrescriptionElement::MED_AUF_IDEM_3:
            if(m_med_auf_idem[2])
                printer << Printer::Text (element.x, element.y, element.text_or_path, element.width_or_size, element.color); 
            break;
        case PrescriptionElement::STAMP:
            if (std::filesystem::exists(Prescription::s_assets_path / "stamp.jfif"))
                printer << Printer::Image(element.x, element.y, (Prescription::s_assets_path / "stamp.jfif").string(), element.width_or_size, element.heigth); 
            break;
        case PrescriptionElement::DOCTOR_SIGNATURE:
            if (std::filesystem::exists(Prescription::s_assets_path / std::format("{}_signature.jfif", m_record_info.doctor_number)))
                printer << Printer::Image (element.x, element.y, std::format("{}_signature.jfif", m_record_info.doctor_number), element.width_or_size, element.heigth); 
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

std::string Prescription::to_string() {
    std::string result;
    std::for_each(m_medications.begin(), m_medications.end(), [&result](auto medication) {
        result += medication + ";";
    });

    return result;
}

bool Prescription::is_like (const Prescription& prescription) const {
    return std::any_of(prescription.m_medications.begin(), prescription.m_medications.end(), [&](auto m) {
        return this -> contains (m);
    });
}

bool Prescription::contains(const std::string& medication) const{
    return std::any_of(m_medications.begin(), m_medications.end(), [&medication](auto m) {
        return !m.empty() && m == medication;
    });
}


template<>
void Book<Prescription>::add(std::shared_ptr<Prescription> element) {
    if (!s_instance)
        s_instance = std::shared_ptr<Book>(new PrescriptionBook);

    if (element -> get_medications().size() == 0) {
        auto msg = std::format("A Prescription must contain at least one medication");
        throw std::invalid_argument (msg.c_str()) ;
    }

    s_instance -> m_elements.push_back(std::move(element));
}

std::shared_ptr<Prescription> docspeak::PRESCRIPTION (const std::string& medication, std::shared_ptr<Record> record) {
    std::shared_ptr<Prescription> prescription (new Prescription(medication));
    
    std::string doctor_number;
    if (auto doctor_observer = record->get_doctor(); auto doctor = doctor_observer.lock()) 
        doctor_number = doctor->get_doctor_number();


    if (auto patient_observer = record->get_patient(); auto patient = patient_observer.lock()) {
        prescription -> set_record_info({
            .patient_name = patient->get_full_name(),
            .patient_number = patient->get_insurance_number(),
            .patient_birth_date = toString(patient->get_birth_date(), Record::s_date_format.c_str()),
            .patient_status = patient->get_status(),
            .insurance_number = patient->get_health_insurance().number,
            .doctor_number = doctor_number,
            .date = toString(record->get_timestamp(), Record::s_date_format.c_str())
        });
    }

    PrescriptionBook::add(prescription);

    record -> set_prescription(prescription);

    return prescription;
}

template<> 
std::vector<std::map<std::string, TemplateElement>> Protocol<Prescription>::s_templates = {
    {
        {"cost_bearer", TemplateElement(Prescription::PrescriptionElement::COST_BEARER, 400/16 + 10, 280 - 15, "cost_bearer", 8)},
        {"patient_name", TemplateElement(Prescription::PrescriptionElement::PATIENT_NAME, 400/16 + 10, 280 - 60, "patient_name", 8)},
        {"patient_birth_date", TemplateElement(Prescription::PrescriptionElement::PATIENT_BIRTH_DATE, 400/16 + 10 + 170, 280 - 60, "patient_birth_date", 8)},
        {"insurance_number", TemplateElement(Prescription::PrescriptionElement::INSURANCE_NUMBER, 400/16 + 10, 280 - 100, "insurance_number", 8)},
        {"patient_number", TemplateElement(Prescription::PrescriptionElement::PATIENT_NUMBER, 400/16 + 10 + 75, 280 - 100, "patient_status", 8)},
        {"patient_status", TemplateElement(Prescription::PrescriptionElement::PATIENT_STATUS, 400/16 + 10 + 170, 280 - 100, "patient_number", 8)},
        {"doctor_number", TemplateElement(Prescription::PrescriptionElement::DOCTOR_NUMBER, 400/16 + 10 + 75, 280 - 125, "doctor_number", 8)},
        {"house_number", TemplateElement(Prescription::PrescriptionElement::HOUSE_NUMBER, 400/16 + 10, 280 - 125, "house_number", 8)},
        {"date", TemplateElement(Prescription::PrescriptionElement::DATE, 400/16 + 10 + 170, 280 - 125, "date", 8)},
        {"medication_1", TemplateElement(Prescription::PrescriptionElement::MEDICATION_1, 400/16 + 10, 280/2 - 22, "medication_1", 8)},
        {"medication_2", TemplateElement(Prescription::PrescriptionElement::MEDICATION_2, 400/16 + 10, 280/2 - 45, "medication_2", 8)},
        {"medication_3", TemplateElement(Prescription::PrescriptionElement::MEDICATION_3, 400/16 + 10, 280/2 - 68, "medication_3", 8)},
        {"med_auf_idem_1", TemplateElement(Prescription::PrescriptionElement::MED_AUF_IDEM_1, 10, 280/2 - 22, "X", 10)},
        {"med_auf_idem_2", TemplateElement(Prescription::PrescriptionElement::MED_AUF_IDEM_2, 10, 280/2 - 45, "X", 10)},
        {"med_auf_idem_3", TemplateElement(Prescription::PrescriptionElement::MED_AUF_IDEM_3, 10, 280/2 - 68, "X", 10)},
        {"charge_type_free", TemplateElement(Prescription::PrescriptionElement::CHARGE_TYPE_FREE, 10,  280 - 20, "X", 10)},
        {"charge_type_apply", TemplateElement(Prescription::PrescriptionElement::CHARGE_TYPE_APPLY, 10, 280 - 35, "X", 10)},
        {"charge_type_nigth", TemplateElement(Prescription::PrescriptionElement::CHARGE_TYPE_NIGTH, 10, 280 - 55, "X", 10)},
        {"charge_type_others", TemplateElement(Prescription::PrescriptionElement::CHARGE_TYPE_OTHERS, 10,280 - 80, "X", 10)},
        {"accident", TemplateElement(Prescription::PrescriptionElement::ACCIDENT, 10, 280 - 105  , "X", 10)},
        {"accident_work", TemplateElement(Prescription::PrescriptionElement::ACCIDENT_WORK, 10, 280 - 130, "X", 10)},
        {"accident_employer", TemplateElement(Prescription::PrescriptionElement::ACCIDENT_EMPLOYER, 10 + 75, 17, "accident_employer", 8)},
        {"accident_date", TemplateElement(Prescription::PrescriptionElement::ACCIDENT_DATE, 12, 17, "accident_date", 8)},
        {"stamp", TemplateElement(Prescription::PrescriptionElement::STAMP, (3*400)/4 + 50 , 280/2 - 5, "path", 50, TemplateElement::Type::Image, 50)},
        {"doctor_signature", TemplateElement(Prescription::PrescriptionElement::DOCTOR_SIGNATURE, (3*400)/4 + 50 , 280/2 - 50, "path", 200, TemplateElement::Type::Image, 10)},
        {"page_dimensions", TemplateElement(400, 280)}
    }
};
