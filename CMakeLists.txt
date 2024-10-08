cmake_minimum_required(VERSION 3.19)
project(tiger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(tiger)

file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
target_sources(tiger PRIVATE ${SRC_FILES})

target_include_directories(tiger PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_compile_features(tiger PUBLIC cxx_std_20)
target_compile_options(tiger PRIVATE -mavx2 -mbmi)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(tiger PRIVATE -Wall -Wextra -Werror -pedantic)
    target_link_options(tiger PRIVATE -fsanitize=address,undefined)
endif()
