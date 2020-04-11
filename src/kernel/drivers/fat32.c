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

#define FAT_READ_ONLY 0x01
#define FAT_HIDDEN 0x02
#define FAT_SYSTEM 0x04
#define FAT_ROOT 0x08
#define FAT_DIR 0x10
#define FAT_ARCHIVE 0x20
#define FAT_LONG_NAME 0x0F

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
// TMP :
// root has type FatEntry but is a cluster (= 1 sector) wide
// static FatEntry *root;

// The cluster we currently read
// (when we have to read only one cluster)
static uint8_t *cluster;

static size_t fatSector;
static size_t dataSector;
static size_t rootSector;

// --- Internal --- //
// Allocates and retrieve the name of the entry
// TODO : Test with long + extensions
static char *entryName(FatEntry *entry)
{
    if (entry->flags == FAT_LONG_NAME)
    {
        // TODO :
        return "LONG";
    }
    else
    {
        // Last char of the name
        char *nameLast = entry->ext + 2;

        // Find the end
        while (nameLast > entry->name && *nameLast == 0x20)
            --nameLast;

        // There is no extension
        if (nameLast < entry->ext)
        {
            size_t count = nameLast - entry->name + 1;
            char *name = malloc(count + 1);

            memcpy(name, entry->name, count);
            name[count] = '\0';

            return (char *)name;
        }
        else
        {
            size_t count = nameLast - entry->name + 2;
            char *name = malloc(count + 1);

            // Copy name
            memcpy(name, entry->name, 8);

            // Copy extension
            memcpy(&name[9], entry->ext, 3);

            // Add point
            name[8] = '.';
            name[count] = '\0';

            return (char *)name;
        }
    }
}

// Generates the entry at index i in the current cluster
// !!! Unsafe : i MUST be within the cluster
static FSEntry *genEntry(size_t i)
{
    FatEntry *rawEntry = (FatEntry*)cluster + i;
    FSEntry *entry = malloc(sizeof(FSEntry));

    // Get name
    // TODO : Test on very long file name
    if (rawEntry->flags == FAT_LONG_NAME)
    {
        // The entry where there are flags
        FatEntry *lastEntry = rawEntry;

        // To retrieve the size of the string
        while (lastEntry->flags == FAT_LONG_NAME)
            ++lastEntry;
        
        // Number of entries of long file name
        size_t count = ((size_t)lastEntry - (size_t)rawEntry) / sizeof(FatEntry);

        // Multiply by the number of chars within each entry
        entry->name = malloc(count * 13);

        // Iterate again to retrieve the name
        lastEntry = rawEntry;
        for (size_t i = 0; i < count; ++i)
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
                entry->name[nameShift * 13 + j] = ((char*)&lastEntry[i])[charPos[j]];
            }
        }

        rawEntry = lastEntry;
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

            entry->name = (char*)name;
        }
        else
        {
            size_t count = nameLast - entry->name + 2;
            char *name = malloc(count + 1);

            // Copy name
            memcpy(name, rawEntry->name, 8);

            // Copy extension
            memcpy(&name[9], rawEntry->ext, 3);

            // Add point
            name[8] = '.';
            name[count] = '\0';

            entry->name = (char *)name;
        }
    }

    // rawEntry is now where flags are valid (in case of long file name)









    // Name
    // entry->name = entryName(rawEntry);

    // Flags
    if (rawEntry->flags & FAT_DIR || rawEntry->flags & FAT_ROOT)
        entry->flags |= FS_DIRECTORY;
    
    if (rawEntry->flags & FAT_HIDDEN)
        entry->flags |= FS_HIDDEN;

    // Data
    entry->data = rawEntry;

    return entry;
}

// Returns all entries within a directory
// Returns NULL if the directory is invalid
// * Reads a cluster
static FatEntry **enumDir(FatEntry *dir)
{
    // Not a directory
    if (!(dir->flags & FAT_DIR || dir->flags & FAT_ROOT))
        return NULL;

    // Read the cluster where there is all entries
    uint32_t dirCluster = (dir->firstClusterHigh << 16) | dir->firstClusterLow;
    hddRead(dataSector + dirCluster, cluster, 1);

    // Temporary buffer
    FatEntry *entries[FAT_CLUSTER_SIZE / sizeof(FatEntry)];

    // For each entry
    // TODO : Not the same with long names
    size_t i = 0;
    for (; i < FAT_CLUSTER_SIZE / sizeof(FatEntry); ++i)
    {
        // No more entries
        if (cluster[i * sizeof(FatEntry)] == 0)
            break;

        // Copy entry
        entries[i] = malloc(sizeof(FatEntry));
        memcpy(entries[i], &((FatEntry *)cluster)[i], sizeof(FatEntry));
    }

    // Copy entries
    FatEntry **result = malloc((i + 1) * sizeof(FatEntry *));
    memcpy(result, entries, i * sizeof(FatEntry *));

    result[i] = NULL;

    return result;
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

    // Load root
    fatSector = FS_SECTOR + bpb->reservedSize;
    dataSector = fatSector + bpb->fats * bpb->fatSize;
    rootSector = dataSector + bpb->rootCluster - 2;

    root = malloc(FAT_CLUSTER_SIZE);
    hddRead(rootSector, root, 1);

    // TMP :
    // // Test //
    // // Get name
    // char *rootName = entryName(root);
    // printf("Name %s\n", rootName);
    // free(rootName);

    // // Enum
    // FatEntry **files = enumDir(root);

    // for (size_t i = 0; files[i] != NULL; ++i)
    // {
    //     char *name = entryName(files[i]);
    //     printf("- %s\n", name);
    //     free(name);
    // }

    // // Free files
    // while (*files != NULL)
    //     free(*(files++));
    // free(files);
}

void fatTerminate()
{
    free(bpb);
    free(root);
    free(fsInfo);
    free(cluster);
}

FSEntry *fatGenRoot()
{
    // Read the root cluster
    hddRead(rootSector, cluster, 1);

    // TMP : 0
    return genEntry(3);
}
