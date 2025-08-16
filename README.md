# MINER OS
A 32bits operation system which is only designed for minesweeper game.

## How to build
What you need to build this project:
- nasm
- i686-elf-gcc
- i686-elf-ld
- make

```
make
```

<br>When you want to run my OS on emulator, you can use `run` script.
```
make run
```
It needs `qemu` or `qemu-system-i386`.<br><br>


When you need to debug my OS, you can use `debug` script.
```
make debug
```
It needs `pwndbg` to use. If you prefer `gdb`, you have to change `DBG` variable in the Makefile.

## Structure
You can get more detail in each directory.
- Dir, boot - Bootloader
- Dir, cpu - CPU communication
- Dir, drivers - Drivers
- Dir, kernel - kernel main
- Dir, game - for game
- LICENSE - MIT License.
- Makefile - Script for building this projects
