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
        src/Sensors.h
        src/Truck.cpp
        src/Truck.h)

#find cppzmq wrapper, installed by make of cppzmq
find_package(cppzmq)
target_link_libraries(DPS_project cppzmq)