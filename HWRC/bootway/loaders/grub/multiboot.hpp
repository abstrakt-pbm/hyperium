#pragma once
#include "../../../hwrctypes.hpp" 

enum MultibootTagType : uint32_t {
    END,
    BOOT_CMD_LINE,
    BOOT_LOADER_NAME,
    MODULE,
    BASIC_MEMINFO,
    BOOTDEV,
    MMAP,
    VBE,
    FRAMEBUFFER,
    ELF_SECTIONS,
    APM,
    EFI32,
    EFI64,
    SMBIOS,
    ACPI_OLD,
    ACPI_NEW,
    NETWORK,
    EFI_MMAP,
    EFI_BS,
    EFI32_IH,
    EFI64_IH,
    LOAD_BASE_ADDR
};

enum MultibootMMAP_MEM_TYPE : uint32_t {
    MULTIBOOT_MEMORY_AVAILABLE = 1,
    MULTIBOOT_MEMORY_RESERVED = 2,
    MULTIBOOT_MEMORY_ACPI_RECLAIMABLE = 3,
    MULTIBOOT_MEMORY_NVS = 4,
    MULTIBOOT_MEMORY_BADRAM = 5
};

enum EFI_MEMORY_DESCRIPTOR_TYPE : uint32_t {
    EfiReservedMemoryType = 0x00000000,  // Зарезервированная память
    EfiLoaderCode = 0x00000001,          // Код загрузчика
    EfiLoaderData = 0x00000002,          // Данные загрузчика
    EfiBootServicesCode = 0x00000003,    // Код сервисов загрузки
    EfiBootServicesData = 0x00000004,    // Данные сервисов загрузки
    EfiRuntimeServicesCode = 0x00000005, // Код сервисов времени выполнения
    EfiRuntimeServicesData = 0x00000006, // Данные сервисов времени выполнения
    EfiConventionalMemory = 0x00000007,  // Обычная доступная память
    EfiUnusableMemory = 0x00000008,      // Недоступная память
    EfiACPIReclaimMemory = 0x00000009,   // Память для восстановления ACPI
    EfiACPIMemoryNVS = 0x0000000A,       // Память ACPI для не-volatile storage
    EfiMemoryMappedIO = 0x0000000B,      // Память для memory-mapped I/O
    EfiMemoryMappedIOPortSpace = 0x0000000C, // Память для пространства портов MMIO
    EfiPalCode = 0x0000000D,             // Платформенно-зависимый код
    EfiPersistentMemory = 0x0000000E,    // Постоянная память
    EfiMaxMemoryType = 0x80000000

};

class alignas(8) MultibootHeader {
    public:
    MultibootTagType type;
    uint32_t size;


};

class alignas(8) MultibootMMAP_Entry {
    public:
    uint64_t addr;
    uint64_t len;
    uint32_t mem_type;
    uint32_t zero; //always zero

};

class alignas(8) MultibootMMAP_Tag{
    public:
    MultibootHeader header;
    uint32_t entry_size;
    uint32_t entry_version;
    MultibootMMAP_Entry entries;

    uint64_t get_entry_count();
    uint64_t get_minimal_addr();
    uint64_t get_maximum_addr();

    MultibootMMAP_Entry* operator[]( size_t index );

};

class alignas(8) Multiboot_ACPI_NEW_Tag {
    public:
    uint32_t type;
    uint32_t size;
    void* rsdp_addr; // & to get addr;
};

class Multiboot_EFI_MMAP_Descriptor { 
    public:
    EFI_MEMORY_DESCRIPTOR_TYPE type;
    uint32_t pad;
    uint64_t physical_start;
    uint64_t virtual_start;
    uint64_t num_pages;
    uint64_t attribute;
};

class Mutliboot_EFI_MMAP_Tag {
    public:
    MultibootHeader header;
    uint32_t descr_size;
    uint32_t descr_version;
    uint8_t mmap[];
};

class MultibootInfo {
    void* multiboot_info_ptr;
    uint32_t total_size;

    public:
    void init( void* start_addr );
    uint32_t get_tag_type_entry_count( MultibootTagType tag_type);
    void* get_particular_tag( MultibootTagType tag_type, uint32_t index );
};