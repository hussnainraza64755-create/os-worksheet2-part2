#include "interrupts.h"
#include "pic.h"
#include "io.h"
#include "frame_buffer.h"
#include "keyboard.h"

struct IDTDescriptor idt_descriptors[256];
struct IDT idt;

void interrupts_init_descriptor(s32int index, u32int address) {
    idt_descriptors[index].offset_high = (address >> 16) & 0xFFFF;
    idt_descriptors[index].offset_low = (address & 0xFFFF);
    idt_descriptors[index].segment_selector = 0x08;
    idt_descriptors[index].reserved = 0x00;
    idt_descriptors[index].type_and_attr = 0x8E;
}

void interrupts_install_idt() {
    interrupts_init_descriptor(33, (u32int) interrupt_handler_33);

    idt.address = (s32int) &idt_descriptors;
    idt.size = sizeof(struct IDTDescriptor) * 256 - 1;
    load_idt((s32int) &idt);

    pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);
    outb(0x21, 0xFD);
    enable_hardware_interrupts();
}

void interrupt_handler(struct cpu_state cpu, u32int interrupt, struct stack_state stack) {
    u8int input;
    u8int ascii;

    if (interrupt == 33) {
        input = keyboard_read_scan_code();
        ascii = keyboard_scan_code_to_ascii(input);
        if (ascii != 0) {
            char str[2];
            str[0] = ascii;
            str[1] = '\0';
            fb_write(str, 1, FB_WHITE); /* Added FB_WHITE color */
        }
        pic_acknowledge(interrupt);
    }
}
