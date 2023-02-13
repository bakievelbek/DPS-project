# DPS Project

Members 
: Furkan Ali Yurdakul
: Bruno Hyska
: Ian Murnane
: Elbek Bakiev

## To Install

git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp

git clone --recursive https://github.com/aws/aws-iot-device-sdk-cpp-v2.git

cd aws-sdk-cpp

mkdir build

cd build

cmake .. -G "Visual Studio 16 2019" -DBUILD_ONLY=iot -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=14
cmake .. -G Ninja -DBUILD_ONLY=iot -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=14
cmake -DCMAKE_INSTALL_PREFIX="C:/code/aws-iot-device-sdk-cpp-v2-build" ../aws-iot-device-sdk-cpp-v2
** cmake -DCMAKE_INSTALL_PREFIX="C:/code/aws-iot-device-sdk-cpp-v2-build" -DCMAKE_BUILD_TYPE="Debug" ../aws-iot-device-sdk-cpp-v2
-DBUILD_SHARED_LIBS=ON

msbuild ALL_BUILD.vcxproj /p:Configuration=Release
ninja

cmake --install . --config Release

## To Compile

Compiler options: ``` -fopenmp ```

Program arguments: ``` id e.g. Fritz ```

## To Run

```
main Fritz
```
