cmake_minimum_required(VERSION 3.20)
project(Newton_Raphson LANGUAGES CUDA CXX)

set(CMAKE_CUDA_STANDARD 14)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)
if(SFML_FOUND)
    message(STATUS "SFML VERSION: ${SFML_VERSION}")
endif()

find_package(CUDA REQUIRED)
include_directories("${CUDA_INCLUDE_DIRS}")

add_executable(Newton_Raphson main.cu Root.cu Root.cuh Scale.cu Scale.cuh Polynomial.cu Polynomial.cuh getColorMap.cuh CUDA_functions.cu CUDA_functions.cuh)
target_link_libraries (Newton_Raphson sfml-graphics sfml-window sfml-system)