global loader                  ; entry symbol for ELF
extern kmain

MAGIC_NUMBER equ 0x1BADB002    ; define the magic number constant
FLAGS        equ 0x0           ; multiboot flags
CHECKSUM     equ -MAGIC_NUMBER ; calculate the checksum. multiboot specification
                               ; requires that MAGIC_NUMBER + FLAGS + CHECKSUM == 0

KERNEL_STACK_SIZE equ 4096     ; size of stack in bytes

section .bss
align 4
kernel_stack:
    resb KERNEL_STACK_SIZE

section .text                  ; start of code
align 4                        ; the code must be 4 bytes aligned
    dd MAGIC_NUMBER            ; write the MAGIC_NUMBER to the machine code,
    dd FLAGS                   ; the flags,
    dd CHECKSUM                ; and the checksum

loader:                        ; the loader label (defined as entry point in linker script)
    mov esp, kernel_stack + KERNEL_STACK_SIZE ; point esp to the start of the
                                              ; stack (end of memory area)

    call kmain ; go to my kmain.c file
