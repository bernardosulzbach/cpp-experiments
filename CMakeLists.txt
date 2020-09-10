cmake_minimum_required(VERSION 3.12)

project(cpp-experiments LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  add_definitions(-Wall)
  add_definitions(-Wextra)
  add_definitions(-Wshadow)
  add_definitions(-DDEBUGGING)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=undefined ")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=leak")
endif()

add_library(cpp-experiments-objects OBJECT src/sso.hpp src/sso.cpp src/memory.hpp src/memory.cpp)

add_executable(cpp-experiments src/main.cpp $<TARGET_OBJECTS:cpp-experiments-objects>)
