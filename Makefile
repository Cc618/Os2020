# Files
BIN = bin/os
CHUNK_BOOT = obj/chunks/boot
CHUNK_KERNEL = obj/chunks/kernel
SRC_BOOT = $(wildcard src/boot/*)
# TODO : Update
OBJ_DIRS = obj/kernel obj/boot obj/chunks

# Tools
TOOL_ASM = nasm
TOOL_VM = qemu-system-i386
# TOOL_C = /media/data/donnees/linux/logiciels/i386-elf-9.1.0/bin/i386-elf-gcc
# TOOL_LINK = /media/data/donnees/linux/logiciels/i386-elf-9.1.0/bin/i386-elf-ld

# Flags
# FLAGS_C = -Wall -Wextra -std=c99 -nostdinc -ffreestanding -nostdlib -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -lgcc -D__is_kernel -m32



all: $(BIN)

$(BIN): mkdirs $(CHUNK_BOOT) $(CHUNK_KERNEL)
	cat $(CHUNK_BOOT) $(CHUNK_KERNEL) > $@

# --- Boot --- #
$(CHUNK_BOOT): $(SRC_BOOT)
	$(TOOL_ASM) -f bin -o $(CHUNK_BOOT) -i src/boot src/boot/bootloader.asm


# --- Kernel --- #
$(CHUNK_KERNEL):
# TODO : Update
	touch $@

# --- Utils --- #
run: $(BIN)
	$(TOOL_VM) $(BIN)

.PHONY: mkdirs
mkdirs:
	mkdir -p bin obj $(OBJ_DIRS)

.PHONY: clean
clean:
	rm -rf obj bin



# TODO
# Include depedencies
# -include $(DEP)

# TODO
# .PHONY: tst
# tst:
# # nasm -f elf32 -o tst.asm.o tst.asm

# # $(CC) $(CFLAGS) -c -o obj/b.o src/boot/b.c
# # $(CC) $(CFLAGS) -c -o obj/k.o src/kernel/k.c

# 	$(LD) -T linker.ld -e pmMain --oformat binary -o tst obj/boot/b.o obj/kernel/k.o




