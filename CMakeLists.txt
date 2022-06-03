cmake_minimum_required(VERSION 3.9)
project(raylib-drawing)

option(BUILD_EXAMPLES "Build the examples." OFF)

add_subdirectory(src)
if (${BUILD_EXAMPLES})
  MESSAGE(STATUS "Building examples is enabled")
  add_subdirectory(examples)
endif()
