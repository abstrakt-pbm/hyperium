ENTRY(_init_env)

KERNEL_VMA = 0xFFFF900000000000;  /* Виртуальный адрес */
KERNEL_LMA = 0x10000;            /* Физический адрес */

SECTIONS {
    . = KERNEL_LMA;

    /* Секция для заголовка Multiboot2 */
    .multiboot2 ALIGN(0x1000) : {
        *(.multiboot2)
    } :multiboot2

    _init_data_lma = ALIGN(0x1000);


    /* Секция инициализации */
    .init.data ALIGN(0x1000) : AT(_init_data_lma) {
        *(.init.data)
        *(.init.data*)
    } :init

    _init_text_lma = ALIGN(0x1000);
    
    .init.text ALIGN(0x1000) : AT(_init_text_lma) {
        *(.init.text)
        *(.init.text*)
    } :init

    _init_end = ALIGN(0x1000);
    _text_lma = ALIGN(0x200000); /* Секция кода ядра начинается после .init */

    
    /* --- ПРОМЕЖУТОЧНЫЙ АДРЕС --- */
    . = KERNEL_VMA; /* Виртуальный адрес ядра начинается с KERNEL_VMA */
    _kernel_virtual_start = .;

    /* Секция кода ядра */
    .text ALIGN(0x1000) : AT(_text_lma) {
        *(.text)
        *(.text.*)
    } :text
    _text_end_lma = ALIGN(0x1000) - KERNEL_VMA + _text_lma;

    _rodata_lma = _text_end_lma;

    /* Секция для только чтения данных */
    .rodata ALIGN(0x1000) : AT(_rodata_lma) {
        *(.rodata)
        *(.rodata.*)
    } :rodata

    _rodata_lma_end = ALIGN(0x1000) - KERNEL_VMA + _text_lma;

    _data_lma = _rodata_lma_end;

    /* Секция данных */
    .data ALIGN(0x1000) : AT(_data_lma) {
        *(.data)
        *(.data.*)
    } :data

    _data_lma_end = ALIGN(0x1000) - KERNEL_VMA + _text_lma;

    _bss_lma = _data_lma_end;

    /* Секция BSS */
    .bss ALIGN(0x1000) : AT(_bss_lma) {
        *(COMMON)
        *(.bss)
    } :bss
    _bss_virtual_end = .;
    _bss_physical_end = ALIGN(0x1000) - KERNEL_VMA + _text_lma; /* Физическое расположение bss */
    
    
    

    /DISCARD/ : {
        *(.comment)
    }
}

PHDRS {
    multiboot2 PT_LOAD FILEHDR PHDRS FLAGS(4);   /* Загрузить заголовок Multiboot2 */
    init       PT_LOAD FLAGS(5);                  /* Загрузить секцию инициализации */
    text       PT_LOAD FLAGS(5);                  /* Загрузить секцию кода ядра */
    rodata     PT_LOAD FLAGS(4);                  /* Загрузить только для чтения данные */
    data       PT_LOAD FLAGS(6);                  /* Загрузить данные (чтение/запись) */
    bss        PT_LOAD FLAGS(6);                  /* Загрузить незаполненную память */
}
