cmake_minimum_required(VERSION 3.20)

project(FactoryManagement CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE FACTORY_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(lab3_cli ${FACTORY_SOURCES} cli/main.cpp)
target_include_directories(lab3_cli PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
