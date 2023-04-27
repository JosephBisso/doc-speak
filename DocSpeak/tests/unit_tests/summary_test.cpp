#include <patient.h>
#include <record.h>

#include <gtest/gtest.h>

using namespace docspeak;
using namespace std::chrono_literals;

// The fixture for testing class Foo.
class SummaryTest : public ::testing::Test {
protected:

    SummaryTest() {
        // You can do set-up work for each test here.
        
    }

    ~SummaryTest() override {
        // You can do clean-up work that doesn"t throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).

        Summary::s_template_pdf_path = std::filesystem::current_path() / "..\\DocSpeak\\tests\\assets\\report.pdf";
        Summary::s_assets_path = std::filesystem::current_path() / "..\\DocSpeak\\tests\\assets\\";
        Summary::set_output_folder(std::filesystem::current_path() / ".\\output\\");
        Summary::s_font_path = std::filesystem::current_path() / "..\\DocSpeak\\tests\\assets\\arial.ttf";

        auto new_template = Summary::new_template();
        auto width = 538.56 / 2, heigth = 792.0/2;
        new_template.second["reason_of_consultation"] = TemplateElement(Summary::SummaryElement::REASON_OF_CONSULTATION, 10, heigth/2 - 50, "reason_of_consultation", 10);
        new_template.second["diagnosis"] = TemplateElement(Summary::SummaryElement::DIAGNOSIS, 10, 7*heigth/8 - 10 , "diagnosis", 10);
        new_template.second["findings"] = TemplateElement(Summary::SummaryElement::FINDINGS, 10, 7*heigth/8 - 50, "findings", 10);
        new_template.second["doctor_number"] = TemplateElement(Summary::SummaryElement::DOCTOR_NUMBER, 400/16 + 10 + 75, 280 - 125, "doctor_number", 8);
        new_template.second["patient_number"] = TemplateElement(Summary::SummaryElement::PATIENT_NUMBER, 400/16 + 10 + 75, 280 - 125, "patient_number", 8);
        new_template.second["date"] = TemplateElement(Summary::SummaryElement::DATE, width/2, 7*heigth/8, "date", 10);
        new_template.second["stamp"] = TemplateElement(Summary::SummaryElement::STAMP, (3*width)/4 , 100, "path", 75, TemplateElement::Type::Image, 75);
        new_template.second["doctor_signature"] = TemplateElement(Summary::SummaryElement::DOCTOR_SIGNATURE, (3*400)/4 + 50 , 280/2 - 50, "path", 200, TemplateElement::Type::Image, 10);
        new_template.second["page_dimensions"] = TemplateElement(197, 280);
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    // Class members declared here can be used by all tests in the test suite

    std::string reason1 = "Reason1: Reason1 has been given by the patient";
    std::string reason2 = "Reason2: Reason2 also has been given by the patient. This is on more Reason";
    std::string reason3 = "Reason3: Reason3 also has been given by the patient. This is on more Reason on top of the last one";

    std::string diagnosis1 = "diagnosis1: diagnosis1 has been given by the doctor";
    std::string diagnosis2 = "diagnosis2: diagnosis2 also has been given by the doctor. This is on more diagnosis";
    std::string diagnosis3 = "diagnosis3: diagnosis3 also has been given by the doctor. This is on more diagnosis on top of the last one";

    std::string finding1 = "finding1: finding1 has been found by the doctor";
    std::string finding2 = "finding2: finding2 also has been found by the doctor. This is on more finding";
    std::string finding3 = "finding3: finding3 also has been found by the doctor. This is on more finding on top of the last one";
};


TEST_F(SummaryTest, PrintReportPDF) {
    Summary summary;

    summary.add_reason_of_consultation(reason1);
    summary.add_reason_of_consultation(reason2);
    summary.add_reason_of_consultation(reason3);
    
    summary.add_diagnosis(diagnosis1);
    summary.add_diagnosis(diagnosis2);
    summary.add_diagnosis(diagnosis3);

    summary.add_findings(finding1);
    summary.add_findings(finding2);
    summary.add_findings(finding3);

    summary.set_doctor_number("D1234");
    summary.set_patient_number("P4564");
    summary.set_date(toString(std::chrono::January/12/2010));

    auto print_status = summary.print();
    EXPECT_TRUE(print_status.success); 
}

