
add_library(DeepSpeech SHARED IMPORTED GLOBAL)

set_target_properties(
    DeepSpeech
    PROPERTIES
    IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/libdeepspeech.so.if.lib
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libdeepspeech.so
)

target_include_directories(DeepSpeech PUBLIC INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/.)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/libdeepspeech.so
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libdeepspeech.so
	COPYONLY
)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/deepspeech-0.9.3-models.pbmm) 
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/deepspeech-0.9.3-models.pbmm
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/deepspeech-0.9.3-models.pbmm
        COPYONLY
    )
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/deepspeech-0.9.3-models.scorer) 
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/deepspeech-0.9.3-models.scorer
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/deepspeech-0.9.3-models.scorer
        COPYONLY
    )
endif()

