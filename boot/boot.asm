bits 16     ; Real Mode
org 0x7C00 

start:
    cli
    ; Segments
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov sp, 0x7C00

    sti

    ; Load kernel 
    mov bx, 0x1000
    mov es, bx
    xor bx, bx

    ; Disk param
    mov ah, 0x02
    mov al, 50
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, 0x80

    ; call bios
    int 0x13

    jc disk_error

    jmp 0x1000:0x0000

disk_error:    
    ; Clear screen
    mov ax, 0x0003      
    int 0x10
    
    ; Set cursor position
    mov ah, 0x02
    mov bh, 0x00
    mov dh, 12
    mov dl, 30
    int 0x10
    
    
    mov si, error_msg
    call print_red
    
    cli
    hlt

print_red:
    pusha
    mov bx, 0x0004      
    
.print_char:
    lodsb               
    or al, al           
    jz .done
    
    mov ah, 0x0E
    int 0x10
    
    jmp .print_char
    
.done:
    popa
    ret

error_msg db "[BOOTLOADER] Disk Error", 0

times 510-($-$$) db 0
dw 0xAA55