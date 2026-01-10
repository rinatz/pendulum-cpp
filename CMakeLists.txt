cmake_minimum_required(VERSION 3.15)

#
# Project settings
#
project(pendulum)

file(READ VERSION PROJECT_VERSION)
configure_file(version.h.in ${PROJECT_SOURCE_DIR}/pendulum/version.h)

#
# Dependencies
#
find_package(cctz REQUIRED)
find_package(tl-optional REQUIRED)
find_package(GTest REQUIRED)

#
# Test settings
#
enable_testing()
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
    cctz::cctz
    tl::optional
    gtest::gtest
    dl
    pthread
)

#
# Install settings
#
install(DIRECTORY ${PROJECT_SOURCE_DIR}/pendulum DESTINATION include)
