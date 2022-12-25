cmake_minimum_required(VERSION 3.23)
project(DPS_project)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(DPS_project
        src/Main.cpp
        src/ThreadSafeQueue.cpp
        "src/Header Files/ThreadSafeQueue.h"
        src/Communication.cpp
        "src/Header Files/Communication.h"
        src/Sensors.cpp
        "src/Header Files/Sensors.h"
        src/Truck.cpp
        "src/Header Files/Truck.h")

#find cppzmq wrapper, installed by make of cppzmq
find_package(cppzmq)
target_link_libraries(DPS_project cppzmq)