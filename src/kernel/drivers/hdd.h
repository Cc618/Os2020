#pragma once

// Hard Disk Drive (ATA) driver

#include <stddef.h>

#define HDD_SECTOR_SIZE 512

// Copies sectorCount sectors from offset src in
// the hdd to address dst in ram
void hddRead(size_t src, void *dst, size_t sectorCount);

