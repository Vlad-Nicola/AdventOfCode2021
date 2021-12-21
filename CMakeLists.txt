# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

enable_testing()
set (CMAKE_CXX_STANDARD 20)

project ("AdventOfCode2021")

foreach(day RANGE 1 21)
    if(day EQUAL 19)
        continue()
    elseif(day LESS 10)
        set(pre "0")
    else()
        set(pre "")
    endif()
    add_subdirectory("Day${pre}${day}")
endforeach()
