
file(GLOB Tests *.cpp)
include_directories(${DOCSPEACK_SRC})

add_executable(
    unit_tests
    ${Tests}
)
target_link_libraries(
    unit_tests
    PUBLIC
    GTest::gtest_main
    doc_speak
)

gtest_discover_tests(unit_tests)

