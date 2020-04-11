#pragma once

// FAT32 file system driver

#include "fs/fs.h"
#include "k/types.h"

void fatInit();

void fatTerminate();

// Generates the root entry
FSEntry *fatGenRoot();
