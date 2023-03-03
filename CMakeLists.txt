project(archiver)
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(Catch REQUIRED)

function(add_catch TARGET)
    add_executable(${TARGET}  ${ARGN})
    target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${TARGET} catch_main)
endfunction()

add_subdirectory(src)
