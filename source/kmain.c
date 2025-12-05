#include "../drivers/frame_buffer.h"
#include "../drivers/interrupts.h"

int sum_of_three(int arg1, int arg2, int arg3) {
    return arg1 + arg2 + arg3;
}

int multiply_two(int a, int b) {
    return a * b;
}

int verify_check(int input) {
    if (input == 0x1234) return 1;
    return 0;
}

int kmain(void) {
    interrupts_install_idt();
    
    fb_clear();
    
    char msg[] = "Welcome to My Tiny OS (Fixed)\n";
    fb_write(msg, sizeof(msg) - 1, FB_GREEN);

    /* Task 2 Logic */
    int sum = sum_of_three(1, 2, 3);
    char sum_msg[] = "Sum (1+2+3): ";
    fb_write(sum_msg, sizeof(sum_msg) - 1, FB_WHITE);
    fb_write_dec(sum, FB_CYAN);
    fb_write("\n", 1, FB_WHITE);

    int prod = multiply_two(10, 5);
    char prod_msg[] = "Product (10*5): ";
    fb_write(prod_msg, sizeof(prod_msg) - 1, FB_WHITE);
    fb_write_dec(prod, FB_CYAN);
    fb_write("\n", 1, FB_WHITE);

    char type_msg[] = "Type below:\n";
    fb_write(type_msg, sizeof(type_msg) - 1, FB_LIGHT_RED);

    return sum + prod;
}
