# Files
BIN = bin/os
CHUNK_STAGE1 = obj/chunks/stage1
CHUNK_STAGE2 = obj/chunks/stage2
CHUNK_KERNEL = obj/chunks/kernel
DIR_STAGE1 = src/stage1
DIR_STAGE2 = src/stage2
DIR_KERNEL = src/kernel
SRC_STAGE1 = $(wildcard $(DIR_STAGE1)/*)
SRC_STAGE2_C = $(wildcard $(DIR_STAGE2)/*.c)
SRC_STAGE2_ASM = $(wildcard $(DIR_STAGE2)/*.asm)

# TODO : Update
OBJ_DIRS = obj/kernel obj/stage1 obj/stage2 obj/chunks

# Tools
TOOL_ASM = nasm
TOOL_VM = qemu-system-i386
TOOL_C = /media/data/donnees/linux/logiciels/i386-elf-9.1.0/bin/i386-elf-gcc
TOOL_LINK = /media/data/donnees/linux/logiciels/i386-elf-9.1.0/bin/i386-elf-ld

# Flags
FLAGS_C = -Wall -Wextra -std=c99 -nostdinc -ffreestanding -nostdlib -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -lgcc -D__is_kernel -m32
# FLAG_C = -Wall -Wextra -nostdinc -ffreestanding -nostdlib -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -lgcc -lk -D__is_kernel -fno-exceptions -fno-rtti -m32 -MMD -fno-use-cxa-atexit


all: $(BIN)

$(BIN): mkdirs $(CHUNK_STAGE1) $(CHUNK_STAGE2)
# TODO : Update (kernel)
	cat $(CHUNK_STAGE1) $(CHUNK_STAGE2) > $@


# --- Stage 1 --- #
$(CHUNK_STAGE1): $(SRC_STAGE1)
	$(TOOL_ASM) -f bin -o $(CHUNK_STAGE1) -i src/stage1 src/stage1/bootloader.asm

# --- Stage 2 --- #
$(CHUNK_STAGE2): $(SRC_STAGE2_ASM) $(SRC_STAGE2_C)
# python3 -c "print('A' * (512 * 3 - 4) + 'BOOT', end='')" > $@
# TODO : Auto
	$(TOOL_ASM) -f elf32 -i src/stage2 -o obj/stage2/sections.o src/stage2/sections.asm
	$(TOOL_C) $(C_FLAGS) -c -I src/stage2 -o obj/stage2/main.o src/stage2/main.c
	$(TOOL_LINK) -T stage2.ld -e main --oformat binary -o $@ obj/stage2/*.o


# --- Kernel --- #
# TODO : Update
$(CHUNK_KERNEL):
	python3 -c "print('A' * (512 * 3 - 4) + 'BOOT', end='')" > $@

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




