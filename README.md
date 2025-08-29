# Enhanced UEFI Hardware Specification Tool

A comprehensive UEFI application designed for refurbished hardware assessment. This tool provides detailed hardware specifications suitable for documentation and customer specifications in the refurbishing industry.

## Features

### System Information
- **System Details**: Manufacturer, model, version, serial number, SKU, and family
- **Firmware Information**: Vendor, revision, and UEFI version

### CPU Information  
- **Processor Details**: Manufacturer, model, current/max speeds
- **Core Configuration**: Physical cores, logical processors, thread count
- **Topology**: Multi-processor system support via MP Services Protocol

### Enhanced Memory Information
- **System Memory**: Total and usable memory in MB/GB
- **Memory Modules**: Individual module specifications including:
  - Capacity per module
  - Memory type (DDR3/DDR4/DDR5)
  - Operating speed in MHz
  - Slot configuration

### Advanced Storage Information
- **Drive Details**: Capacity in MB/GB, block size
- **Drive Properties**: Fixed/removable, read-only/read-write, write caching
- **Drive Type Detection**: SSD vs HDD estimation based on formatting
- **Interface Information**: Legacy vs advanced format detection

### Graphics Information
- **Display Configuration**: Current resolution and pixel format
- **Frame Buffer**: Size and available display modes
- **Graphics Output Protocol**: Current mode information

### Battery Information (Laptops)
- **Battery Details**: Manufacturer, capacity, chemistry type
- **Health Information**: Design capacity and specifications
- **Multiple Battery Support**: Detection of all battery packs

## Technical Implementation

The tool utilizes advanced UEFI protocols:
- **SMBIOS Protocol**: For detailed hardware specifications
- **MP Services Protocol**: For multi-processor information  
- **Graphics Output Protocol**: For display capabilities
- **Block IO Protocol**: For enhanced storage analysis
- **Memory Map Services**: For comprehensive memory reporting

## Project Structure

```
UEFI-HardwareSpec/
├── src/
│   └── hardware_spec.c    # Main application source code
├── include/
│   ├── efi.h             # UEFI protocol definitions
│   └── efi_lib.h         # UEFI library helper functions
├── kernel.ld             # Linker script
├── write_gpt.exe         # Disk image creator tool
├── bios64.bin           # OVMF UEFI firmware
├── Makefile              # Build configuration
└── README.md             # This file
```

## Building

To build the UEFI application:

```bash
make build
```

To create a bootable disk image:

```bash
make image
```

## Running

To build, create a bootable disk image, and run in QEMU:

```bash
make run
```

## Creating Bootable Media

### Option 1: Bootable USB Drive (Recommended)

**Quick Method (Requires Administrator privileges):**
```bash
make usb DRIVE=E:
```
Replace `E:` with your USB drive letter. This will format the USB drive and copy the EFI application.

**Manual Method:**
1. Format USB drive as FAT32
2. Create folder structure: `\EFI\BOOT\`
3. Copy `BOOTX64.EFI` to `\EFI\BOOT\BOOTX64.EFI`

### Option 2: Using Third-Party Tools

**With Rufus:**
1. Download Rufus from https://rufus.ie/
2. Select your USB drive
3. Choose "Non bootable" 
4. Select "GPT" partition scheme for UEFI
5. Select "UEFI (non CSM)" target system
6. Start process, then copy `BOOTX64.EFI` to `\EFI\BOOT\`


## Usage Instructions

1. **Prepare Bootable Media**: Use one of the methods above
2. **Configure Target System**: 
   - Enable UEFI boot mode in BIOS/UEFI settings
   - Disable Secure Boot (if present)
   - Set USB/CD as first boot device
3. **Boot and Assess**: Insert media and restart system
4. **Review Results**: The tool will display comprehensive hardware information
5. **Export Data**: Note down specifications for documentation

## System Requirements

**Target Hardware:**
- UEFI-capable system (most computers 2010+)
- x86_64 architecture
- Minimum 512MB RAM

**Development Environment:**
- Windows with MinGW/GCC compiler
- QEMU for testing (optional)

## Important Notes

- **Secure Boot**: May need to be disabled for unsigned EFI applications (see Secure Boot section below)
- **Boot Priority**: Use BIOS boot menu (F12/F11/ESC) or change boot order
- **Drive Letters**: Check Windows Explorer for correct USB drive letter
- **Admin Rights**: USB creation requires administrator privileges
- **Compatibility**: Works on most UEFI systems manufactured after 2010

## Secure Boot Compatibility

### Current Status
The application currently runs as an **unsigned EFI application**, which requires **Secure Boot to be disabled** on most systems.

### Enabling Secure Boot Support

To make this application work with Secure Boot enabled, you have several options:

#### Option 1: Code Signing (Recommended for Production)

**Requirements:**
- Authenticode code signing certificate from a CA trusted by UEFI
- Microsoft's SignTool or similar signing utility

**Steps:**
```bash
# Sign the EFI binary with your certificate
signtool sign /fd SHA256 /f your_certificate.pfx /p password BOOTX64.EFI

# Verify signature
signtool verify /pa BOOTX64.EFI
```

**Certificate Options:**
- **Commercial CA**: DigiCert, Sectigo, GlobalSign (expensive but widely trusted)
- **Microsoft UEFI CA**: For production hardware manufacturers
- **Custom CA**: Install your own root certificate in UEFI firmware

#### Option 2: Self-Signing with Custom Keys

**For Development/Internal Use:**
```bash
# Generate your own signing keys
openssl req -new -x509 -newkey rsa:2048 -keyout platform.key -out platform.crt -days 365 -nodes

# Convert to PFX format
openssl pkcs12 -export -out platform.pfx -inkey platform.key -in platform.crt

# Sign the EFI file
signtool sign /fd SHA256 /f platform.pfx BOOTX64.EFI
```

**Install Certificate in UEFI:**
1. Boot to UEFI Setup
2. Navigate to Secure Boot settings
3. Add your certificate to the allowed database (db)
4. Or temporarily add to Machine Owner Key (MOK) database

#### Option 3: Shim Bootloader

**Using Signed Shim:**
```bash
# Download signed shim from your distribution or Microsoft
# Chain-load your application through shim
# Requires MOK (Machine Owner Key) enrollment
```

#### Option 4: UEFI Shell Method

**For Development/Testing:**
1. Boot to UEFI Shell (usually signed by default)
2. Run your application from shell: `BOOTX64.EFI`
3. Bypass Secure Boot chain-of-trust

### Production Deployment with Secure Boot

**For Commercial/Professional Use:**

1. **Obtain Certificate**: Purchase from trusted CA (~$200-800/year)
2. **Sign Application**: Use certificate to sign BOOTX64.EFI
3. **Test Thoroughly**: Verify on multiple UEFI implementations
4. **Distribution**: Signed application works with Secure Boot enabled

**Makefile Integration:**
```makefile
# Add signing target (requires certificate)
sign: build
	signtool sign /fd SHA256 /f certificate.pfx /p $(CERT_PASSWORD) BOOTX64.EFI
	@echo Application signed for Secure Boot

# Create signed USB
usb-secure: sign
	make usb DRIVE=$(DRIVE)
```

### Security Considerations

**Benefits of Secure Boot:**
- Prevents unauthorized code execution during boot
- Protects against bootkits and rootkits
- Required for some enterprise environments
- Industry standard for modern systems

**Implementation Notes:**
- Code signing adds ~$200-800 annual cost
- Requires certificate management and renewal
- Testing required across different UEFI vendors
- Some older systems have limited Secure Boot support

### Alternative: Custom Firmware Integration

**For Hardware Manufacturers:**
- Integrate hardware specification tool into UEFI firmware
- Sign with platform key during manufacturing
- Provides seamless Secure Boot compatibility
- Requires UEFI firmware development expertise

## Manual Steps for Advanced Users

If you prefer manual control over the process:

### 1. Build the Application
```bash
gcc -Iinclude -std=c11 -fno-stack-protector -fno-strict-aliasing -mno-red-zone -ffreestanding -fshort-wchar -nostdlib -Wall -c -o hardware_spec.o src/hardware_spec.c

gcc -nostdlib -Wl,--subsystem,10 -e efi_main -o BOOTX64.EFI hardware_spec.o
```

### 2. Create USB Manually
```bash
# Format USB as FAT32
format E: /fs:fat32 /q

# Create EFI directory structure
mkdir E:\EFI\BOOT

# Copy the EFI file
copy BOOTX64.EFI E:\EFI\BOOT\
```

### 3. Alternative: Create GPT Disk Image
```bash
write_gpt.exe BOOTX64.EFI custom.hdd
```

## Troubleshooting

**Common Issues:**

1. **"Boot device not found"**
   - Check UEFI boot mode is enabled
   - Verify USB is formatted as FAT32
   - Ensure EFI file is in correct path: `\EFI\BOOT\BOOTX64.EFI`

2. **"Secure Boot Violation"**
   - Disable Secure Boot in BIOS/UEFI settings
   - Some systems allow one-time boot override

3. **"Application fails to start"**
   - Verify target system is x86_64 architecture
   - Check system has sufficient RAM (512MB minimum)
   - Try different USB port

4. **"Incomplete hardware detection"**
   - Some virtual machines have limited SMBIOS data
   - Real hardware will show more complete information
   - Older systems may have partial UEFI protocol support

**Testing Tips:**
- Always test in QEMU first: `make run`
- Use a dedicated USB drive for bootable media
- Keep backups of important data before testing on production systems

## For Refurbished Hardware Assessment

This tool is specifically designed for hardware refurbishing operations:

- **Inventory Management**: Capture complete system specifications
- **Customer Documentation**: Generate professional hardware reports  
- **Quality Assurance**: Verify all components are detected and functional
- **Pricing Decisions**: Identify SSD vs HDD, RAM configurations, CPU performance
- **Warranty Claims**: Document hardware specifications before refurbishment

If you prefer to run the steps manually:

1. Build the application: `make build`
2. Create disk image: `write_gpt.exe BOOTX64.EFI test.hdd`
3. Run in QEMU: `qemu-system-x86_64 -machine q35 -drive "if=pflash,format=raw,file=bios64.bin" -drive "format=raw,file=test.hdd" -m 512M`

## Dependencies

- GCC compiler (MinGW)
- QEMU emulator
