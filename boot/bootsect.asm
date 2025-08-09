mov ah, 0x0E
mov al, 'H'
int 0x10
mov al, 'I'
int 0x10

jmp $

times 510-($-$$) db 0
dw 0xaa55
