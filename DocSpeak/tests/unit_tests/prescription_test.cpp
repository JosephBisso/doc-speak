#include <patient.h>
#include <record.h>

#include <gtest/gtest.h>

using namespace docspeak;
using namespace std::chrono_literals;

// The fixture for testing class Foo.
class PrescriptionTest : public ::testing::Test {
protected:

    PrescriptionTest() {
        // You can do set-up work for each test here.
        
    }

    ~PrescriptionTest() override {
        // You can do clean-up work that doesn"t throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
        doctor1 = DOCTOR("Doctor1", "King1", "m", "012345");

        patient1 = PATIENT("Patient1", "BisBi1", "f", insurance1, "52345", "180", std::chrono::July / 11 / 1991);

        Prescription::s_template_pdf_path = std::filesystem::current_path() / "..\\DocSpeak\\tests\\assets\\prescription.pdf";
        Prescription::set_output_folder(std::filesystem::current_path() / ".\\output\\");
        Prescription::s_assets_path = std::filesystem::current_path() / "..\\DocSpeak\\tests\\assets\\";
        Prescription::s_font_path = std::filesystem::current_path() / "..\\DocSpeak\\tests\\assets\\arial.ttf";

        auto width = 400, heigth = 280;
        auto [index, prescription_template] = Prescription::new_template();
        prescription_template["cost_bearer"] = TemplateElement(Prescription::PrescriptionElement::COST_BEARER, width/16 + 10, heigth - 15, "cost_bearer", 8);
        prescription_template["patient_name"] = TemplateElement(Prescription::PrescriptionElement::PATIENT_NAME, width/16 + 10, heigth - 60, "patient_name", 8);
        prescription_template["patient_birth_date"] = TemplateElement(Prescription::PrescriptionElement::PATIENT_BIRTH_DATE, width/16 + 10 + 170, heigth - 60, "patient_birth_date", 8);
        prescription_template["insurance_number"] = TemplateElement(Prescription::PrescriptionElement::INSURANCE_NUMBER, width/16 + 10, heigth - 100, "insurance_number", 8);
        prescription_template["patient_number"] = TemplateElement(Prescription::PrescriptionElement::PATIENT_NUMBER, width/16 + 10 + 75, heigth - 100, "patient_status", 8);
        prescription_template["patient_status"] = TemplateElement(Prescription::PrescriptionElement::PATIENT_STATUS, width/16 + 10 + 170, heigth - 100, "patient_number", 8);
        prescription_template["doctor_number"] = TemplateElement(Prescription::PrescriptionElement::DOCTOR_NUMBER, width/16 + 10 + 75, heigth - 125, "doctor_number", 8);
        prescription_template["house_number"] = TemplateElement(Prescription::PrescriptionElement::HOUSE_NUMBER, width/16 + 10, heigth - 125, "house_number", 8);
        prescription_template["date"] = TemplateElement(Prescription::PrescriptionElement::DATE, width/16 + 10 + 170, heigth - 125, "date", 8);
        prescription_template["medication_1"] = TemplateElement(Prescription::PrescriptionElement::MEDICATION_1, width/16 + 10, heigth/2 - 22, "medication_1", 8);
        prescription_template["medication_2"] = TemplateElement(Prescription::PrescriptionElement::MEDICATION_2, width/16 + 10, heigth/2 - 45, "medication_2", 8);
        prescription_template["medication_3"] = TemplateElement(Prescription::PrescriptionElement::MEDICATION_3, width/16 + 10, heigth/2 - 68, "medication_3", 8);
        prescription_template["med_auf_idem_1"] = TemplateElement(Prescription::PrescriptionElement::MED_AUF_IDEM_1, 10, heigth/2 - 22, "X", 10);
        prescription_template["med_auf_idem_2"] = TemplateElement(Prescription::PrescriptionElement::MED_AUF_IDEM_2, 10, heigth/2 - 45, "X", 10);
        prescription_template["med_auf_idem_3"] = TemplateElement(Prescription::PrescriptionElement::MED_AUF_IDEM_3, 10, heigth/2 - 68, "X", 10);
        prescription_template["charge_type_free"] = TemplateElement(Prescription::PrescriptionElement::CHARGE_TYPE_FREE, 10,  heigth - 20, "X", 10);
        prescription_template["charge_type_apply"] = TemplateElement(Prescription::PrescriptionElement::CHARGE_TYPE_APPLY, 10, heigth - 35, "X", 10);
        prescription_template["charge_type_nigth"] = TemplateElement(Prescription::PrescriptionElement::CHARGE_TYPE_NIGTH, 10, heigth - 55, "X", 10);
        prescription_template["charge_type_others"] = TemplateElement(Prescription::PrescriptionElement::CHARGE_TYPE_OTHERS, 10,heigth - 80, "X", 10);
        prescription_template["accident"] = TemplateElement(Prescription::PrescriptionElement::ACCIDENT, 10, heigth - 105  , "X", 10);
        prescription_template["accident_work"] = TemplateElement(Prescription::PrescriptionElement::ACCIDENT_WORK, 10, heigth - 130, "X", 10);
        prescription_template["accident_employer"] = TemplateElement(Prescription::PrescriptionElement::ACCIDENT_EMPLOYER, 10 + 75, 17, "accident_employer", 8);
        prescription_template["accident_date"] = TemplateElement(Prescription::PrescriptionElement::ACCIDENT_DATE, 12, 17, "accident_date", 8);
        prescription_template["stamp"] = TemplateElement(Prescription::PrescriptionElement::STAMP, (3*width)/4 + 50 , heigth/2 - 5, "path", 50, TemplateElement::Type::Image, 50);
        prescription_template["doctor_signature"] = TemplateElement(Prescription::PrescriptionElement::DOCTOR_SIGNATURE, (3*width)/4 + 50 , heigth/2 - 50, "path", 200, TemplateElement::Type::Image, 10);

    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
        DoctorBook::clear_book();
        PatientBook::clear_book();
        RecordBook::clear_book();
        PrescriptionBook::clear_book();
    }

    // Class members declared here can be used by all tests in the test suite
    std::shared_ptr<Doctor> doctor1;

    std::shared_ptr<Patient> patient1;

    Insurance insurance1 {"Insurance1", "467", Insurance::PUBLIC};

    std::string medication1 = "Medication 1 10 Tabl.  1x day, 1 Tabl.";
    std::string medication2 = "Medication 2 5 Tabl.  1x 2days, 1 Tabl.";
    std::string medication3 = "Medication 3 2 Tabl.  1x day, 1 Tabl.";
};

TEST_F(PrescriptionTest, CheckChargeType) {
    auto p = Prescription(medication1);
    
    EXPECT_TRUE(p.charge_type() | Prescription::ChargeType::Free);

    auto charge = p.charge_type();

    charge = Prescription::ChargeType::Apply | Prescription::ChargeType::Nigth;

    EXPECT_FALSE(p.charge_type() & Prescription::ChargeType::Free);
    EXPECT_FALSE(p.charge_type() & Prescription::ChargeType::Others);

    EXPECT_TRUE(p.charge_type() & Prescription::ChargeType::Apply);

}

TEST_F(PrescriptionTest, CheckNumOfMedication) {
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now);
    auto record = RECORD(timestamp, patient1);

    auto prescription = PRESCRIPTION(medication1, record);

    (*prescription) << medication2;
        
    EXPECT_EQ(RecordBook::size(), 1); 
    EXPECT_EQ(PrescriptionBook::size(), 1); 

    EXPECT_EQ(prescription -> get_num_of_medications(), 2); 

}

TEST_F(PrescriptionTest, PrescriptionPDFTemplate) {
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now);
    auto record = RECORD(timestamp, patient1, doctor1);

    auto prescription = PRESCRIPTION(medication1, record);

    (*prescription) << medication2 << medication3;
        
    EXPECT_EQ(RecordBook::size(), 1); 
    EXPECT_EQ(PrescriptionBook::size(), 1); 

    EXPECT_EQ(prescription -> get_num_of_medications(), 3); 

    prescription -> set_auf_idem(1);
    prescription -> set_auf_idem(2);
    prescription -> set_auf_idem(3);
    prescription -> set_cost_bearer(patient1->get_health_insurance().name);
    prescription -> set_house_number ("01234");
    prescription -> charge_type() = Prescription::Apply | Prescription::Free | Prescription::Nigth | Prescription::Others;
    prescription -> set_accident({
        .accident_type = Prescription::Accident::Work,
        .employer = "Employer GmbH",
        .accident_date = std::chrono::December / 25 / 2001
    });


    auto print_status = prescription -> print();
    EXPECT_TRUE(print_status.success); 

}

TEST_F(PrescriptionTest, TranscriptPrescriptionAndPrintToPDF) {
#ifdef CI
    GTEST_SKIP();
#endif
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now);
    auto record = RECORD(timestamp, patient1);

    auto prescription = PRESCRIPTION(record);

    auto transcription_status = prescription->transcript();

    EXPECT_TRUE(transcription_status.success);

    auto streamed_medication = prescription -> get_medications();
    auto all_transcriptions = prescription -> get_transcripter() -> get_transcription();


    if (
        std::count_if(all_transcriptions.begin(), all_transcriptions.end(),
        [](auto t){return !t.empty();}) 
        > 0
    ) {

        EXPECT_GT(streamed_medication.size(), 0);

        prescription -> set_auf_idem(1);

        if (streamed_medication.size() < 3) {

            bool all_transcripted_words_present = std::all_of(all_transcriptions.begin(), all_transcriptions.end(), 
                        [&streamed_medication](auto transcripted_word) {
                        
                        if (transcripted_word.empty() || transcripted_word.find("next") != std::string::npos || transcripted_word.find("|end|") != std::string::npos)
                            return true;
                            
                        return std::find_if(streamed_medication.begin(), streamed_medication.end(),
                                        [&transcripted_word](auto medication) {return medication.find(transcripted_word) != std::string::npos;}
                                ) != streamed_medication.end();
            });
            
            EXPECT_TRUE(all_transcripted_words_present);
        }

    }

    prescription -> set_cost_bearer(patient1->get_health_insurance().name);
    prescription -> set_house_number ("01234");
    prescription -> charge_type() = Prescription::Apply;

    auto print_status = prescription -> print();

    EXPECT_TRUE(print_status.success); 

}

