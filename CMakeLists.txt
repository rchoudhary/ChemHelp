cmake_minimum_required(VERSION 3.2)
project(chemhelp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES)
add_executable(chemhelp ${SOURCE_FILES})

add_subdirectory(chemhelp_core)
include_directories(chemhelp_core)
target_link_libraries(chemhelp chemhelp_core)