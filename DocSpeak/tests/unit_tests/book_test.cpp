#include <patient.h>
#include <record.h>

#include <gtest/gtest.h>

using namespace docspeak;
using namespace std::chrono_literals;

// Demonstrate some basic assertions.
TEST(BookTest, Add_Patient_Doctor_Records) {
  auto doctor = DOCTOR("joseph", "King", 'm', "012345");

  auto insurance = docspeak::Insurance {"BARMER", "467", Insurance::PUBLIC};
  auto patient = PATIENT("Loic", "BisBi", 'f', insurance, "52345");
  auto patient2 = PATIENT("Loic2", "BisBi2", 'f', insurance, "52345e");

  for (auto i : {1,2}) {
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now + i*1h);
    RECORD(timestamp, patient);
  }

  auto now = std::chrono::system_clock::now();
  auto timestamp = std::chrono::system_clock::to_time_t(now);
  RECORD(timestamp, patient2);
  
  EXPECT_EQ(Book<Doctor>::size(), 1);

  EXPECT_EQ(Book<Patient>::size(), 2);

  EXPECT_EQ(Book<Record>::size(), 3);

  EXPECT_EQ(patient -> get_records_size(), 2);

  DoctorBook::clear_book();
  PatientBook::clear_book();
  RecordBook::clear_book();

}