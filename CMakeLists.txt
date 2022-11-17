cmake_minimum_required(VERSION 3.23)
project(DPS_project)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(DPS_project
        src/Main.cpp
        src/Main.h threads/Threads.cpp)
