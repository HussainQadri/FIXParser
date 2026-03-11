cmake_minimum_required(VERSION 3.15...3.25)

project(MyProject
  VERSION 1.0
  DESCRIPTION "FIX Parser"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Define Library
add_library(fixparser
  src/FIXMessage.cpp
  src/FIXMessage.h
)
target_include_directories(fixparser PUBLIC ${CMAKE_SOURCE_DIR}/src)

# 2. Main Executable
add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE fixparser)

# 3. Enable Testing (Must be before add_executable for tests)
enable_testing()

# 4. Fetch Catch2
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.8.1
)
FetchContent_MakeAvailable(Catch2)

# 5. Test Executable
add_executable(tests tests/test_fixmessage.cpp)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(tests PRIVATE fixparser Catch2::Catch2WithMain)

# 6. Test Discovery
include(CTest)
include(${catch2_SOURCE_DIR}/extras/Catch.cmake)
catch_discover_tests(tests)
