CC = gcc
AS = nasm
LD = ld

CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I. -c
ASFLAGS = -f elf
LDFLAGS = -T link.ld -melf_i386

C_SOURCES = $(shell find -name "*.c")
AS_SOURCES = $(shell find -name "*.s")
OBJECTS = $(C_SOURCES:.c=.o) $(AS_SOURCES:.s=.o)

all: kernel.elf

kernel.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

os.iso: kernel.elf
	mv kernel.elf iso/boot/kernel.elf
	grub-mkrescue -o $@ iso

run: os.iso
	qemu-system-i386 -m 32 -cdrom os.iso -boot d -monitor stdio

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) os.iso iso/boot/kernel.elf
