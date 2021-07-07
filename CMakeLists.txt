cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(fsst)

set(CMAKE_CXX_STANDARD 17)


function(link_threads LIBRARY)
    if (CMAKE_VERSION VERSION_LESS "3.1")
        target_link_libraries(${LIBRARY} pthread)

    else()
        target_link_libraries(${LIBRARY} Threads::Threads)
    endif()
endfunction()


function(add_library_unity NAME MODE)
    set(SRCS ${ARGN})
    add_library(${NAME} OBJECT ${SRCS})
endfunction()


add_subdirectory(third_party)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(benchmark)

