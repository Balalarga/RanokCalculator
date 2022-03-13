cmake_minimum_required(VERSION 3.5)

project(RanokLanguageApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Tester Tester)
add_subdirectory(RanokLanguageCore RanokLanguageCore)
