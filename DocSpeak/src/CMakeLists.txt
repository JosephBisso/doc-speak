set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

file(GLOB SRCS *.cpp)
add_library( doc_speak STATIC  ${SRCS}) 

target_include_directories(
    doc_speak 
    PUBLIC 
    ${LIB_PATH}
)

target_link_libraries(
    doc_speak 
    PUBLIC 
    ${LIB_PATH}/DeepSpeech/libdeepspeech.so.if.lib
    ${LIB_PATH}/PDF-Writer/lib/FreeType.lib
    ${LIB_PATH}/PDF-Writer/lib/LibAesgm.lib
    ${LIB_PATH}/PDF-Writer/lib/LibJpeg.lib
    ${LIB_PATH}/PDF-Writer/lib/LibPng.lib
    ${LIB_PATH}/PDF-Writer/lib/LibTiff.lib
    ${LIB_PATH}/PDF-Writer/lib/PDFWriter.lib
    ${LIB_PATH}/PDF-Writer/lib/Zlib.lib
)
