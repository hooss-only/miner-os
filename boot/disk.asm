; read a disk from 2nd sector and load it to ES:BX
; DH for amount of sector
; DL for number of drive to read
load_disk:
  pusha
  push dx

  mov ah, 2 ; disk read

  ; amount of sector
  ; a sector = 512 bytes
  mov al, dh

  mov ch, 0

  ; num of starting sector
  ; 1 - bootsect, 2 and more - out of bootsect
  mov cl, 2

  mov dh, 0

  int 0x13 ; disk interrupt
  jc disk_error ; jmp when error

  pop dx
  cmp al, dh ; al = sectors read, dh = sectors need to be read
  jne sector_error ; jmp when al != dh

  popa
  ret

disk_error:
  mov bx, DISK_ERR
  call print
  call print_nl
  mov dh, ah
  call print_hex

  jmp $

sector_error:
  mov bx, SECTOR_ERR
  call print
  jmp $

DISK_ERR db "Disk read error", 0
SECTOR_ERR db "Incorrect number of sectors read", 0
