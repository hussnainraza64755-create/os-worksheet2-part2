OBJECTS = source/loader.o source/kmain.o drivers/io.o drivers/frame_buffer.o drivers/pic.o drivers/interrupts.o drivers/interrupt_handlers.o drivers/hardware_interrupt_enabler.o drivers/interrupt_asm.o drivers/keyboard.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -c
LDFLAGS = -T source/link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -A os -input-charset utf8 -quiet -boot-info-table -o os.iso iso

run: os.iso
	qemu-system-i386 -nographic -boot d -cdrom os.iso -m 32 -d cpu -D logQ.txt

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf source/*.o kernel.elf os.iso iso/boot/kernel.elf logQ.txt
