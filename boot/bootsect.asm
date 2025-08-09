[org 0x7c00]
mov bx, msg
call print

jmp $

%include "boot/print.asm"

msg db "Hello, World", 0

times 510-($-$$) db 0
dw 0xaa55
