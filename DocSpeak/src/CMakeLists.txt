
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

file(GLOB SRCS *.cpp)
add_library( doc_speak STATIC  ${SRCS}) 
target_include_directories(doc_speak PUBLIC ${DEEP_SPEECH_PATH})
target_link_libraries(doc_speak PUBLIC ${DEEP_SPEECH_PATH}/libdeepspeech.so.if.lib)
