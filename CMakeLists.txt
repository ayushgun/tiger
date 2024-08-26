cmake_minimum_required(VERSION 3.19)
project(tiger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(tiger)

target_sources(tiger PRIVATE src/string/short_string.cpp)

target_include_directories(tiger PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

target_compile_features(tiger PUBLIC cxx_std_20)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(tiger PRIVATE -Wall -Wextra -Werror -pedantic)
    target_link_options(tiger PRIVATE -fsanitize=address,undefined)
endif()
