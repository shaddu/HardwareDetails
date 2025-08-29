# UEFI Hardware Specification Tool
# Simple Makefile for Windows

CC = gcc
TARGET = BOOTX64.EFI
SOURCE = src/hardware_spec.c
OBJECT = hardware_spec.o

# Compiler flags
CFLAGS = -Iinclude -std=c11 -fno-stack-protector -fno-strict-aliasing \
         -mno-red-zone -ffreestanding -fshort-wchar -nostdlib -Wall

# Linker flags  
LDFLAGS = -nostdlib -Wl,--subsystem,10 -e efi_main

# Default target
all: build

# Build the UEFI application
build: $(TARGET)
	@echo Build completed: $(TARGET)

$(TARGET): $(OBJECT)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECT)

$(OBJECT): $(SOURCE)
	$(CC) $(CFLAGS) -c -o $(OBJECT) $(SOURCE)

# Create bootable image and run
run: $(TARGET)
	write_gpt.exe $(TARGET) test.hdd
	qemu-system-x86_64 -machine q35 -drive "if=pflash,format=raw,file=bios64.bin" -drive "format=raw,file=test.hdd" -m 512M

# Create bootable image only
image: $(TARGET)
	write_gpt.exe $(TARGET) test.hdd
	@echo Bootable image created: test.hdd

# Create bootable USB (requires admin privileges)
# Usage: make usb DRIVE=E:
usb: build
	@echo Creating bootable USB on drive $(DRIVE)
	@echo WARNING: This will format the drive!
	@pause
	format $(DRIVE) /fs:fat32 /q /y
	mkdir "$(DRIVE)\EFI\BOOT" 2>nul || echo Directory exists
	copy BOOTX64.EFI "$(DRIVE)\EFI\BOOT\"
	copy DSKIMG.INF "$(DRIVE)\EFI\BOOT\" 2>nul || echo Info file not required
	@echo Bootable USB created on $(DRIVE)

# Create ISO file
iso: build create_iso.bat
	create_iso.bat

# Sign for Secure Boot (requires certificate)
# Set CERT_FILE and CERT_PASSWORD environment variables
sign: build
	@echo Signing BOOTX64.EFI for Secure Boot compatibility...
	sign_for_secure_boot.bat

# Create test certificate for development
test-cert:
	@echo Creating test certificate for Secure Boot development...
	create_test_certificate.bat

# Create signed USB for Secure Boot systems
usb-secure: sign
	@echo Creating Secure Boot compatible USB on drive $(DRIVE)
	@echo WARNING: This will format the drive!
	@pause
	format $(DRIVE) /fs:fat32 /q /y
	mkdir "$(DRIVE)\EFI\BOOT" 2>nul || echo Directory exists
	copy BOOTX64.EFI "$(DRIVE)\EFI\BOOT\"
	copy DSKIMG.INF "$(DRIVE)\EFI\BOOT\" 2>nul || echo Info file not required
	@echo Secure Boot compatible USB created on $(DRIVE)

# Clean build artifacts
clean:
	del /Q $(TARGET) $(OBJECT) test.hdd 2>nul || echo Clean completed

# Help
help:
	@echo Available targets:
	@echo   build      - Compile the UEFI application
	@echo   image      - Create bootable disk image
	@echo   run        - Build, create image, and run in QEMU  
	@echo   usb        - Create bootable USB drive (usage: make usb DRIVE=E:)
	@echo   usb-secure - Create Secure Boot compatible USB (requires signing)
	@echo   iso        - Create bootable ISO file
	@echo   sign       - Sign application for Secure Boot (requires certificate)
	@echo   test-cert  - Create test certificate for development
	@echo   clean      - Remove build artifacts
	@echo   help       - Show this help
	@echo.
	@echo Secure Boot Usage:
	@echo   1. Set certificate: set CERT_FILE=your_cert.pfx
	@echo   2. Set password: set CERT_PASSWORD=your_password  
	@echo   3. Run: make sign
	@echo   4. Create USB: make usb-secure DRIVE=E:

.PHONY: all build image run clean help usb iso sign test-cert usb-secure
