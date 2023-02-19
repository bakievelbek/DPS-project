cmake_minimum_required(VERSION 3.12)
# note: cxx-17 requires cmake 3.8, cxx-20 requires cmake 3.12
project(DPS-project CXX)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SRC_FILES
    "*.cpp"
    "src/*.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 14)

#set warnings
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /wd4068 -openmp)
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wno-long-long -pedantic -Werror -fopenmp)
endif ()

find_package(aws-crt-cpp REQUIRED)
find_package(RapidJSON REQUIRED)

target_link_libraries(${PROJECT_NAME} AWS::aws-crt-cpp rapidjson)
