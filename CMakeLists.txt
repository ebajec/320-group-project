cmake_minimum_required(VERSION 3.27)
project(spin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
find_package(raylib 4.0 REQUIRED)

file(GLOB SOURCES "src/*.cpp" "src/*.c")

add_executable(spin ${SOURCES})
target_include_directories(spin PRIVATE src)
target_link_libraries(spin PRIVATE raylib)

include(clang-warnings spin)



