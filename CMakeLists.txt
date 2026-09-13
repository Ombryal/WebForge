cmake_minimum_required(VERSION 3.22)

project(WebForge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Core compiler library
add_library(webforge_lib STATIC
    src/webforge/lexer/Lexer.cpp
    src/webforge/parser/Parser.cpp
)
target_include_directories(webforge_lib PUBLIC src)

# CLI executable
add_executable(webforge
    src/main.cpp
)
target_link_libraries(webforge PRIVATE webforge_lib)
