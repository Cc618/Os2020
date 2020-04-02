# Memory

This explains where all sections are in memory

## Chunks

A chunk is a part of the os, there is 3 chunks :

- Stage1 : The first 512 bytes of the os, loads the stage 2 bootloader in memory and switchs to protected mode (Asm, 16 -> 32 bits)
- Stage2 : Loads the kernel and memory (Asm / C, 32 bits)
- Kernel : Contains everything except the bootloaders : core, libc, apps... (Asm / C, 32 bits) 

## Memory Map

| Section   | Start       |
| -------   | -----       |
| Stage1    | 0x7C00      |
| Stage2    | 0x7E00      |
| Kernel    | 0x1000000   |

Kernel memory map :

| Section            | Start     |
| -------            | -----     |
| Kernel + Libc code | 0x1000000 |
| Heap               | 0x1500000 |
| Stack / Heap limit | 0x1D00000 |
| Stack (Top)        | 0x2000000 |

## Specs

Each chunk contains a magic number at the end to be dynamically loaded.
