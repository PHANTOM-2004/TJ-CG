cmake_minimum_required(VERSION 3.25.0)

project(neon_cube)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/target/)

# compiler check
if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  message("[COMPILER]MSVC Compiler Detected")
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
  set(BUILD_SHARED_LIBS ON)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU OR ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
  message("[COMPILER]GCC or Clang Compiler Detected")
else()
  # Unsupported
  message(FATAL "Unsupported Compiler/Platform")
endif()

# opengl library
find_package(OpenGL REQUIRED)

include(scripts/functions.cmake)
include(scripts/config.cmake)

# local lib
add_subdirectory(lib/glfw-3.4/) # compile glfw3.4 locally
add_subdirectory(lib/glad/) # compile glad
add_subdirectory(lib/elements)

# default flags
set(MSVC_CXX_FLAGS /W3 /WX /utf-8)
set(GCC_CLANG_CXX_FLAGS -ggdb3 -Wall -Werror)

set(DEFAULT_INCLUDE
  ${CMAKE_SOURCE_DIR}/include/
  ${CMAKE_SOURCE_DIR}/lib/glfw/include/
  ${CMAKE_SOURCE_DIR}/shaders/
)

# libelements
SET(ELEMENTS_SRC
${CMAKE_SOURCE_DIR}/lib/elements/elements.cc
${CMAKE_SOURCE_DIR}/lib/elements/shaders.cc
)

# other target
add_subdirectory(src/cube/)
add_subdirectory(src/sphere/)
