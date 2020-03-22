# TODO : Remove dir stage1 (useless)

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
OBJ_STAGE2_C = $(addsuffix .o, $(subst src/,obj/, $(SRC_STAGE2_C)))
OBJ_STAGE2_ASM = $(addsuffix .o, $(subst src/,obj/, $(SRC_STAGE2_ASM)))
DEP_STAGE2 = $(OBJ_STAGE2_C:.o=.d)

# TODO : Update
OBJ_DIRS = obj/kernel obj/stage1 obj/stage2 obj/chunks

# Tools
TOOL_ASM = nasm
TOOL_VM = qemu-system-i386
TOOL_C = /media/data/donnees/linux/logiciels/i386-elf-9.1.0/bin/i386-elf-gcc
TOOL_LINK = /media/data/donnees/linux/logiciels/i386-elf-9.1.0/bin/i386-elf-ld

# Flags
FLAGS_C = -Wall -Wextra -std=c99 -nostdinc -ffreestanding -nostdlib -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -lgcc -D__is_kernel -m32 -MMD


all: $(BIN)

$(BIN): mkdirs $(CHUNK_STAGE1) $(CHUNK_STAGE2) $(CHUNK_KERNEL)
	cat $(CHUNK_STAGE1) $(CHUNK_STAGE2) $(CHUNK_KERNEL) > $@


# --- Stage 1 --- #
$(CHUNK_STAGE1): $(SRC_STAGE1)
	$(TOOL_ASM) -f bin -o $(CHUNK_STAGE1) -i src/stage1 src/stage1/bootloader.asm


# --- Stage 2 --- #
$(CHUNK_STAGE2): $(OBJ_STAGE2_ASM) $(OBJ_STAGE2_C)
# TODO : Auto (+deps)
	@echo $(DEP_STAGE2)
	$(TOOL_LINK) -T stage2.ld -e main --oformat binary -o $@ $^

obj/stage2/%.c.o: src/stage2/%.c
	$(TOOL_C) $(FLAGS_C) -c -I src/stage2 -o $@ $<

obj/stage2/%.asm.o: src/stage2/%.asm
	$(TOOL_ASM) -f elf32 -i src/stage2 -o $@ $<


# --- Kernel --- #
# TODO : Update
$(CHUNK_KERNEL):
	python3 -c "print('KERNEL' + 'A' * (512 - 6 - 4) + 'CORE', end='')" > $@


# --- Utils --- #
run: $(BIN)
	$(TOOL_VM) -drive format=raw,if=ide,index=0,file=$(BIN)
	# $(TOOL_VM) $(BIN)
	# qemu-system-x86_64 -hda $(BIN)
	# qemu-system-i386 -hda $(BIN)
	# $(TOOL_VM) -drive id=disk,file=$(BIN),if=none -device ahci,id=ahci -device ide-drive,drive=disk,bus=ahci.0
	# $(TOOL_VM) -drive format=raw,id=disk,file=$(BIN),if=none -device ahci,id=ahci -device ide-drive,drive=disk,bus=ahci.0
	# $(TOOL_VM) -hda $(BIN)

.PHONY: mkdirs
mkdirs:
	mkdir -p bin obj $(OBJ_DIRS)

.PHONY: clean
clean:
	rm -rf obj bin



# TODO : Other deps
# Include depedencies
-include $(DEP_STAGE2)

# TODO
# .PHONY: tst
# tst:
# # nasm -f elf32 -o tst.asm.o tst.asm

# # $(CC) $(CFLAGS) -c -o obj/b.o src/boot/b.c
# # $(CC) $(CFLAGS) -c -o obj/k.o src/kernel/k.c

# 	$(LD) -T linker.ld -e pmMain --oformat binary -o tst obj/boot/b.o obj/kernel/k.o




