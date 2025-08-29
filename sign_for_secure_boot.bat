@echo off
REM Script to sign UEFI application for Secure Boot compatibility
REM Requires: Windows SDK with SignTool, valid code signing certificate

echo Signing UEFI Hardware Specification Tool for Secure Boot...

REM Check if BOOTX64.EFI exists
if not exist "BOOTX64.EFI" (
    echo Error: BOOTX64.EFI not found. Run 'make build' first.
    pause
    exit /b 1
)

REM Check for certificate file
if not exist "%CERT_FILE%" (
    echo Error: Certificate file not found: %CERT_FILE%
    echo Please set CERT_FILE environment variable to your .pfx certificate
    echo Example: set CERT_FILE=my_certificate.pfx
    pause
    exit /b 1
)

REM Check for certificate password
if "%CERT_PASSWORD%"=="" (
    echo Error: Certificate password not set
    echo Please set CERT_PASSWORD environment variable
    echo Example: set CERT_PASSWORD=mypassword
    pause
    exit /b 1
)

REM Sign the EFI binary
echo Signing with certificate: %CERT_FILE%
signtool sign /fd SHA256 /f "%CERT_FILE%" /p "%CERT_PASSWORD%" /t http://timestamp.digicert.com BOOTX64.EFI

if %ERRORLEVEL% equ 0 (
    echo.
    echo Success! BOOTX64.EFI signed for Secure Boot compatibility
    echo.
    echo Verifying signature...
    signtool verify /pa BOOTX64.EFI
    echo.
    echo The application can now run with Secure Boot enabled
) else (
    echo.
    echo Error: Signing failed
    echo Check certificate file and password
)

pause
