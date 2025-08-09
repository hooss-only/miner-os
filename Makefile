ASM = nasm
EMU = qemu-system-x86_64

C_SOURCES = $(wildcard kernel/*.c)
HEADERS = $(wildcard kernel/*.h)
OBJS = $(C_SOURCES:.c=.o)

CC = i686-elf-gcc
CFLAGS = -g

all: miner-os.bin

run: miner-os.bin
	$(EMU) -fda boot/bootsect.bin

miner-os.bin: boot/bootsect.bin kernel.bin
	cat $^ > miner-os.bin

kernel.bin: boot/kernel_entry.o $(OBJS)
	i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

%.bin: %.asm
	$(ASM) -f bin $< -o $@

%.o: %.c {HEADERS}
	$(CC) $(CFLAGS) --ffreestanding -c $< -o $@
	
%.o: %.asm
	nasm -f elf $< -o $@

clean:
	rm -rf *.bin *.o
	rm -rf boot/*.bin kernel/*.o
