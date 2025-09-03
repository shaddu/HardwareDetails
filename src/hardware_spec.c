#include <stdarg.h>
#include "../include/efi.h"
#include "../include/efi_lib.h"

// Missing GUIDs and types definitions
#define EFI_SIMPLE_NETWORK_PROTOCOL_GUID \
{0xA19832B9,0xAC25,0x11D3,\
0x9A,0x2D,{0x00,0x90,0x27,0x3F,0xC1,0x4D}}

#define EFI_USB_IO_PROTOCOL_GUID \
{0x2B2F68D6,0x0CD2,0x44cf,\
0x8E,0x8B,{0xBB,0xA2,0x0B,0x1B,0x5B,0x75}}

// ATA Pass Thru Protocol GUID
// ATA Pass Thru Protocol structures

// NVMe Pass Thru Protocol structures

// SCSI Pass Thru Protocol structures
typedef struct {
    UINT64 Timeout;
    VOID *DataBuffer;
    VOID *SenseData;
    VOID *Cdb;
    UINT32 TransferLength;
    UINT8 CdbLength;
    UINT8 DataDirection;
    UINT8 HostAdapterStatus;
    UINT8 TargetStatus;
    UINT8 SenseDataLength;
} EFI_SCSI_IO_SCSI_REQUEST_PACKET;

typedef struct {
    UINT64 Revision;
    EFI_STATUS (EFIAPI *GetDeviceType)(VOID *This, UINT8 *DeviceType, UINT8 *DeviceQualifier);
    EFI_STATUS (EFIAPI *GetDeviceLocation)(VOID *This, UINT8 **Target, UINT64 *Lun);
    EFI_STATUS (EFIAPI *ResetBus)(VOID *This);
    EFI_STATUS (EFIAPI *ResetDevice)(VOID *This);
    EFI_STATUS (EFIAPI *ExecuteScsiCommand)(VOID *This, EFI_SCSI_IO_SCSI_REQUEST_PACKET *Packet, EFI_EVENT Event);
    EFI_STATUS (EFIAPI *GetNextTargetLun)(VOID *This, UINT8 **Target, UINT64 *Lun);
    EFI_STATUS (EFIAPI *GetNextTarget)(VOID *This, UINT8 **Target);
} EFI_SCSI_IO_PROTOCOL;

// Network protocol types
typedef struct {
    UINT8 Addr[32];
} EFI_MAC_ADDRESS;

typedef struct {
    UINT8 Addr[16];
} EFI_IP_ADDRESS;

typedef struct {
    UINT64 RxTotalFrames;
    UINT64 RxGoodFrames;
    UINT64 RxUndersizeFrames;
    UINT64 RxOversizeFrames;
    UINT64 RxDroppedFrames;
    UINT64 RxUnicastFrames;
    UINT64 RxBroadcastFrames;
    UINT64 RxMulticastFrames;
    UINT64 RxCrcErrorFrames;
    UINT64 RxTotalBytes;
    UINT64 TxTotalFrames;
    UINT64 TxGoodFrames;
    UINT64 TxUndersizeFrames;
    UINT64 TxOversizeFrames;
    UINT64 TxDroppedFrames;
    UINT64 TxUnicastFrames;
    UINT64 TxBroadcastFrames;
    UINT64 TxMulticastFrames;
    UINT64 TxCrcErrorFrames;
    UINT64 TxTotalBytes;
    UINT64 Collisions;
    UINT64 UnsupportedProtocol;
    UINT64 RxDuplicatedFrames;
    UINT64 RxDecryptErrorFrames;
    UINT64 TxErrorFrames;
    UINT64 TxRetryFrames;
} EFI_NETWORK_STATISTICS;

typedef struct {
    UINT32 State;
    UINT32 HwAddressSize;
    UINT32 MediaHeaderSize;
    UINT32 MaxPacketSize;
    UINT32 NvRamSize;
    UINT32 NvRamAccessSize;
    UINT32 ReceiveFilterMask;
    UINT32 ReceiveFilterSetting;
    UINT32 MaxMCastFilterCount;
    UINT32 MCastFilterCount;
    EFI_MAC_ADDRESS MCastFilter[16];
    EFI_MAC_ADDRESS CurrentAddress;
    EFI_MAC_ADDRESS BroadcastAddress;
    EFI_MAC_ADDRESS PermanentAddress;
    UINT8 IfType;
    BOOLEAN MediaPresentSupported;
    BOOLEAN MediaPresent;
} EFI_SIMPLE_NETWORK_MODE;

// EFI_SIMPLE_NETWORK_PROTOCOL definition
typedef struct {
    UINT64 Revision;
    EFI_STATUS (EFIAPI *Start)(VOID *This);
    EFI_STATUS (EFIAPI *Stop)(VOID *This);
    EFI_STATUS (EFIAPI *Initialize)(VOID *This, UINTN ExtraRxBufferSize, UINTN ExtraTxBufferSize);
    EFI_STATUS (EFIAPI *Reset)(VOID *This, BOOLEAN ExtendedVerification);
    EFI_STATUS (EFIAPI *Shutdown)(VOID *This);
    EFI_STATUS (EFIAPI *ReceiveFilters)(VOID *This, UINT32 Enable, UINT32 Disable, BOOLEAN ResetMCastFilter, UINTN MCastFilterCnt, EFI_MAC_ADDRESS *MCastFilter);
    EFI_STATUS (EFIAPI *StationAddress)(VOID *This, BOOLEAN Reset, EFI_MAC_ADDRESS *New);
    EFI_STATUS (EFIAPI *Statistics)(VOID *This, BOOLEAN Reset, UINTN *StatisticsSize, EFI_NETWORK_STATISTICS *StatisticsTable);
    EFI_STATUS (EFIAPI *MCastIpToMac)(VOID *This, BOOLEAN IPv6, EFI_IP_ADDRESS *IP, EFI_MAC_ADDRESS *MAC);
    EFI_STATUS (EFIAPI *NvData)(VOID *This, BOOLEAN ReadWrite, UINTN Offset, UINTN BufferSize, VOID *Buffer);
    EFI_STATUS (EFIAPI *GetStatus)(VOID *This, UINT32 *InterruptStatus, VOID **TxBuf);
    EFI_STATUS (EFIAPI *Transmit)(VOID *This, UINTN HeaderSize, UINTN BufferSize, VOID *Buffer, EFI_MAC_ADDRESS *SrcAddr, EFI_MAC_ADDRESS *DestAddr, UINT16 *Protocol);
    EFI_STATUS (EFIAPI *Receive)(VOID *This, UINTN *HeaderSize, UINTN *BufferSize, VOID *Buffer, EFI_MAC_ADDRESS *SrcAddr, EFI_MAC_ADDRESS *DestAddr, UINT16 *Protocol);
    EFI_SIMPLE_NETWORK_MODE *Mode;
} EFI_SIMPLE_NETWORK_PROTOCOL;

// USB protocol types
typedef struct {
    UINT8 Length;
    UINT8 DescriptorType;
    UINT16 BcdUSB;
    UINT8 DeviceClass;
    UINT8 DeviceSubClass;
    UINT8 DeviceProtocol;
    UINT8 MaxPacketSize0;
    UINT16 IdVendor;
    UINT16 IdProduct;
    UINT16 BcdDevice;
    UINT8 Manufacturer;
    UINT8 Product;
    UINT8 SerialNumber;
    UINT8 NumConfigurations;
} EFI_USB_DEVICE_DESCRIPTOR;

typedef struct {
    UINT8 Length;
    UINT8 DescriptorType;
    UINT16 TotalLength;
    UINT8 NumInterfaces;
    UINT8 ConfigurationValue;
    UINT8 Configuration;
    UINT8 Attributes;
    UINT8 MaxPower;
} EFI_USB_CONFIG_DESCRIPTOR;

typedef struct {
    UINT8 Length;
    UINT8 DescriptorType;
    UINT8 InterfaceNumber;
    UINT8 AlternateSetting;
    UINT8 NumEndpoints;
    UINT8 InterfaceClass;
    UINT8 InterfaceSubClass;
    UINT8 InterfaceProtocol;
    UINT8 Interface;
} EFI_USB_INTERFACE_DESCRIPTOR;

typedef struct {
    UINT8 Length;
    UINT8 DescriptorType;
    UINT8 EndpointAddress;
    UINT8 Attributes;
    UINT16 MaxPacketSize;
    UINT8 Interval;
} EFI_USB_ENDPOINT_DESCRIPTOR;

typedef struct {
    UINT64 Revision;
    EFI_STATUS (EFIAPI *UsbGetDeviceDescriptor)(VOID *This, EFI_USB_DEVICE_DESCRIPTOR *DeviceDescriptor);
    EFI_STATUS (EFIAPI *UsbGetConfigDescriptor)(VOID *This, EFI_USB_CONFIG_DESCRIPTOR *ConfigurationDescriptor);
    EFI_STATUS (EFIAPI *UsbGetInterfaceDescriptor)(VOID *This, EFI_USB_INTERFACE_DESCRIPTOR *InterfaceDescriptor);
    EFI_STATUS (EFIAPI *UsbGetEndpointDescriptor)(VOID *This, UINT8 EndpointIndex, EFI_USB_ENDPOINT_DESCRIPTOR *EndpointDescriptor);
    EFI_STATUS (EFIAPI *UsbGetStringDescriptor)(VOID *This, UINT16 LangID, UINT8 StringID, CHAR16 *String);
    EFI_STATUS (EFIAPI *UsbGetSupportedLanguages)(VOID *This, UINT16 **LangIDTable, UINT16 *TableSize);
    EFI_STATUS (EFIAPI *UsbPortReset)(VOID *This);
} EFI_USB_IO_PROTOCOL;

// SMBIOS Type 9 - System Slots
typedef struct {
    UINT8  HeaderType;
    UINT8  Length;
    UINT16 Handle;
    UINT8  SlotDesignation;
    UINT8  SlotType;
    UINT8  SlotDataBusWidth;
    UINT8  CurrentUsage;
    UINT8  SlotLength;
    UINT16 SlotID;
    UINT8  SlotCharacteristics1;
    UINT8  SlotCharacteristics2;
    UINT16 SegmentGroupNum;
    UINT8  BusNum;
    UINT8  DevFuncNum;
    UINT8  DataBusWidth;
    UINT8  PeerGroupingCount;
    UINT8  PeerGroups[1];
} SMBIOS_TYPE9_SYSTEM_SLOTS;

// SMBIOS Type 0 - BIOS Information
typedef struct {
    UINT8  HeaderType;
    UINT8  Length;
    UINT16 Handle;
    UINT8  Vendor;
    UINT8  BiosVersion;
    UINT16 BiosStartingAddressSegment;
    UINT8  BiosReleaseDate;
    UINT8  BiosRomSize;
    UINT64 BiosCharacteristics;
    UINT8  BiosCharacteristicsExtensionBytes[2];
    UINT8  SystemBiosMajorRelease;
    UINT8  SystemBiosMinorRelease;
    UINT8  EmbeddedControllerFirmwareMajorRelease;
    UINT8  EmbeddedControllerFirmwareMinorRelease;
    UINT16 ExtendedBiosRomSize;
} SMBIOS_TYPE0_BIOS_INFORMATION;

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

// Scroll buffer system - removed, now outputs directly
#define MAX_OUTPUT_LINES 200
#define SCREEN_LINES 22
UINTN SCREEN_LINES_VAR = 22;  // Dynamic variable for screen lines
// Removed: CHAR16 *output_buffer[MAX_OUTPUT_LINES];
// Removed: UINTN total_lines = 0;
// Removed: UINTN current_scroll = 0;

// Get screen dimensions and calculate split positions
void get_screen_dimensions(UINTN *cols, UINTN *rows) {
    if (cout == NULL) {
        *cols = 80;
        *rows = 25;
        return;
    }

    EFI_STATUS status = cout->QueryMode(cout, cout->Mode->Mode, cols, rows);
    if (status != EFI_SUCCESS) {
        *cols = 80;
        *rows = 25;
    }
}

// Set cursor position
void set_cursor_position(UINTN col, UINTN row) {
    if (cout != NULL) {
        cout->SetCursorPosition(cout, col, row);
    }
}

// Clear screen region (from row_start to row_end)
void clear_screen_region(UINTN row_start, UINTN row_end) {
    if (cout == NULL) return;

    UINTN cols, rows;
    get_screen_dimensions(&cols, &rows);

    for (UINTN row = row_start; row <= row_end && row < rows; row++) {
        set_cursor_position(0, row);
        for (UINTN col = 0; col < cols; col++) {
            simple_printf(u" ");
        }
    }
}

// Initialize global variables and protocols
void init_globals(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    cout = SystemTable->ConOut;
    cin = SystemTable->ConIn;
    st = SystemTable;
    bs = st->BootServices;
    rs = st->RuntimeServices;
    image = ImageHandle;
    
    // Set the largest available text mode for maximum screen space
    set_largest_text_mode();
    
    // Locate SMBIOS protocol
    EFI_GUID smbios_guid = EFI_SMBIOS_PROTOCOL_GUID;
    bs->LocateProtocol(&smbios_guid, NULL, (VOID**)&smbios_protocol);
    
    // Locate MP Services protocol
    EFI_GUID mp_guid = EFI_MP_SERVICES_PROTOCOL_GUID;
    bs->LocateProtocol(&mp_guid, NULL, (VOID**)&mp_services);
    
    // Locate Graphics Output Protocol
    EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    bs->LocateProtocol(&gop_guid, NULL, (VOID**)&gop);
    
    // Initialize output buffer - removed, now outputs directly
    // Removed: for (UINTN i = 0; i < MAX_OUTPUT_LINES; i++) {
    // Removed:     output_buffer[i] = NULL;
    // Removed: }
    // Removed: total_lines = 0;
    // Removed: current_scroll = 0;
}

// Simple printf implementation that outputs directly to console
void simple_printf(CHAR16 *format, ...) {
    cout->OutputString(cout, format);
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

// Set the largest available text mode
void set_largest_text_mode(void) {
    if (cout != NULL) {
        // Try to set the largest text mode available
        UINTN max_mode = cout->Mode->MaxMode;
        for (UINTN mode = max_mode - 1; mode > 0; mode--) {
            EFI_STATUS status = cout->SetMode(cout, mode);
            if (status == EFI_SUCCESS) {
                break; // Successfully set the mode
            }
        }
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

// Get drive serial number and manufacturer using ATA IDENTIFY DEVICE command
EFI_STATUS get_ata_drive_info(EFI_HANDLE handle, CHAR16 *serial_buffer, UINTN serial_size, CHAR16 *model_buffer, UINTN model_size) {
    EFI_STATUS status;
    EFI_ATA_PASS_THRU_PROTOCOL *ata_pass_thru = NULL;
    EFI_GUID ata_guid = EFI_ATA_PASS_THRU_PROTOCOL_GUID;

    // Try to get ATA Pass Thru protocol
    status = bs->OpenProtocol(handle, &ata_guid, (VOID**)&ata_pass_thru, image, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    if (status != EFI_SUCCESS) {
        return status;
    }

    // Allocate buffer for IDENTIFY DEVICE data (512 bytes)
    UINT8 *identify_data = NULL;
    status = bs->AllocatePool(EfiLoaderData, 512, (VOID**)&identify_data);
    if (status != EFI_SUCCESS) {
        bs->CloseProtocol(handle, &ata_guid, image, NULL);
        return status;
    }

    // Set up ATA command packet
    EFI_ATA_PASS_THRU_COMMAND_PACKET packet;
    memset(&packet, 0, sizeof(packet));

    packet.InTransferLength = 1;  // 1 sector
    packet.OutTransferLength = 0;
    packet.SectorCount = 1;

    // ATA IDENTIFY DEVICE command
    packet.HostCommand[0] = 0xEC;  // IDENTIFY DEVICE
    packet.HostCommand[1] = 0;
    packet.HostCommand[2] = 0;
    packet.HostCommand[3] = 0;
    packet.HostCommand[4] = 0;
    packet.HostCommand[5] = 0;
    packet.HostCommand[6] = 0;
    packet.HostCommand[7] = 0;
    packet.HostCommand[8] = 0;
    packet.HostCommand[9] = 0;
    packet.HostCommand[10] = 0;
    packet.HostCommand[11] = 0;

    // Execute command
    status = ata_pass_thru->PassThru(ata_pass_thru, 0, 0, &packet, NULL);

    if (status == EFI_SUCCESS) {
        // For ATA Pass Thru, we need to read the data using a different method
        // Let's try to use the BLOCK_IO protocol to read the IDENTIFY data
        EFI_BLOCK_IO_PROTOCOL *block_io = NULL;
        EFI_GUID block_guid = EFI_BLOCK_IO_PROTOCOL_GUID;

        if (bs->OpenProtocol(handle, &block_guid, (VOID**)&block_io, image, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL) == EFI_SUCCESS) {
            // Try to read the IDENTIFY data from the drive
            // This is a simplified approach - in practice, this might not work for all drives
            status = block_io->ReadBlocks(block_io, block_io->Media->MediaId, 0, 512, identify_data);

            if (status == EFI_SUCCESS) {
                // Parse IDENTIFY DEVICE data
                // Serial number is at offset 20-39 (20 bytes, little endian words)
                UINTN serial_offset = 20;
                UINTN serial_len = 0;

                for (UINTN i = 0; i < 20 && serial_len < serial_size - 1; i += 2) {
                    UINT8 byte1 = identify_data[serial_offset + i];
                    UINT8 byte2 = identify_data[serial_offset + i + 1];

                    if (byte1 != 0 && byte1 != ' ') {
                        serial_buffer[serial_len++] = byte1;
                    }
                    if (byte2 != 0 && byte2 != ' ') {
                        serial_buffer[serial_len++] = byte2;
                    }
                }
                serial_buffer[serial_len] = 0;

                // Model number is at offset 54-93 (40 bytes, little endian words)
                UINTN model_offset = 54;
                UINTN model_len = 0;

                for (UINTN i = 0; i < 40 && model_len < model_size - 1; i += 2) {
                    UINT8 byte1 = identify_data[model_offset + i];
                    UINT8 byte2 = identify_data[model_offset + i + 1];

                    if (byte1 != 0 && byte1 != ' ') {
                        model_buffer[model_len++] = byte1;
                    }
                    if (byte2 != 0 && byte2 != ' ') {
                        model_buffer[model_len++] = byte2;
                    }
                }
                model_buffer[model_len] = 0;
            }

            bs->CloseProtocol(handle, &block_guid, image, NULL);
        }
    }

    bs->FreePool(identify_data);
    bs->CloseProtocol(handle, &ata_guid, image, NULL);
    return status;
}

// Get drive serial number and manufacturer using NVMe Identify Controller command
EFI_STATUS get_nvme_drive_info(EFI_HANDLE handle, CHAR16 *serial_buffer, UINTN serial_size, CHAR16 *model_buffer, UINTN model_size) {
    EFI_STATUS status;
    EFI_NVME_PASS_THRU_PROTOCOL *nvme_pass_thru = NULL;
    EFI_GUID nvme_guid = EFI_NVME_PASS_THRU_PROTOCOL_GUID;

    // Try to get NVMe Pass Thru protocol
    status = bs->OpenProtocol(handle, &nvme_guid, (VOID**)&nvme_pass_thru, image, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    if (status != EFI_SUCCESS) {
        return status;
    }

    // Allocate buffer for Identify Controller data (4096 bytes)
    UINT8 *identify_data = NULL;
    status = bs->AllocatePool(EfiLoaderData, 4096, (VOID**)&identify_data);
    if (status != EFI_SUCCESS) {
        bs->CloseProtocol(handle, &nvme_guid, image, NULL);
        return status;
    }

    // Set up NVMe command
    EFI_NVME_COMMAND nvme_command;
    memset(&nvme_command, 0, sizeof(nvme_command));

    EFI_NVME_COMPLETION nvme_completion;
    memset(&nvme_completion, 0, sizeof(nvme_completion));

    nvme_command.Opcode = 0x06;  // Identify Controller
    nvme_command.Nsid = 0xFFFFFFFF;  // Controller NSID

    // Set up PRP (Physical Region Page)
    EFI_NVME_PASS_THRU_COMMAND_PACKET packet;
    memset(&packet, 0, sizeof(packet));

    packet.QueueType = 0;  // Admin queue
    packet.QueueId = 0;
    packet.NvmeCmd = &nvme_command;
    packet.NvmeCompletion = &nvme_completion;
    packet.CommandTimeout = 0;
    packet.TransferBuffer = identify_data;
    packet.TransferLength = 4096;
    packet.MetadataBuffer = NULL;
    packet.MetadataLength = 0;

    // Execute command
    status = nvme_pass_thru->PassThru(nvme_pass_thru, 0, &packet, NULL);

    if (status == EFI_SUCCESS) {
        // Parse Identify Controller data
        // Serial number is at offset 4-23 (20 bytes)
        UINTN serial_len = 0;
        for (UINTN i = 4; i < 24 && serial_len < serial_size - 1; i++) {
            if (identify_data[i] != 0 && identify_data[i] != ' ') {
                serial_buffer[serial_len++] = identify_data[i];
            }
        }
        serial_buffer[serial_len] = 0;

        // Model number is at offset 24-63 (40 bytes)
        UINTN model_len = 0;
        for (UINTN i = 24; i < 64 && model_len < model_size - 1; i++) {
            if (identify_data[i] != 0 && identify_data[i] != ' ') {
                model_buffer[model_len++] = identify_data[i];
            }
        }
        model_buffer[model_len] = 0;
    }

    bs->FreePool(identify_data);
    bs->CloseProtocol(handle, &nvme_guid, image, NULL);
    return status;
}

// Get drive manufacturer from PCI vendor ID
EFI_STATUS get_drive_manufacturer_from_pci(EFI_HANDLE handle, CHAR16 *manufacturer_buffer, UINTN buffer_size) {
    EFI_STATUS status;
    EFI_PCI_IO_PROTOCOL *pci_io = NULL;
    EFI_GUID pci_guid = EFI_PCI_IO_PROTOCOL_GUID;

    status = bs->OpenProtocol(handle, &pci_guid, (VOID**)&pci_io, image, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    if (status != EFI_SUCCESS) {
        return status;
    }

    // Read PCI configuration space to get vendor and device IDs
    UINT16 vendor_id = 0;
    UINT16 device_id = 0;

    status = pci_io->Pci.Read(pci_io, EfiPciIoWidthUint16, 0, 1, &vendor_id);
    if (status != EFI_SUCCESS) {
        bs->CloseProtocol(handle, &pci_guid, image, NULL);
        return status;
    }

    status = pci_io->Pci.Read(pci_io, EfiPciIoWidthUint16, 2, 1, &device_id);
    if (status != EFI_SUCCESS) {
        bs->CloseProtocol(handle, &pci_guid, image, NULL);
        return status;
    }

    // Map vendor ID to manufacturer name
    CHAR16 *manufacturer = u"Unknown";

    switch (vendor_id) {
        case 0x8086: manufacturer = u"Intel"; break;
        case 0x10DE: manufacturer = u"NVIDIA"; break;
        case 0x1002: manufacturer = u"AMD"; break;
        case 0x1022: manufacturer = u"AMD"; break;
        case 0x1B4B: manufacturer = u"Marvell"; break;
        case 0x144D: manufacturer = u"Samsung"; break;
        case 0x15B7: manufacturer = u"Sandisk"; break;
        case 0x1179: manufacturer = u"Toshiba"; break;
        case 0x1C5C: manufacturer = u"SK Hynix"; break;
        case 0x1987: manufacturer = u"Phison"; break;
        case 0x1CC1: manufacturer = u"ADATA"; break;
        case 0x1E0F: manufacturer = u"KIOXIA"; break;
        case 0x1E49: manufacturer = u"Union Memory"; break;
        case 0x1E4B: manufacturer = u"Maxio"; break;
        case 0x1F40: manufacturer = u"Netac"; break;
        case 0x2646: manufacturer = u"Kingston"; break;
        case 0x2C23: manufacturer = u"China Flash"; break;
        case 0x5555: manufacturer = u"Apacer"; break;
        case 0x5E83: manufacturer = u"WDC"; break;
        case 0x62C7: manufacturer = u"Shenzhen Longsys"; break;
        case 0x6469: manufacturer = u"KingDian"; break;
        case 0x67B3: manufacturer = u"Shenzhen Longsys"; break;
        case 0x6B7F: manufacturer = u"Silicon Motion"; break;
        case 0x76B8: manufacturer = u"JMicron"; break;
        case 0x7A67: manufacturer = u"Biwin"; break;
        case 0x7C51: manufacturer = u"Biwin"; break;
        case 0x7E56: manufacturer = u"OCZ"; break;
        case 0x7F51: manufacturer = u"Biwin"; break;
        case 0x7FB8: manufacturer = u"JMicron"; break;
        case 0x8087: manufacturer = u"Intel"; break;
        case 0x8088: manufacturer = u"Intel"; break;
        case 0x8089: manufacturer = u"Intel"; break;
        case 0x808A: manufacturer = u"Intel"; break;
        case 0x808B: manufacturer = u"Intel"; break;
        case 0x808C: manufacturer = u"Intel"; break;
        case 0x808D: manufacturer = u"Intel"; break;
        case 0x808E: manufacturer = u"Intel"; break;
        case 0x808F: manufacturer = u"Intel"; break;
        case 0x80EE: manufacturer = u"VirtualBox"; break;
        case 0x80EF: manufacturer = u"VirtualBox"; break;
        case 0x80F0: manufacturer = u"VirtualBox"; break;
        case 0x80F1: manufacturer = u"VirtualBox"; break;
        case 0x80F2: manufacturer = u"VirtualBox"; break;
        case 0x80F3: manufacturer = u"VirtualBox"; break;
        case 0x80F4: manufacturer = u"VirtualBox"; break;
        case 0x80F5: manufacturer = u"VirtualBox"; break;
        case 0x80F6: manufacturer = u"VirtualBox"; break;
        case 0x80F7: manufacturer = u"VirtualBox"; break;
        case 0x80F8: manufacturer = u"VirtualBox"; break;
        case 0x80F9: manufacturer = u"VirtualBox"; break;
        case 0x80FA: manufacturer = u"VirtualBox"; break;
        case 0x80FB: manufacturer = u"VirtualBox"; break;
        case 0x80FC: manufacturer = u"VirtualBox"; break;
        case 0x80FD: manufacturer = u"VirtualBox"; break;
        case 0x80FE: manufacturer = u"VirtualBox"; break;
        case 0x80FF: manufacturer = u"VirtualBox"; break;
        case 0x8E89: manufacturer = u"VMware"; break;
        case 0x8E8A: manufacturer = u"VMware"; break;
        case 0x8E8B: manufacturer = u"VMware"; break;
        case 0x8E8C: manufacturer = u"VMware"; break;
        case 0x8E8D: manufacturer = u"VMware"; break;
        case 0x8E8E: manufacturer = u"VMware"; break;
        case 0x8E8F: manufacturer = u"VMware"; break;
        case 0x8E90: manufacturer = u"VMware"; break;
        case 0x8E91: manufacturer = u"VMware"; break;
        case 0x8E92: manufacturer = u"VMware"; break;
        case 0x8E93: manufacturer = u"VMware"; break;
        case 0x8E94: manufacturer = u"VMware"; break;
        case 0x8E95: manufacturer = u"VMware"; break;
        case 0x8E96: manufacturer = u"VMware"; break;
        case 0x8E97: manufacturer = u"VMware"; break;
        case 0x8E98: manufacturer = u"VMware"; break;
        case 0x8E99: manufacturer = u"VMware"; break;
        case 0x8E9A: manufacturer = u"VMware"; break;
        case 0x8E9B: manufacturer = u"VMware"; break;
        case 0x8E9C: manufacturer = u"VMware"; break;
        case 0x8E9D: manufacturer = u"VMware"; break;
        case 0x8E9E: manufacturer = u"VMware"; break;
        case 0x8E9F: manufacturer = u"VMware"; break;
        case 0x9710: manufacturer = u"NETAPP"; break;
        case 0x9D32: manufacturer = u"Beijing Starblaze"; break;
        case 0xA044: manufacturer = u"NT-ware"; break;
        case 0xA0F8: manufacturer = u"HP"; break;
        case 0xA1E7: manufacturer = u"Quanta"; break;
        case 0xA208: manufacturer = u"Lenovo"; break;
        case 0xA328: manufacturer = u"SYBA"; break;
        case 0xA35E: manufacturer = u"ASUS"; break;
        case 0xA3D1: manufacturer = u"Intel"; break;
        case 0xA42A: manufacturer = u"ASMedia"; break;
        case 0xA4E7: manufacturer = u"OnStream"; break;
        case 0xA559: manufacturer = u"Intel"; break;
        case 0xA65F: manufacturer = u"Trident"; break;
        case 0xA68E: manufacturer = u"Intel"; break;
        case 0xA704: manufacturer = u"Intel"; break;
        case 0xA71A: manufacturer = u"Intel"; break;
        case 0xA71B: manufacturer = u"Intel"; break;
        case 0xA71C: manufacturer = u"Intel"; break;
        case 0xA71D: manufacturer = u"Intel"; break;
        case 0xA71E: manufacturer = u"Intel"; break;
        case 0xA71F: manufacturer = u"Intel"; break;
        case 0xA720: manufacturer = u"Intel"; break;
        case 0xA721: manufacturer = u"Intel"; break;
        case 0xA722: manufacturer = u"Intel"; break;
        case 0xA723: manufacturer = u"Intel"; break;
        case 0xA724: manufacturer = u"Intel"; break;
        case 0xA725: manufacturer = u"Intel"; break;
        case 0xA726: manufacturer = u"Intel"; break;
        case 0xA727: manufacturer = u"Intel"; break;
        case 0xA728: manufacturer = u"Intel"; break;
        case 0xA729: manufacturer = u"Intel"; break;
        case 0xA72A: manufacturer = u"Intel"; break;
        case 0xA72B: manufacturer = u"Intel"; break;
        case 0xA72C: manufacturer = u"Intel"; break;
        case 0xA72D: manufacturer = u"Intel"; break;
        case 0xA72E: manufacturer = u"Intel"; break;
        case 0xA72F: manufacturer = u"Intel"; break;
        case 0xA730: manufacturer = u"Intel"; break;
        case 0xA731: manufacturer = u"Intel"; break;
        case 0xA732: manufacturer = u"Intel"; break;
        case 0xA733: manufacturer = u"Intel"; break;
        case 0xA734: manufacturer = u"Intel"; break;
        case 0xA735: manufacturer = u"Intel"; break;
        case 0xA736: manufacturer = u"Intel"; break;
        case 0xA737: manufacturer = u"Intel"; break;
        case 0xA738: manufacturer = u"Intel"; break;
        case 0xA739: manufacturer = u"Intel"; break;
        case 0xA73A: manufacturer = u"Intel"; break;
        case 0xA73B: manufacturer = u"Intel"; break;
        case 0xA73C: manufacturer = u"Intel"; break;
        case 0xA73D: manufacturer = u"Intel"; break;
        case 0xA73E: manufacturer = u"Intel"; break;
        case 0xA73F: manufacturer = u"Intel"; break;
        case 0xA740: manufacturer = u"Intel"; break;
        case 0xA741: manufacturer = u"Intel"; break;
        case 0xA742: manufacturer = u"Intel"; break;
        case 0xA743: manufacturer = u"Intel"; break;
        case 0xA744: manufacturer = u"Intel"; break;
        case 0xA745: manufacturer = u"Intel"; break;
        case 0xA746: manufacturer = u"Intel"; break;
        case 0xA747: manufacturer = u"Intel"; break;
        case 0xA748: manufacturer = u"Intel"; break;
        case 0xA749: manufacturer = u"Intel"; break;
        case 0xA74A: manufacturer = u"Intel"; break;
        case 0xA74B: manufacturer = u"Intel"; break;
        case 0xA74C: manufacturer = u"Intel"; break;
        case 0xA74D: manufacturer = u"Intel"; break;
        case 0xA74E: manufacturer = u"Intel"; break;
        case 0xA74F: manufacturer = u"Intel"; break;
        case 0xA750: manufacturer = u"Intel"; break;
        case 0xA751: manufacturer = u"Intel"; break;
        case 0xA752: manufacturer = u"Intel"; break;
        case 0xA753: manufacturer = u"Intel"; break;
        case 0xA754: manufacturer = u"Intel"; break;
        case 0xA755: manufacturer = u"Intel"; break;
        case 0xA756: manufacturer = u"Intel"; break;
        case 0xA757: manufacturer = u"Intel"; break;
        case 0xA758: manufacturer = u"Intel"; break;
        case 0xA759: manufacturer = u"Intel"; break;
        case 0xA75A: manufacturer = u"Intel"; break;
        case 0xA75B: manufacturer = u"Intel"; break;
        case 0xA75C: manufacturer = u"Intel"; break;
        case 0xA75D: manufacturer = u"Intel"; break;
        case 0xA75E: manufacturer = u"Intel"; break;
        case 0xA75F: manufacturer = u"Intel"; break;
        case 0xA760: manufacturer = u"Intel"; break;
        case 0xA761: manufacturer = u"Intel"; break;
        case 0xA762: manufacturer = u"Intel"; break;
        case 0xA763: manufacturer = u"Intel"; break;
        case 0xA764: manufacturer = u"Intel"; break;
        case 0xA765: manufacturer = u"Intel"; break;
        case 0xA766: manufacturer = u"Intel"; break;
        case 0xA767: manufacturer = u"Intel"; break;
        case 0xA768: manufacturer = u"Intel"; break;
        case 0xA769: manufacturer = u"Intel"; break;
        case 0xA76A: manufacturer = u"Intel"; break;
        case 0xA76B: manufacturer = u"Intel"; break;
        case 0xA76C: manufacturer = u"Intel"; break;
        case 0xA76D: manufacturer = u"Intel"; break;
        case 0xA76E: manufacturer = u"Intel"; break;
        case 0xA76F: manufacturer = u"Intel"; break;
        case 0xA770: manufacturer = u"Intel"; break;
        case 0xA771: manufacturer = u"Intel"; break;
        case 0xA772: manufacturer = u"Intel"; break;
        case 0xA773: manufacturer = u"Intel"; break;
        case 0xA774: manufacturer = u"Intel"; break;
        case 0xA775: manufacturer = u"Intel"; break;
        case 0xA776: manufacturer = u"Intel"; break;
        case 0xA777: manufacturer = u"Intel"; break;
        case 0xA778: manufacturer = u"Intel"; break;
        case 0xA779: manufacturer = u"Intel"; break;
        case 0xA77A: manufacturer = u"Intel"; break;
        case 0xA77B: manufacturer = u"Intel"; break;
        case 0xA77C: manufacturer = u"Intel"; break;
        case 0xA77D: manufacturer = u"Intel"; break;
        case 0xA77E: manufacturer = u"Intel"; break;
        case 0xA77F: manufacturer = u"Intel"; break;
        case 0xA780: manufacturer = u"Intel"; break;
        case 0xA781: manufacturer = u"Intel"; break;
        case 0xA782: manufacturer = u"Intel"; break;
        case 0xA783: manufacturer = u"Intel"; break;
        case 0xA784: manufacturer = u"Intel"; break;
        case 0xA785: manufacturer = u"Intel"; break;
        case 0xA786: manufacturer = u"Intel"; break;
        case 0xA787: manufacturer = u"Intel"; break;
        case 0xA788: manufacturer = u"Intel"; break;
        case 0xA789: manufacturer = u"Intel"; break;
        case 0xA78A: manufacturer = u"Intel"; break;
        case 0xA78B: manufacturer = u"Intel"; break;
        case 0xA78C: manufacturer = u"Intel"; break;
        case 0xA78D: manufacturer = u"Intel"; break;
        case 0xA78E: manufacturer = u"Intel"; break;
        case 0xA78F: manufacturer = u"Intel"; break;
        case 0xA790: manufacturer = u"Intel"; break;
        case 0xA791: manufacturer = u"Intel"; break;
        case 0xA792: manufacturer = u"Intel"; break;
        case 0xA793: manufacturer = u"Intel"; break;
        case 0xA794: manufacturer = u"Intel"; break;
        case 0xA795: manufacturer = u"Intel"; break;
        case 0xA796: manufacturer = u"Intel"; break;
        case 0xA797: manufacturer = u"Intel"; break;
        case 0xA798: manufacturer = u"Intel"; break;
        case 0xA799: manufacturer = u"Intel"; break;
        case 0xA79A: manufacturer = u"Intel"; break;
        case 0xA79B: manufacturer = u"Intel"; break;
        case 0xA79C: manufacturer = u"Intel"; break;
        case 0xA79D: manufacturer = u"Intel"; break;
        case 0xA79E: manufacturer = u"Intel"; break;
        case 0xA79F: manufacturer = u"Intel"; break;
        case 0xA7A0: manufacturer = u"Intel"; break;
        case 0xA7A1: manufacturer = u"Intel"; break;
        case 0xA7A2: manufacturer = u"Intel"; break;
        case 0xA7A3: manufacturer = u"Intel"; break;
        case 0xA7A4: manufacturer = u"Intel"; break;
        case 0xA7A5: manufacturer = u"Intel"; break;
        case 0xA7A6: manufacturer = u"Intel"; break;
        case 0xA7A7: manufacturer = u"Intel"; break;
        case 0xA7A8: manufacturer = u"Intel"; break;
        case 0xA7A9: manufacturer = u"Intel"; break;
        case 0xA7AA: manufacturer = u"Intel"; break;
        case 0xA7AB: manufacturer = u"Intel"; break;
        case 0xA7AC: manufacturer = u"Intel"; break;
        case 0xA7AD: manufacturer = u"Intel"; break;
        case 0xA7AE: manufacturer = u"Intel"; break;
        case 0xA7AF: manufacturer = u"Intel"; break;
        case 0xA7B0: manufacturer = u"Intel"; break;
        case 0xA7B1: manufacturer = u"Intel"; break;
        case 0xA7B2: manufacturer = u"Intel"; break;
        case 0xA7B3: manufacturer = u"Intel"; break;
        case 0xA7B4: manufacturer = u"Intel"; break;
        case 0xA7B5: manufacturer = u"Intel"; break;
        case 0xA7B6: manufacturer = u"Intel"; break;
        case 0xA7B7: manufacturer = u"Intel"; break;
        case 0xA7B8: manufacturer = u"Intel"; break;
        case 0xA7B9: manufacturer = u"Intel"; break;
        case 0xA7BA: manufacturer = u"Intel"; break;
        case 0xA7BB: manufacturer = u"Intel"; break;
        case 0xA7BC: manufacturer = u"Intel"; break;
        case 0xA7BD: manufacturer = u"Intel"; break;
        case 0xA7BE: manufacturer = u"Intel"; break;
        case 0xA7BF: manufacturer = u"Intel"; break;
        case 0xA7C0: manufacturer = u"Intel"; break;
        case 0xA7C1: manufacturer = u"Intel"; break;
        case 0xA7C2: manufacturer = u"Intel"; break;
        case 0xA7C3: manufacturer = u"Intel"; break;
        case 0xA7C4: manufacturer = u"Intel"; break;
        case 0xA7C5: manufacturer = u"Intel"; break;
        case 0xA7C6: manufacturer = u"Intel"; break;
        case 0xA7C7: manufacturer = u"Intel"; break;
        case 0xA7C8: manufacturer = u"Intel"; break;
        case 0xA7C9: manufacturer = u"Intel"; break;
        case 0xA7CA: manufacturer = u"Intel"; break;
        case 0xA7CB: manufacturer = u"Intel"; break;
        case 0xA7CC: manufacturer = u"Intel"; break;
        case 0xA7CD: manufacturer = u"Intel"; break;
        case 0xA7CE: manufacturer = u"Intel"; break;
        case 0xA7CF: manufacturer = u"Intel"; break;
        case 0xA7D0: manufacturer = u"Intel"; break;
        case 0xA7D1: manufacturer = u"Intel"; break;
        case 0xA7D2: manufacturer = u"Intel"; break;
        case 0xA7D3: manufacturer = u"Intel"; break;
        case 0xA7D4: manufacturer = u"Intel"; break;
        case 0xA7D5: manufacturer = u"Intel"; break;
        case 0xA7D6: manufacturer = u"Intel"; break;
        case 0xA7D7: manufacturer = u"Intel"; break;
        case 0xA7D8: manufacturer = u"Intel"; break;
        case 0xA7D9: manufacturer = u"Intel"; break;
        case 0xA7DA: manufacturer = u"Intel"; break;
        case 0xA7DB: manufacturer = u"Intel"; break;
        case 0xA7DC: manufacturer = u"Intel"; break;
        case 0xA7DD: manufacturer = u"Intel"; break;
        case 0xA7DE: manufacturer = u"Intel"; break;
        case 0xA7DF: manufacturer = u"Intel"; break;
        case 0xA7E0: manufacturer = u"Intel"; break;
        case 0xA7E1: manufacturer = u"Intel"; break;
        case 0xA7E2: manufacturer = u"Intel"; break;
        case 0xA7E3: manufacturer = u"Intel"; break;
        case 0xA7E4: manufacturer = u"Intel"; break;
        case 0xA7E5: manufacturer = u"Intel"; break;
        case 0xA7E6: manufacturer = u"Intel"; break;
        case 0xA7E7: manufacturer = u"Intel"; break;
        case 0xA7E8: manufacturer = u"Intel"; break;
        case 0xA7E9: manufacturer = u"Intel"; break;
        case 0xA7EA: manufacturer = u"Intel"; break;
        case 0xA7EB: manufacturer = u"Intel"; break;
        case 0xA7EC: manufacturer = u"Intel"; break;
        case 0xA7ED: manufacturer = u"Intel"; break;
        case 0xA7EE: manufacturer = u"Intel"; break;
        case 0xA7EF: manufacturer = u"Intel"; break;
        case 0xA7F0: manufacturer = u"Intel"; break;
        case 0xA7F1: manufacturer = u"Intel"; break;
        case 0xA7F2: manufacturer = u"Intel"; break;
        case 0xA7F3: manufacturer = u"Intel"; break;
        case 0xA7F4: manufacturer = u"Intel"; break;
        case 0xA7F5: manufacturer = u"Intel"; break;
        case 0xA7F6: manufacturer = u"Intel"; break;
        case 0xA7F7: manufacturer = u"Intel"; break;
        case 0xA7F8: manufacturer = u"Intel"; break;
        case 0xA7F9: manufacturer = u"Intel"; break;
        case 0xA7FA: manufacturer = u"Intel"; break;
        case 0xA7FB: manufacturer = u"Intel"; break;
        case 0xA7FC: manufacturer = u"Intel"; break;
        case 0xA7FD: manufacturer = u"Intel"; break;
        case 0xA7FE: manufacturer = u"Intel"; break;
        case 0xA7FF: manufacturer = u"Intel"; break;
        case 0xBDBD: manufacturer = u"Black Box"; break;
        case 0xC0A9: manufacturer = u"Micron"; break;
        case 0xCABD: manufacturer = u"Cambricon"; break;
        case 0xCACE: manufacturer = u"CACE Technologies"; break;
        case 0xCAD7: manufacturer = u"Cadence"; break;
        case 0xCAFE: manufacturer = u"Chrysalis"; break;
        case 0xCC53: manufacturer = u"ScaleFlux"; break;
        case 0xCCCC: manufacturer = u"Catapult Communications"; break;
        case 0xCDDD: manufacturer = u"T-Platforms"; break;
        case 0xD161: manufacturer = u"CNEX Labs"; break;
        case 0xD4D4: manufacturer = u"D4D Technologies"; break;
        case 0xD84D: manufacturer = u"Exablaze"; break;
        case 0xDADA: manufacturer = u"Dada"; break;
        case 0xDCBA: manufacturer = u"Dynamic Network Factory"; break;
        case 0xDEAD: manufacturer = u"Indie Semiconductor"; break;
        case 0xDEAF: manufacturer = u"Trylon"; break;
        case 0xDEDA: manufacturer = u"XIMEA"; break;
        case 0xE000: manufacturer = u"WEKA"; break;
        case 0xE159: manufacturer = u"Tiger Lake-LP"; break;
        case 0xE1E1: manufacturer = u"Ekko"; break;
        case 0xE4BF: manufacturer = u"EKF"; break;
        case 0xE5E5: manufacturer = u"Brainchip"; break;
        case 0xEA60: manufacturer = u"RME"; break;
        case 0xEACE: manufacturer = u"Endace"; break;
        case 0xEB4E: manufacturer = u"Kamstrup"; break;
        case 0xEC80: manufacturer = u"Novachips"; break;
        case 0xEDD8: manufacturer = u"ARK Logic"; break;
        case 0xF1D0: manufacturer = u"ESWIN"; break;
        case 0xF5F5: manufacturer = u"F5 Networks"; break;
        case 0xFA57: manufacturer = u"FASTWEL"; break;
        case 0xFAB7: manufacturer = u"Fabric7"; break;
        case 0xFEBD: manufacturer = u"Ultravision"; break;
        case 0xFEDE: manufacturer = u"Fedevel Academy"; break;
        case 0xFFFD: manufacturer = u"XenSource"; break;
        case 0xFFFE: manufacturer = u"VMware"; break;
        default: {
            // Format as hex for unknown vendors
            CHAR16 hex_str[8];
            uint_to_string(vendor_id, hex_str, 8);
            simple_strcpy(manufacturer_buffer, u"Vendor 0x");
            simple_strcpy(&manufacturer_buffer[8], hex_str);
            bs->CloseProtocol(handle, &pci_guid, image, NULL);
            return EFI_SUCCESS;
        }
    }

    simple_strcpy(manufacturer_buffer, manufacturer);
    bs->CloseProtocol(handle, &pci_guid, image, NULL);
    return EFI_SUCCESS;
}

// Display distilled specifications in single lines (left 2/3)
void display_distilled_specs(UINTN max_width) {
    CHAR16 buffer[256];
    UINTN current_row = 4;

    // BIOS Version - Get from SMBIOS Type 0 (BIOS Information)
    set_cursor_position(0, current_row++);
    simple_printf(u"BIOS Version: ");
    if (smbios_protocol != NULL) {
        EFI_SMBIOS_HANDLE smbios_handle = 0xFFFE;
        EFI_SMBIOS_TYPE smbios_type = SMBIOS_TYPE_BIOS_INFORMATION;
        EFI_SMBIOS_TABLE_HEADER *record = NULL;

        EFI_STATUS status = smbios_protocol->GetNext(smbios_protocol, &smbios_handle, &smbios_type, &record, NULL);
        if (status == EFI_SUCCESS && record != NULL) {
            SMBIOS_TYPE0_BIOS_INFORMATION *bios_info = (SMBIOS_TYPE0_BIOS_INFORMATION*)record;
            CHAR8 *bios_version = get_smbios_string(record, bios_info->BiosVersion);
            if (bios_version != NULL && !is_placeholder_string(bios_version)) {
                CHAR16 bios_unicode[64];
                ascii_to_char16(bios_version, bios_unicode, 64);
                simple_printf(bios_unicode);
            } else {
                // Fallback to UEFI version if BIOS version not available
                CHAR16 uefi_str[16];
                uint_to_string((st->Hdr.Revision >> 16), uefi_str, 16);
                simple_printf(uefi_str);
                simple_printf(u".");
                uint_to_string((st->Hdr.Revision & 0xFFFF), uefi_str, 16);
                simple_printf(uefi_str);
                simple_printf(u" (UEFI)");
            }
        } else {
            // Fallback to UEFI version
            CHAR16 uefi_str[16];
            uint_to_string((st->Hdr.Revision >> 16), uefi_str, 16);
            simple_printf(uefi_str);
            simple_printf(u".");
            uint_to_string((st->Hdr.Revision & 0xFFFF), uefi_str, 16);
            simple_printf(uefi_str);
            simple_printf(u" (UEFI)");
        }
    } else {
        // Fallback to UEFI version
        CHAR16 uefi_str[16];
        uint_to_string((st->Hdr.Revision >> 16), uefi_str, 16);
        simple_printf(uefi_str);
        simple_printf(u".");
        uint_to_string((st->Hdr.Revision & 0xFFFF), uefi_str, 16);
        simple_printf(uefi_str);
        simple_printf(u" (UEFI)");
    }

    // Manufacturer and Product Name
    set_cursor_position(0, current_row++);
    simple_printf(u"Manufacturer: ");
    if (smbios_protocol != NULL) {
        EFI_SMBIOS_HANDLE smbios_handle = 0xFFFE;
        EFI_SMBIOS_TYPE smbios_type = SMBIOS_TYPE_SYSTEM_INFORMATION;
        EFI_SMBIOS_TABLE_HEADER *record = NULL;

        EFI_STATUS status = smbios_protocol->GetNext(smbios_protocol, &smbios_handle, &smbios_type, &record, NULL);
        if (status == EFI_SUCCESS && record != NULL) {
            SMBIOS_TYPE1_SYSTEM_INFORMATION *sys_info = (SMBIOS_TYPE1_SYSTEM_INFORMATION*)record;
            
            // Manufacturer
            CHAR8 *manufacturer = get_smbios_string(record, sys_info->Manufacturer);
            if (manufacturer != NULL) {
                CHAR16 mfg_unicode[64];
                ascii_to_char16(manufacturer, mfg_unicode, 64);
                simple_printf(mfg_unicode);
            } else {
                simple_printf(u"N/A");
            }
            
            // Product Name on next line
            CHAR8 *product = get_smbios_string(record, sys_info->ProductName);
            if (product != NULL && !is_placeholder_string(product)) {
                set_cursor_position(0, current_row++);
                simple_printf(u"Product Name: ");
                CHAR16 prod_unicode[64];
                ascii_to_char16(product, prod_unicode, 64);
                simple_printf(prod_unicode);
            }
        } else {
            simple_printf(u"N/A");
        }
    } else {
        simple_printf(u"N/A");
    }

    // Serial Number
    set_cursor_position(0, current_row++);
    simple_printf(u"Serial Number: ");
    if (smbios_protocol != NULL) {
        EFI_SMBIOS_HANDLE smbios_handle = 0xFFFE;
        EFI_SMBIOS_TYPE smbios_type = SMBIOS_TYPE_SYSTEM_INFORMATION;
        EFI_SMBIOS_TABLE_HEADER *record = NULL;

        EFI_STATUS status = smbios_protocol->GetNext(smbios_protocol, &smbios_handle, &smbios_type, &record, NULL);
        if (status == EFI_SUCCESS && record != NULL) {
            SMBIOS_TYPE1_SYSTEM_INFORMATION *sys_info = (SMBIOS_TYPE1_SYSTEM_INFORMATION*)record;
            if (sys_info->SerialNumber > 0) {
                CHAR8 *serial = get_smbios_string(record, sys_info->SerialNumber);
                if (serial != NULL && !is_placeholder_string(serial)) {
                    CHAR16 ser_unicode[64];
                    ascii_to_char16(serial, ser_unicode, 64);
                    simple_printf(ser_unicode);
                } else {
                    simple_printf(u"N/A");
                }
            } else {
                simple_printf(u"N/A");
            }
        } else {
            simple_printf(u"N/A");
        }
    } else {
        simple_printf(u"N/A");
    }

    // SKU
    set_cursor_position(0, current_row++);
    simple_printf(u"SKU: ");
    if (smbios_protocol != NULL) {
        EFI_SMBIOS_HANDLE smbios_handle = 0xFFFE;
        EFI_SMBIOS_TYPE smbios_type = SMBIOS_TYPE_SYSTEM_INFORMATION;
        EFI_SMBIOS_TABLE_HEADER *record = NULL;

        EFI_STATUS status = smbios_protocol->GetNext(smbios_protocol, &smbios_handle, &smbios_type, &record, NULL);
        if (status == EFI_SUCCESS && record != NULL) {
            SMBIOS_TYPE1_SYSTEM_INFORMATION *sys_info = (SMBIOS_TYPE1_SYSTEM_INFORMATION*)record;
            if (sys_info->SKUNumber > 0) {
                CHAR8 *sku = get_smbios_string(record, sys_info->SKUNumber);
                if (sku != NULL && !is_placeholder_string(sku)) {
                    CHAR16 sku_unicode[64];
                    ascii_to_char16(sku, sku_unicode, 64);
                    simple_printf(sku_unicode);
                } else {
                    simple_printf(u"N/A");
                }
            } else {
                simple_printf(u"N/A");
            }
        } else {
            simple_printf(u"N/A");
        }
    } else {
        simple_printf(u"N/A");
    }

    // UUID
    set_cursor_position(0, current_row++);
    simple_printf(u"UUID: ");
    if (smbios_protocol != NULL) {
        EFI_SMBIOS_HANDLE smbios_handle = 0xFFFE;
        EFI_SMBIOS_TYPE smbios_type = SMBIOS_TYPE_SYSTEM_INFORMATION;
        EFI_SMBIOS_TABLE_HEADER *record = NULL;

        EFI_STATUS status = smbios_protocol->GetNext(smbios_protocol, &smbios_handle, &smbios_type, &record, NULL);
        if (status == EFI_SUCCESS && record != NULL) {
            SMBIOS_TYPE1_SYSTEM_INFORMATION *sys_info = (SMBIOS_TYPE1_SYSTEM_INFORMATION*)record;
            for (int i = 0; i < 16; i++) {
                CHAR16 hex_str[8];
                UINT8 byte_val = sys_info->Uuid[i];
                CHAR16 hex_chars[] = u"0123456789ABCDEF";
                hex_str[0] = hex_chars[(byte_val >> 4) & 0xF];
                hex_str[1] = hex_chars[byte_val & 0xF];
                hex_str[2] = 0;
                simple_printf(hex_str);
                if (i == 3 || i == 5 || i == 7 || i == 9) {
                    simple_printf(u"-");
                }
            }
        } else {
            simple_printf(u"N/A");
        }
    } else {
        simple_printf(u"N/A");
    }

    // Processor - Combined name, cores, and frequency
    set_cursor_position(0, current_row++);
    simple_printf(u"Processor: ");
    if (smbios_protocol != NULL) {
        EFI_SMBIOS_HANDLE smbios_handle = 0xFFFE;
        EFI_SMBIOS_TYPE smbios_type = SMBIOS_TYPE_PROCESSOR_INFORMATION;
        EFI_SMBIOS_TABLE_HEADER *record = NULL;

        EFI_STATUS status = smbios_protocol->GetNext(smbios_protocol, &smbios_handle, &smbios_type, &record, NULL);
        if (status == EFI_SUCCESS && record != NULL) {
            SMBIOS_TYPE4_PROCESSOR_INFORMATION *cpu_info = (SMBIOS_TYPE4_PROCESSOR_INFORMATION*)record;
            
            // Processor name/version
            CHAR8 *version = get_smbios_string(record, cpu_info->ProcessorVersion);
            if (version != NULL) {
                CHAR16 ver_unicode[64];
                ascii_to_char16(version, ver_unicode, 64);
                simple_printf(ver_unicode);
            } else {
                simple_printf(u"Unknown CPU");
            }
            
            // Add core count
            if (mp_services != NULL) {
                UINTN total_processors = 0;
                UINTN enabled_processors = 0;
                EFI_STATUS mp_status = mp_services->GetNumberOfProcessors(mp_services, &total_processors, &enabled_processors);
                if (mp_status == EFI_SUCCESS) {
                    simple_printf(u" (");
                    uint_to_string(enabled_processors, buffer, 256);
                    simple_printf(buffer);
                    simple_printf(u" cores");
                    
                    // Add frequency if available
                    if (cpu_info->CurrentSpeed > 0) {
                        simple_printf(u" @ ");
                        uint_to_string(cpu_info->CurrentSpeed, buffer, 256);
                        simple_printf(buffer);
                        simple_printf(u" MHz");
                    }
                    simple_printf(u")");
                }
            } else {
                // Add frequency if available (without core count)
                if (cpu_info->CurrentSpeed > 0) {
                    simple_printf(u" (");
                    uint_to_string(cpu_info->CurrentSpeed, buffer, 256);
                    simple_printf(buffer);
                    simple_printf(u" MHz)");
                }
            }
        } else {
            simple_printf(u"N/A");
        }
    } else if (mp_services != NULL) {
        UINTN total_processors = 0;
        UINTN enabled_processors = 0;
        EFI_STATUS status = mp_services->GetNumberOfProcessors(mp_services, &total_processors, &enabled_processors);
        if (status == EFI_SUCCESS) {
            uint_to_string(enabled_processors, buffer, 256);
            simple_printf(buffer);
            simple_printf(u" cores");
        } else {
            simple_printf(u"N/A");
        }
    } else {
        simple_printf(u"N/A");
    }

    // RAM - Show individual modules with slot breakdown
    set_cursor_position(0, current_row++);
    simple_printf(u"RAM: ");
    EFI_STATUS status;
    if (smbios_protocol != NULL) {
        EFI_SMBIOS_HANDLE smbios_handle = 0xFFFE;
        EFI_SMBIOS_TYPE smbios_type = SMBIOS_TYPE_MEMORY_DEVICE;
        EFI_SMBIOS_TABLE_HEADER *record = NULL;
        UINT64 total_memory_mb = 0;
        UINTN module_count = 0;
        UINTN total_slots = 0;
        CHAR16 slot_breakdown[256] = u"";
        UINTN slot_len = 0;

        // First get total number of slots from Physical Memory Array (Type 16)
        EFI_SMBIOS_HANDLE array_handle = 0xFFFE;
        EFI_SMBIOS_TYPE array_type = SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY;
        EFI_SMBIOS_TABLE_HEADER *array_record = NULL;
        status = smbios_protocol->GetNext(smbios_protocol, &array_handle, &array_type, &array_record, NULL);
        if (status == EFI_SUCCESS && array_record != NULL) {
            SMBIOS_TYPE16_PHYSICAL_MEMORY_ARRAY *mem_array = (SMBIOS_TYPE16_PHYSICAL_MEMORY_ARRAY*)array_record;
            total_slots = mem_array->NumberOfMemoryDevices;
        }

        // Enumerate all memory devices to build slot breakdown
        smbios_handle = 0xFFFE;
        smbios_type = SMBIOS_TYPE_MEMORY_DEVICE;
        UINTN slot_index = 0;

        while (smbios_protocol->GetNext(smbios_protocol, &smbios_handle, &smbios_type, &record, NULL) == EFI_SUCCESS) {
            if (record != NULL) {
                SMBIOS_TYPE17_MEMORY_DEVICE *mem_device = (SMBIOS_TYPE17_MEMORY_DEVICE*)record;
                slot_index++;

                if (mem_device->Size > 0 && mem_device->Size != 0xFFFF) {
                    module_count++;

                    // Calculate size in MB
                    UINT32 size_mb;
                    if (mem_device->Size == 0x7FFF) {
                        size_mb = mem_device->ExtendedSize;
                    } else if (mem_device->Size & 0x8000) {
                        size_mb = (mem_device->Size & 0x7FFF) * 1024; // Size in KB
                    } else {
                        size_mb = mem_device->Size; // Size in MB
                    }

                    total_memory_mb += size_mb;

                    // Add to slot breakdown (show max 8 slots)
                    if (slot_index <= 8) {
                        if (slot_len > 0) {
                            simple_strcpy(&slot_breakdown[slot_len], u" / ");
                            slot_len += 3;
                        }

                        CHAR16 size_str[16];
                        uint_to_string(size_mb / 1024, size_str, 16); // Convert to GB
                        UINTN size_len = 0;
                        while (size_str[size_len] != 0) size_len++;
                        simple_strcpy(&slot_breakdown[slot_len], size_str);
                        slot_len += size_len;
                        simple_strcpy(&slot_breakdown[slot_len], u"GB");
                        slot_len += 2;
                    }
                } else {
                    // Empty slot
                    if (slot_index <= 8) {
                        if (slot_len > 0) {
                            simple_strcpy(&slot_breakdown[slot_len], u" / ");
                            slot_len += 3;
                        }
                        simple_strcpy(&slot_breakdown[slot_len], u"0GB");
                        slot_len += 3;
                    }
                }
            }
        }

        if (total_memory_mb > 0) {
            UINT64 total_gb = total_memory_mb / 1024;
            uint_to_string(total_gb, buffer, 256);
            simple_printf(buffer);
            simple_printf(u"GB");

            if (slot_len > 0) {
                simple_printf(u" (");
                simple_printf(slot_breakdown);
                if (total_slots > 8) {
                    simple_printf(u" / ...");
                }
                simple_printf(u")");
            }
        } else {
            // Fallback to EFI memory map
            EFI_STATUS status;
            UINTN map_size = 0;
            EFI_MEMORY_DESCRIPTOR *memory_map = NULL;
            UINTN map_key;
            UINTN descriptor_size;
            UINT32 descriptor_version;

            status = bs->GetMemoryMap(&map_size, memory_map, &map_key, &descriptor_size, &descriptor_version);
            if (status == EFI_BUFFER_TOO_SMALL) {
                map_size += 2 * descriptor_size;
                status = bs->AllocatePool(EfiLoaderData, map_size, (VOID**)&memory_map);

                if (status == EFI_SUCCESS) {
                    status = bs->GetMemoryMap(&map_size, memory_map, &map_key, &descriptor_size, &descriptor_version);

                    if (status == EFI_SUCCESS) {
                        UINTN num_descriptors = map_size / descriptor_size;
                        UINT64 total_memory = 0;

                        EFI_MEMORY_DESCRIPTOR *desc = memory_map;
                        for (UINTN i = 0; i < num_descriptors; i++) {
                            UINT64 memory_size = desc->NumberOfPages * 4096;
                            total_memory += memory_size;
                            desc = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)desc + descriptor_size);
                        }

                        UINT64 total_gb = total_memory / (1024 * 1024 * 1024);
                        uint_to_string(total_gb, buffer, 256);
                        simple_printf(buffer);
                        simple_printf(u"GB");
                    }
                    bs->FreePool(memory_map);
                }
            }
            if (status != EFI_SUCCESS) {
                simple_printf(u"N/A");
            }
        }
    } else {
        simple_printf(u"N/A");
    }

    // Storage - Show individual drives only (no total)
    set_cursor_position(0, current_row++);
    simple_printf(u"Storage: ");
    EFI_GUID block_io_guid = EFI_BLOCK_IO_PROTOCOL_GUID;
    UINTN num_handles = 0;
    EFI_HANDLE *handle_buffer = NULL;

    status = bs->LocateHandleBuffer(ByProtocol, &block_io_guid, NULL, &num_handles, &handle_buffer);
    if (status == EFI_SUCCESS) {
        UINTN disk_count = 0;
        CHAR16 drives_info[256] = u"";
        UINTN drives_len = 0;

        for (UINTN i = 0; i < num_handles; i++) {
            EFI_BLOCK_IO_PROTOCOL *block_io;
            status = bs->OpenProtocol(handle_buffer[i],
                                    &block_io_guid,
                                    (VOID**)&block_io,
                                    image,
                                    NULL,
                                    EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

            if (status == EFI_SUCCESS) {
                if (!block_io->Media->LogicalPartition && block_io->Media->LastBlock > 0) {
                    disk_count++;
                    UINT64 size_bytes = (block_io->Media->LastBlock + 1) * block_io->Media->BlockSize;
                    UINT64 size_gb = size_bytes / (1024 * 1024 * 1024);

                    // Add to drives string with SSD/HDD detection
                    if (disk_count <= 6) {  // Show max 6 drives in summary
                        if (drives_len > 0) {
                            simple_strcpy(&drives_info[drives_len], u" / ");
                            drives_len += 3;
                        }

                        CHAR16 size_str[16];
                        uint_to_string(size_gb, size_str, 16);
                        UINTN size_len = 0;
                        while (size_str[size_len] != 0) size_len++;

                        simple_strcpy(&drives_info[drives_len], size_str);
                        drives_len += size_len;
                        simple_strcpy(&drives_info[drives_len], u"GB ");
                        drives_len += 3;

                        // Enhanced SSD/HDD detection
                        BOOLEAN is_ssd = FALSE;
                        BOOLEAN is_removable = block_io->Media->RemovableMedia;

                        if (!is_removable) {
                            // SSD detection heuristics
                            if (block_io->Media->IoAlign >= 4096 || block_io->Media->BlockSize >= 4096) {
                                is_ssd = TRUE;
                            } else if (block_io->Media->OptimalTransferLengthGranularity > 1) {
                                is_ssd = TRUE;
                            } else if ((size_gb >= 120 && size_gb <= 128) ||
                                      (size_gb >= 240 && size_gb <= 256) ||
                                      (size_gb >= 480 && size_gb <= 512) ||
                                      (size_gb >= 960 && size_gb <= 1024) ||
                                      (size_gb >= 1920 && size_gb <= 2048) ||
                                      size_gb <= 64) {
                                is_ssd = TRUE;
                            }
                        }

                        if (is_removable) {
                            simple_strcpy(&drives_info[drives_len], u"R");
                            drives_len += 1;
                        } else if (is_ssd) {
                            simple_strcpy(&drives_info[drives_len], u"SSD");
                            drives_len += 3;
                        } else {
                            simple_strcpy(&drives_info[drives_len], u"HDD");
                            drives_len += 3;
                        }
                    }
                }
                bs->CloseProtocol(handle_buffer[i], &block_io_guid, image, NULL);
            }
        }

        if (disk_count > 0) {
            simple_printf(drives_info);
            if (disk_count > 6) {
                simple_printf(u" / ...");
            }
        } else {
            simple_printf(u"N/A");
        }

        bs->FreePool(handle_buffer);
    } else {
        simple_printf(u"N/A");
    }

    // Network Cards - Show detailed information with names
    set_cursor_position(0, current_row++);
    simple_printf(u"Network: ");
    EFI_GUID network_guid = EFI_SIMPLE_NETWORK_PROTOCOL_GUID;
    UINTN network_handles = 0;
    EFI_HANDLE *network_buffer = NULL;

    status = bs->LocateHandleBuffer(ByProtocol, &network_guid, NULL, &network_handles, &network_buffer);
    if (status == EFI_SUCCESS && network_handles > 0) {
        uint_to_string(network_handles, buffer, 256);
        simple_printf(buffer);
        simple_printf(u" adapter");
        if (network_handles > 1) simple_printf(u"s");
        
        // Try to get more details about network interfaces
        UINTN detailed_count = 0;
        CHAR16 network_info[256] = u"";
        UINTN network_len = 0;
        
        for (UINTN i = 0; i < network_handles && detailed_count < 3; i++) {  // Show details for max 3 adapters
            EFI_SIMPLE_NETWORK_PROTOCOL *snp;
            status = bs->OpenProtocol(network_buffer[i],
                                    &network_guid,
                                    (VOID**)&snp,
                                    image,
                                    NULL,
                                    EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
            
            if (status == EFI_SUCCESS) {
                // Get network interface details
                if (snp->Mode != NULL) {
                    detailed_count++;
                    
                    // Add separator if not first
                    if (network_len > 0) {
                        simple_strcpy(&network_info[network_len], u" / ");
                        network_len += 3;
                    }
                    
                    // Try to get device name from PCI or other protocols
                    CHAR16 device_name[32] = u"";
                    BOOLEAN has_name = FALSE;
                    
                    // Check for PCI IO protocol to get device info
                    EFI_GUID pci_io_guid = EFI_PCI_IO_PROTOCOL_GUID;
                    EFI_PCI_IO_PROTOCOL *pci_io;
                    if (bs->OpenProtocol(network_buffer[i], &pci_io_guid, (VOID**)&pci_io, image, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL) == EFI_SUCCESS) {
                        // Get PCI device info
                        UINT8 class_code[3];
                        if (pci_io->Pci.Read(pci_io, EfiPciIoWidthUint8, 0x0B, 3, &class_code) == EFI_SUCCESS) {
                            // Network controller class is 0x02
                            if (class_code[2] == 0x02) {
                                switch (class_code[1]) {
                                    case 0x00: simple_strcpy(device_name, u"Ethernet"); has_name = TRUE; break;
                                    case 0x01: simple_strcpy(device_name, u"TokenRing"); has_name = TRUE; break;
                                    case 0x02: simple_strcpy(device_name, u"FDDI"); has_name = TRUE; break;
                                    case 0x03: simple_strcpy(device_name, u"ATM"); has_name = TRUE; break;
                                    case 0x04: simple_strcpy(device_name, u"ISDN"); has_name = TRUE; break;
                                    case 0x05: simple_strcpy(device_name, u"WorldFip"); has_name = TRUE; break;
                                    case 0x06: simple_strcpy(device_name, u"PICMG"); has_name = TRUE; break;
                                    case 0x07: simple_strcpy(device_name, u"Infiniband"); has_name = TRUE; break;
                                    case 0x08: simple_strcpy(device_name, u"Fabric"); has_name = TRUE; break;
                                    case 0x80: simple_strcpy(device_name, u"Wireless"); has_name = TRUE; break;
                                }
                            }
                        }
                        bs->CloseProtocol(network_buffer[i], &pci_io_guid, image, NULL);
                    }
                    
                    // If no specific name, use generic
                    if (!has_name) {
                        simple_strcpy(device_name, u"NIC");
                        has_name = TRUE;
                    }
                    
                    // Add device name
                    UINTN name_len = 0;
                    while (device_name[name_len] != 0) name_len++;
                    simple_strcpy(&network_info[network_len], device_name);
                    network_len += name_len;
                    
                    // Add MAC address
                    simple_strcpy(&network_info[network_len], u" ");
                    network_len += 1;
                    
                    for (UINTN j = 0; j < snp->Mode->HwAddressSize; j++) {
                        CHAR16 mac_str[4];
                        uint_to_string(snp->Mode->CurrentAddress.Addr[j], mac_str, 4);
                        if (mac_str[1] == 0) {  // Single digit
                            simple_strcpy(&network_info[network_len], u"0");
                            network_len += 1;
                        }
                        UINTN mac_len = 0;
                        while (mac_str[mac_len] != 0) mac_len++;
                        simple_strcpy(&network_info[network_len], mac_str);
                        network_len += mac_len;
                        if (j < snp->Mode->HwAddressSize - 1) {
                            simple_strcpy(&network_info[network_len], u":");
                            network_len += 1;
                        }
                    }
                    
                    // Add link state
                    if (snp->Mode->MediaPresent) {
                        simple_strcpy(&network_info[network_len], u" Up");
                        network_len += 3;
                    } else {
                        simple_strcpy(&network_info[network_len], u" Down");
                        network_len += 5;
                    }
                }
                bs->CloseProtocol(network_buffer[i], &network_guid, image, NULL);
            }
        }
        
        if (network_len > 0) {
            simple_printf(u" (");
            simple_printf(network_info);
            if (network_handles > 3) {
                simple_printf(u" / ...");
            }
            simple_printf(u")");
        }
        
        bs->FreePool(network_buffer);
    } else {
        simple_printf(u"N/A");
    }

    // PCIe Slots
    set_cursor_position(0, current_row++);
    simple_printf(u"PCIe Slots: ");
    if (smbios_protocol != NULL) {
        EFI_SMBIOS_HANDLE smbios_handle = 0xFFFE;
        EFI_SMBIOS_TYPE smbios_type = SMBIOS_TYPE_SYSTEM_SLOTS;
        EFI_SMBIOS_TABLE_HEADER *record = NULL;
        UINTN pcie_count = 0;

        while (smbios_protocol->GetNext(smbios_protocol, &smbios_handle, &smbios_type, &record, NULL) == EFI_SUCCESS) {
            if (record != NULL) {
                SMBIOS_TYPE9_SYSTEM_SLOTS *slot = (SMBIOS_TYPE9_SYSTEM_SLOTS*)record;
                // Check if it's a PCIe slot (common slot types for PCIe)
                if (slot->SlotType >= 0xA0 && slot->SlotType <= 0xB0) {
                    pcie_count++;
                }
            }
        }

        if (pcie_count > 0) {
            uint_to_string(pcie_count, buffer, 256);
            simple_printf(buffer);
            simple_printf(u" slots");
        } else {
            simple_printf(u"N/A");
        }
    } else {
        simple_printf(u"N/A");
    }

    // USB Ports - Show detailed information with names
    set_cursor_position(0, current_row++);
    simple_printf(u"USB Ports: ");
    EFI_GUID usb_guid = EFI_USB_IO_PROTOCOL_GUID;
    UINTN usb_handles = 0;
    EFI_HANDLE *usb_buffer = NULL;

    status = bs->LocateHandleBuffer(ByProtocol, &usb_guid, NULL, &usb_handles, &usb_buffer);
    if (status == EFI_SUCCESS && usb_handles > 0) {
        uint_to_string(usb_handles, buffer, 256);
        simple_printf(buffer);
        simple_printf(u" device");
        if (usb_handles > 1) simple_printf(u"s");
        
        // Try to get more details about USB devices
        UINTN detailed_count = 0;
        CHAR16 usb_info[256] = u"";
        UINTN usb_len = 0;
        
        for (UINTN i = 0; i < usb_handles && detailed_count < 4; i++) {  // Show details for max 4 devices
            EFI_USB_IO_PROTOCOL *usb_io;
            status = bs->OpenProtocol(usb_buffer[i],
                                    &usb_guid,
                                    (VOID**)&usb_io,
                                    image,
                                    NULL,
                                    EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
            
            if (status == EFI_SUCCESS) {
                // Get USB device details
                EFI_USB_DEVICE_DESCRIPTOR device_desc;
                status = usb_io->UsbGetDeviceDescriptor(usb_io, &device_desc);
                
                if (status == EFI_SUCCESS) {
                    detailed_count++;
                    
                    // Add separator if not first
                    if (usb_len > 0) {
                        simple_strcpy(&usb_info[usb_len], u" / ");
                        usb_len += 3;
                    }
                    
                    // Get device name from string descriptors if available
                    CHAR16 device_name[32] = u"";
                    BOOLEAN has_name = FALSE;
                    
                    // Try to get product string
                    if (device_desc.Product > 0) {
                        CHAR16 product_str[32];
                        if (usb_io->UsbGetStringDescriptor(usb_io, 0x0409, device_desc.Product, product_str) == EFI_SUCCESS) {
                            // Copy first part of product string (truncate if too long)
                            UINTN copy_len = 0;
                            while (product_str[copy_len] != 0 && copy_len < 15) {
                                device_name[copy_len] = product_str[copy_len];
                                copy_len++;
                            }
                            device_name[copy_len] = 0;
                            has_name = TRUE;
                        }
                    }
                    
                    // If no product string, use class-based name
                    if (!has_name) {
                        switch (device_desc.DeviceClass) {
                            case 0x00: simple_strcpy(device_name, u"Generic"); break;
                            case 0x01: simple_strcpy(device_name, u"Audio"); break;
                            case 0x02: simple_strcpy(device_name, u"Comm"); break;
                            case 0x03: simple_strcpy(device_name, u"HID"); break;
                            case 0x05: simple_strcpy(device_name, u"Physical"); break;
                            case 0x06: simple_strcpy(device_name, u"Image"); break;
                            case 0x07: simple_strcpy(device_name, u"Printer"); break;
                            case 0x08: simple_strcpy(device_name, u"Storage"); break;
                            case 0x09: simple_strcpy(device_name, u"Hub"); break;
                            case 0x0A: simple_strcpy(device_name, u"CDC-Data"); break;
                            case 0x0B: simple_strcpy(device_name, u"SmartCard"); break;
                            case 0x0D: simple_strcpy(device_name, u"Security"); break;
                            case 0x0E: simple_strcpy(device_name, u"Video"); break;
                            case 0x0F: simple_strcpy(device_name, u"Healthcare"); break;
                            case 0x10: simple_strcpy(device_name, u"AV"); break;
                            case 0x11: simple_strcpy(device_name, u"Billboard"); break;
                            case 0x12: simple_strcpy(device_name, u"USBType-C"); break;
                            case 0xDC: simple_strcpy(device_name, u"Diagnostic"); break;
                            case 0xE0: simple_strcpy(device_name, u"Wireless"); break;
                            case 0xEF: simple_strcpy(device_name, u"Misc"); break;
                            case 0xFE: simple_strcpy(device_name, u"AppSpec"); break;
                            case 0xFF: simple_strcpy(device_name, u"VendorSpec"); break;
                            default: simple_strcpy(device_name, u"USB"); break;
                        }
                        has_name = TRUE;
                    }
                    
                    // Add device name
                    UINTN name_len = 0;
                    while (device_name[name_len] != 0) name_len++;
                    simple_strcpy(&usb_info[usb_len], device_name);
                    usb_len += name_len;
                    
                    // Add USB version
                    simple_strcpy(&usb_info[usb_len], u" ");
                    usb_len += 1;
                    
                    CHAR16 ver_str[16];
                    uint_to_string(device_desc.BcdUSB >> 8, ver_str, 16);
                    UINTN ver_len = 0;
                    while (ver_str[ver_len] != 0) ver_len++;
                    simple_strcpy(&usb_info[usb_len], ver_str);
                    usb_len += ver_len;
                    simple_strcpy(&usb_info[usb_len], u".");
                    usb_len += 1;
                    uint_to_string((device_desc.BcdUSB & 0xFF) >> 4, ver_str, 16);
                    ver_len = 0;
                    while (ver_str[ver_len] != 0) ver_len++;
                    simple_strcpy(&usb_info[usb_len], ver_str);
                    usb_len += ver_len;
                }
                bs->CloseProtocol(usb_buffer[i], &usb_guid, image, NULL);
            }
        }
        
        if (usb_len > 0) {
            simple_printf(u" (");
            simple_printf(usb_info);
            if (usb_handles > 4) {
                simple_printf(u" / ...");
            }
            simple_printf(u")");
        }
        
        bs->FreePool(usb_buffer);
    } else {
        simple_printf(u"N/A");
    }
}

// Display additional details in the right 1/3 (detailed information)
void display_additional_details(UINTN left_split) {
    UINTN current_row = 4;

    // System Details
    set_cursor_position(left_split + 2, current_row++);
    cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_WHITE, EFI_BLUE));
    simple_printf(u"=== SYSTEM DETAILS ===");

    // Firmware Details
    set_cursor_position(left_split + 2, current_row++);
    cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));
    simple_printf(u"Firmware Vendor: ");
    if (st != NULL && st->FirmwareVendor != NULL) {
        simple_printf(st->FirmwareVendor);
    } else {
        simple_printf(u"N/A");
    }

    // UEFI Version
    set_cursor_position(left_split + 2, current_row++);
    simple_printf(u"UEFI Version: ");
    if (st != NULL) {
        CHAR16 uefi_str[16];
        uint_to_string((st->Hdr.Revision >> 16), uefi_str, 16);
        simple_printf(uefi_str);
        simple_printf(u".");
        uint_to_string((st->Hdr.Revision & 0xFFFF), uefi_str, 16);
        simple_printf(uefi_str);
    } else {
        simple_printf(u"N/A");
    }

    // Memory Details - Enhanced with form factor, server check, and removability
    set_cursor_position(left_split + 2, ++current_row);
    cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_WHITE, EFI_BLUE));
    simple_printf(u"=== MEMORY DETAILS ===");

    // Get detailed memory information
    EFI_STATUS status;
    UINTN map_size = 0;
    EFI_MEMORY_DESCRIPTOR *memory_map = NULL;
    UINTN map_key;
    UINTN descriptor_size;
    UINT32 descriptor_version;

    status = bs->GetMemoryMap(&map_size, memory_map, &map_key, &descriptor_size, &descriptor_version);
    if (status == EFI_BUFFER_TOO_SMALL) {
        map_size += 2 * descriptor_size;
        status = bs->AllocatePool(EfiLoaderData, map_size, (VOID**)&memory_map);

        if (status == EFI_SUCCESS) {
            status = bs->GetMemoryMap(&map_size, memory_map, &map_key, &descriptor_size, &descriptor_version);

            if (status == EFI_SUCCESS) {
                UINTN num_descriptors = map_size / descriptor_size;
                UINT64 conventional_memory = 0;
                UINT64 acpi_memory = 0;
                UINT64 reserved_memory = 0;

                EFI_MEMORY_DESCRIPTOR *desc = memory_map;
                for (UINTN i = 0; i < num_descriptors; i++) {
                    UINT64 memory_size = desc->NumberOfPages * 4096;

                    switch (desc->Type) {
                        case EfiConventionalMemory:
                        case EfiLoaderCode:
                        case EfiLoaderData:
                            conventional_memory += memory_size;
                            break;
                        case EfiACPIReclaimMemory:
                        case EfiACPIMemoryNVS:
                            acpi_memory += memory_size;
                            break;
                        case EfiReservedMemoryType:
                        case EfiUnusableMemory:
                            reserved_memory += memory_size;
                            break;
                    }

                    desc = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)desc + descriptor_size);
                }

                set_cursor_position(left_split + 2, ++current_row);
                cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));
                CHAR16 mem_str[32];
                uint_to_string(conventional_memory / (1024 * 1024), mem_str, 32);
                simple_printf(u"Conventional: ");
                simple_printf(mem_str);
                simple_printf(u" MB");

                set_cursor_position(left_split + 2, ++current_row);
                uint_to_string(acpi_memory / (1024 * 1024), mem_str, 32);
                simple_printf(u"ACPI: ");
                simple_printf(mem_str);
                simple_printf(u" MB");

                set_cursor_position(left_split + 2, ++current_row);
                uint_to_string(reserved_memory / (1024 * 1024), mem_str, 32);
                simple_printf(u"Reserved: ");
                simple_printf(mem_str);
                simple_printf(u" MB");
            }
            bs->FreePool(memory_map);
        }
    }

    // Enhanced memory slot details with form factor and server detection
    if (smbios_protocol != NULL) {
        EFI_SMBIOS_HANDLE smbios_handle = 0xFFFE;
        EFI_SMBIOS_TYPE smbios_type = SMBIOS_TYPE_MEMORY_DEVICE;
        EFI_SMBIOS_TABLE_HEADER *record = NULL;
        UINTN slot_count = 0;
        UINTN populated_slots = 0;
        BOOLEAN has_server_ram = FALSE;
        BOOLEAN has_ecc_ram = FALSE;

        // Get system information to detect server vs desktop
        EFI_SMBIOS_HANDLE sys_handle = 0xFFFE;
        EFI_SMBIOS_TYPE sys_type = SMBIOS_TYPE_SYSTEM_INFORMATION;
        EFI_SMBIOS_TABLE_HEADER *sys_record = NULL;
        CHAR8 *sys_manufacturer = NULL;
        CHAR8 *sys_product = NULL;

        status = smbios_protocol->GetNext(smbios_protocol, &sys_handle, &sys_type, &sys_record, NULL);
        if (status == EFI_SUCCESS && sys_record != NULL) {
            SMBIOS_TYPE1_SYSTEM_INFORMATION *sys_info = (SMBIOS_TYPE1_SYSTEM_INFORMATION*)sys_record;
            sys_manufacturer = get_smbios_string(sys_record, sys_info->Manufacturer);
            sys_product = get_smbios_string(sys_record, sys_info->ProductName);
        }

        // Check each memory device
        while (smbios_protocol->GetNext(smbios_protocol, &smbios_handle, &smbios_type, &record, NULL) == EFI_SUCCESS) {
            if (record != NULL) {
                SMBIOS_TYPE17_MEMORY_DEVICE *mem_device = (SMBIOS_TYPE17_MEMORY_DEVICE*)record;
                slot_count++;

                if (mem_device->Size > 0 && mem_device->Size != 0xFFFF) {
                    populated_slots++;

                    // Check for ECC (server RAM indicator)
                    if (mem_device->TypeDetail & 0x10) { // ECC bit
                        has_ecc_ram = TRUE;
                    }

                    // Server RAM detection based on various factors
                    if (mem_device->MemoryType >= 24 && mem_device->MemoryType <= 26) { // DDR3, DDR4, DDR5
                        if (mem_device->Speed >= 2133) { // High speed RAM often used in servers
                            has_server_ram = TRUE;
                        }
                        if (mem_device->DataWidth >= 72) { // ECC width
                            has_server_ram = TRUE;
                        }
                    }

                    // Show detailed info for first few slots
                    if (slot_count <= 4) {
                        set_cursor_position(left_split + 2, ++current_row);
                        cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));

                        CHAR16 slot_str[16];
                        uint_to_string(slot_count, slot_str, 16);
                        simple_printf(u"Slot ");
                        simple_printf(slot_str);
                        simple_printf(u": ");

                        // Memory size
                        UINT32 size_mb;
                        if (mem_device->Size == 0x7FFF) {
                            size_mb = mem_device->ExtendedSize;
                        } else if (mem_device->Size & 0x8000) {
                            size_mb = (mem_device->Size & 0x7FFF) * 1024;
                        } else {
                            size_mb = mem_device->Size;
                        }

                        CHAR16 size_str[16];
                        uint_to_string(size_mb, size_str, 16);
                        simple_printf(size_str);
                        simple_printf(u"MB ");

                        // Memory type
                        CHAR16 *mem_type = u"Unknown";
                        switch (mem_device->MemoryType) {
                            case 24: mem_type = u"DDR3"; break;
                            case 26: mem_type = u"DDR4"; break;
                            case 34: mem_type = u"DDR5"; break;
                            case 18: mem_type = u"DDR"; break;
                            case 20: mem_type = u"DDR2"; break;
                            case 25: mem_type = u"DDR3L"; break;
                            case 27: mem_type = u"LPDDR"; break;
                            case 28: mem_type = u"LPDDR2"; break;
                            case 29: mem_type = u"LPDDR3"; break;
                            case 30: mem_type = u"LPDDR4"; break;
                            case 33: mem_type = u"LPDDR5"; break;
                        }
                        simple_printf(mem_type);

                        // Form factor (SO-DIMM vs DIMM)
                        CHAR16 *form_factor = u"Unknown";
                        BOOLEAN is_removable = TRUE;

                        switch (mem_device->FormFactor) {
                            case 1: form_factor = u"Other"; break;
                            case 2: form_factor = u"Unknown"; break;
                            case 3: form_factor = u"SIMM"; break;
                            case 4: form_factor = u"SIP"; is_removable = FALSE; break;
                            case 5: form_factor = u"Chip"; is_removable = FALSE; break;
                            case 6: form_factor = u"DIP"; break;
                            case 7: form_factor = u"ZIP"; break;
                            case 8: form_factor = u"Proprietary"; break;
                            case 9: form_factor = u"DIMM"; break;
                            case 10: form_factor = u"TSOP"; is_removable = FALSE; break;
                            case 11: form_factor = u"RowChip"; is_removable = FALSE; break;
                            case 12: form_factor = u"RIMM"; break;
                            case 13: form_factor = u"SO-DIMM"; break;
                            case 14: form_factor = u"SRIMM"; break;
                            case 15: form_factor = u"FB-DIMM"; break;
                        }

                        simple_printf(u" ");
                        simple_printf(form_factor);

                        if (is_removable) {
                            simple_printf(u" (Removable)");
                        } else {
                            simple_printf(u" (Soldered)");
                        }

                        // Speed if available
                        if (mem_device->Speed > 0) {
                            CHAR16 speed_str[16];
                            uint_to_string(mem_device->Speed, speed_str, 16);
                            simple_printf(u" @ ");
                            simple_printf(speed_str);
                            simple_printf(u"MHz");
                        }
                    }
                }
            }
        }

        // Summary information
        set_cursor_position(left_split + 2, ++current_row);
        cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));
        CHAR16 summary_str[16];
        uint_to_string(populated_slots, summary_str, 16);
        simple_printf(u"Populated: ");
        simple_printf(summary_str);
        simple_printf(u"/");
        uint_to_string(slot_count, summary_str, 16);
        simple_printf(summary_str);
        simple_printf(u" slots");

        // Server RAM detection
        set_cursor_position(left_split + 2, ++current_row);
        if (has_ecc_ram) {
            simple_printf(u"RAM Type: Server (ECC detected)");
        } else if (has_server_ram) {
            simple_printf(u"RAM Type: Likely Server");
        } else {
            simple_printf(u"RAM Type: Desktop/Consumer");
        }

        // Additional server detection based on system info
        if (sys_manufacturer != NULL) {
            BOOLEAN is_server_system = FALSE;

            // Check for known server manufacturers/products
            if ((sys_manufacturer[0] == 'D' && sys_manufacturer[1] == 'e') || // Dell
                (sys_manufacturer[0] == 'H' && sys_manufacturer[1] == 'e') || // Hewlett Packard
                (sys_manufacturer[0] == 'L' && sys_manufacturer[1] == 'e') || // Lenovo
                (sys_manufacturer[0] == 'S' && sys_manufacturer[1] == 'u')) {  // Supermicro
                is_server_system = TRUE;
            }

            if (sys_product != NULL && !is_placeholder_string(sys_product)) {
                // Check product name for server indicators
                if ((sys_product[0] == 'P' && sys_product[1] == 'o') || // PowerEdge
                    (sys_product[0] == 'P' && sys_product[1] == 'r') || // ProLiant
                    (sys_product[0] == 'S' && sys_product[1] == 'y') || // System x
                    (sys_product[0] == 'T' && sys_product[1] == 'h')) {  // ThinkServer
                    is_server_system = TRUE;
                }
            }

            if (is_server_system) {
                set_cursor_position(left_split + 2, ++current_row);
                simple_printf(u"System: Server Platform");
            }
        }
    }

    // Storage Details - Enhanced with names and serial numbers
    set_cursor_position(left_split + 2, ++current_row);
    cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_WHITE, EFI_BLUE));
    simple_printf(u"=== STORAGE DETAILS ===");

    EFI_GUID block_io_guid = EFI_BLOCK_IO_PROTOCOL_GUID;
    UINTN num_handles = 0;
    EFI_HANDLE *handle_buffer = NULL;

    status = bs->LocateHandleBuffer(ByProtocol, &block_io_guid, NULL, &num_handles, &handle_buffer);
    if (status == EFI_SUCCESS) {
        UINTN disk_count = 0;

        for (UINTN i = 0; i < num_handles && disk_count < 4; i++) {  // Show max 4 drives in details
            EFI_BLOCK_IO_PROTOCOL *block_io;
            status = bs->OpenProtocol(handle_buffer[i],
                                    &block_io_guid,
                                    (VOID**)&block_io,
                                    image,
                                    NULL,
                                    EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

            if (status == EFI_SUCCESS) {
                if (!block_io->Media->LogicalPartition && block_io->Media->LastBlock > 0) {
                    disk_count++;

                    set_cursor_position(left_split + 2, ++current_row);
                    cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));

                    CHAR16 disk_num[16];
                    uint_to_string(disk_count, disk_num, 16);
                    simple_printf(u"Drive ");
                    simple_printf(disk_num);
                    simple_printf(u": ");

                    UINT64 size_gb = (block_io->Media->LastBlock + 1) * block_io->Media->BlockSize / (1024 * 1024 * 1024);
                    uint_to_string(size_gb, disk_num, 16);
                    simple_printf(disk_num);
                    simple_printf(u"GB ");

                    // Enhanced SSD/HDD detection with more detail
                    BOOLEAN is_ssd = FALSE;
                    BOOLEAN is_removable = block_io->Media->RemovableMedia;

                    if (!is_removable) {
                        // SSD detection heuristics
                        if (block_io->Media->IoAlign >= 4096 || block_io->Media->BlockSize >= 4096) {
                            is_ssd = TRUE;
                        } else if (block_io->Media->OptimalTransferLengthGranularity > 1) {
                            is_ssd = TRUE;
                        } else if ((size_gb >= 120 && size_gb <= 128) ||
                                  (size_gb >= 240 && size_gb <= 256) ||
                                  (size_gb >= 480 && size_gb <= 512) ||
                                  (size_gb >= 960 && size_gb <= 1024) ||
                                  (size_gb >= 1920 && size_gb <= 2048) ||
                                  size_gb <= 64) {
                            is_ssd = TRUE;
                        }
                    }

                    if (is_removable) {
                        simple_printf(u"(Removable)");
                    } else if (is_ssd) {
                        simple_printf(u"(SSD)");
                    } else {
                        simple_printf(u"(HDD)");
                    }

                    // Try to get drive name and serial from SMBIOS or PCI
                    BOOLEAN found_info = FALSE;

                    // Try to get drive serial and model using ATA/NVMe protocols
                    CHAR16 serial_buffer[64] = u"";
                    CHAR16 model_buffer[128] = u"";
                    EFI_STATUS drive_status = EFI_UNSUPPORTED;

                    // Try ATA first
                    drive_status = get_ata_drive_info(handle_buffer[i], serial_buffer, 64, model_buffer, 128);
                    if (drive_status != EFI_SUCCESS) {
                        // Try NVMe if ATA failed
                        drive_status = get_nvme_drive_info(handle_buffer[i], serial_buffer, 64, model_buffer, 128);
                    }

                    if (drive_status == EFI_SUCCESS && (serial_buffer[0] != 0 || model_buffer[0] != 0)) {
                        set_cursor_position(left_split + 2, ++current_row);
                        cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));

                        if (model_buffer[0] != 0) {
                            simple_printf(u"Model: ");
                            simple_printf(model_buffer);
                            simple_printf(u"\r\n");
                        }

                        if (serial_buffer[0] != 0) {
                            set_cursor_position(left_split + 2, ++current_row);
                            simple_printf(u"Serial: ");
                            simple_printf(serial_buffer);
                            simple_printf(u"\r\n");
                        }

                        found_info = TRUE;
                    }

                    // Check for PCI IO protocol to get device info
                    EFI_GUID pci_io_guid = EFI_PCI_IO_PROTOCOL_GUID;
                    EFI_PCI_IO_PROTOCOL *pci_io;
                    if (bs->OpenProtocol(handle_buffer[i], &pci_io_guid, (VOID**)&pci_io, image, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL) == EFI_SUCCESS) {
                        // Get PCI device info
                        UINT8 class_code[3];
                        if (pci_io->Pci.Read(pci_io, EfiPciIoWidthUint8, 0x0B, 3, &class_code) == EFI_SUCCESS) {
                            // Mass storage controller class is 0x01
                            if (class_code[2] == 0x01) {
                                set_cursor_position(left_split + 2, ++current_row);
                                simple_printf(u"Controller: ");

                                switch (class_code[1]) {
                                    case 0x00: simple_printf(u"SCSI"); break;
                                    case 0x01: simple_printf(u"IDE"); break;
                                    case 0x02: simple_printf(u"Floppy"); break;
                                    case 0x03: simple_printf(u"IPI"); break;
                                    case 0x04: simple_printf(u"RAID"); break;
                                    case 0x05: simple_printf(u"ATA"); break;
                                    case 0x06: simple_printf(u"SATA"); break;
                                    case 0x07: simple_printf(u"SAS"); break;
                                    case 0x08: simple_printf(u"NVMe"); break;
                                    case 0x09: simple_printf(u"UFS"); break;
                                    default: simple_printf(u"Storage"); break;
                                }
                                found_info = TRUE;
                            }
                        }
                        bs->CloseProtocol(handle_buffer[i], &pci_io_guid, image, NULL);
                    }

                    // Try to get additional info from SMBIOS (if available)
                    if (smbios_protocol != NULL) {
                        // Look for storage device information in SMBIOS
                        EFI_SMBIOS_HANDLE smbios_handle = 0xFFFE;
                        EFI_SMBIOS_TYPE smbios_type = SMBIOS_TYPE_SYSTEM_SLOTS; // Try various types
                        EFI_SMBIOS_TABLE_HEADER *record = NULL;

                        // Note: SMBIOS doesn't have a direct type for storage devices like it does for memory
                        // The best we can do is show what controller type we detected above
                    }

                    // Show block size info
                    set_cursor_position(left_split + 2, ++current_row);
                    CHAR16 block_str[16];
                    uint_to_string(block_io->Media->BlockSize, block_str, 16);
                    simple_printf(u"Block Size: ");
                    simple_printf(block_str);
                    simple_printf(u" bytes");

                    // Show additional properties
                    if (block_io->Media->ReadOnly) {
                        simple_printf(u" (Read-Only)");
                    } else if (block_io->Media->WriteCaching) {
                        simple_printf(u" (Write-Cached)");
                    }
                }
                bs->CloseProtocol(handle_buffer[i], &block_io_guid, image, NULL);
            }
        }

        if (disk_count == 0) {
            set_cursor_position(left_split + 2, ++current_row);
            cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));
            simple_printf(u"No storage devices detected");
        } else {
            set_cursor_position(left_split + 2, ++current_row);
            cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));
            CHAR16 count_str[16];
            uint_to_string(disk_count, count_str, 16);
            simple_printf(u"Total Drives: ");
            simple_printf(count_str);
        }

        bs->FreePool(handle_buffer);
    } else {
        set_cursor_position(left_split + 2, ++current_row);
        cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));
        simple_printf(u"Storage information unavailable");
    }

    // Network Interfaces
    set_cursor_position(left_split + 2, ++current_row);
    cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_WHITE, EFI_BLUE));
    simple_printf(u"=== NETWORK ===");

    EFI_GUID network_guid = EFI_SIMPLE_NETWORK_PROTOCOL_GUID;
    UINTN network_handles = 0;
    EFI_HANDLE *network_buffer = NULL;

    status = bs->LocateHandleBuffer(ByProtocol, &network_guid, NULL, &network_handles, &network_buffer);
    if (status == EFI_SUCCESS && network_handles > 0) {
        set_cursor_position(left_split + 2, ++current_row);
        cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));
        CHAR16 net_str[16];
        uint_to_string(network_handles, net_str, 16);
        simple_printf(net_str);
        simple_printf(u" network interface(s)");
        bs->FreePool(network_buffer);
    } else {
        set_cursor_position(left_split + 2, ++current_row);
        cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));
        simple_printf(u"No network interfaces");
    }

    // USB Devices
    set_cursor_position(left_split + 2, ++current_row);
    cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_WHITE, EFI_BLUE));
    simple_printf(u"=== USB DEVICES ===");

    EFI_GUID usb_guid = EFI_USB_IO_PROTOCOL_GUID;
    UINTN usb_handles = 0;
    EFI_HANDLE *usb_buffer = NULL;

    status = bs->LocateHandleBuffer(ByProtocol, &usb_guid, NULL, &usb_handles, &usb_buffer);
    if (status == EFI_SUCCESS && usb_handles > 0) {
        set_cursor_position(left_split + 2, ++current_row);
        cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));
        CHAR16 usb_str[16];
        uint_to_string(usb_handles, usb_str, 16);
        simple_printf(usb_str);
        simple_printf(u" USB device(s)");
        bs->FreePool(usb_buffer);
    } else {
        set_cursor_position(left_split + 2, ++current_row);
        cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));
        simple_printf(u"No USB devices");
    }
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
            
            // // CPU Speed
            // if (cpu_info->CurrentSpeed > 0) {
            //     CHAR16 speed_str[16];
            //     uint_to_string(cpu_info->CurrentSpeed, speed_str, 16);
            //     simple_printf(u"Current Speed: ");
            //     simple_printf(speed_str);
            //     simple_printf(u" MHz\r\n");
            // }
            
            // if (cpu_info->MaxSpeed > 0) {
            //     CHAR16 speed_str[16];
            //     uint_to_string(cpu_info->MaxSpeed, speed_str, 16);
            //     simple_printf(u"Max Speed: ");
            //     simple_printf(speed_str);
            //     simple_printf(u" MHz\r\n");
            // }
            
            // // Core Count
            // if (cpu_info->CoreCount > 0) {
            //     CHAR16 core_str[16];
            //     uint_to_string(cpu_info->CoreCount, core_str, 16);
            //     simple_printf(u"Physical Cores: ");
            //     simple_printf(core_str);
            //     simple_printf(u"\r\n");
            // }
            
            // // Thread Count
            // if (cpu_info->ThreadCount > 0) {
            //     CHAR16 thread_str[16];
            //     uint_to_string(cpu_info->ThreadCount, thread_str, 16);
            //     simple_printf(u"Logical Processors: ");
            //     simple_printf(thread_str);
            //     simple_printf(u"\r\n");
            // }
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

// Display enhanced memory information with slot details
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
    
    // Memory array and slot details from SMBIOS
    if (smbios_protocol != NULL) {
        // First, check for Physical Memory Array (Type 16)
        EFI_SMBIOS_HANDLE smbios_handle = 0xFFFE;
        EFI_SMBIOS_TYPE smbios_type = SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY;
        EFI_SMBIOS_TABLE_HEADER *record = NULL;
        UINT16 total_slots = 0;
        UINT32 max_capacity_mb = 0;
        
        status = smbios_protocol->GetNext(smbios_protocol, &smbios_handle, &smbios_type, &record, NULL);
        if (status == EFI_SUCCESS && record != NULL) {
            SMBIOS_TYPE16_PHYSICAL_MEMORY_ARRAY *mem_array = (SMBIOS_TYPE16_PHYSICAL_MEMORY_ARRAY*)record;
            
            total_slots = mem_array->NumberOfMemoryDevices;
            
            simple_printf(u"\r\nMemory Array Information:\r\n");
            
            CHAR16 slot_str[16];
            uint_to_string(total_slots, slot_str, 16);
            simple_printf(u"  Total Memory Slots: ");
            simple_printf(slot_str);
            simple_printf(u"\r\n");
            
            // Maximum capacity
            if (mem_array->MaximumCapacity != 0x80000000) {
                max_capacity_mb = mem_array->MaximumCapacity / 1024; // Convert from KB to MB
                if (max_capacity_mb > 0) {
                    CHAR16 cap_str[32];
                    uint_to_string(max_capacity_mb, cap_str, 32);
                    simple_printf(u"  Maximum Capacity: ");
                    simple_printf(cap_str);
                    simple_printf(u" MB");
                    
                    if (max_capacity_mb >= 1024) {
                        UINT32 cap_gb = max_capacity_mb / 1024;
                        uint_to_string(cap_gb, cap_str, 32);
                        simple_printf(u" (~");
                        simple_printf(cap_str);
                        simple_printf(u" GB)");
                    }
                    simple_printf(u"\r\n");
                }
            } else if (mem_array->ExtendedMaximumCapacity > 0) {
                // Extended capacity in bytes
                UINT64 ext_cap_mb = mem_array->ExtendedMaximumCapacity / (1024 * 1024);
                CHAR16 cap_str[32];
                uint_to_string(ext_cap_mb, cap_str, 32);
                simple_printf(u"  Maximum Capacity: ");
                simple_printf(cap_str);
                simple_printf(u" MB");
                
                if (ext_cap_mb >= 1024) {
                    UINT64 cap_gb = ext_cap_mb / 1024;
                    uint_to_string(cap_gb, cap_str, 32);
                    simple_printf(u" (~");
                    simple_printf(cap_str);
                    simple_printf(u" GB)");
                }
                simple_printf(u"\r\n");
            }
            
            // Memory error correction
            CHAR16 *ecc_type = u"Unknown";
            switch (mem_array->MemoryErrorCorrection) {
                case 1: ecc_type = u"Other"; break;
                case 2: ecc_type = u"Unknown"; break;
                case 3: ecc_type = u"None"; break;
                case 4: ecc_type = u"Parity"; break;
                case 5: ecc_type = u"Single-bit ECC"; break;
                case 6: ecc_type = u"Multi-bit ECC"; break;
                case 7: ecc_type = u"CRC"; break;
            }
            simple_printf(u"  Error Correction: ");
            simple_printf(ecc_type);
            simple_printf(u"\r\n");
        } else {
            simple_printf(u"\r\nMemory Array: Information not available\r\n");
        }
        
        // Now enumerate all memory devices (Type 17)
        simple_printf(u"\r\nMemory Slot Details:\r\n");
        
        smbios_handle = 0xFFFE;
        smbios_type = SMBIOS_TYPE_MEMORY_DEVICE;
        UINTN slot_count = 0;
        UINTN populated_slots = 0;
        
        // Find all memory devices
        while (smbios_protocol->GetNext(smbios_protocol, &smbios_handle, &smbios_type, &record, NULL) == EFI_SUCCESS) {
            if (record != NULL) {
                SMBIOS_TYPE17_MEMORY_DEVICE *mem_device = (SMBIOS_TYPE17_MEMORY_DEVICE*)record;
                slot_count++;
                
                CHAR16 slot_str[16];
                uint_to_string(slot_count, slot_str, 16);
                simple_printf(u"  Slot ");
                simple_printf(slot_str);
                
                // Get slot locator string
                CHAR8 *locator = get_smbios_string(record, mem_device->DeviceLocator);
                if (locator != NULL && !is_placeholder_string(locator)) {
                    CHAR16 loc_unicode[64];
                    ascii_to_char16(locator, loc_unicode, 64);
                    simple_printf(u" (");
                    simple_printf(loc_unicode);
                    simple_printf(u")");
                }
                simple_printf(u": ");
                
                // Check if slot is populated
                if (mem_device->Size == 0) {
                    simple_printf(u"Empty\r\n");
                } else {
                    populated_slots++;
                    
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
                        case 25: mem_type = u"DDR3L"; break;
                        case 27: mem_type = u"LPDDR"; break;
                        case 28: mem_type = u"LPDDR2"; break;
                        case 29: mem_type = u"LPDDR3"; break;
                        case 30: mem_type = u"LPDDR4"; break;
                        case 33: mem_type = u"LPDDR5"; break;
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
                    
                    // Form factor and removability detection
                    CHAR16 *form_factor = u"Unknown";
                    BOOLEAN is_removable = TRUE; // Default assumption
                    
                    switch (mem_device->FormFactor) {
                        case 1: form_factor = u"Other"; break;
                        case 2: form_factor = u"Unknown"; break;
                        case 3: form_factor = u"SIMM"; break;
                        case 4: form_factor = u"SIP"; break;
                        case 5: form_factor = u"Chip"; is_removable = FALSE; break;
                        case 6: form_factor = u"DIP"; break;
                        case 7: form_factor = u"ZIP"; break;
                        case 8: form_factor = u"Proprietary Card"; break;
                        case 9: form_factor = u"DIMM"; break;
                        case 10: form_factor = u"TSOP"; is_removable = FALSE; break;
                        case 11: form_factor = u"Row of chips"; is_removable = FALSE; break;
                        case 12: form_factor = u"RIMM"; break;
                        case 13: form_factor = u"SODIMM"; break;
                        case 14: form_factor = u"SRIMM"; break;
                        case 15: form_factor = u"FB-DIMM"; break;
                    }
                    
                    simple_printf(u"\r\n");
                    simple_printf(u"           Form Factor: ");
                    simple_printf(form_factor);
                    
                    if (is_removable) {
                        simple_printf(u" (Removable)");
                    } else {
                        simple_printf(u" (Soldered/Fixed)");
                    }
                    simple_printf(u"\r\n");
                    
                    // Bank locator if available
                    CHAR8 *bank = get_smbios_string(record, mem_device->BankLocator);
                    if (bank != NULL && !is_placeholder_string(bank)) {
                        CHAR16 bank_unicode[64];
                        ascii_to_char16(bank, bank_unicode, 64);
                        simple_printf(u"           Bank: ");
                        simple_printf(bank_unicode);
                        simple_printf(u"\r\n");
                    }
                    
                    // Manufacturer if available
                    CHAR8 *manufacturer = get_smbios_string(record, mem_device->Manufacturer);
                    if (manufacturer != NULL && !is_placeholder_string(manufacturer)) {
                        CHAR16 mfg_unicode[64];
                        ascii_to_char16(manufacturer, mfg_unicode, 64);
                        simple_printf(u"           Manufacturer: ");
                        simple_printf(mfg_unicode);
                        simple_printf(u"\r\n");
                    }
                    
                    // Part number if available
                    CHAR8 *part_number = get_smbios_string(record, mem_device->PartNumber);
                    if (part_number != NULL && !is_placeholder_string(part_number)) {
                        CHAR16 part_unicode[64];
                        ascii_to_char16(part_number, part_unicode, 64);
                        simple_printf(u"           Part Number: ");
                        simple_printf(part_unicode);
                        simple_printf(u"\r\n");
                    }
                }
            }
        }
        
        // Summary
        simple_printf(u"\r\nMemory Summary:\r\n");
        CHAR16 summary_str[16];
        uint_to_string(populated_slots, summary_str, 16);
        simple_printf(u"  Populated Slots: ");
        simple_printf(summary_str);
        simple_printf(u" of ");
        uint_to_string(slot_count, summary_str, 16);
        simple_printf(summary_str);
        simple_printf(u"\r\n");
        
        if (slot_count == 0) {
            simple_printf(u"  No memory slots detected in SMBIOS\r\n");
        }
    } else {
        simple_printf(u"\r\nDetailed memory information not available (SMBIOS not found)\r\n");
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
                    
                    // Enhanced drive type detection
                    BOOLEAN is_ssd = FALSE;
                    BOOLEAN is_removable = block_io->Media->RemovableMedia;
                    
                    if (is_removable) {
                        simple_printf(u"  Type: Removable Media\r\n");
                    } else {
                        // SSD detection heuristics
                        if (block_io->Media->IoAlign >= 4096 || block_io->Media->BlockSize >= 4096) {
                            is_ssd = TRUE;
                        } else if (block_io->Media->OptimalTransferLengthGranularity > 1) {
                            is_ssd = TRUE;
                        } else if ((size_gb >= 120 && size_gb <= 128) ||
                                  (size_gb >= 240 && size_gb <= 256) ||
                                  (size_gb >= 480 && size_gb <= 512) ||
                                  (size_gb >= 960 && size_gb <= 1024) ||
                                  (size_gb >= 1920 && size_gb <= 2048) ||
                                  size_gb <= 64) {
                            is_ssd = TRUE;
                        }
                        
                        if (is_ssd) {
                            simple_printf(u"  Type: SSD");
                            if (block_io->Media->BlockSize >= 4096) {
                                simple_printf(u" (4K Native)");
                            } else if (block_io->Media->IoAlign >= 4096) {
                                simple_printf(u" (4K Aligned)");
                            }
                            simple_printf(u"\r\n");
                        } else {
                            simple_printf(u"  Type: HDD (Traditional Spinning Disk)\r\n");
                        }
                    }
                    
                    // Block size
                    CHAR16 block_str[16];
                    uint_to_string(block_io->Media->BlockSize, block_str, 16);
                    simple_printf(u"  Block Size: ");
                    simple_printf(block_str);
                    simple_printf(u" bytes\r\n");
                    
                    // Media properties
                    simple_printf(u"  Properties: ");
                    if (block_io->Media->ReadOnly) {
                        simple_printf(u"Read-Only ");
                    } else {
                        simple_printf(u"Read-Write ");
                    }
                    
                    if (block_io->Media->WriteCaching) {
                        simple_printf(u"Write-Cached");
                    }
                    simple_printf(u"\r\n");
                    
                    // Try to get controller information
                    EFI_GUID pci_io_guid = EFI_PCI_IO_PROTOCOL_GUID;
                    EFI_PCI_IO_PROTOCOL *pci_io;
                    if (bs->OpenProtocol(handle_buffer[i], &pci_io_guid, (VOID**)&pci_io, image, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL) == EFI_SUCCESS) {
                        UINT8 class_code[3];
                        if (pci_io->Pci.Read(pci_io, EfiPciIoWidthUint8, 0x0B, 3, &class_code) == EFI_SUCCESS) {
                            if (class_code[2] == 0x01) { // Mass storage controller
                                simple_printf(u"  Controller: ");
                                switch (class_code[1]) {
                                    case 0x05: simple_printf(u"ATA\r\n"); break;
                                    case 0x06: simple_printf(u"SATA\r\n"); break;
                                    case 0x07: simple_printf(u"SAS\r\n"); break;
                                    case 0x08: simple_printf(u"NVMe\r\n"); break;
                                    default: simple_printf(u"Storage\r\n"); break;
                                }
                            }
                        }
                        bs->CloseProtocol(handle_buffer[i], &pci_io_guid, image, NULL);
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
    
    simple_printf(u"\r\nNote: Drive model and serial number detection is limited in UEFI.\r\n");
    simple_printf(u"Boot into OS for detailed drive identification.\r\n");
}

// Main entry point
EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    init_globals(ImageHandle, SystemTable);
    
    // Clear screen and set colors
    cout->ClearScreen(cout);
    cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));
    
    simple_printf(u" Hardware Specification Tool\r\n");
    simple_printf(u"=========================================\r\n\r\n");
    
    // Display all hardware information directly
    display_all_info();
    
    // Wait for user to exit
    EFI_INPUT_KEY key;
    while (TRUE) {
        key = get_key();
        
        if (key.ScanCode == SCAN_ESC || key.UnicodeChar == u'q' || key.UnicodeChar == u'Q') {
            break;
        }
    }
    
    return EFI_SUCCESS;
}

// Display header and collect all hardware information
void display_all_info(void) {
    UINTN cols, rows;
    get_screen_dimensions(&cols, &rows);

    // Calculate split positions (2/3 and 1/3)
    UINTN left_split = (cols * 2) / 3;

    cout->ClearScreen(cout);
    cout->SetAttribute(cout, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));

    // Header
    simple_printf(u" Hardware Specification Tool\r\n");
    simple_printf(u"=========================================\r\n\r\n");

    // Display distilled information in left 2/3
    display_distilled_specs(left_split);

    // Display additional details in right 1/3
    display_additional_details(left_split);

    simple_printf(u"\r\n\r\nPress ESC or 'q' to exit...\r\n");
}
