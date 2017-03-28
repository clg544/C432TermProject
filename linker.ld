/* Linking script for ld for exact linking. */
/* First iteration from wiki.osdev.org      */

/* rammap */
MEMORY
{
    ram : ORIGIN = 0x80200000, LENGTH = 0x10000
}

SECTIONS
{
    .text : { *(.text*) } > ram
}
