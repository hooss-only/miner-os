[org 0x7c00]
KERNEL_OFFSET equ 0x1000
  mov bp, 0x9000
  mov sp, bp

  mov ah, 0
  mov al, 3
  int 0x10 ; Clear

  mov bx, MSG_BL_START
  call print
  call print_nl
  
  mov bx, MSG_KNL_LOAD
  call print
  call print_nl

  call load_kernel
  call switch_to_pm

  jmp $ ; loop

%include "boot/print.asm"
%include "boot/print_hex.asm"
%include "boot/disk.asm"
%include "boot/32bits_print.asm"
%include "boot/gdt.asm"
%include "boot/switch_pm.asm"

[bits 16]
load_kernel:
  ; SIZE OF KERNEL
  ; 0x40 = 64
  ; 32 sectors = 16 KBs
  mov dh, 0x20
  mov bx, KERNEL_OFFSET
  mov dl, [BOOT_DRIVE]

  call load_disk

  ret

[bits 32]
BEGIN_PM:
  mov ebx, MSG_PM_START
  call print_pm
  call KERNEL_OFFSET
  jmp $

BOOT_DRIVE db 0
MSG_BL_START db "Bootloader started.", 0
MSG_KNL_LOAD db "Loading Kernel", 0
MSG_PM_START db "Protected mode started", 0

times 510-($-$$) db 0
dw 0xaa55
