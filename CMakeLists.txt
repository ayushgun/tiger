cmake_minimum_required(VERSION 3.19)
project(tiger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEV_WARNINGS "-Wall -Wextra -Werror -pedantic")
    set(SANITIZERS "-fsanitize=address,undefined")
endif()

add_library(tiger INTERFACE)
target_include_directories(tiger INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(tiger INTERFACE
    $<$<CONFIG:Debug>:${DEV_WARNINGS} ${SANITIZERS}>
)

