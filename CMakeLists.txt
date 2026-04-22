cmake_minimum_required(VERSION 3.15...3.25)

project(MyProject
  VERSION 1.0
  DESCRIPTION "FIX Parser"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Fetch Dependencies
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.8.1
)
FetchContent_Declare(
  pugixml
  GIT_REPOSITORY https://github.com/zeux/pugixml.git
  GIT_TAG v1.14
)
FetchContent_MakeAvailable(Catch2 pugixml)

# 2. Define Library
add_library(fixparser
  src/FIXMessage.cpp
  src/FIXMessage.h
  src/FIXDictionary.cpp
  src/FIXDictionary.h
  src/Heartbeat.cpp
  src/Heartbeat.h
  src/TypedMessage.h
  src/Logon.h
  src/Logon.cpp
  src/Logout.h
  src/Logout.cpp
)
target_include_directories(fixparser PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(fixparser PUBLIC pugixml::pugixml)
target_compile_definitions(fixparser PRIVATE FIX_DICTIONARY_PATH="${CMAKE_SOURCE_DIR}/src/FIX42.xml")

# 3. Main Executable
add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE fixparser)

# Copy the FIX42.xml file to the build directory
configure_file(${CMAKE_SOURCE_DIR}/src/FIX42.xml ${CMAKE_BINARY_DIR}/FIX42.xml COPYONLY)

# 4. Enable Testing (Must be before add_executable for tests)
enable_testing()

# 5. Test Executable
add_executable(tests tests/test_fixmessage.cpp)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(tests PRIVATE fixparser Catch2::Catch2WithMain)
add_executable(dict_tests tests/test_fixdictionary.cpp)
target_include_directories(dict_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(dict_tests PRIVATE fixparser Catch2::Catch2WithMain)
add_executable(heartbeat_tests tests/test_heartbeat.cpp)
target_include_directories(heartbeat_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(heartbeat_tests PRIVATE fixparser Catch2::Catch2WithMain)
# 6. Test Discovery
include(CTest)
include(${catch2_SOURCE_DIR}/extras/Catch.cmake)
catch_discover_tests(tests)
catch_discover_tests(dict_tests)
catch_discover_tests(heartbeat_tests)
