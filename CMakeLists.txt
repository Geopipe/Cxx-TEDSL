cmake_minimum_required(VERSION 3.1)

foreach(lang C CXX)
	set(CMAKE_${lang}_STANDARD_REQUIRED ON)
	set(CMAKE_${lang}_STANDARD 11)
	set(CMAKE_${lang}_EXTENSIONS OFF)
endforeach(lang)

project(Cxx-TEDSL)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

find_package(Boost)

add_subdirectory(deps/fmt)
include_directories(include deps/Mach7/code deps/fmt ${Boost_INCLUDE_DIRS})
add_subdirectory(src)
add_subdirectory(test)
