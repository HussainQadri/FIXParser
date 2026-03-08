cmake_minimum_required(VERSION 3.15...3.25)

project(MyProject
  VERSION
    1.0
  DESCRIPTION
    "FIX Parser"
  LANGUAGES
    CXX
)

add_executable(main src/main.cpp src/FIXMessage.cpp)
