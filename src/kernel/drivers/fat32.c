#include "fat32.h"

#include "hdd.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

// --- Constants --- //
// Defined in sections.asm
// See sections.asm for more details
extern int FS_SECTOR;

#define FAT_CLUSTER_SIZE HDD_SECTOR_SIZE

#define FAT_READ_ONLY   0x01
#define FAT_HIDDEN      0x02
#define FAT_SYSTEM      0x04
#define FAT_ROOT        0x08
#define FAT_DIR         0x10
#define FAT_ARCHIVE     0x20
#define FAT_LONG_NAME   0x0F

// --- Structs --- //
// The boot sector (extended)
typedef struct FatBPB_t
{
    uint8_t code[3];
    uint8_t oem[8];
    // Bytes / sector
    uint16_t sectorSize;
    // Sector / cluster
    uint8_t clusterSize;
    // In sectors, where FATs are
    uint16_t reservedSize;
    // FAT amount
    uint8_t fats;
    uint16_t rootEntries;
    // Number of sectors
    uint16_t nbSectors16;
    uint8_t media;
    uint16_t fatSize16;
    uint16_t sectorPerTrack;
    uint16_t nbHeads;
    uint32_t nbHidden;
    uint32_t nbSectors;
    uint32_t fatSize;
    uint16_t flags;
    uint16_t version;
    uint32_t rootCluster;
    uint16_t fsInfoCluster;
    uint16_t bpbCopy;
    uint8_t reserved[12];
    uint8_t drive;
    uint8_t reserved2;
    uint8_t bootSignature;
    uint32_t volId;
    uint8_t volLabel[11];
    uint8_t fsLabel[8];
} __attribute__((packed)) FatBPB;

// An entry is a directory or file (32 bytes)
// Use FatLongEntry for lonf file names
typedef struct FatEntry_t
{
    char name[8];
    char ext[3];
    uint8_t flags;
    uint8_t reserved;
    uint8_t createdTimeTenth;
    uint16_t createdTime;
    uint16_t createdDate;
    uint16_t accessedDate;
    uint16_t firstClusterHigh;
    uint16_t writtenTime;
    uint16_t writtenDate;
    uint16_t firstClusterLow;
    uint32_t fileSize;
} __attribute__((packed)) FatEntry;

// When a FatEntry has a long file name
typedef struct FatEntryLFN_t
{
    u8 order;
    u16 name1[5];
    u8 flags;
    u8 type;
    u8 checksum;
    u16 name2[6];
    u16 zero;
    u16 name3[2];
} __attribute__((packed)) FatEntryLFN;

// The FSInfo structure (512 bytes)
typedef struct FSInfo_t
{
    uint32_t leadSignature;
    uint8_t reserved[480];
    uint32_t signature;
    uint32_t nFree;
    uint32_t nextFree;
    uint8_t reserved2[12];
    uint32_t endSignature;
} __attribute__((packed)) FSInfo;

// --- Globals --- //
static FatBPB *bpb;
static FSInfo *fsInfo;

// The cluster we currently read
// (when we have to read only one cluster)
static uint8_t *cluster;

static size_t fatSector;
static size_t dataSector;
static size_t rootSector;

// --- Internal --- //
// Generates the entry at index i in the current cluster
// !!! Unsafe : i MUST be within the cluster
// * entryIndex is modified such as it points to the next entry
// * May returns NULL if the entry is end of directory
static FSEntry *genEntry(size_t *entryIndex)
{
    // Check end of listing
    for (;; ++*entryIndex)
    {
        if (*entryIndex >= FAT_CLUSTER_SIZE / sizeof(FSEntry))
            return NULL;

        // Entry number 'entryIndex'
        FatEntry *entry = &((FatEntry*)cluster)[*entryIndex];

        // End of entries
        if (((u8*) entry)[0] == 0)
            return NULL;

        // If not unused, continue
        if (((u8*) entry)[0] != 0xE5)
            break;
    }

    FatEntry *rawEntry = (FatEntry*)cluster + *entryIndex;

    char *entryName;
    u8 entryFlags = 0;
    FatFSEntryData *entryData = malloc(sizeof(FatFSEntryData));

    // Get name
    if (rawEntry->flags == FAT_LONG_NAME)
    {
        // The entry where there are flags
        FatEntry *lastEntry = rawEntry;

        // To retrieve the size of the string
        while (lastEntry->flags == FAT_LONG_NAME)
        {
            ++lastEntry;
            ++*entryIndex;
        }
        ++*entryIndex;

        // Number of entries of long file name
        size_t count = ((size_t)lastEntry - (size_t)rawEntry) / sizeof(FatEntry);

        // 13 is the number of chars within each entry
        entryName = malloc(count * 13);

        // Iterate again to retrieve the name
        lastEntry = rawEntry;
        size_t i = 0;
        for ( ; i < count; ++i)
        {
            // All indices of chars
            size_t charPos[] = {
                1, 3, 5, 7, 9,
                14, 16, 18, 20, 22, 24,
                28, 30
            };

            // This semi byte tells the LFN entry number
            size_t nameShift = (((char*)&lastEntry[i])[0] & 0xF) - 1;
            for (size_t j = 0; j < sizeof(charPos) / sizeof(size_t); ++j)
            {
                // Set the char by the char located at the index charPos[j]
                // in the i entry
                entryName[nameShift * 13 + j] = ((char*)&lastEntry[i])[charPos[j]];
            }
        }

        rawEntry = lastEntry + i;
    }
    else
    {
        // Last char of the name
        char *nameLast = rawEntry->ext + 2;

        // Find the end
        while (nameLast > rawEntry->name && *nameLast == 0x20)
            --nameLast;

        // There is no extension
        if (nameLast < rawEntry->ext)
        {
            size_t count = nameLast - rawEntry->name + 1;
            char *name = malloc(count + 1);

            memcpy(name, rawEntry->name, count);
            name[count] = '\0';

            entryName = (char*)name;
        }
        else
        {
            // + 2 for point (last is the last char not the end)
            size_t count = nameLast - rawEntry->name + 2;
            char *name = malloc(count + 1);

            // Copy name
            memcpy(name, rawEntry->name, 8);

            // Copy extension
            memcpy(&name[9], rawEntry->ext, 3);

            // Add point
            name[8] = '.';
            name[count] = '\0';

            entryName = (char *)name;
        }

        ++*entryIndex;
    }

    // rawEntry is now where flags are valid (in case of long file name)

    // Flags
    if (rawEntry->flags & FAT_DIR || rawEntry->flags & FAT_ROOT)
        entryFlags |= FS_DIRECTORY;

    if (rawEntry->flags & FAT_HIDDEN)
        entryFlags |= FS_HIDDEN;

    // Size
    size_t entrySize = rawEntry->fileSize;

    // Data
    entryData->cluster = (rawEntry->firstClusterHigh << 16) | rawEntry->firstClusterLow;

    // To .. to root
    if (entryData->cluster == 0)
        entryData->cluster = 2;

    return fatFSEntry_new(entryName, entryFlags, entrySize, entryData);
}

// --- Methods --- //
void fatInit()
{
    // Allocate the cluster
    cluster = malloc(FAT_CLUSTER_SIZE);

    // Load the BPB
    bpb = malloc(sizeof(FatBPB));
    hddRead(FS_SECTOR, bpb, 1);

    // TODO : Verify version + sector / cluster

    // Load FSInfo
    fsInfo = malloc(sizeof(FSInfo));
    hddRead(FS_SECTOR + bpb->fsInfoCluster, fsInfo, 1);

    // Parse root cluster
    fatSector = FS_SECTOR + bpb->reservedSize;
    // We retrieve 2 since the 2 first clusters are special ones
    dataSector = fatSector + bpb->fats * bpb->fatSize - 2;
    rootSector = dataSector + bpb->rootCluster;
}

void fatTerminate()
{
    free(bpb);
    free(root);
    free(fsInfo);
    free(cluster);
}

FSEntry *fatFSEntry_new(const char *name, u8 flags, size_t size, FatFSEntryData *data)
{
    return FSEntry_new(name, flags, size, data, fatGenFSEntryOps());
}

// TODO : rm when use FSEntryOps
void fatFSEntry_del(FSEntry *entry)
{
    // Free data
    FatFSEntryData_del(entry->data);

    // Free fs common fields
    FSEntry_del(entry);
}

void FatFSEntryData_del(FatFSEntryData *data)
{
    free(data);
}

// Reads a data cluster
void readCluster(size_t clusterNb, void *buffer, size_t bytesToLoad)
{
    // Read the cluster
    hddRead(dataSector + clusterNb, cluster, 1);

    // Load and copy
    memcpy(buffer, cluster, bytesToLoad);
}

size_t fatFSEntry_read(FSEntry *file, void *buffer, size_t count)
{
    if (count == 0)
        return;

    if (count > file->size)
        count = file->size;

    FatFSEntryData *f = (FatFSEntryData*)file->data;

    // This file has no content
    if (f->cluster == 0)
        return 0;

    // Bytes read
    size_t n = 0;

    u32 *fat = malloc(HDD_SECTOR_SIZE);

    // Current sector loaded in fat
    size_t currentFatSector = (size_t)-1;

    // How many fat entries in the fat
    size_t entriesPerSector = HDD_SECTOR_SIZE / 32;

    size_t currentCluster = f->cluster;
    for (size_t loadedClusters = 0; count != 0; ++loadedClusters)
    {
        // Copy the cluster
        size_t bytesToLoad = count > FAT_CLUSTER_SIZE ? FAT_CLUSTER_SIZE : count;
        readCluster(currentCluster, buffer, bytesToLoad);

        count -= bytesToLoad;
        n += bytesToLoad;
        buffer = (u8*)buffer + bytesToLoad;

        // The location in the fat of the cluster entry
        size_t fatCluster = (f->cluster + loadedClusters) / FAT_CLUSTER_SIZE;

        // Update fat if necessary
        if (currentFatSector != fatCluster)
        {
            currentFatSector = fatCluster;

            // Load it
            hddRead(fatSector + currentFatSector, fat, 1);
        }

        // The cluster to read into the buffer
        size_t nextCluster = fat[(f->cluster + loadedClusters) % entriesPerSector] & 0x0FFFFFFF;

        // This is the end (don't read bad sectors)
        if (nextCluster >= 0x0FFFFFF7)
            break;

        currentCluster = nextCluster;
    }

    free(fat);

    return n;
}

// Returns an array of entry
// The array is NULL terminated
FSEntry **fatFSEntry_list(FSEntry *dir)
{
    // Load the good cluster
    hddRead(dataSector + ((FatFSEntryData*) dir->data)->cluster, cluster, 1);

    // TODO : Multiple clusters
    size_t maxCount = FAT_CLUSTER_SIZE / sizeof(FatEntry);
    FSEntry **entries = malloc(sizeof(FSEntry*) * (maxCount + 1));

    size_t entryIndex = 0;
    for (size_t i = 0; i < maxCount; ++i)
    {
        // May be NULL to terminate the buffer (end of listing)
        entries[i] = genEntry(&entryIndex);

        if (entries[i] == NULL)
            break;
    }

    entries[maxCount] = NULL;

    return entries;
}

FSEntry *fatGenRoot()
{
    // Read the root cluster
    hddRead(rootSector, cluster, 1);

    size_t i = 0;
    FSEntry *root = genEntry(&i);

    // Root is not at cluster 0
    ((FatFSEntryData*) root->data)->cluster = rootSector - dataSector;

    return root;
}

FSEntryOps *fatGenFSEntryOps()
{
    FSEntryOps *ops = malloc(sizeof(FSEntryOps));

    *ops = (FSEntryOps) {
        .del = fatFSEntry_del,
        .list = fatFSEntry_list,
        .read = fatFSEntry_read,
    };

    return ops;
}

