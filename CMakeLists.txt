cmake_minimum_required(VERSION 3.15)

project(spell-checker-app LANGUAGES CXX)

# The console colouring and timing use the Win32 API, so this builds on Windows.
if(NOT WIN32)
    message(WARNING "This project relies on <windows.h> and only builds on Windows.")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(spell-checker src/main.cpp)

# The program reads data/words.txt and data/high_scores.txt relative to the
# working directory, so run it from the project root.
