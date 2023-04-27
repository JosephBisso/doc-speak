include(FetchContent)

FetchContent_Declare(
  PDFHummus
  GIT_REPOSITORY https://github.com/galkahana/PDF-Writer.git
  GIT_TAG        v4.5.6
  FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(PDFHummus)

FetchContent_Declare(
  plog 
  GIT_REPOSITORY https://github.com/SergiusTheBest/plog.git
  GIT_TAG        1.1.9
  FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(plog)


set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

file(GLOB SRCS *.cpp)
add_library( doc_speak STATIC  ${SRCS}) 

target_include_directories(
    doc_speak 
    PUBLIC 
    ${LIB_PATH}
    # ${plog_SOURCE_DIR}/include
)

target_link_libraries(
    doc_speak 
    PUBLIC 
    ${LIB_PATH}/DeepSpeech/libdeepspeech.so.if.lib
    PDFHummus::PDFWriter
    plog::plog
)
