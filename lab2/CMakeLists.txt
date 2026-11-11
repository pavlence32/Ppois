cmake_minimum_required(VERSION 3.20)

project(HotelManagement CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_COVERAGE "Enable gcov/llvm-cov instrumentation" OFF)

file(GLOB_RECURSE HOTEL_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
file(GLOB_RECURSE HOTEL_TESTS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*_test.cpp"
)

find_package(Catch2 3 REQUIRED)

add_executable(lab2_tests ${HOTEL_SOURCES} ${HOTEL_TESTS})
target_include_directories(lab2_tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(lab2_tests PRIVATE Catch2::Catch2WithMain)

add_executable(lab2_cli ${HOTEL_SOURCES} cli/main.cpp)
target_include_directories(lab2_cli PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(lab2_tests PRIVATE --coverage -O0 -g)
    target_link_options(lab2_tests PRIVATE --coverage)
endif()

include(CTest)
include(Catch)
catch_discover_tests(lab2_tests)
