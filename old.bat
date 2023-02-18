@echo off

.\Release\old.exe --cert .\certs\PlatoonDevice4\certificate.pem.crt --client_id PlatoonDevice4 --endpoint avb6i8gas5stv-ats.iot.us-east-1.amazonaws.com --key .\certs\PlatoonDevice4\private.pem.key --topic platoon/channel

if %ERRORLEVEL% neq 0 (
    echo Program exited with an error level of %ERRORLEVEL%.
) else (
    echo Program exited successfully.
)
