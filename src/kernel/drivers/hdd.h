#pragma once

// Hard Disk Drive (ATA) driver

#include <stddef.h>

#define HDD_SECTOR_SIZE 512

// Copies sectorCount sectors from offset src in
// the hdd to address dst in ram
void hddRead(size_t src, void *dst, size_t sectorCount);

// Copies sectorCount sectors from address src in ram to
// offset dst in the hdd
void hddWrite(void *src, size_t dst, size_t sectorCount);

