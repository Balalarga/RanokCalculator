cmake_minimum_required(VERSION 3.5)

project(RanokLanguageCore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(THIRDPARTY_DIR ../ThirdParty)
add_subdirectory(${THIRDPARTY_DIR}/fmt fmt)

file(GLOB_RECURSE PROJECT_SOURCES Sources/*.cpp Include/*.h Include/*.hpp)

add_library(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC fmt)

set(INCLUDE_INTERNAL_PATH Ranok/LanguageCore)

target_include_directories(${PROJECT_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include/${INCLUDE_INTERNAL_PATH}>
    $<INSTALL_INTERFACE:Include/${INCLUDE_INTERNAL_PATH}>
)
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
    $<INSTALL_INTERFACE:Include>
)