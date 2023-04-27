include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include(GoogleTest)

include_directories(${DOCSPEACK_SRC})

link_libraries(
  GTest::gtest_main
  doc_speak
)

set(MAIN_CPP ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

FILE(GLOB all_test_types RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
  SET(all_tests "")

# Iterate througth Test Folders and add it as executable
FOREACH(test_type ${all_test_types})
  IF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${test_type})
  
    file(GLOB all_test_files ${CMAKE_CURRENT_SOURCE_DIR}/${test_type}/*.cpp)

    LIST(APPEND all_tests ${all_test_files})
    list(LENGTH all_test_files number_of_tests)

    if (${number_of_tests} LESS 1)
      file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${test_type})
      file(GLOB non_test_files ${CMAKE_CURRENT_SOURCE_DIR}/${test_type}/*.*)
      file(COPY ${non_test_files} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${test_type})
      continue()
    endif()

    message(STATUS "-> Test Type ${test_type}: ${number_of_tests} Tests")

    add_executable(
      ${test_type}
      ${MAIN_CPP}
      ${all_test_files}
    )

    gtest_discover_tests(${test_type})

  # Iterate througth each Test of the Test Folders and add it as separate executable
    foreach(test_file ${all_test_files})
      get_filename_component(test ${test_file} NAME_WE)
      message(STATUS "|->> Tests ${test}")

      add_executable(
        ${test}
        ${MAIN_CPP}
        ${test_file}
      )

      gtest_discover_tests(${test})

    endforeach()
    

  ENDIF()
ENDFOREACH()
