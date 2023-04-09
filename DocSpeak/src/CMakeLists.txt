
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

file(GLOB SRCS *.cpp)
add_library( doc_speak STATIC  ${SRCS}) 

target_include_directories(
    doc_speak 
    PUBLIC 
    ${LIB_PATH}
)

target_link_libraries(doc_speak PUBLIC ${LIB_PATH}/DeepSpeech/libdeepspeech.so.if.lib)
