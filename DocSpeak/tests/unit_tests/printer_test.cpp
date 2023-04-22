#include <printer.h>

#include <gtest/gtest.h>

using namespace docspeak;

// The fixture for testing class Foo.
class PrinterTest : public ::testing::Test {
 protected:

  PrinterTest(): printer(new Printer) {
     // You can do set-up work for each test here.
  }

  ~PrinterTest() override {
     // You can do clean-up work that doesn"t throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
      auto font_set = printer -> set_font_path(assets_path/"arial.ttf");
      EXPECT_TRUE(font_set.success); 

      auto assets_set = printer -> set_assets_folder_path(assets_path);
      EXPECT_TRUE(assets_set.success); 
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  std::shared_ptr<Printer> printer;
  std::filesystem::path assets_path = std::filesystem::current_path() /= "..\\DocSpeak\\tests\\assets\\";
};

TEST_F(PrinterTest, OpenPDFFile) {

    auto input_path = assets_path / "blank.pdf";
    auto input_path_set = printer -> set_input_path(input_path);
    EXPECT_TRUE(input_path_set.success); 

    auto output_path = std::filesystem::current_path() / "OpenPDFFile.pdf";
    auto output_path_set = printer -> set_output_path(output_path);
    EXPECT_TRUE(output_path_set.success); 

    auto test_status = printer -> test();
    EXPECT_TRUE(test_status.success); 

    EXPECT_TRUE(std::filesystem::exists(output_path)); 

}

