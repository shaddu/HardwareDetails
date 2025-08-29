@echo off
REM Script to create self-signed certificate for development/testing
REM WARNING: This creates a test certificate only - not for production use

echo Creating self-signed certificate for UEFI Secure Boot testing...
echo WARNING: This is for development/testing only!
echo.

REM Check for OpenSSL
where openssl >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo Error: OpenSSL not found in PATH
    echo Please install OpenSSL or Git for Windows
    pause
    exit /b 1
)

REM Generate private key
echo Generating private key...
openssl genrsa -out hardware_spec_test.key 2048

REM Create certificate
echo Creating self-signed certificate...
openssl req -new -x509 -key hardware_spec_test.key -out hardware_spec_test.crt -days 365 -subj "/C=US/ST=Test/L=Test/O=Hardware Spec Tool/CN=Test Certificate"

REM Convert to PFX format
echo Converting to PFX format...
openssl pkcs12 -export -out hardware_spec_test.pfx -inkey hardware_spec_test.key -in hardware_spec_test.crt -passout pass:test123

echo.
echo Test certificate created: hardware_spec_test.pfx
echo Password: test123
echo.
echo To use this certificate:
echo 1. Set environment variables:
echo    set CERT_FILE=hardware_spec_test.pfx
echo    set CERT_PASSWORD=test123
echo 2. Run: sign_for_secure_boot.bat
echo 3. Install certificate in UEFI firmware MOK database
echo.
echo NOTE: This certificate must be enrolled in UEFI firmware
echo before Secure Boot will accept the signed application.

pause
