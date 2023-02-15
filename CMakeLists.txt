cmake_minimum_required(VERSION 3.23)
project(DPS_project)

include_directories(src "./include")

add_executable(DPS_project src/Main.cpp)

target_compile_options(DPS_project PUBLIC -fopenmp)

list(APPEND CMAKE_PREFIX_PATH "../aws-iot-device-sdk-cpp-v2-build")

find_package(aws-crt-cpp)

target_link_libraries(DPS_project AWS::aws-crt-cpp)
