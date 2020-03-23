# TODO : Remove dir stage1 (useless)

# Files
BIN = bin/os
CHUNK_STAGE1 = obj/chunks/stage1
CHUNK_STAGE2 = obj/chunks/stage2
CHUNK_KERNEL = obj/chunks/kernel
DIR_STAGE1 = src/stage1
DIR_STAGE2 = src/stage2
DIR_KERNEL = src/kernel
DIR_LIBC = src/libc
SRC_STAGE1 = $(wildcard $(DIR_STAGE1)/*)
SRC_STAGE2_C = $(wildcard $(DIR_STAGE2)/*.c)
SRC_STAGE2_ASM = $(wildcard $(DIR_STAGE2)/*.asm)
OBJ_STAGE2_C = $(addsuffix .o, $(subst src/,obj/, $(SRC_STAGE2_C)))
OBJ_STAGE2_ASM = $(addsuffix .o, $(subst src/,obj/, $(SRC_STAGE2_ASM)))
DEP_STAGE2 = $(OBJ_STAGE2_C:.o=.d)
SRC_KERNEL_C = $(wildcard $(DIR_KERNEL)/*.c)
SRC_KERNEL_C += $(wildcard $(DIR_KERNEL)/*/*.c)
SRC_KERNEL_ASM = $(wildcard $(DIR_KERNEL)/*.asm)
SRC_KERNEL_ASM += $(wildcard $(DIR_KERNEL)/*/*.asm)
OBJ_KERNEL_C = $(addsuffix .o, $(subst src/,obj/, $(SRC_KERNEL_C)))
OBJ_KERNEL_ASM = $(addsuffix .o, $(subst src/,obj/, $(SRC_KERNEL_ASM)))
DEP_KERNEL = $(OBJ_KERNEL_C:.o=.d)
SRC_LIBC_C = $(wildcard $(DIR_LIBC)/*.c)
SRC_LIBC_C += $(wildcard $(DIR_LIBC)/*/*.c)
SRC_LIBC_ASM = $(wildcard $(DIR_LIBC)/*.asm)
OBJ_LIBC_C = $(addsuffix .o, $(subst src/,obj/, $(SRC_LIBC_C)))
OBJ_LIBC_ASM = $(addsuffix .o, $(subst src/,obj/, $(SRC_LIBC_ASM)))
DEP_LIBC = $(OBJ_LIBC_C:.o=.d)

OBJ_DIRS = obj/kernel obj/libc obj/stage1 obj/stage2 obj/chunks
OBJ_DIRS += $(dir $(OBJ_KERNEL_C))
OBJ_DIRS += $(dir $(OBJ_LIBC_C))

# Tools
TOOL_ASM = nasm
TOOL_VM = qemu-system-i386
TOOL_C = /media/data/donnees/linux/logiciels/i386-elf-9.1.0/bin/i386-elf-gcc
TOOL_LINK = /media/data/donnees/linux/logiciels/i386-elf-9.1.0/bin/i386-elf-ld

# Flags
FLAGS_C = -Wall -Wextra -std=c99 -nostdinc -ffreestanding -nostdlib -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -lgcc -D__is_kernel -m32 -MMD


# --- Main --- #
all: $(BIN)

$(BIN): mkdirs $(CHUNK_STAGE1) $(CHUNK_STAGE2) $(CHUNK_KERNEL)
	cat $(CHUNK_STAGE1) $(CHUNK_STAGE2) $(CHUNK_KERNEL) > $@


# --- Stage 1 --- #
$(CHUNK_STAGE1): $(SRC_STAGE1)
	$(TOOL_ASM) -f bin -o $(CHUNK_STAGE1) -i $(DIR_STAGE1) $(DIR_STAGE1)/bootloader.asm


# --- Stage 2 --- #
$(CHUNK_STAGE2): $(OBJ_STAGE2_ASM) $(OBJ_STAGE2_C)
	$(TOOL_LINK) -T stage2.ld -e main --oformat binary -o $@ $^

obj/stage2/%.c.o: src/stage2/%.c
	$(TOOL_C) $(FLAGS_C) -c -I $(DIR_STAGE2) -o $@ $<

obj/stage2/%.asm.o: src/stage2/%.asm
	$(TOOL_ASM) -f elf32 -i $(DIR_STAGE2) -o $@ $<


# --- Kernel --- #
$(CHUNK_KERNEL): $(OBJ_KERNEL_ASM) $(OBJ_KERNEL_C) $(OBJ_LIBC_ASM) $(OBJ_LIBC_C)
	$(TOOL_LINK) -T kernel.ld -e main --oformat binary -o $@ $^

obj/kernel/%.c.o: src/kernel/%.c
	$(TOOL_C) $(FLAGS_C) -c -I $(DIR_KERNEL) -I $(DIR_LIBC) -o $@ $<

obj/kernel/%.asm.o: src/kernel/%.asm
	$(TOOL_ASM) -f elf32 -i $(DIR_KERNEL) -o $@ $<


# --- Libc --- #
obj/libc/%.c.o: src/libc/%.c
	$(TOOL_C) $(FLAGS_C) -c -I $(DIR_LIBC) -o $@ $<

obj/libc/%.asm.o: src/libc/%.asm
	$(TOOL_ASM) -f elf32 -i $(DIR_LIBC) -o $@ $<


# --- Utils --- #
run: $(BIN)
	$(TOOL_VM) -drive format=raw,if=ide,index=0,file=$(BIN)

.PHONY: mkdirs
mkdirs:
	mkdir -p bin obj $(OBJ_DIRS)

.PHONY: clean
clean:
	rm -rf obj bin


# --- Depedencies --- #
-include $(DEP_STAGE2)
-include $(DEP_KERNEL)
