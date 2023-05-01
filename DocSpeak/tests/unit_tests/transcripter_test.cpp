#include <transcripter.h>

#include <gtest/gtest.h>

using namespace docspeak;

// The fixture for testing class Foo.
class TranscripterTest : public ::testing::Test {
 protected:

  TranscripterTest(): transcripter(new Transcripter) {
     // You can do set-up work for each test here.
      auto model_set = Transcripter::set_model_path(std::filesystem::current_path() / "deepspeech-0.9.3-models.pbmm");
      EXPECT_TRUE(model_set); 

      auto scorer_set = Transcripter::set_scorer_path(std::filesystem::current_path() / "deepspeech-0.9.3-models.scorer");
      EXPECT_TRUE(scorer_set); 

      auto audio_dir_set = Transcripter::set_audio_input_directory(assets_path);
      EXPECT_TRUE(audio_dir_set); 

      audio_dir_set = Transcripter::set_audio_output_directory(std::filesystem::current_path()/"output");
      EXPECT_TRUE(audio_dir_set); 

      transcripter -> set_observer(TranscripterTest::transcript_observer);
  }

  ~TranscripterTest() override {
     // You can do clean-up work that doesn"t throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
     observed_transcriptions.clear();
  }

   static void transcript_observer(std::string transcripted) {
      observed_transcriptions.push_back(transcripted);
   }

  // Class members declared here can be used by all tests in the test suite
  std::shared_ptr<Transcripter> transcripter;
  std::filesystem::path assets_path = std::filesystem::current_path() / "..\\DocSpeak\\tests\\assets\\";
  static std::vector<std::string> observed_transcriptions;
};

std::vector<std::string> TranscripterTest::observed_transcriptions;

TEST_F(TranscripterTest, RecordAudio) {

    auto results = transcripter->record_test();

    EXPECT_EQ(results, 0);
}

TEST_F(TranscripterTest, TranscriptStream) {

#ifdef CI
    GTEST_SKIP()
#endif
    auto result = transcripter->transcript<TranscripterTest>();

    EXPECT_TRUE(result.success);

   auto transcriptions = transcripter -> get_transcription();
   EXPECT_GT(transcriptions.size(), 0);
   EXPECT_EQ(transcriptions.size(), observed_transcriptions.size());
}

TEST_F(TranscripterTest, TranscriptGivenAudio) {

    auto results = transcripter->transcript_test();

    EXPECT_GT(results.size(), 0);
}


