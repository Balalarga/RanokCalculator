cmake_minimum_required(VERSION 3.5)

project(RanokLibraryCore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(OpenCL REQUIRED)

set(THIRDPARTY_DIR ../ThirdParty)
add_subdirectory(${THIRDPARTY_DIR}/RanokLanguage/RanokLanguageCore RanokLanguageCore)
add_subdirectory(${THIRDPARTY_DIR}/RapidJson RapidJson)


file(GLOB_RECURSE PROJECT_SOURCES Sources/*.cpp Sources/*.h Sources/*.hpp)
add_library(${PROJECT_NAME} ${PROJECT_SOURCES})

# GLM
target_include_directories(${PROJECT_NAME} PUBLIC
    ${THIRDPARTY_DIR}/Glm/glm
    ${OpenCL_INCLUDE_DIRS})

# Ranok lang lib
target_link_libraries(${PROJECT_NAME} PUBLIC
    RanokLanguageCore
    ${OpenCL_LIBRARY})


set(INCLUDE_INTERNAL_PATH Ranok/Core)
target_include_directories(${PROJECT_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sources/${INCLUDE_INTERNAL_PATH}>
    $<INSTALL_INTERFACE:Sources/${INCLUDE_INTERNAL_PATH}>
    )
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sources>
    $<INSTALL_INTERFACE:Sources>
    )
