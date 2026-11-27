cmake_minimum_required(VERSION 3.20)

project(FactoryManagement CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_COVERAGE "Enable gcov/llvm-cov instrumentation" OFF)

file(GLOB_RECURSE FACTORY_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
file(GLOB_RECURSE FACTORY_TESTS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*_test.cpp"
)

find_package(Catch2 3 REQUIRED)

add_executable(lab3_tests ${FACTORY_SOURCES} ${FACTORY_TESTS})
target_include_directories(lab3_tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(lab3_tests PRIVATE Catch2::Catch2WithMain)

add_executable(lab3_cli ${FACTORY_SOURCES} cli/main.cpp)
target_include_directories(lab3_cli PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(lab3_tests PRIVATE --coverage -O0 -g)
    target_link_options(lab3_tests PRIVATE --coverage)
endif()

include(CTest)
include(Catch)
catch_discover_tests(lab3_tests)
