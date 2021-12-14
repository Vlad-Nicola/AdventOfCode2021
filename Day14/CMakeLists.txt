cmake_minimum_required (VERSION 3.8)

set (CMAKE_CXX_STANDARD 20)

set(current_day Day14)
add_executable (${current_day} "${current_day}.cpp")

add_custom_command(
            TARGET ${current_day} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/input.txt" "${CMAKE_CURRENT_BINARY_DIR}/input.txt"
        )

add_test(NAME test${current_day}
         COMMAND ${current_day})
