@echo off

@REM del Release\*.exe
@REM cmake -DCMAKE_PREFIX_PATH="C:\code\aws-iot-device-sdk-cpp-v2-build" -DCMAKE_BUILD_TYPE="Release" .
@REM cmake --build . --config "Release"

.\Release\DPS-project.exe --cert .\certs\PlatoonDevice2\certificate.pem.crt --client_id PlatoonDevice2 --endpoint avb6i8gas5stv-ats.iot.us-east-1.amazonaws.com --key .\certs\PlatoonDevice2\private.pem.key --topic platoon/channel

if %ERRORLEVEL% neq 0 (
    echo Program exited with an error level of %ERRORLEVEL%.
) else (
    echo Program exited successfully.
)
