#include <stdarg.h>
#include "../include/efi.h"
#include "../include/efi_lib.h"

// Global variables
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *cout = NULL;
EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *cin  = NULL;
EFI_BOOT_SERVICES    *bs;
EFI_RUNTIME_SERVICES *rs;
EFI_SYSTEM_TABLE     *st;
EFI_HANDLE image = NULL;

// Protocol pointers
EFI_SMBIOS_PROTOCOL       *smbios_protocol = NULL;
EFI_MP_SERVICES_PROTOCOL  *mp_services = NULL;
EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = NULL;

// Initialize global variables and protocols
void init_globals(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    cout = SystemTable->ConOut;
    cin = SystemTable->ConIn;
    st = SystemTable;
    bs = st->BootServices;
    rs = st->RuntimeServices;
    image = ImageHandle;
    
    // Locate SMBIOS protocol
    EFI_GUID smbios_guid = EFI_SMBIOS_PROTOCOL_GUID;
    bs->LocateProtocol(&smbios_guid, NULL, (VOID**)&smbios_protocol);
    
    // Locate MP Services protocol
    EFI_GUID mp_guid = EFI_MP_SERVICES_PROTOCOL_GUID;
    bs->LocateProtocol(&mp_guid, NULL, (VOID**)&mp_services);
    
    // Locate Graphics Output Protocol
    EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    bs->LocateProtocol(&gop_guid, NULL, (VOID**)&gop);
}

// Simple printf implementation
void simple_printf(CHAR16 *format, ...) {
    cout->OutputString(cout, format);
}

// Convert number to string
void uint_to_string(UINT64 value, CHAR16 *buffer, UINTN buffer_size) {
    if (buffer_size < 2) return;
    
    if (value == 0) {
        buffer[0] = u'0';
        buffer[1] = u'\0';
        return;
    }
    
    UINTN digits = 0;
    UINT64 temp = value;
    while (temp > 0) {
        temp /= 10;
        digits++;
    }
    
    if (digits >= buffer_size) {
        buffer[0] = u'?';
        buffer[1] = u'\0';
        return;
    }
    
    buffer[digits] = u'\0';
    for (UINTN i = digits; i > 0; i--) {
        buffer[i-1] = u'0' + (value % 10);
        value /= 10;
    }
}

// Get SMBIOS string by index
CHAR8* get_smbios_string(EFI_SMBIOS_TABLE_HEADER *record, UINT8 string_number) {
    if (string_number == 0) return NULL;
    
    CHAR8 *string_ptr = (CHAR8*)record + record->Length;
    UINT8 current_string = 1;
    
    while (current_string < string_number && *string_ptr != 0) {
        while (*string_ptr != 0) string_ptr++;
        string_ptr++;
        current_string++;
    }
    
    return (*string_ptr != 0) ? string_ptr : NULL;
}

// Convert ASCII string to CHAR16
void ascii_to_char16(CHAR8 *ascii, CHAR16 *unicode, UINTN max_len) {
    UINTN i = 0;
    while (ascii[i] != 0 && i < max_len - 1) {
        unicode[i] = ascii[i];
        i++;
    }
    unicode[i] = u'\0';
}

// Display comprehensive CPU information
void display_cpu_info(void) {
    simple_printf(u"\r\n=== CPU INFORMATION ===\r\n");
    
    // Get CPU count from MP Services
    if (mp_services != NULL) {
        UINTN total_processors = 0;
        UINTN enabled_processors = 0;
        
        EFI_STATUS status = mp_services->GetNumberOfProcessors(mp_services, &total_processors, &enabled_processors);
        if (status == EFI_SUCCESS) {
            CHAR16 count_str[16];
            uint_to_string(total_processors, count_str, 16);
            simple_printf(u"Total Processor Cores: ");
            simple_printf(count_str);
            simple_printf(u"\r\n");
            
            uint_to_string(enabled_processors, count_str, 16);
            simple_printf(u"Enabled Processor Cores: ");
            simple_printf(count_str);
            simple_printf(u"\r\n");
        }
    }
    
    // Get CPU details from SMBIOS
    if (smbios_protocol != NULL) {
        EFI_SMBIOS_HANDLE smbios_handle = 0xFFFE;
        EFI_SMBIOS_TYPE smbios_type = SMBIOS_TYPE_PROCESSOR_INFORMATION;
        EFI_SMBIOS_TABLE_HEADER *record = NULL;
        
        EFI_STATUS status = smbios_protocol->GetNext(smbios_protocol, &smbios_handle, &smbios_type, &record, NULL);
        if (status == EFI_SUCCESS && record != NULL) {
            SMBIOS_TYPE4_PROCESSOR_INFORMATION *cpu_info = (SMBIOS_TYPE4_PROCESSOR_INFORMATION*)record;
            
            // CPU Manufacturer
            CHAR8 *manufacturer = get_smbios_string(record, cpu_info->ProcessorManufacturer);
            if (manufacturer != NULL) {
                CHAR16 mfg_unicode[64];
                ascii_to_char16(manufacturer, mfg_unicode, 64);
                simple_printf(u"Manufacturer: ");
                simple_printf(mfg_unicode);
                simple_printf(u"\r\n");
            }
            
            // CPU Version (Model)
            CHAR8 *version = get_smbios_string(record, cpu_info->ProcessorVersion);
            if (version != NULL) {
                CHAR16 ver_unicode[128];
                ascii_to_char16(version, ver_unicode, 128);
                simple_printf(u"Model: ");
                simple_printf(ver_unicode);
                simple_printf(u"\r\n");
            }
            
            // CPU Speed
            if (cpu_info->CurrentSpeed > 0) {
                CHAR16 speed_str[16];
                uint_to_string(cpu_info->CurrentSpeed, speed_str, 16);
                simple_printf(u"Current Speed: ");
                simple_printf(speed_str);
                simple_printf(u" MHz\r\n");
            }
            
            if (cpu_info->MaxSpeed > 0) {
                CHAR16 speed_str[16];
                uint_to_string(cpu_info->MaxSpeed, speed_str, 16);
                simple_printf(u"Max Speed: ");
                simple_printf(speed_str);
                simple_printf(u" MHz\r\n");
            }
            
            // Core Count
            if (cpu_info->CoreCount > 0) {
                CHAR16 core_str[16];
                uint_to_string(cpu_info->CoreCount, core_str, 16);
                simple_printf(u"Physical Cores: ");
                simple_printf(core_str);
                simple_printf(u"\r\n");
            }
            
            // Thread Count
            if (cpu_info->ThreadCount > 0) {
                CHAR16 thread_str[16];
                uint_to_string(cpu_info->ThreadCount, thread_str, 16);
                simple_printf(u"Logical Processors: ");
                simple_printf(thread_str);
                simple_printf(u"\r\n");
            }
        }
    }
    
    if (smbios_protocol == NULL && mp_services == NULL) {
        simple_printf(u"CPU information not available (SMBIOS/MP Services not found)\r\n");
    }
}

// Get key from user
EFI_INPUT_KEY get_key(void) {
    EFI_EVENT events[1];
    EFI_INPUT_KEY key;
    UINTN index = 0;

    events[0] = cin->WaitForKey;
    bs->WaitForEvent(1, events, &index);
    cin->ReadKeyStroke(cin, &key);
    return key;
}

// Display enhanced memory information
void display_enhanced_memory_info(void) {
    simple_printf(u"\r\n=== MEMORY INFORMATION ===\r\n");
    
    // Basic memory map information
    EFI_STATUS status;
    UINTN map_size = 0;
    EFI_MEMORY_DESCRIPTOR *memory_map = NULL;
    UINTN map_key;
    UINTN descriptor_size;
    UINT32 descriptor_version;
    
    // First call to get the size needed
    status = bs->GetMemoryMap(&map_size, memory_map, &map_key, &descriptor_size, &descriptor_version);
    
    if (status == EFI_BUFFER_TOO_SMALL) {
        // Allocate buffer
        map_size += 2 * descriptor_size;
        status = bs->AllocatePool(EfiLoaderData, map_size, (VOID**)&memory_map);
        
        if (status == EFI_SUCCESS) {
            status = bs->GetMemoryMap(&map_size, memory_map, &map_key, &descriptor_size, &descriptor_version);
            
            if (status == EFI_SUCCESS) {
                UINTN num_descriptors = map_size / descriptor_size;
                UINT64 total_memory = 0;
                UINT64 usable_memory = 0;
                
                EFI_MEMORY_DESCRIPTOR *desc = memory_map;
                for (UINTN i = 0; i < num_descriptors; i++) {
                    UINT64 memory_size = desc->NumberOfPages * 4096;
                    total_memory += memory_size;
                    
                    if (desc->Type == EfiConventionalMemory || desc->Type == EfiLoaderCode || desc->Type == EfiLoaderData) {
                        usable_memory += memory_size;
                    }
                    
                    desc = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)desc + descriptor_size);
                }
                
                // Display memory in MB and GB
                UINT64 total_mb = total_memory / (1024 * 1024);
                UINT64 total_gb = total_mb / 1024;
                UINT64 usable_mb = usable_memory / (1024 * 1024);
                
                CHAR16 mem_str[32];
                uint_to_string(total_mb, mem_str, 32);
                simple_printf(u"Total System Memory: ");
                simple_printf(mem_str);
                simple_printf(u" MB");
                
                if (total_gb > 0) {
                    uint_to_string(total_gb, mem_str, 32);
                    simple_printf(u" (~");
                    simple_printf(mem_str);
                    simple_printf(u" GB)");
                }
                simple_printf(u"\r\n");
                
                uint_to_string(usable_mb, mem_str, 32);
                simple_printf(u"Usable Memory: ");
                simple_printf(mem_str);
                simple_printf(u" MB\r\n");
            }
            bs->FreePool(memory_map);
        }
    }
    
    // Memory module details from SMBIOS
    if (smbios_protocol != NULL) {
        simple_printf(u"\r\nMemory Modules:\r\n");
        
        EFI_SMBIOS_HANDLE smbios_handle = 0xFFFE;
        EFI_SMBIOS_TYPE smbios_type = SMBIOS_TYPE_MEMORY_DEVICE;
        EFI_SMBIOS_TABLE_HEADER *record = NULL;
        UINTN module_count = 0;
        
        // Find all memory devices
        smbios_handle = 0xFFFE;
        while (smbios_protocol->GetNext(smbios_protocol, &smbios_handle, &smbios_type, &record, NULL) == EFI_SUCCESS) {
            if (record != NULL) {
                SMBIOS_TYPE17_MEMORY_DEVICE *mem_device = (SMBIOS_TYPE17_MEMORY_DEVICE*)record;
                
                // Skip empty slots
                if (mem_device->Size == 0) continue;
                
                module_count++;
                CHAR16 module_str[16];
                uint_to_string(module_count, module_str, 16);
                simple_printf(u"  Module ");
                simple_printf(module_str);
                simple_printf(u": ");
                
                // Memory size
                UINT32 size_mb;
                if (mem_device->Size == 0x7FFF) {
                    size_mb = mem_device->ExtendedSize;
                } else if (mem_device->Size & 0x8000) {
                    size_mb = (mem_device->Size & 0x7FFF) * 1024; // Size in KB
                } else {
                    size_mb = mem_device->Size; // Size in MB
                }
                
                CHAR16 size_str[16];
                uint_to_string(size_mb, size_str, 16);
                simple_printf(size_str);
                simple_printf(u" MB");
                
                // Memory type
                CHAR16 *mem_type = u"Unknown";
                switch (mem_device->MemoryType) {
                    case 24: mem_type = u"DDR3"; break;
                    case 26: mem_type = u"DDR4"; break;
                    case 34: mem_type = u"DDR5"; break;
                    case 18: mem_type = u"DDR"; break;
                    case 20: mem_type = u"DDR2"; break;
                }
                simple_printf(u" ");
                simple_printf(mem_type);
                
                // Memory speed
                if (mem_device->Speed > 0) {
                    CHAR16 speed_str[16];
                    uint_to_string(mem_device->Speed, speed_str, 16);
                    simple_printf(u" @ ");
                    simple_printf(speed_str);
                    simple_printf(u" MHz");
                }
                
                simple_printf(u"\r\n");
            }
        }
        
        if (module_count == 0) {
            simple_printf(u"  No memory modules detected in SMBIOS\r\n");
        }
    }
}

// Display system information
void display_system_info(void) {
    simple_printf(u"\r\n=== SYSTEM INFORMATION ===\r\n");
    
    if (smbios_protocol != NULL) {
        EFI_SMBIOS_HANDLE smbios_handle = 0xFFFE;
        EFI_SMBIOS_TYPE smbios_type = SMBIOS_TYPE_SYSTEM_INFORMATION;
        EFI_SMBIOS_TABLE_HEADER *record = NULL;
        
        EFI_STATUS status = smbios_protocol->GetNext(smbios_protocol, &smbios_handle, &smbios_type, &record, NULL);
        if (status == EFI_SUCCESS && record != NULL) {
            SMBIOS_TYPE1_SYSTEM_INFORMATION *sys_info = (SMBIOS_TYPE1_SYSTEM_INFORMATION*)record;
            
            // System Manufacturer
            CHAR8 *manufacturer = get_smbios_string(record, sys_info->Manufacturer);
            if (manufacturer != NULL) {
                CHAR16 mfg_unicode[64];
                ascii_to_char16(manufacturer, mfg_unicode, 64);
                simple_printf(u"Manufacturer: ");
                simple_printf(mfg_unicode);
                simple_printf(u"\r\n");
            }
            
            // Product Name
            CHAR8 *product = get_smbios_string(record, sys_info->ProductName);
            if (product != NULL) {
                CHAR16 prod_unicode[64];
                ascii_to_char16(product, prod_unicode, 64);
                simple_printf(u"Product Name: ");
                simple_printf(prod_unicode);
                simple_printf(u"\r\n");
            }
            
            // Version
            CHAR8 *version = get_smbios_string(record, sys_info->Version);
            if (version != NULL) {
                CHAR16 ver_unicode[64];
                ascii_to_char16(version, ver_unicode, 64);
                simple_printf(u"Version: ");
                simple_printf(ver_unicode);
                simple_printf(u"\r\n");
            }
            
            // Serial Number
            CHAR8 *serial = get_smbios_string(record, sys_info->SerialNumber);
            if (serial != NULL) {
                CHAR16 ser_unicode[64];
                ascii_to_char16(serial, ser_unicode, 64);
                simple_printf(u"Serial Number: ");
                simple_printf(ser_unicode);
                simple_printf(u"\r\n");
            }
            
            // SKU Number
            if (sys_info->SKUNumber > 0) {
                CHAR8 *sku = get_smbios_string(record, sys_info->SKUNumber);
                if (sku != NULL) {
                    CHAR16 sku_unicode[64];
                    ascii_to_char16(sku, sku_unicode, 64);
                    simple_printf(u"SKU: ");
                    simple_printf(sku_unicode);
                    simple_printf(u"\r\n");
                }
            }
            
            // Family
            if (sys_info->Family > 0) {
                CHAR8 *family = get_smbios_string(record, sys_info->Family);
                if (family != NULL) {
                    CHAR16 fam_unicode[64];
                    ascii_to_char16(family, fam_unicode, 64);
                    simple_printf(u"Family: ");
                    simple_printf(fam_unicode);
                    simple_printf(u"\r\n");
                }
            }
        }
    } else {
        simple_printf(u"System information not available (SMBIOS not found)\r\n");
    }
}

// Display graphics information
void display_graphics_info(void) {
    simple_printf(u"\r\n=== GRAPHICS INFORMATION ===\r\n");
    
    if (gop != NULL) {
        simple_printf(u"Current Display Mode:\r\n");
        
        CHAR16 res_str[16];
        uint_to_string(gop->Mode->Info->HorizontalResolution, res_str, 16);
        simple_printf(u"  Resolution: ");
        simple_printf(res_str);
        simple_printf(u" x ");
        
        uint_to_string(gop->Mode->Info->VerticalResolution, res_str, 16);
        simple_printf(res_str);
        simple_printf(u"\r\n");
        
        // Pixel format
        CHAR16 *pixel_format = u"Unknown";
        switch (gop->Mode->Info->PixelFormat) {
            case PixelRedGreenBlueReserved8BitPerColor:
                pixel_format = u"RGB Reserved 8-bit";
                break;
            case PixelBlueGreenRedReserved8BitPerColor:
                pixel_format = u"BGR Reserved 8-bit";
                break;
            case PixelBitMask:
                pixel_format = u"Bit Mask";
                break;
            case PixelBltOnly:
                pixel_format = u"Blt Only";
                break;
            case PixelFormatMax:
                pixel_format = u"Max Format";
                break;
        }
        simple_printf(u"  Pixel Format: ");
        simple_printf(pixel_format);
        simple_printf(u"\r\n");
        
        // Frame buffer size
        UINT64 fb_size_mb = gop->Mode->FrameBufferSize / (1024 * 1024);
        if (fb_size_mb > 0) {
            CHAR16 fb_str[16];
            uint_to_string(fb_size_mb, fb_str, 16);
            simple_printf(u"  Frame Buffer Size: ");
            simple_printf(fb_str);
            simple_printf(u" MB\r\n");
        }
        
        // Available modes
        CHAR16 mode_str[16];
        uint_to_string(gop->Mode->MaxMode, mode_str, 16);
        simple_printf(u"  Available Modes: ");
        simple_printf(mode_str);
        simple_printf(u"\r\n");
    } else {
        simple_printf(u"Graphics information not available (GOP not found)\r\n");
    }
}

// Display battery information (for laptops)
void display_battery_info(void) {
    simple_printf(u"\r\n=== BATTERY INFORMATION ===\r\n");
    
    if (smbios_protocol != NULL) {
        EFI_SMBIOS_HANDLE smbios_handle = 0xFFFE;
        EFI_SMBIOS_TYPE smbios_type = SMBIOS_TYPE_PORTABLE_BATTERY;
        EFI_SMBIOS_TABLE_HEADER *record = NULL;
        UINTN battery_count = 0;
        
        // Find all batteries
        smbios_handle = 0xFFFE;
        while (smbios_protocol->GetNext(smbios_protocol, &smbios_handle, &smbios_type, &record, NULL) == EFI_SUCCESS) {
            if (record != NULL) {
                SMBIOS_TYPE22_PORTABLE_BATTERY *battery = (SMBIOS_TYPE22_PORTABLE_BATTERY*)record;
                battery_count++;
                
                CHAR16 bat_str[16];
                uint_to_string(battery_count, bat_str, 16);
                simple_printf(u"Battery ");
                simple_printf(bat_str);
                simple_printf(u":\r\n");
                
                // Manufacturer
                CHAR8 *manufacturer = get_smbios_string(record, battery->Manufacturer);
                if (manufacturer != NULL) {
                    CHAR16 mfg_unicode[64];
                    ascii_to_char16(manufacturer, mfg_unicode, 64);
                    simple_printf(u"  Manufacturer: ");
                    simple_printf(mfg_unicode);
                    simple_printf(u"\r\n");
                }
                
                // Device Name
                CHAR8 *device_name = get_smbios_string(record, battery->DeviceName);
                if (device_name != NULL) {
                    CHAR16 name_unicode[64];
                    ascii_to_char16(device_name, name_unicode, 64);
                    simple_printf(u"  Name: ");
                    simple_printf(name_unicode);
                    simple_printf(u"\r\n");
                }
                
                // Design Capacity
                if (battery->DesignCapacity > 0) {
                    CHAR16 cap_str[16];
                    uint_to_string(battery->DesignCapacity, cap_str, 16);
                    simple_printf(u"  Design Capacity: ");
                    simple_printf(cap_str);
                    simple_printf(u" mWh\r\n");
                }
                
                // Chemistry
                CHAR16 *chemistry = u"Unknown";
                switch (battery->DeviceChemistry) {
                    case 1: chemistry = u"Other"; break;
                    case 2: chemistry = u"Unknown"; break;
                    case 3: chemistry = u"Lead Acid"; break;
                    case 4: chemistry = u"Nickel Cadmium"; break;
                    case 5: chemistry = u"Nickel Metal Hydride"; break;
                    case 6: chemistry = u"Lithium Ion"; break;
                    case 7: chemistry = u"Zinc Air"; break;
                    case 8: chemistry = u"Lithium Polymer"; break;
                }
                simple_printf(u"  Chemistry: ");
                simple_printf(chemistry);
                simple_printf(u"\r\n");
            }
        }
        
        if (battery_count == 0) {
            simple_printf(u"No batteries detected\r\n");
        }
    } else {
        simple_printf(u"Battery information not available (SMBIOS not found)\r\n");
    }
}

// Display enhanced firmware information
void display_firmware_info(void) {
    simple_printf(u"=== FIRMWARE INFORMATION ===\r\n");
    simple_printf(u"Firmware Vendor: ");
    simple_printf(st->FirmwareVendor);
    simple_printf(u"\r\nFirmware Revision: ");
    
    // Convert firmware revision to string using our utility function
    CHAR16 rev_str[16];
    uint_to_string(st->FirmwareRevision, rev_str, 16);
    simple_printf(rev_str);
    simple_printf(u"\r\n");
    
    // UEFI Version
    CHAR16 uefi_str[16];
    uint_to_string((st->Hdr.Revision >> 16), uefi_str, 16);
    simple_printf(u"UEFI Version: ");
    simple_printf(uefi_str);
    simple_printf(u".");
    uint_to_string((st->Hdr.Revision & 0xFFFF), uefi_str, 16);
    simple_printf(uefi_str);
    simple_printf(u"\r\n");
}


// Display enhanced storage information
void display_storage_info(void) {
    simple_printf(u"\r\n=== STORAGE INFORMATION ===\r\n");
    
    EFI_GUID block_io_guid = EFI_BLOCK_IO_PROTOCOL_GUID;
    UINTN num_handles = 0;
    EFI_HANDLE *handle_buffer = NULL;
    
    EFI_STATUS status = bs->LocateHandleBuffer(ByProtocol, &block_io_guid, NULL, &num_handles, &handle_buffer);
    
    if (status == EFI_SUCCESS) {
        UINTN disk_count = 0;
        
        for (UINTN i = 0; i < num_handles; i++) {
            EFI_BLOCK_IO_PROTOCOL *block_io;
            status = bs->OpenProtocol(handle_buffer[i], 
                                    &block_io_guid,
                                    (VOID**)&block_io,
                                    image,
                                    NULL,
                                    EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
            
            if (status == EFI_SUCCESS) {
                // Only count actual disks (not partitions)
                if (!block_io->Media->LogicalPartition && block_io->Media->LastBlock > 0) {
                    disk_count++;
                    
                    CHAR16 disk_num[16];
                    uint_to_string(disk_count, disk_num, 16);
                    simple_printf(u"Drive ");
                    simple_printf(disk_num);
                    simple_printf(u":\r\n");
                    
                    // Calculate size in MB and GB
                    UINT64 size_bytes = (block_io->Media->LastBlock + 1) * block_io->Media->BlockSize;
                    UINT64 size_mb = size_bytes / (1024 * 1024);
                    UINT64 size_gb = size_mb / 1024;
                    
                    // Display size
                    CHAR16 size_str[32];
                    uint_to_string(size_mb, size_str, 32);
                    simple_printf(u"  Capacity: ");
                    simple_printf(size_str);
                    simple_printf(u" MB");
                    
                    if (size_gb > 0) {
                        uint_to_string(size_gb, size_str, 32);
                        simple_printf(u" (~");
                        simple_printf(size_str);
                        simple_printf(u" GB)");
                    }
                    simple_printf(u"\r\n");
                    
                    // Block size
                    CHAR16 block_str[16];
                    uint_to_string(block_io->Media->BlockSize, block_str, 16);
                    simple_printf(u"  Block Size: ");
                    simple_printf(block_str);
                    simple_printf(u" bytes\r\n");
                    
                    // Media properties
                    simple_printf(u"  Properties: ");
                    if (block_io->Media->RemovableMedia) {
                        simple_printf(u"Removable ");
                    } else {
                        simple_printf(u"Fixed ");
                    }
                    
                    if (block_io->Media->ReadOnly) {
                        simple_printf(u"Read-Only ");
                    } else {
                        simple_printf(u"Read-Write ");
                    }
                    
                    if (block_io->Media->WriteCaching) {
                        simple_printf(u"Write-Cached");
                    }
                    simple_printf(u"\r\n");
                    
                    // Estimate drive type based on characteristics
                    if (block_io->Media->RemovableMedia) {
                        simple_printf(u"  Type: Removable Media\r\n");
                    } else {
                        // Try to determine if SSD or HDD based on block alignment
                        if (block_io->Media->IoAlign >= 4096 || block_io->Media->BlockSize >= 4096) {
                            simple_printf(u"  Type: Likely SSD (Advanced Format)\r\n");
                        } else {
                            simple_printf(u"  Type: Likely HDD (Legacy Format)\r\n");
                        }
                    }
                    
                    simple_printf(u"\r\n");
                }
                
                bs->CloseProtocol(handle_buffer[i], &block_io_guid, image, NULL);
            }
        }
        
        if (disk_count == 0) {
            simple_printf(u"No storage devices found\r\n");
        }
        
        bs->FreePool(handle_buffer);
    } else {
        simple_printf(u"Failed to enumerate storage devices\r\n");
    }
}

// Main entry point
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    init_globals(ImageHandle, SystemTable);
    
    // Clear screen and set colors
    cout->ClearScreen(cout);
    cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));
    
    simple_printf(u"UEFI Hardware Specification Tool\r\n");
    simple_printf(u"=========================================\r\n\r\n");
    
    // Display comprehensive hardware information
    display_firmware_info();
    display_system_info();
    display_cpu_info();
    display_enhanced_memory_info();
    display_graphics_info();
    display_storage_info();
    display_battery_info();

    simple_printf(u"\r\nPress any key to exit...\r\n");
    get_key();
    
    return EFI_SUCCESS;
}
