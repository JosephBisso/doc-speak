#include <doctor.h>
#include <patient.h>
#include <gtest/gtest.h>

using namespace docspeak;

TEST(PersonBookTest, Add_Doctor) {

  auto doctor = DOCTOR("joseph2", "King2", 'm', "0123452");

  auto insurance = docspeak::Insurance {"BARMER", "467", Insurance::PUBLIC};
  auto patient = PATIENT("Loic22", "BisBi22", 'f', insurance, "5234522");

  EXPECT_EQ(Book<Doctor>::size(), 1); 
  EXPECT_EQ(Book<Patient>::size(), 1); 

  DoctorBook::clear_book();
  PatientBook::clear_book();

}