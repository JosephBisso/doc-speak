include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

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

include(GoogleTest)
gtest_discover_tests(unit_tests)

