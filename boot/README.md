# BOOT
This directory is about the boot sector, loading and to entry the kernel.

## Files
Read comments in each files for more detail.
- bootsect.asm - Main code of boot sector.
- print.asm - Define a print string function.
- print_hex.asm - Define a print hex function.
- disk.asm - Define a function that read and load the disk.
- gdt.asm - Define the GDT and the GDTR.
- switch_pm.asm - A process to switch 32bits protected mode.
- 32bits_print.asm - Define a print string function works on PM.
- kernel_entry.asm - Call kernel_main.
