ASM = nasm
EMU = qemu-system-i386

C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h)
OBJS = $(C_SOURCES:.c=.o)

CC = i686-elf-gcc
CFLAGS = -g

DBG = pwndbg

all: miner-os.bin

run: all
	$(EMU) -fda miner-os.bin

debug: miner-os.bin kernel.elf
	$(EMU) -S -s -fda miner-os.bin -d guest_errors,int &
	$(DBG) -ex "target remote :1234" -ex "symbol-file kernel.elf"

miner-os.bin: boot/bootsect.bin kernel.bin
	cat $^ > miner-os.bin

kernel.bin: boot/kernel_entry.o $(OBJS)
	i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/kernel_entry.o $(OBJS)
	i686-elf-ld -o $@ -Ttext 0x1000 $^

%.bin: %.asm
	$(ASM) -f bin $< -o $@

%.o: %.c {HEADERS}
	$(CC) $(CFLAGS) --ffreestanding -c $< -o $@
	
%.o: %.asm
	nasm -f elf $< -o $@

clean:
	rm -rf *.bin *.o
	rm -rf boot/*.bin kernel/*.o drivers/*.o boot/*.o cpu/*.o
