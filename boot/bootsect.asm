[org 0x7c00]
KERNEL_OFFSET equ 0x1000
  mov bp, 0x9000
  mov sp, bp

  mov ah, 0
  mov al, 3
  int 0x10 ; Clear

  call load_kernel

  mov ah, 0
  mov al, 0x13
  int 0x10 ; Clear, change to color pixel mode

  call switch_to_pm

  jmp $ ; loop

%include "boot/print.asm"
%include "boot/print_hex.asm"
%include "boot/disk.asm"
%include "boot/gdt.asm"
%include "boot/switch_pm.asm"

[bits 16]
load_kernel:
  ; SIZE OF KERNEL
  ; 0x20 = 32
  ; 32 sectors = 16 KBs
  mov dh, 0x20
  mov bx, KERNEL_OFFSET
  mov dl, [BOOT_DRIVE]

  call load_disk

  ret

[bits 32]
BEGIN_PM:
  call KERNEL_OFFSET
  jmp $

BOOT_DRIVE db 0
MSG_BL_START db "Bootloader started.", 0
MSG_KNL_LOAD db "Loading Kernel", 0
MSG_PM_START db "Protected mode started", 0

times 510-($-$$) db 0
dw 0xaa55
