#include <patient.h>
#include <record.h>

#include <gtest/gtest.h>

using namespace docspeak;
using namespace std::chrono_literals;

// Demonstrate some basic assertions.
TEST(SearchTest, Search_Record_Patient_Doctor) {
  auto doctor = DOCTOR("joseph", "King", 'm', "012345");
  auto doctor2 = DOCTOR("joseph2", "King2", 'm', "012345d");

  auto insurance = docspeak::Insurance {"BARMER", "467", Insurance::PUBLIC};
  auto insurance2 = docspeak::Insurance {"BARMERS", "467", Insurance::PUBLIC};
  auto insurance3 = docspeak::Insurance {"SBK", "4673", Insurance::PUBLIC};
  auto patient = PATIENT("Loic", "BisBi", 'f', insurance, "52345");
  auto patient2 = PATIENT("Loic", "BisBi2", 'f', insurance2, "52345e");
  auto patient3 = PATIENT("Loic3", "BisBi4", 'f', insurance3, "52345ed");

  time_t times[3];

  for (auto i : std::views::iota(0, 3)) {
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now + i*1h);
    RECORD(timestamp, patient, doctor);
    times[i] = timestamp;
  }

  RECORD(times[1], patient2, doctor2);

  
  EXPECT_EQ(Book<Doctor>::size(), 2);

  EXPECT_EQ(Book<Patient>::size(), 3);

  EXPECT_EQ(Book<Record>::size(), 4);

  EXPECT_EQ(patient -> get_records_size(), 3);


  std::shared_ptr<Doctor> joseph (new Doctor("joseph", "", 'd', ""));

  EXPECT_EQ(Book<Doctor>::find(*joseph).size(), 1);

  EXPECT_EQ(Book<Patient>::find(Patient("Loic", "", 'm', insurance, "")).size(), 2);

  auto record1 = Record(times[1]);
  EXPECT_EQ(Book<Record>::find(record1).size(), 2);

  auto record2 = Record(0);
  record2.set_doctor(joseph);

  EXPECT_EQ(Book<Record>::find(record2).size(), 3);


  DoctorBook::clear_book();
  PatientBook::clear_book();
  RecordBook::clear_book();

}