PROJECT_NAME = miner-os

ASM = nasm
EMU = qemu-system-i386

C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c game/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h game/*.h)
OBJS = $(C_SOURCES:.c=.o)

CC = i686-elf-gcc
CFLAGS = -g

DBG = pwndbg

OUTPUT = $(PROJECT_NAME).bin

all: $(OUTPUT)

run: all
	$(EMU) -fda $(OUTPUT)

# Pause emulator and remote with pwndbg.
debug: $(OUTPUT) kernel.elf
	$(EMU) -S -s -fda $(OUTPUT) -d guest_errors,int &
	$(DBG) -ex "target remote :1234" -ex "symbol-file kernel.elf"

miner-os.bin: boot/bootsect.bin kernel.bin
	cat $^ > miner-os.bin

kernel.bin: boot/kernel_entry.o $(OBJS)
	i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# Symbols for debuging
kernel.elf: boot/kernel_entry.o $(OBJS)
	i686-elf-ld -o $@ -Ttext 0x1000 $^

%.bin: %.asm
	$(ASM) -f bin $< -o $@

%.o: %.c {HEADERS}
	$(CC) $(CFLAGS) --ffreestanding -c $< -o $@
	
%.o: %.asm
	nasm -f elf $< -o $@

clean:
	rm -rf *.bin *.o *.elf
	rm -rf boot/*.bin kernel/*.o drivers/*.o boot/*.o cpu/*.o
