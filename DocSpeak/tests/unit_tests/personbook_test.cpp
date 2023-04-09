#include <doctor.h>
#include <patient.h>
#include <gtest/gtest.h>

using namespace docspeak;

TEST(PersonBookTest, AddPatientAndDoctor) {
  auto person_book = PersonBook::getPersonBook();
  person_book->clearPersons();

  auto doctor = std::make_shared<Doctor>("joseph", "King", 'm', "012345");

  auto insurance = docspeak::Insurance {"BARMER", "467", Insurance::PUBLIC};
  auto patient = std::make_shared<Patient>("Loic", "BisBi", 'f', insurance, "52345");


  person_book->addPerson(doctor);
  person_book->addPerson(patient);
  
  auto persons = person_book -> getPersons();
  EXPECT_EQ(persons.size(), 2);
}