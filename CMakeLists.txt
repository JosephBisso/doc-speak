
cmake_minimum_required(VERSION 3.22)
PROJECT(DocSpeak VERSION 1.0 LANGUAGES CXX  C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(CI "Deactivate some tests that do not run well in the CI" OFF)

# Because of cmake download policy warning
cmake_policy(SET CMP0135 NEW)

set(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/)
set(DOCSPEACK_SRC ${CMAKE_CURRENT_SOURCE_DIR}/DocSpeak/src)

add_subdirectory(lib)
add_subdirectory(DocSpeak)