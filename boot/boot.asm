bits 16     ; Real Mode
org 0x7C00 

start:
    ; Segments
    xor ax, ax 
    mov bx, ax
    mov es, ax

    ; Load kernel 
    mov bx, 0x1000
    mov es, bx
    xor bx, bx

    ; Disk param
    mov ah, 0x02
    mov al, 35
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0x80

    ; call bios
    int 0x13

    jmp 0x1000:0x0000

times 510-($-$$) db 0
dw 0xAA55