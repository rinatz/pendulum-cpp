cmake_minimum_required(VERSION 3.0)
enable_testing()

#
# Project settings
#
project(pendulum)

file(READ VERSION PROJECT_VERSION)
configure_file(version.h.in ${PROJECT_SOURCE_DIR}/pendulum/version.h)

#
# Conan settings
#
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

#
# Test settings
#
set(TEST_EXECUTABLE_NAME ${PROJECT_NAME}_test)

file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(${TEST_EXECUTABLE_NAME} ${TEST_SOURCES})
add_test(NAME ${TEST_EXECUTABLE_NAME} COMMAND ${TEST_EXECUTABLE_NAME} --gtest_shuffle)

target_compile_features(
    ${TEST_EXECUTABLE_NAME}
    PUBLIC
    cxx_std_11
)

target_include_directories(
    ${TEST_EXECUTABLE_NAME}
    PUBLIC
    ${CMAKE_SOURCE_DIR}
)

target_compile_definitions(
    ${TEST_EXECUTABLE_NAME}
    PUBLIC
    -D_GLIBCXX_USE_CXX11_ABI=0
)

target_compile_options(
    ${TEST_EXECUTABLE_NAME}
    PUBLIC
    -Wall
    -Wextra
    -Werror
    -Wno-error=missing-field-initializers
)

target_link_libraries(
    ${TEST_EXECUTABLE_NAME}
    ${CONAN_LIBS}
    dl
    pthread
)
