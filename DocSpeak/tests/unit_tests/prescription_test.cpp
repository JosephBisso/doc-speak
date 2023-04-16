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

