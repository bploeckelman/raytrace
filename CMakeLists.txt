# https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/
# https://cliutils.gitlab.io/modern-cmake/chapters/basics.html
cmake_minimum_required(VERSION 3.1...3.15)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(raytrace VERSION 0.1.0
        DESCRIPTION "Game engine"
        LANGUAGES C)

set(CMAKE_C_STANDARD 99)

add_executable(raytrace main.c)
