include(FetchContent)

############################# PDFHummus #############################
FetchContent_Declare(
  PDFHummus
  GIT_REPOSITORY https://github.com/galkahana/PDF-Writer.git
  GIT_TAG        v4.5.6
  FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(PDFHummus)

############################# plog #############################
FetchContent_Declare(
  plog 
  GIT_REPOSITORY https://github.com/SergiusTheBest/plog.git
  GIT_TAG        1.1.9
  FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(plog)

############################# PortAudio #############################
FetchContent_Declare(
  PortAudio 
  GIT_REPOSITORY https://github.com/PortAudio/portaudio.git
  GIT_TAG        181dc2277bc16785c93fe3c74ce6b2f74ae53581 #v19.8
  FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(PortAudio)

############################# libfvad #############################
FetchContent_Declare(
  libfvad 
  GIT_REPOSITORY https://github.com/dpirch/libfvad.git
  GIT_TAG        master
  FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(libfvad)

target_include_directories(fvad PUBLIC ${libfvad_SOURCE_DIR}/include)

############################# tinywav #############################
FetchContent_Declare(
  tinywav 
  GIT_REPOSITORY https://github.com/JosephBisso/tinywav.git
  GIT_TAG        master
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  FIND_PACKAGE_ARGS
)
FetchContent_GetProperties(tinywav)
if(NOT tinywav_POPULATED)
  FetchContent_Populate(tinywav)
endif()
add_library(tinywav STATIC ${tinywav_SOURCE_DIR}/tinywav.c)
target_include_directories(tinywav PUBLIC ${tinywav_SOURCE_DIR}/)


############################# Others #############################
add_subdirectory(DeepSpeech)