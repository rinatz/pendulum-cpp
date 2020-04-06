cmake_minimum_required(VERSION 3.0)

project(pendulum)

file(READ VERSION PROJECT_VERSION)
configure_file(version.h.in ${PROJECT_SOURCE_DIR}/pendulum/version.h)

add_definitions(
    -D_GLIBCXX_USE_CXX11_ABI=0
)

add_compile_options(
    -std=c++11
    -Wall
    -Wextra
    -Werror
)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/libs/include
)

link_directories(
    ${PROJECT_SOURCE_DIR}/libs/lib
)

file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})

target_link_libraries(
    ${PROJECT_NAME}_test
    cctz
    gmock
    gtest
    dl
    pthread
)

enable_testing()
add_test(NAME ${PROJECT_NAME}_test COMMAND pendulum_test)
