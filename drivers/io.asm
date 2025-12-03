global outb
global inb

; outb - send a byte to an I/O port
outb:
    mov al, [esp + 8]
    mov dx, [esp + 4]
    out dx, al
    ret

; inb - read a byte from an I/O port
inb:
    mov dx, [esp + 4]
    in al, dx
    ret
