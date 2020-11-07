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
file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})
add_test(NAME ${PROJECT_NAME}_test COMMAND pendulum_test --gtest_shuffle)

target_include_directories(
    ${PROJECT_NAME}_test
    PUBLIC
    ${CMAKE_SOURCE_DIR}
)

target_compile_definitions(
    ${PROJECT_NAME}_test
    PUBLIC
    -D_GLIBCXX_USE_CXX11_ABI=0
)

target_compile_features(
    ${PROJECT_NAME}_test
    PUBLIC
    cxx_std_11
)

target_compile_options(
    ${PROJECT_NAME}_test
    PUBLIC
    -Wall
    -Wextra
    -Werror
    -Wno-error=missing-field-initializers
)

target_link_libraries(
    ${PROJECT_NAME}_test
    cctz
    gmock
    gtest
    dl
    pthread
)
