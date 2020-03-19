# Files
BIN = bin/os
CHUNK_BOOT = obj/boot
CHUNK_KERNEL = obj/kernel
SRC_BOOT = $(wildcard src/boot/*)

# Tools
TOOL_ASM = nasm
# TOOL_C = /media/data/donnees/linux/logiciels/i386-elf-9.1.0/bin/i386-elf-gcc
# TOOL_LINK = /media/data/donnees/linux/logiciels/i386-elf-9.1.0/bin/i386-elf-ld

# Flags
# FLAGS_C = -Wall -Wextra -std=c99 -nostdinc -ffreestanding -nostdlib -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -lgcc -D__is_kernel -m32



all: $(BIN)

$(BIN): $(CHUNK_BOOT) $(CHUNK_KERNEL)
	cat $(CHUNK_BOOT) $(CHUNK_KERNEL) > $@

$(CHUNK_BOOT): $(SRC_BOOT)
	$(TOOL_ASM) -f bin -o $(CHUNK_BOOT) -i src/boot src/boot/bootloader.asm

run: $(BIN)
	qemu-system-i386 $(BIN)






# TODO
.PHONY: tst
tst:
# nasm -f elf32 -o tst.asm.o tst.asm

# $(CC) $(CFLAGS) -c -o obj/b.o src/boot/b.c
# $(CC) $(CFLAGS) -c -o obj/k.o src/kernel/k.c

	$(LD) -T linker.ld -e pmMain --oformat binary -o tst obj/boot/b.o obj/kernel/k.o













.PHONY: clean
clean:
	rm -rf obj bin

# TODO
# Include depedencies
# -include $(DEP)
