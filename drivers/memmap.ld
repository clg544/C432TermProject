OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

MEMORY {
    ram : ORIGIN = 0x402F0400, LENGTH = 0x1B400
}

SECTIONS
{
    .reset : {
	start_old.o (.text)
	. = ALIGN(0x4);
    } > ram

    .text : { 
	*(.text .text.*)
	. = ALIGN(0x4);
	*(.rodata .rodata.*)
    } > ram

    .data : { 
	__data_load = LOADADDR (.data);
	__data_start = .;
	*(.data .data.*) 
	. = ALIGN(0x4);
	__edata = .;
    } > ram

    .bss : { 
	__bss_start__ = . ;
	*(.bss .bss.*)
	. = ALIGN (4);
	__bss_end__ = .;
	__bss_size__ = __bss_end__ - __bss_start__;
     } > ram
}
