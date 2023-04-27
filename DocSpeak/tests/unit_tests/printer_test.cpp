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

TEST_F(PrinterTest, CreateTestPDF) {

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

TEST_F(PrinterTest, PrintPDFFromJob) {

   auto input_path = assets_path / "blank.pdf";
   auto input_path_set = printer -> set_input_path(input_path);
   EXPECT_TRUE(input_path_set.success); 

   auto output_path = std::filesystem::current_path() / "PrintPDFFromJob.pdf";
   auto output_path_set = printer -> set_output_path(output_path);
   EXPECT_TRUE(output_path_set.success); 

   Printer::Text title (Printer::x_mid, Printer::y_top, "DocSpeak Print Start", 20);
   title.adjust_to_middle = true;
   (*printer) << title;

   int yPos = Printer::y_top - Printer::empty_line;

   (*printer) << Printer::Text (Printer::x_left, yPos, "Line Block", 18)
              << Printer::Text (Printer::x_left, yPos - Printer::line_break, "Line 1, Size 12, Color Black", 12)
              << Printer::Text (Printer::x_left, yPos - 2* Printer::line_break, "Line 3, Size 12, Color Red", 12, 0xc4101c)
              << Printer::Text (Printer::x_left, yPos - 3* Printer::line_break, "Line 3, Size 12, Color Green", 12, 0x0de00d)
              << Printer::Text (Printer::x_left, yPos - 4* Printer::line_break, "Line 2, Size 12, Color Blue", 12, 0x082270);


   yPos -= (4* Printer::line_break + Printer::empty_line);

   Printer::Text title2 (Printer::x_mid, yPos, "DocSpeak Color Title", 24, 0x08c996);
   title2.adjust_to_middle = true;
   (*printer) << title2;

   yPos -= Printer::empty_line;

   (*printer) << Printer::Text (Printer::x_left, yPos - Printer::line_break , "Image on one Side", 18)
              << Printer::Image (Printer::x_left, yPos - 2* Printer::line_break, (assets_path/"logo.jfif"), 200, 200)
              << Printer::Text (Printer::x_mid, yPos - Printer::line_break, "Text on the other Side", 18)
              << Printer::Text (Printer::x_mid, yPos - 2* Printer::line_break, "DocSpeck Print.", 12)
              << Printer::Text (Printer::x_mid, yPos - 3* Printer::line_break, "Text in Blue rigth to an Image", 12, 0x082270);


   Printer::Text title3 (Printer::x_mid, Printer::y_bottom, "DocSpeak Print End", 20);
   title3.adjust_to_middle = true;
   (*printer) << title3;

   EXPECT_GT(printer -> get_print_job().size(), 0); 

   auto print_status = printer -> print();
   EXPECT_TRUE(print_status.success); 

   EXPECT_EQ(printer -> get_print_job().size(), 0); 

   EXPECT_TRUE(std::filesystem::exists(output_path)); 

}

