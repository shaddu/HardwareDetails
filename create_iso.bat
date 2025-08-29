@echo off
REM Create ISO for UEFI bootable CD/DVD

REM Create temporary directory structure
mkdir temp_iso\EFI\BOOT
copy BOOTX64.EFI temp_iso\EFI\BOOT\
copy DSKIMG.INF temp_iso\EFI\BOOT\

REM Create ISO (requires oscdimg from Windows SDK)
REM Download from: https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk/
oscdimg -u2 -udfver102 -bootdata:2#p0,e,b"temp_iso\EFI\BOOT\BOOTX64.EFI"#pEF,e,b"temp_iso\EFI\BOOT\BOOTX64.EFI" temp_iso hardware_spec.iso

REM Clean up
rmdir /s /q temp_iso

echo ISO created: hardware_spec.iso
pause
