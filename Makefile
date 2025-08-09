ASM = nasm
EMU = qemu-system-x86_64
dir = build

all: boot/bootsect.bin

run: boot/bootsect.bin
	$(EMU) -fda boot/bootsect.bin

%.bin: %.asm
	$(ASM) -f bin $< -o $@

clean:
	rm -rf *.bin
	rm -rf boot/*.bin
