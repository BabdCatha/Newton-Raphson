cmake_minimum_required(VERSION 3.20)
project(Newton_Raphson)

set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)
if(SFML_FOUND)
    message(STATUS "SFML VERSION: ${SFML_VERSION}")
endif()

add_executable(Newton_Raphson main.cpp)
target_link_libraries (Newton_Raphson sfml-graphics sfml-window sfml-system)