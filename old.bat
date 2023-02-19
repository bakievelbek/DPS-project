@echo off

del .\Release\old.exe
copy .\Release\DPS-project.exe .\Release\old.exe

.\Release\old.exe --cert .\certs\PlatoonDevice3\certificate.pem.crt --client_id PlatoonDevice3 --endpoint avb6i8gas5stv-ats.iot.us-east-1.amazonaws.com --key .\certs\PlatoonDevice3\private.pem.key --topic platoon/channel

if %ERRORLEVEL% neq 0 (
    echo Program exited with an error level of %ERRORLEVEL%.
) else (
    echo Program exited successfully.
)
