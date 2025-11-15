cmake_minimum_required(VERSION 3.27)
project(spin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)

if(WIN32)
	set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
	include(FetchContent)

	set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
	set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
	set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

	FetchContent_Declare(
		raylib
		GIT_REPOSITORY https://github.com/raysan5/raylib.git
		GIT_TAG 5.0
	)

	FetchContent_MakeAvailable(raylib)
else()
	find_package(raylib 4.0 REQUIRED)
endif()

file(GLOB SOURCES "src/*.cpp" "src/*.c")

add_executable(spin ${SOURCES})
target_include_directories(spin PUBLIC src)
target_link_libraries(spin PRIVATE raylib)
target_compile_definitions(spin PRIVATE RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/")

option(ENABLE_ASAN "Enable AddressSanitizer" OFF)

if(ENABLE_ASAN AND (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU"))
message(STATUS "AddressSanitizer Enabled")
	target_compile_options(spin PUBLIC
		-fsanitize=address
		-fno-omit-frame-pointer    
		-g                          
	)
	target_link_options(spin PUBLIC
    	-fsanitize=address
  	)
endif()


include(clang-warnings spin)



