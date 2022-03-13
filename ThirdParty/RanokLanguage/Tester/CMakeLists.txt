cmake_minimum_required(VERSION 3.14)

project(RanokLanguageTester LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE EXAMPLES ../CodeExamples/*.txt)
add_executable(${PROJECT_NAME} main.cpp ${EXAMPLES})

target_link_libraries (${PROJECT_NAME} LINK_PUBLIC RanokLanguageCore)
target_include_directories(${PROJECT_NAME} PUBLIC RanokLanguageCore)