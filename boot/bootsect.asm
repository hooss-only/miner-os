[org 0x7c00]
  mov ah, 0
  mov al, 3
  int 0x10 ; Clear

  mov bx, MSG_BL_START
  call print

  call switch_to_pm

  jmp $ ; loop

%include "boot/print.asm"
%include "boot/32bits_print.asm"
%include "boot/gdt.asm"
%include "boot/switch_pm.asm"

[bits 32]
BEGIN_PM:
  mov ebx, MSG_PM_START
  call print_pm
  
  jmp $

MSG_BL_START db "Bootloader started.", 0
MSG_PM_START db "Protected mode started", 0

times 510-($-$$) db 0
dw 0xaa55
