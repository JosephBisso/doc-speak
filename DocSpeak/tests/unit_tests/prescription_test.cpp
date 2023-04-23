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

        patient1 = PATIENT("Patient1", "BisBi1", "f", insurance1, "52345");

        Prescription::s_template_pdf_path = std::filesystem::current_path() / "..\\DocSpeak\\tests\\assets\\prescription.pdf";
        Prescription::s_assets_path = std::filesystem::current_path() / "..\\DocSpeak\\tests\\assets\\";
        Prescription::s_font_path = std::filesystem::current_path() / "..\\DocSpeak\\tests\\assets\\arial.ttf";

        auto width = 400, heigth = 280;
        Prescription::s_templates["template1"] = {
            {"cost_bearer", new Printer::Text(width/16 + 10, heigth - 15, "cost_bearer", 8)},
            {"patient_name", new Printer::Text(width/16 + 10, heigth - 60, "patient_name", 8)},
            {"insurance_number", new Printer::Text(width/16 + 10, heigth - 100, "insurance_number", 8)},
            {"patient_number", new Printer::Text(width/16 + 10 + 75, heigth - 100, "patient_number", 8)},
            {"doctor_number", new Printer::Text(width/16 + 10 + 75, heigth - 125, "doctor_number", 8)},
            {"date", new Printer::Text(width/16 + 10 + 170, heigth - 125, "date", 8)},
            {"medication_1", new Printer::Text(width/16 + 10, heigth/2 - 22, "medication_1", 8)},
            {"medication_2", new Printer::Text(width/16 + 10, heigth/2 - 45, "medication_2", 8)},
            {"medication_3", new Printer::Text(width/16 + 10, heigth/2 - 80, "medication_3", 8)},
            {"med_auf_idem_1", new Printer::Text(10, heigth/2 - 22, "X", 10)},
            {"med_auf_idem_2", new Printer::Text(10, heigth/2 - 45, "X", 10)},
            {"med_auf_idem_3", new Printer::Text(10, heigth/2 - 70, "X", 10)},
            {"charge_type_free", new Printer::Text(10,  heigth - 20, "X", 10)},
            {"charge_type_apply", new Printer::Text(10, heigth - 35, "X", 10)},
            {"charge_type_nigth", new Printer::Text(10, heigth - 55, "X", 10)},
            {"charge_type_others", new Printer::Text(10,heigth - 80, "X", 10)},
            {"accident", new Printer::Text(10, heigth - 105  , "X", 10)},
            {"accident_work", new Printer::Text(10, heigth - 130, "X", 10)},
            {"accident_employer", new Printer::Text(10 + 100, 50, "accident_employer", 8)},
            {"accident_date", new Printer::Text(10, 50, "accident_date", 8)},
            {"stamp", new Printer::Image((3/4) * width, (1/2)*heigth, "path", 50, 50)},
            {"doctor_signature", new Printer::Image(-1,0, "path", 200, 100)}

        };

        Prescription::s_current_template_name = "template1";
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

    std::string medication1 = "Medication 123 10 Tabl.  1x day, 1 Tabl.";
    std::string medication2 = "Medication 456 5 Tabl.  1x 2days, 1 Tabl.";
    std::string medication3 = "Medication 678 2 Tabl.  1x day, 1 Tabl.";
};

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

TEST_F(PrescriptionTest, PrintPrescriptionToPDF) {
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now);
    auto record = RECORD(timestamp, patient1);

    auto prescription = PRESCRIPTION(medication1, record);

    (*prescription) << medication2;
        
    EXPECT_EQ(RecordBook::size(), 1); 
    EXPECT_EQ(PrescriptionBook::size(), 1); 

    EXPECT_EQ(prescription -> get_num_of_medications(), 2); 

    prescription -> set_auf_idem(2);
    prescription -> set_cost_bearer(patient1->get_health_insurance().name);

    auto print_status = prescription -> print();
    EXPECT_TRUE(print_status.success); 

}

