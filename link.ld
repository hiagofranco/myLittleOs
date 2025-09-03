ENTRY(loader) /* Same entry label */

SECTIONS {
    . = 0x00100000;         /* the code should be loaded at 1 MB */

    .text ALIGN(0x1000) :   /* Align at 4KB */
    {
        *(.text)            /* all the sections from all files */
    }

    .rodata ALIGN(0x1000) : /* Align at 4KB */
    {
        *(.rodata*)          /* all the read-only data sections from all files */
    }

    .data ALIGN(0x1000) : /* Align at 4KB */
    {
        *(.data)          /* all the data sections from all files */
    }

    .bss ALIGN(0x1000) : /* Align at 4KB */
    {
        *(COMMON)
        *(.bss)          /* all the data sections from all files */
    }
}