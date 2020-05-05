#include "fat32.h"

#include "hdd.h"
#include "syscalls/syscalls.h"
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
// Use FatLongEntry for long file names
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
// !!! Unsafe : entryIndex MUST be within the cluster
// * entryIndex is modified such as it points to the next entry
// * May returns NULL if the entry is end of directory
static FSEntry *genEntry(size_t entryCluster, size_t *entryIndex)
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
    entryData->entryI = *entryIndex - 1;
    entryData->entryCluster = entryCluster;

    // To .. to root
    if (entryData->cluster == 0)
        entryData->cluster = 2;

    return fatFSEntry_new(entryName, entryFlags, entrySize, entryData);
}

// Returns the next free cluster
// * Changes fsInfo->nextFree and mark this cluster as terminal in the FAT
static size_t allocateCluster()
{
    u32 *fatEntries = malloc(FAT_CLUSTER_SIZE);

    // Current loaded sector in fatEntries
    size_t loadedFatSector = -1;
    size_t nextFree = fsInfo->nextFree;

    // Default value
    if (nextFree == -1)
        nextFree = 2;

    // Index of nextFree in fatEntries
    size_t fatEntry;

    do
    {
        // Check whether we have to update the loaded sector
        size_t sectorToLoad = nextFree * sizeof(u32) / HDD_SECTOR_SIZE;
        if (sectorToLoad != loadedFatSector)
        {
            loadedFatSector = sectorToLoad;
            hddRead(fatSector + loadedFatSector, fatEntries, 1);
        }

        // Index of the entry
        fatEntry = nextFree % (HDD_SECTOR_SIZE / sizeof(u32));

        // Empty cluster found
        if (fatEntries[fatEntry] == 0)
            break;

        ++nextFree;

        if (nextFree > bpb->nbSectors)
            sys_fatal("Can't write Fat32 hard drive (no empty space)");
    } while (1);

    // Update fsInfo
    fsInfo->nextFree = nextFree;

    // Mark this cluster as terminal
    fatEntries[fatEntry] = 0x0FFFFFF8;

    // Update fat
    hddWrite(fatEntries, fatSector + loadedFatSector, 1);

    free(fatEntries);

    return nextFree;
}

// Unallocates a chain of clusters
// * Updates fsInfo
static void unallocateClusters(size_t first)
{
    size_t cluster = first;
    u32 *fat = malloc(HDD_SECTOR_SIZE);
    
    // Load FAT
    hddRead(fatSector + cluster / (HDD_SECTOR_SIZE / sizeof(u32)), fat, 1);
    
    while (1)
    {
        u32 nextCluster = fat[cluster % (HDD_SECTOR_SIZE / sizeof(u32))] & 0x0FFFFFFF;

        // Mark this cluster as free
        fat[cluster % (HDD_SECTOR_SIZE / sizeof(u32))] = 0;
        if (cluster < fsInfo->nextFree)
            fsInfo->nextFree = cluster;

        // End of chain
        if (nextCluster >= 0x0FFFFFF7)
            break;

        if (cluster % (HDD_SECTOR_SIZE / sizeof(u32)) != nextCluster % (HDD_SECTOR_SIZE / sizeof(u32)))
        {
            // Update FAT
            hddWrite(fat, fatSector + cluster / (HDD_SECTOR_SIZE / sizeof(u32)), 1);
            hddRead(fatSector + nextCluster / (HDD_SECTOR_SIZE / sizeof(u32)), fat, 1);
        }

        cluster = nextCluster;
    }
    
    // Update FAT
    hddWrite(fat, fatSector + cluster / (HDD_SECTOR_SIZE / sizeof(u32)), 1);
    
    free(fat);
}

// Extends 
static size_t extendCluster(size_t cluster)
{
    // Allocate new cluster and add the entry
    u32 *fat = malloc(HDD_SECTOR_SIZE);

    // Allocate
    size_t newCluster = allocateCluster();

    // Update fat
    hddRead(fatSector + cluster / (HDD_SECTOR_SIZE / sizeof(u32)), fat, 1);
    fat[cluster % (HDD_SECTOR_SIZE / sizeof(u32))] = newCluster;
    hddWrite(fat, fatSector + cluster / (HDD_SECTOR_SIZE / sizeof(u32)), 1);

    free(fat);

    return newCluster;
}

// Allocates a chain of clusters and writes content into
// * Returns the first cluster of the chain
static size_t writeContent(void *content, size_t length)
{
    if (length == 0)
        return 0;
    
    size_t count = length / FAT_CLUSTER_SIZE;
    size_t first = allocateCluster();

    size_t cluster = first;

    // First clusters
    for (size_t i = 0; i < count; ++i)
    {
        // Write the content on disk
        hddWrite((u8*)content + i * FAT_CLUSTER_SIZE, dataSector + cluster, 1);

        // Allocate next cluster
        cluster = extendCluster(cluster);
    }

    // Last one
    hddWrite((u8*)content + count * FAT_CLUSTER_SIZE, dataSector + cluster, 1);

    return first;
}

// Used to follow a cluster chain in an iterator way.
// * To close the iterator, just set clusterId to -1
// !!! fat is automatically allocated but not clusterContent
// Example :
// * void *fat = NULL;
// * u32 clusterId = 5;
// * char *clusterContent = malloc(FAT_CLUSTER_SIZE);
// * while (clusterIter(&fat, &clusterId, clusterContent))
// *    // Do something with clusterContent
// * free(clusterContent);
static bool clusterIter(u32 **fat, u32 *clusterId, void *clusterContent)
{
    // First call : Read fat
    if (!*fat)
    {
        *fat = malloc(HDD_SECTOR_SIZE);
        hddRead(fatSector + *clusterId * sizeof(u32) / HDD_SECTOR_SIZE, *fat, 1);
    }

    // Terminal / bad cluster
    if (*clusterId >= 0x0FFFFFF7)
    {
        free(*fat);
        *fat = NULL;

        return false;
    }

    // The entry describing the next cluster or terminal cluster
    u32 fatValue = (*fat)[(*clusterId) % (HDD_SECTOR_SIZE / sizeof(u32))];

    // Terminal / bad cluster
    if (fatValue < 0x0FFFFFF7)
    {
        // Update fat if necessary
        size_t fatEntryCluster = fatValue * sizeof(u32) / HDD_SECTOR_SIZE;
        if (*clusterId * sizeof(u32) / HDD_SECTOR_SIZE != fatEntryCluster)
            hddRead(fatSector + fatEntryCluster, *fat, 1);
    }

    // Load the content of the current cluster
    hddRead(dataSector + *clusterId, clusterContent, 1);

    *clusterId = fatValue;

    // Continue to iterate
    return true;
}

// Returns the checksum for LFN
// * tinyName has 11 chars
static u8 lfnSum(const char *tinyName)
{
    u8 sum = 0;
    for (size_t i = 0; i < 11; i++)
        sum = (((sum & 1) << 7) | ((sum & 0xFE) >> 1)) + tinyName[i];

    return sum;
}

// Generates an entry of a directory
// - contentCluster is the id of the first cluster of the content
// - outEntryLength is the number of items in outEntry
// * Returns are out*
static void genDirEntry(const char *name, bool directory, size_t fileSize, size_t contentCluster, FatEntry **outEntry, size_t *outEntryLength)
{
    size_t nameLength = strlen(name);

    // Name in the last entry (fill empty space)
    char *tinyName = strdup("___________");
    memcpy(tinyName, name, nameLength > 8 ? 8 : nameLength);
    u8 checksum = lfnSum(tinyName);

    // Required entries
    *outEntryLength = (nameLength - 1) / 13 + 2;

    *outEntry = malloc(sizeof(FatEntry) * *outEntryLength);

    memset(*outEntry, 0, sizeof(FatEntry) * *outEntryLength);

    size_t nameI = 0;

    // All indices of chars in LFN entry
    size_t charPos[] = {
        1, 3, 5, 7, 9,
        14, 16, 18, 20, 22, 24,
        28, 30
    };

    // Long name entry
    for (size_t i = 0; i < *outEntryLength - 1; ++i)
    {
        u8 entryNb = *outEntryLength - i - 2;

        // Name
        for (size_t j = 0; j < 13; ++j)
        {
            if (nameI >= nameLength)
                break;

            // Set letter
            FatEntry *entry = &(*outEntry)[entryNb];
            ((u8*)entry)[charPos[j]] = name[nameI];

            ++nameI;
        }

        // Order
        ((FatEntryLFN*)*outEntry)[i].order = (entryNb + 1) | (i == 0 ? 0x40 : 0);

        // Flags
        ((FatEntryLFN*)*outEntry)[i].flags = FAT_LONG_NAME;

        // LFN specials
        ((FatEntryLFN*)*outEntry)[i].checksum = checksum;
        ((FatEntryLFN*)*outEntry)[i].type = 0;
    }

    // Last entry
    (*outEntry)[*outEntryLength - 1].flags = directory ? FAT_DIR : 0;

    // Content
    (*outEntry)[*outEntryLength - 1].firstClusterLow = contentCluster & 0x0000FFFF;
    (*outEntry)[*outEntryLength - 1].firstClusterHigh = (contentCluster & 0xFFFF0000) >> 16;
    (*outEntry)[*outEntryLength - 1].fileSize = fileSize;

    // Name
    memcpy((*outEntry)[*outEntryLength - 1].name, tinyName, 11);
    free(tinyName);
}

// Adds an entry (may be a stack of entries for long file names)
// and writes it to dir
// - outEntryCluster : Cluster of the directory where the entry
// - outEntryI : Index of the entry within this cluster (first entry if LFN)
static void addDirEntry(u32 dirCluster, FatEntry *entry, size_t entryLength, size_t *outEntryCluster, size_t *outEntryI)
{
    // We have to find a string of 'entryLength' entries in dirCluster

    void *fat = NULL;
    FatEntry *clusterContent = malloc(FAT_CLUSTER_SIZE);
    *outEntryCluster = dirCluster;

    // Where to put entry
    *outEntryI = 0;

    // Whether we have found a suitable place in this entry
    bool allocated = false;

    // For each cluster of this directory
    while (clusterIter(&fat, &dirCluster, clusterContent))
    {
        for (*outEntryI = 0; *outEntryI < FAT_CLUSTER_SIZE / sizeof(FatEntry) - entryLength; ++*outEntryI)
        {
            size_t j = 0;
            for ( ; j < entryLength; ++j)
                // If this string is not empty
                if (!(clusterContent[*outEntryI + j].name[0] == 0 ||
                    clusterContent[*outEntryI + j].name[0] == 0xE5))
                    goto notEmpty;

            // This string is suitable
            break;

        notEmpty:;
            *outEntryI += j;
        }

        // We have found a suitable place at index *outEntryI
        if (*outEntryI != FAT_CLUSTER_SIZE / sizeof(FatEntry) - entryLength)
        {
            // Like a break
            dirCluster = -1;
            allocated = true;
            continue;
        }

        if (dirCluster < 0x0FFFFFF7)
            *outEntryCluster = dirCluster;
    }

    // Allocate a new cluster
    if (!allocated)
    {
        // Mark terminal entries as deleted
        for ( ; *outEntryI < FAT_CLUSTER_SIZE / sizeof(FatEntry); ++*outEntryI)
            if (clusterContent[*outEntryI].name[0] == 0)
                clusterContent[*outEntryI].name[0] = 0xE5;

        // Update dir on disk
        hddWrite(clusterContent, dataSector + *outEntryCluster, 1);

        size_t newCluster = extendCluster(*outEntryCluster);

        // Append the entry
        void *newClusterContent = malloc(FAT_CLUSTER_SIZE);

        // No more entry after
        memset(newClusterContent, 0, FAT_CLUSTER_SIZE);

        memcpy(newClusterContent, entry, entryLength * sizeof(FatEntry));
        hddWrite(newClusterContent, dataSector + newCluster, 1);

        free(newClusterContent);
    }
    else
    {
        memcpy(&clusterContent[*outEntryI], entry, entryLength * sizeof(FatEntry));

        // Write to the hdd
        hddWrite(clusterContent, dataSector + *outEntryCluster, 1);
    }

    free(clusterContent);
}

// Creates the content of an empty directory
// * Allocates a new cluster in the partition
void emptyDir(u32 *outCluster, u32 parentCluster)
{
    void *content = malloc(FAT_CLUSTER_SIZE);
    *outCluster = allocateCluster();

    // Null terminate the cluster
    memset(content, 0, FAT_CLUSTER_SIZE);

    // . directory
    // Fill name + ext
    memset(((FatEntry*) content)[0].name, 0x20, 11);
    ((FatEntry*) content)[0].name[0] = '.';
    ((FatEntry*) content)[0].firstClusterHigh = ((0xFF00 & *outCluster) >> 16);
    ((FatEntry*) content)[0].firstClusterLow = 0xFF & *outCluster;
    ((FatEntry*) content)[0].flags = FAT_DIR;

    // .. directory
    // Fill name + ext
    memset(((FatEntry*) content)[1].name, 0x20, 11);
    ((FatEntry*) content)[1].name[0] = '.';
    ((FatEntry*) content)[1].name[1] = '..';
    ((FatEntry*) content)[1].firstClusterHigh = ((0xFF00 & parentCluster) >> 16);
    ((FatEntry*) content)[1].firstClusterLow = 0xFF & parentCluster;
    ((FatEntry*) content)[1].flags = FAT_DIR;

    // Write content
    hddWrite(content, dataSector + *outCluster, 1);

    free(content);
}

// Creates a file to the directory
// * name is assumed valid (valid chars and non zero length)
// * dir is assumed to be a directory
// * If directory and null content, creates an empty directory
static FSEntry *fatAllocate(FSEntry *dir, const char *name, bool directory, u32 size, void *content)
{
    // Write the content
    size_t contentCluster;

    if (directory && size == 0)
        emptyDir(&contentCluster, ((FatFSEntryData*)dir->data)->cluster);
    else if (size != 0)
        contentCluster = writeContent(content, size);
    else
        // No content
        contentCluster = 0;

    FatEntry *entry;
    size_t entryLength;

    // Generate in RAM the directory entry (can be multiple entries for long file names)
    genDirEntry(name, directory, size, contentCluster, &entry, &entryLength);

    size_t entryCluster;
    size_t entryI;

    // Add it to dir
    addDirEntry(((FatFSEntryData*)dir->data)->cluster, entry, entryLength, &entryCluster, &entryI);

    free(entry);

    // Generate the entry data
    FatFSEntryData *data = malloc(sizeof(FatFSEntryData));
    data->cluster = contentCluster;
    data->entryCluster = entryCluster;
    // Last entry
    data->entryI = entryI + entryLength - 1;

    return FSEntry_new(name, directory ? FS_DIRECTORY : 0, size, data, fatGenFSEntryOps());
}

// Unallocates and replaces the content of
// the entry
// * To remove content : firstCluster = 0 and size = 0
static void replaceContent(FSEntry *f, u32 firstCluster, u32 size)
{
    FatFSEntryData *data = f->data;

    // Remove content
    if (data->cluster > 2)
        unallocateClusters(data->cluster);

    // Update size & cluster
    hddRead(dataSector + data->entryCluster, cluster, 1);

    ((FatEntry*) cluster)[data->entryI].fileSize = size;
    ((FatEntry*) cluster)[data->entryI].firstClusterHigh = (firstCluster & 0xFF00) >> 16;
    ((FatEntry*) cluster)[data->entryI].firstClusterLow = firstCluster & 0xFF;
    
    hddWrite(cluster, dataSector + data->entryCluster, 1);
}

// --- Methods --- //
void fatInit()
{
    // Allocate the cluster
    cluster = malloc(FAT_CLUSTER_SIZE);

    // Load the BPB
    bpb = malloc(sizeof(FatBPB));
    hddRead(FS_SECTOR, bpb, 1);

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
    // Update fsInfo
    hddWrite(fsInfo, FS_SECTOR + bpb->fsInfoCluster, 1);

    free(bpb);
    free(root);
    free(fsInfo);
    free(cluster);
}

FSEntry *fatFSEntry_new(const char *name, u8 flags, size_t size, FatFSEntryData *data)
{
    return FSEntry_new(name, flags, size, data, fatGenFSEntryOps());
}

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

size_t fatFSEntry_write(FSEntry *f, void *buffer, size_t n)
{
    // Overwrite content
    size_t newContent = writeContent(buffer, n);

    replaceContent(f, newContent, n);

    return n;
}

// Returns an array of entry
// The array is NULL terminated
FSEntry **fatFSEntry_list(FSEntry *dir, size_t *outCount)
{
    size_t dirCluster = ((FatFSEntryData*) dir->data)->cluster;

    // Load the good cluster
    hddRead(dataSector + dirCluster, cluster, 1);

    // TODO : Multiple clusters
    size_t maxCount = FAT_CLUSTER_SIZE / sizeof(FatEntry);
    FSEntry **entries = malloc(sizeof(FSEntry*) * maxCount);

    size_t entryIndex = 0;
    for (*outCount = 0; *outCount < maxCount; ++*outCount)
    {
        // May be NULL to terminate the buffer (end of listing)
        entries[*outCount] = genEntry(dirCluster, &entryIndex);

        if (entries[*outCount] == NULL)
            break;
    }

    return entries;
}

// Adds a new file without content
// * name is assumed valid (valid chars and non zero length)
// * dir is assumed to be a directory
FSEntry *fatFSEntry_touch(FSEntry *dir, const char *name, u8 flags)
{
    return fatAllocate(dir, name, (flags & FS_DIRECTORY) != 0, 0, NULL);
}

FSEntry *fatGenRoot()
{
    // Read the root cluster
    hddRead(rootSector, cluster, 1);

    size_t i = 0;
    FSEntry *root = genEntry(2, &i);

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
        .write = fatFSEntry_write,
        .touch = fatFSEntry_touch,
    };

    return ops;
}

