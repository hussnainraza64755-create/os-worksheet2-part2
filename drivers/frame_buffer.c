#include "io.h"
#include "frame_buffer.h"

char *fb = (char *) 0x000B8000;
unsigned short cursor_pos = 0;

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg) {
    fb[i * 2] = c;
    fb[i * 2 + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

void fb_move_cursor(unsigned short pos) {
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}

void fb_clear() {
    unsigned int i;
    for (i = 0; i < 80 * 25; i++) {
        fb_write_cell(i, ' ', FB_WHITE, FB_BLACK);
    }
    cursor_pos = 0;
    fb_move_cursor(cursor_pos);
}

void fb_write(char *buf, unsigned int len, unsigned char color) {
    unsigned int i;
    for (i = 0; i < len; i++) {
        if (buf[i] == '\n') {
            cursor_pos += 80 - (cursor_pos % 80);
        } else {
            fb_write_cell(cursor_pos, buf[i], color, FB_BLACK);
            cursor_pos++;
        }
        
        if (cursor_pos >= 80 * 25) {
            cursor_pos = 0;
        }
        fb_move_cursor(cursor_pos);
    }
}

void fb_write_dec(u32int i, unsigned char color) {
    char buf[32];
    int pos = 0;
    
    if (i == 0) {
        fb_write("0", 1, color);
        return;
    }

    while (i > 0) {
        buf[pos++] = '0' + (i % 10);
        i /= 10;
    }

    char rev_buf[32];
    int j;
    for (j = 0; j < pos; j++) {
        rev_buf[j] = buf[pos - 1 - j];
    }
    fb_write(rev_buf, pos, color);
}
