#include <gtest/gtest.h>

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  plog::init(plog::verbose, "docspeak.log");
  PLOGI << "\n\n";
  PLOGI << "################################### DocSpeak Start ###################################";

  return RUN_ALL_TESTS();
}