cmake_minimum_required(VERSION 3.12)

project(cpp-experiments LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  message("Compiler is GCC/Clang, using a lot of warnings.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wunreachable-code -Wreturn-type")
endif()

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug" AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=undefined ")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=leak")
endif()

add_library(
  cpp-experiments-objects OBJECT
  src/sso.hpp
  src/sso.cpp
  src/memory.hpp
  src/memory.cpp
  src/sorting.hpp
  src/sorting.cpp
  src/shared_ptr.hpp
  src/shared_ptr.cpp
  src/formatting.hpp
  src/formatting.cpp
  src/container_growth.hpp
  src/container_growth.cpp
  src/types.hpp
  src/experiment_runner.hpp
  src/experiment_runner.cpp
  src/special_member_function_monitor.hpp
  src/special_member_function_monitor.cpp)

add_executable(cpp-experiments src/main.cpp $<TARGET_OBJECTS:cpp-experiments-objects>)
