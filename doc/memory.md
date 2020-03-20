# Memory

This explains where all sections are in memory

## Chunks

A chunk is a part of the os, there is 3 chunks :

- Stage1 : The first 512 bytes of the os, loads the stage 2 bootloader in memory (Asm, 16 bits)
- Stage2 : Prepare kernel entry (protected mode, GDT, IDT...) and loads the kernel and memory (Asm / C, 16 -> 32 bits)
- Kernel : Contains everything except the bootloaders : core, libc, apps... (C, 32 bits) 

## Memory Map


| Section   | Start       |
| -------   | -----       |
| Stage1    | 0x7C00      |
| Stage2    | 0x7E00      |
| Kernel    | 0x1000000   |

## Specs

Each chunk contains a magic number at the end to be dynamically loaded.

