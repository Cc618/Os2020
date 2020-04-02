#include "stdlib.h"

#include "syscall.h"

// Addresses of the heap
#define HEAP_START  0x1500000
// TODO :
#define HEAP_END    (0x1500000 + (16 + 4) * 3)

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

    // Update last block
    if (header == lastBlock)
        lastBlock = header->previous;

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

        // Find another block at the start of the heap
        if (headerOffset >= HEAP_END)
        {
            puts("OK");

            // We can allocate memory at HEAP_START offset
            if (HEAP_START + sizeof(MallocHeader) + size <= firstBlock)
                headerOffset = (MallocHeader*)HEAP_START;
            else
            {
                // Iterate through all blocks until it
                // finds enough free space
                MallocHeader *block = firstBlock;
                
                // Find a good place
                while (1)
                {
                    if (!block->next)
                    {
                        // TODO : msg = Not enough space in heap to allocate data
                        SYSC0(SYS_FATAL);
                    }

                    // End of the current block
                    size_t blockEnd = (size_t)block + sizeof(MallocHeader) + block->size;

                    // Space between both blocks
                    size_t freeSpace = (size_t)block->next - blockEnd;

                    if (freeSpace > size)
                    {
                        headerOffset = (MallocHeader*)blockEnd;
                        break;
                    }

                    // Not enough space, continue
                    block = block->next;
                }
            }
        }
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

