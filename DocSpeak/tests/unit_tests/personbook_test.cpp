#include <doctor.h>
#include <patient.h>
#include <gtest/gtest.h>

using namespace docspeak;

TEST(PersonBookTest, Add_Doctor) {

  auto actual_size = Book<Person>::size();
  auto doctor = std::make_shared<Doctor>("joseph2", "King2", 'm', "0123452");

  auto insurance = docspeak::Insurance {"BARMER", "467", Insurance::PUBLIC};
  auto patient = std::make_shared<Patient>("Loic22", "BisBi22", 'f', insurance, "5234522");

  EXPECT_EQ(Book<Person>::size(), actual_size+2); 

}