@echo off

del Release\*.exe
cmake -DCMAKE_PREFIX_PATH="C:\code\aws-iot-device-sdk-cpp-v2-build" -DCMAKE_BUILD_TYPE="Release" .
cmake --build . --config "Release"

.\Release\DPS-project.exe --cert .\certs\985f54b692625fa585d1e985ab82d7fb9b18e43ced9445768f045c503bf2fb17-certificate.pem.crt --client_id PlatoonDevice1 --endpoint avb6i8gas5stv-ats.iot.us-east-1.amazonaws.com --key .\certs\985f54b692625fa585d1e985ab82d7fb9b18e43ced9445768f045c503bf2fb17-private.pem.key --topic platoon/channel

if %ERRORLEVEL% neq 0 (
    echo Program exited with an error level of %ERRORLEVEL%.
) else (
    echo Program exited successfully.
)
