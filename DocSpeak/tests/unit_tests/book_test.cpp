#include <patient.h>
#include <record.h>

#include <gtest/gtest.h>

using namespace docspeak;
using namespace std::chrono_literals;

// The fixture for testing class Foo.
class BookTest : public ::testing::Test {
 protected:

  BookTest() {
     // You can do set-up work for each test here.
    
  }

  ~BookTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
    doctor1 = DOCTOR("Doctor1", "King1", 'm', "012345");
    doctor2 = DOCTOR("Doctor2", "King2", 'f', "456");

    patient1 = PATIENT("Patient1", "BisBi1", 'f', insurance1, "52345");
    patient2 = PATIENT("Patient2", "BisBi2", 'm', insurance2, "2345");
    patient3 = PATIENT("Patient3", "BisBi3", 'f', insurance3, "689");
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
      DoctorBook::clear_book();
      PatientBook::clear_book();
      RecordBook::clear_book();
  }

  // Class members declared here can be used by all tests in the test suite
  std::shared_ptr<Doctor> doctor1;
  std::shared_ptr<Doctor> doctor2;

  std::shared_ptr<Patient> patient1;
  std::shared_ptr<Patient> patient2;
  std::shared_ptr<Patient> patient3;

  Insurance insurance1 {"Insurance1", "467", Insurance::PUBLIC};
  Insurance insurance2  {"Insurance2", "567", Insurance::PRIVATE};
  Insurance insurance3  {"Insurence3", "4673", Insurance::PUBLIC};
};

TEST_F(BookTest, CheckBookSize) {

  EXPECT_EQ(Book<Doctor>::size(), 2); 
  EXPECT_EQ(Book<Patient>::size(), 3); 

}

// Demonstrate some basic assertions.
TEST_F(BookTest, CheckBookSizeWithRecords) {

  for (auto i : {1,2}) {
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now + i*1h);
    RECORD(timestamp, patient1);
  }

  auto now = std::chrono::system_clock::now();
  auto timestamp = std::chrono::system_clock::to_time_t(now);
  RECORD(timestamp, patient2);
  
  EXPECT_EQ(Book<Doctor>::size(), 2);

  EXPECT_EQ(Book<Patient>::size(), 3);

  EXPECT_EQ(Book<Record>::size(), 3);

  EXPECT_EQ(patient1 -> get_records_size(), 2);

}

TEST_F(BookTest, SearchDoctorPatientAndRecord) {
  time_t times[3];

  for (auto i : std::views::iota(0, 3)) {
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now + i*1h);
    RECORD(timestamp, patient1, doctor1);
    times[i] = timestamp;
  }

  RECORD(times[1], patient2, doctor2);

  
  EXPECT_EQ(Book<Doctor>::size(), 2);

  EXPECT_EQ(Book<Patient>::size(), 3);

  EXPECT_EQ(Book<Record>::size(), 4);

  EXPECT_EQ(patient1 -> get_records_size(), 3);


  std::shared_ptr<Doctor> d1 (new Doctor("Doctor1", "", 'd', ""));

  EXPECT_EQ(Book<Doctor>::find(*d1).size(), 1);

  EXPECT_EQ(Book<Patient>::find(Patient("Patient1", "", 'm', insurance1, "")).size(), 2);

  auto record1 = Record(times[1]);
  EXPECT_EQ(Book<Record>::find(record1).size(), 2);

  auto record2 = Record(0);
  record2.set_doctor(d1);

  EXPECT_EQ(Book<Record>::find(record2).size(), 3);

}