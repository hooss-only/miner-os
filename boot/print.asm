; a function to print string.
; bx: string pointer
print:
  pusha

print_loop:
  mov al, [bx]
  cmp al, 0
  je print_done

  mov ah, 0x0e
  int 0x10

  add bx, 1
  jmp print_loop

print_done:
  popa
  ret
