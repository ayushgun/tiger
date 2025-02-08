cmake_minimum_required(VERSION 3.19)
project(tiger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(tiger)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
target_sources(tiger PRIVATE ${SRC_FILES})

target_include_directories(tiger PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_compile_options(tiger PRIVATE
    $<$<CONFIG:Debug>:-Wall;-Wextra;-Werror;-pedantic;-fsanitize=address,undefined>
    $<$<CONFIG:Release>:-Ofast;-funroll-loops;-ffast-math;-funsafe-math-optimizations;-march=native>
)
target_link_options(tiger PRIVATE
  $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)
