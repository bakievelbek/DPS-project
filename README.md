# DPS Project

Members 
: Furkan Ali Yurdakul
: Bruno Hyska
: Ian Murnane
: Elbek Bakiev

## Libraries
https://github.com/Tencent/rapidjson.git

https://github.com/aws/aws-iot-device-sdk-cpp-v2.git

## To Install

git clone --recursive https://github.com/aws/aws-iot-device-sdk-cpp-v2.git

cd aws-sdk-cpp

mkdir build

cd build

```
[from aws-iot-device-sdk-cpp-v2-build folder]
cmake -DCMAKE_INSTALL_PREFIX="C:/code/aws-iot-device-sdk-cpp-v2-build" -DCMAKE_BUILD_TYPE="Release" ../aws-iot-device-sdk-cpp-v2
```

```
msbuild ALL_BUILD.vcxproj /p:Configuration=Release
```

```
[from DPS-project folder]
cmake --install ../aws-iot-device-sdk-cpp-v2-build --config Release
```

## To Compile

Compiler options: ``` -fopenmp -laws-crt-cpp -Llib -Iinclude ```

Program arguments: ``` id e.g. Fritz ```

## To Run

```
main Fritz
```
