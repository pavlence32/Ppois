cmake_minimum_required(VERSION 3.14)

project(AlgoritmsMarkov CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(ENABLE_COVERAGE "Enable code coverage" OFF)


include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2
)

FetchContent_MakeAvailable(Catch2)


add_executable(tests
    algos.cpp
    tests/test.cpp
)

target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(tests PRIVATE
    Catch2::Catch2WithMain
)


if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(tests PRIVATE
        --coverage
        -O0
        -g
    )

    target_link_options(tests PRIVATE
        --coverage
    )
endif()


enable_testing()

add_test(
    NAME unit_tests
    COMMAND tests
)

