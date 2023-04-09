#include <patient.h>
#include <record.h>

#include <gtest/gtest.h>

using namespace docspeak;

// Demonstrate some basic assertions.
TEST(PatientTest, AddRecords) {
  auto person_book = PersonBook::getPersonBook();
  person_book->clearPersons();

  auto doctor = std::make_shared<Doctor>("joseph", "King", 'm', "012345");

  auto insurance = docspeak::Insurance {"BARMER", "467", Insurance::PUBLIC};
  auto patient = std::make_shared<Patient>("Loic", "BisBi", 'f', insurance, "52345");
  person_book->addPerson(doctor);
  person_book->addPerson(patient);

  for (auto i : {1,2}) {
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now);
    auto record = std::make_shared<Record>(timestamp);
    patient->add_record(record);
  }
  
  auto persons = person_book -> getPersons();
  EXPECT_EQ(persons.size(), 2);

  auto records = patient -> get_records();
  EXPECT_EQ(records.size(), 2);

}