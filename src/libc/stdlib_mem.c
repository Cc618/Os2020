#include "stdlib.h"

#include "syscall.h"

// Addresses of the heap
#define HEAP_START  0x1500000
#define HEAP_END    0x1900000

// The header before a dynamically allocated
// memory block
// This struct is 16 bytes wide
typedef struct malloc_header_t
{
    // Size of the data block
    size_t size;

    // Next or previous block
    // NULL if first / last
    struct malloc_header_t *previous;
    struct malloc_header_t *next;

    // Make this struct 16 bits aligned
    size_t padding;
} __attribute__((packed)) MallocHeader;

// Points to the last block header or NULL if
// no block allocated
static MallocHeader *lastBlock = NULL;
static MallocHeader *firstBlock = NULL;

void free(void *ptr)
{
    if (!ptr)
    {
        // TODO : Error message nullptr freed
        SYSC0(SYS_FATAL);
    }

    // The header associated to the data
    MallocHeader *header = (MallocHeader*)((size_t)ptr - sizeof(MallocHeader));

    // Update first block
    if (header == firstBlock)
        firstBlock = header->next;

    // Change next / previous
    if (header->previous)
    {
        // The previous block must have next pointing
        // to the next block (may be NULL)
        header->previous->next = header->next;
    }

    if (header->next)
    {
        // The next block must have previous pointing
        // to the previous block (may be NULL)
        header->next->previous = header->previous;
    }
}

void *malloc(size_t size)
{
    MallocHeader *headerOffset;

    if (!lastBlock)
    {
        // First block to allocate
        firstBlock = (MallocHeader*)HEAP_START;
        headerOffset = firstBlock;
    }
    else
    {
        // Compute from previous
        headerOffset = (MallocHeader*)((size_t)lastBlock + sizeof(MallocHeader) + lastBlock->size);

        // TODO : Find another block when heap overflow
    }

    // Prepare header
    headerOffset->size = size;

    // Set previous
    headerOffset->previous = lastBlock;

    // Update next from last block
    if (lastBlock)
        lastBlock->next = headerOffset;

    // Pointer to the allocated data
    void *dataOffset = (void*)(headerOffset + 1);

    // Update last block
    lastBlock = headerOffset;

    return dataOffset;
}


void dbgHead(void *p)
{
    MallocHeader *h = (void*)((size_t)p - sizeof(MallocHeader));

    printf("Size   0x%p\n", h->size);
    printf("Prev   0x%p\n", h->previous);
    printf("Next   0x%p\n", h->next);
    printf("Pad    0x%p\n", h->padding);
}

