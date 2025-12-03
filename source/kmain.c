#include "../drivers/frame_buffer.h"
#include "../drivers/interrupts.h"

int kmain(void) {
    interrupts_install_idt();

    char msg[] = "Welcome to Tiny OS. Type below:\n";
    fb_write(msg, sizeof(msg) - 1);

    return 0;
}
