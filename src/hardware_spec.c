#include <stdarg.h>
#include "../include/efi.h"
#include "../include/efi_lib.h"

// Scan code definitions
#define SCAN_UP      0x01
#define SCAN_DOWN    0x02
#define SCAN_PAGE_UP 0x09
#define SCAN_PAGE_DOWN 0x0A
#define SCAN_ESC     0x17

// Function declarations
void uint_to_string(UINT64 value, CHAR16 *buffer, UINTN buffer_size);
void simple_strcpy(CHAR16 *dest, CHAR16 *src);
void ascii_to_char16(CHAR8 *ascii, CHAR16 *unicode, UINTN max_len);

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

// Scroll buffer system
#define MAX_OUTPUT_LINES 200
#define SCREEN_LINES 22
CHAR16 *output_buffer[MAX_OUTPUT_LINES];
UINTN total_lines = 0;
UINTN current_scroll = 0;

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
    
    // Initialize output buffer
    for (UINTN i = 0; i < MAX_OUTPUT_LINES; i++) {
        output_buffer[i] = NULL;
    }
    total_lines = 0;
    current_scroll = 0;
}

// Simple printf implementation that stores output in buffer
void simple_printf(CHAR16 *format, ...) {
    if (total_lines < MAX_OUTPUT_LINES) {
        // Allocate memory for this line
        UINTN len = 0;
        CHAR16 *temp = format;
        while (*temp != 0) { len++; temp++; }  // Calculate length
        
        bs->AllocatePool(EfiLoaderData, (len + 1) * sizeof(CHAR16), (VOID**)&output_buffer[total_lines]);
        if (output_buffer[total_lines] != NULL) {
            // Copy string
            for (UINTN i = 0; i <= len; i++) {
                output_buffer[total_lines][i] = format[i];
            }
            total_lines++;
        }
    }
}

// String copy function
void simple_strcpy(CHAR16 *dest, CHAR16 *src) {
    UINTN i = 0;
    while (src[i] != 0) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = 0;
}

// Display scrollable output
void display_scrollable_output(void) {
    cout->ClearScreen(cout);
    cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));
    
    // Calculate end line
    UINTN end_line = current_scroll + SCREEN_LINES;
    if (end_line > total_lines) end_line = total_lines;
    
    // Display lines
    for (UINTN i = current_scroll; i < end_line; i++) {
        if (output_buffer[i] != NULL) {
            cout->OutputString(cout, output_buffer[i]);
        }
    }
    
    // Display scroll info
    if (total_lines > SCREEN_LINES) {
        cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_WHITE, EFI_BLACK));
        cout->SetCursorPosition(cout, 0, SCREEN_LINES);
        
        CHAR16 scroll_info[128];
        CHAR16 current_str[16], total_str[16];
        uint_to_string(current_scroll + 1, current_str, 16);
        uint_to_string(total_lines, total_str, 16);
        
        simple_strcpy(scroll_info, u"Lines ");
        UINTN pos = 6;
        UINTN i = 0;
        while (current_str[i] != 0) scroll_info[pos++] = current_str[i++];
        scroll_info[pos++] = u'-';
        i = 0;
        CHAR16 end_str[16];
        uint_to_string((end_line > total_lines ? total_lines : end_line), end_str, 16);
        while (end_str[i] != 0) scroll_info[pos++] = end_str[i++];
        scroll_info[pos++] = u' ';
        scroll_info[pos++] = u'o';
        scroll_info[pos++] = u'f';
        scroll_info[pos++] = u' ';
        i = 0;
        while (total_str[i] != 0) scroll_info[pos++] = total_str[i++];
        scroll_info[pos++] = u' ';
        scroll_info[pos++] = u'|';
        scroll_info[pos++] = u' ';
        scroll_info[pos++] = u'U';
        scroll_info[pos++] = u'p';
        scroll_info[pos++] = u'/';
        scroll_info[pos++] = u'D';
        scroll_info[pos++] = u'o';
        scroll_info[pos++] = u'w';
        scroll_info[pos++] = u'n';
        scroll_info[pos++] = u' ';
        scroll_info[pos++] = u'A';
        scroll_info[pos++] = u'r';
        scroll_info[pos++] = u'r';
        scroll_info[pos++] = u'o';
        scroll_info[pos++] = u'w';
        scroll_info[pos++] = u's';
        scroll_info[pos++] = u',';
        scroll_info[pos++] = u' ';
        scroll_info[pos++] = u'E';
        scroll_info[pos++] = u'S';
        scroll_info[pos++] = u'C';
        scroll_info[pos++] = u' ';
        scroll_info[pos++] = u't';
        scroll_info[pos++] = u'o';
        scroll_info[pos++] = u' ';
        scroll_info[pos++] = u'e';
        scroll_info[pos++] = u'x';
        scroll_info[pos++] = u'i';
        scroll_info[pos++] = u't';
        scroll_info[pos] = 0;
        
        cout->OutputString(cout, scroll_info);
    }
}

// Handle scroll input
BOOLEAN handle_scroll_input(EFI_INPUT_KEY key) {
    if (total_lines <= SCREEN_LINES) return FALSE;  // No scrolling needed
    
    if (key.ScanCode == SCAN_UP) {
        if (current_scroll > 0) {
            current_scroll--;
            return TRUE;
        }
    } else if (key.ScanCode == SCAN_DOWN) {
        if (current_scroll + SCREEN_LINES < total_lines) {
            current_scroll++;
            return TRUE;
        }
    } else if (key.ScanCode == SCAN_PAGE_UP) {
        if (current_scroll >= SCREEN_LINES) {
            current_scroll -= SCREEN_LINES;
        } else {
            current_scroll = 0;
        }
        return TRUE;
    } else if (key.ScanCode == SCAN_PAGE_DOWN) {
        if (current_scroll + 2 * SCREEN_LINES < total_lines) {
            current_scroll += SCREEN_LINES;
        } else if (current_scroll + SCREEN_LINES < total_lines) {
            current_scroll = total_lines - SCREEN_LINES;
        }
        return TRUE;
    }
    return FALSE;
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
    
    // Navigate to the string area and find the correct string
    while (current_string < string_number) {
        // Skip to next string
        while (*string_ptr != 0) string_ptr++;
        string_ptr++;
        current_string++;
        
        // Check if we've reached the end of strings (double null terminator)
        if (*string_ptr == 0) return NULL;
    }
    
    return (*string_ptr != 0) ? string_ptr : NULL;
}

// Check if a string appears to be a placeholder or default value
BOOLEAN is_placeholder_string(CHAR8 *str) {
    if (str == NULL || *str == 0) return TRUE;
    
    // Check for common placeholder patterns
    if ((*str == '0' && *(str+1) == 0) ||                    // "0"
        (*str == 'N' && *(str+1) == '/' && *(str+2) == 'A') ||   // "N/A"
        (*str == 'T' && *(str+1) == 'o' && *(str+2) == ' ') ||   // "To be filled..."
        (*str == 'n' && *(str+1) == 'o' && *(str+2) == 't') ||   // "not specified"
        (*str == 'D' && *(str+1) == 'e' && *(str+2) == 'f') ||   // "Default"
        (*str == '(' && *(str+1) == 'T' && *(str+2) == 'o') ||   // "(To be filled...)"
        (*str == 'U' && *(str+1) == 'n' && *(str+2) == 'k')) {   // "Unknown"
        return TRUE;
    }
    
    return FALSE;
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
            } else {
                simple_printf(u"Manufacturer: (not available)\r\n");
            }
            
            // Product Name
            CHAR8 *product = get_smbios_string(record, sys_info->ProductName);
            if (product != NULL && !is_placeholder_string(product)) {
                CHAR16 prod_unicode[64];
                ascii_to_char16(product, prod_unicode, 64);
                simple_printf(u"Product Name: ");
                simple_printf(prod_unicode);
                simple_printf(u"\r\n");
            } else {
                simple_printf(u"Product Name: (not available)\r\n");
            }
            
            // Version
            CHAR8 *version = get_smbios_string(record, sys_info->Version);
            if (version != NULL && !is_placeholder_string(version)) {
                CHAR16 ver_unicode[64];
                ascii_to_char16(version, ver_unicode, 64);
                simple_printf(u"Version: ");
                simple_printf(ver_unicode);
                simple_printf(u"\r\n");
            } else {
                simple_printf(u"Version: (not available)\r\n");
            }
            
            // Serial Number (with comprehensive handling)
            if (sys_info->SerialNumber == 0) {
                simple_printf(u"Serial Number: (not provided by system)\r\n");
            } else {
                CHAR8 *serial = get_smbios_string(record, sys_info->SerialNumber);
                if (serial != NULL && !is_placeholder_string(serial)) {
                    CHAR16 ser_unicode[64];
                    ascii_to_char16(serial, ser_unicode, 64);
                    simple_printf(u"Serial Number: ");
                    simple_printf(ser_unicode);
                    simple_printf(u"\r\n");
                } else {
                    simple_printf(u"Serial Number: (not available)\r\n");
                }
            }
            
            // // SKU Number
            // if (sys_info->SKUNumber > 0) {
            //     CHAR8 *sku = get_smbios_string(record, sys_info->SKUNumber);
            //     if (sku != NULL && !is_placeholder_string(sku)) {
            //         CHAR16 sku_unicode[64];
            //         ascii_to_char16(sku, sku_unicode, 64);
            //         simple_printf(u"SKU: ");
            //         simple_printf(sku_unicode);
            //         simple_printf(u"\r\n");
            //     }
            // }
            
            // // Family
            // if (sys_info->Family > 0) {
            //     CHAR8 *family = get_smbios_string(record, sys_info->Family);
            //     if (family != NULL && !is_placeholder_string(family)) {
            //         CHAR16 fam_unicode[64];
            //         ascii_to_char16(family, fam_unicode, 64);
            //         simple_printf(u"Family: ");
            //         simple_printf(fam_unicode);
            //         simple_printf(u"\r\n");
            //     }
            // }
            
            // Display UUID if available
            simple_printf(u"System UUID: ");
            for (int i = 0; i < 16; i++) {
                CHAR16 hex_str[8];
                UINT8 byte_val = sys_info->Uuid[i];
                
                // Convert byte to hex string manually
                CHAR16 hex_chars[] = u"0123456789ABCDEF";
                hex_str[0] = hex_chars[(byte_val >> 4) & 0xF];
                hex_str[1] = hex_chars[byte_val & 0xF];
                hex_str[2] = 0;
                simple_printf(hex_str);
                
                if (i == 3 || i == 5 || i == 7 || i == 9) {
                    simple_printf(u"-");
                }
            }
            simple_printf(u"\r\n");
            
            // Check if this looks like a virtual environment
            if (manufacturer != NULL) {
                // Check for common virtual machine manufacturers
                if ((manufacturer[0] == 'Q' && manufacturer[1] == 'E') ||  // QEMU
                    (manufacturer[0] == 'i' && manufacturer[1] == 'n') ||  // innotek (VirtualBox)
                    (manufacturer[0] == 'V' && manufacturer[1] == 'M') ||  // VMware
                    (manufacturer[0] == 'M' && manufacturer[1] == 'i')) {  // Microsoft (Hyper-V)
                    simple_printf(u"\r\nNote: Virtual machine detected. Hardware serial numbers\r\n");
                    simple_printf(u"      may not be available or may be virtualized.\r\n");
                }
            }
            
        } else {
            simple_printf(u"System information not available (SMBIOS Type 1 not found)\r\n");
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
                    
                    // Enhanced drive type detection with multiple factors
                    if (block_io->Media->RemovableMedia) {
                        simple_printf(u"  Type: Removable Media\r\n");
                    } else {
                        // Advanced SSD/HDD detection based on multiple factors
                        BOOLEAN likely_ssd = FALSE;
                        
                        // Factor 1: Block alignment (SSDs typically use 4KB sectors)
                        if (block_io->Media->IoAlign >= 4096 || block_io->Media->BlockSize >= 4096) {
                            likely_ssd = TRUE;
                        }
                        
                        // Factor 2: Optimal transfer characteristics (SSDs often have specific patterns)
                        if (block_io->Media->OptimalTransferLengthGranularity > 1) {
                            likely_ssd = TRUE;
                        }
                        
                        // Factor 3: Size patterns (common SSD sizes vs HDD sizes)
                        if ((size_gb >= 120 && size_gb <= 128) ||   // 120-128GB SSD
                            (size_gb >= 240 && size_gb <= 256) ||   // 240-256GB SSD
                            (size_gb >= 480 && size_gb <= 512) ||   // 480-512GB SSD
                            (size_gb >= 960 && size_gb <= 1024) ||  // 960GB-1TB SSD
                            (size_gb >= 1920 && size_gb <= 2048)) { // 2TB SSD
                            likely_ssd = TRUE;
                        }
                        
                        // Factor 4: Very small drives are likely SSDs (eMMC, etc.)
                        if (size_gb <= 64) {
                            likely_ssd = TRUE;
                        }
                        
                        if (likely_ssd) {
                            simple_printf(u"  Type: Likely SSD");
                            if (block_io->Media->BlockSize >= 4096) {
                                simple_printf(u" (4K Native)");
                            } else if (block_io->Media->IoAlign >= 4096) {
                                simple_printf(u" (4K Aligned)");
                            }
                            simple_printf(u"\r\n");
                        } else {
                            simple_printf(u"  Type: Likely HDD (Traditional Spinning Disk)\r\n");
                        }
                        
                        // Additional characteristics
                        if (block_io->Media->OptimalTransferLengthGranularity > 1) {
                            CHAR16 opt_str[16];
                            uint_to_string(block_io->Media->OptimalTransferLengthGranularity, opt_str, 16);
                            simple_printf(u"  Optimal Transfer: ");
                            simple_printf(opt_str);
                            simple_printf(u" blocks\r\n");
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
    
    // Try to detect storage controllers for additional drive information
    simple_printf(u"\r\nStorage Controllers:\r\n");
    
    // Check for NVMe controllers (typically SSDs)
    EFI_GUID nvme_guid = {0x52c78312, 0x8edc, 0x4233, 0x98, 0xf2, 0x1a, 0x1a, 0xa5, 0xe3, 0x88, 0xa5};
    UINTN nvme_handles = 0;
    EFI_HANDLE *nvme_buffer = NULL;
    status = bs->LocateHandleBuffer(ByProtocol, &nvme_guid, NULL, &nvme_handles, &nvme_buffer);
    if (status == EFI_SUCCESS && nvme_handles > 0) {
        CHAR16 nvme_str[16];
        uint_to_string(nvme_handles, nvme_str, 16);
        simple_printf(u"  NVMe Controllers: ");
        simple_printf(nvme_str);
        simple_printf(u" (SSD type drives)\r\n");
        bs->FreePool(nvme_buffer);
    }
    
    // Check for ATA controllers  
    EFI_GUID ata_guid = {0x1d3de7f0, 0x0807, 0x424f, 0xaa, 0x69, 0x11, 0xa5, 0x4e, 0x19, 0xa4, 0x6f};
    UINTN ata_handles = 0;
    EFI_HANDLE *ata_buffer = NULL;
    status = bs->LocateHandleBuffer(ByProtocol, &ata_guid, NULL, &ata_handles, &ata_buffer);
    if (status == EFI_SUCCESS && ata_handles > 0) {
        CHAR16 ata_str[16];
        uint_to_string(ata_handles, ata_str, 16);
        simple_printf(u"  ATA Controllers: ");
        simple_printf(ata_str);
        simple_printf(u" (SATA/IDE drives)\r\n");
        bs->FreePool(ata_buffer);
    }
    
    // Check for SCSI controllers
    EFI_GUID scsi_guid = {0xa59e8fcf, 0xbda0, 0x43bb, 0x90, 0xb1, 0xd3, 0x73, 0x2e, 0xca, 0xa8, 0x77};
    UINTN scsi_handles = 0;
    EFI_HANDLE *scsi_buffer = NULL;
    status = bs->LocateHandleBuffer(ByProtocol, &scsi_guid, NULL, &scsi_handles, &scsi_buffer);
    if (status == EFI_SUCCESS && scsi_handles > 0) {
        CHAR16 scsi_str[16];
        uint_to_string(scsi_handles, scsi_str, 16);
        simple_printf(u"  SCSI Controllers: ");
        simple_printf(scsi_str);
        simple_printf(u" (SCSI/SAS drives)\r\n");
        bs->FreePool(scsi_buffer);
    }
    
    if (nvme_handles == 0 && ata_handles == 0 && scsi_handles == 0) {
        simple_printf(u"  No specific storage controllers detected\r\n");
        simple_printf(u"  (Using generic Block I/O protocol only)\r\n");
    }
    
    simple_printf(u"\r\nNote: Drive model detection is limited in UEFI.\r\n");
    simple_printf(u"Boot into OS for detailed drive identification.\r\n");
}

// Main entry point
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    init_globals(ImageHandle, SystemTable);
    
    // Clear screen and set colors
    cout->ClearScreen(cout);
    cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));
    
    simple_printf(u"UEFI Hardware Specification Tool\r\n");
    simple_printf(u"=========================================\r\n\r\n");
    
    // Collect comprehensive hardware information
    // display_firmware_info();
    display_system_info();
    display_cpu_info();
    display_enhanced_memory_info();
    display_graphics_info();
    display_storage_info();
    display_battery_info();

    // Display with scroll functionality
    display_scrollable_output();
    
    // Handle scrolling input
    EFI_INPUT_KEY key;
    while (TRUE) {
        key = get_key();
        
        if (key.ScanCode == SCAN_ESC || key.UnicodeChar == u'q' || key.UnicodeChar == u'Q') {
            break;
        }
        
        if (handle_scroll_input(key)) {
            display_scrollable_output();
        }
    }
    
    // Clean up allocated memory
    for (UINTN i = 0; i < total_lines; i++) {
        if (output_buffer[i] != NULL) {
            bs->FreePool(output_buffer[i]);
        }
    }
    
    return EFI_SUCCESS;
}
