
file(GLOB SRCS *.cpp)
add_library( doc_speak-src STATIC  ${SRCS}) 

target_include_directories(
    doc_speak-src 
    PUBLIC 
    ${LIB_PATH}
)

target_link_libraries(
    doc_speak-src 
    PUBLIC 
    PDFHummus::PDFWriter
    plog::plog
    PortAudio
    tinywav
    DeepSpeech
    fvad 
)

add_library(DocSpeak::Common ALIAS doc_speak-src)