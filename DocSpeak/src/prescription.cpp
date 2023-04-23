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
    auto printer = get_printer();
    auto file_name = std::format("prescription_{}.pdf", get_id());
    auto output_set = printer -> set_output_path(get_output_folder()/file_name);
    if (!output_set.success) {
        return Printer::StatusInfo(false, output_set.error_message);
    }

    auto current_template = Prescription::get_current_template();
    
    dynamic_cast<Printer::Text*>(current_template["cost_bearer"]) -> text = m_cost_bearer;
    dynamic_cast<Printer::Text*>(current_template["num_of_medications"]) -> text = std::to_string(m_num_of_medications);
    dynamic_cast<Printer::Text*>(current_template["charge_type_free"]) -> y = m_charge_type || ChargeType::Free ? 10 : -1;
    dynamic_cast<Printer::Text*>(current_template["charge_type_apply"]) -> y = m_charge_type || ChargeType::Apply ? 15 : -1;
    dynamic_cast<Printer::Text*>(current_template["charge_type_nigth"]) -> y = m_charge_type || ChargeType::Nigth ? 20 : -1;
    dynamic_cast<Printer::Text*>(current_template["charge_type_others"]) -> y = m_charge_type || ChargeType::Others ? 25 : -1;
    
    if (m_accident.accident_type != Accident::None) {
        dynamic_cast<Printer::Text*>(current_template["accident"]) -> y = 50;
        dynamic_cast<Printer::Text*>(current_template["accident_work"]) -> y = m_accident.accident_type == Accident::Work ? 100 : -1;
        dynamic_cast<Printer::Text*>(current_template["accident_employer"]) -> text = m_accident.employer;
        dynamic_cast<Printer::Text*>(current_template["accident_date"]) -> text = std::format("{}", m_accident.accident_date);
    } else {
        dynamic_cast<Printer::Text*>(current_template["accident"]) -> y = -1;
        dynamic_cast<Printer::Text*>(current_template["accident_work"]) -> y = -1;
        dynamic_cast<Printer::Text*>(current_template["accident_employer"]) -> y = -1;
        dynamic_cast<Printer::Text*>(current_template["accident_date"]) -> y = -1;
    }

    for (auto i = 0; i < m_medications.size(); i++) {
        dynamic_cast<Printer::Text*>(current_template[std::format("medication_{}", i + 1)]) -> y = 200;
        dynamic_cast<Printer::Text*>(current_template[std::format("medication_{}", i + 1)]) -> text = m_medications[i];
        dynamic_cast<Printer::Text*>(current_template[std::format("med_auf_idem_{}", i + 1)]) -> y = m_med_auf_idem[i] ? 200 : -1;
    }

    for (auto [name, element] : current_template) {
        (*printer) << *element;
    }

    return printer -> print();

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

template<> 
std::map<std::string, Protocol<Prescription>::Template> Protocol<Prescription>::s_templates = {
    {"default", {
            {"cost_bearer", new Printer::Text(0, 0, "cost_bearer", 14)},
            {"num_of_medications", new Printer::Text(0, 0, "num_of_medications", 14)},
            {"charge_type_free", new Printer::Text(0, 0, "X", 20)},
            {"charge_type_apply", new Printer::Text(0, 0, "X", 20)},
            {"charge_type_nigth", new Printer::Text(0, 0, "X", 20)},
            {"charge_type_apply", new Printer::Text(0, 0, "X", 20)},
            {"charge_type_others", new Printer::Text(0, 0, "X", 20)},
            {"accident", new Printer::Text(0, 0, "X", 20)},
            {"accident_work", new Printer::Text(0, 0, "X", 20)},
            {"accident_employer", new Printer::Text(0, 0, "accident_employer", 14)},
            {"accident_date", new Printer::Text(0, 0, "accident_date", 14)},
            {"medication_1", new Printer::Text(0, 0, "medication_1", 14)},
            {"medication_2", new Printer::Text(0, -1, "medication_2", 14)},
            {"medication_3", new Printer::Text(0, -1, "medication_3", 14)},
            {"med_auf_idem_1", new Printer::Text(0, -1, "X", 20)},
            {"med_auf_idem_2", new Printer::Text(0, -1, "X", 20)},
            {"med_auf_idem_3", new Printer::Text(0, -1, "X", 20)},
            // {"stamp", new Printer::Image(0,0, "path", 200, 100)},
            // {"doctor_signature", new Printer::Image(0,0, "path", 200, 100)}
        }
    }
};