[org 0x7c00]
  mov ah, 0
  mov al, 3
  int 0x10
  mov bx, MSG_BL_START
  call print

jmp $

%include "boot/print.asm"

MSG_BL_START db "Bootloader started.", 0

times 510-($-$$) db 0
dw 0xaa55
