cmake_minimum_required(VERSION 3.23)
project(DPS_project)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(DPS_project
        src/Main.cpp
        src/ThreadSafeQueue.cpp
        src/ThreadSafeQueue.h
        src/Communication.cpp
        src/Communication.h
        src/Sensors.cpp
        src/Sensors.h)
