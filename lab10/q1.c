#include <stdio.h>
#include <stdlib.h>

struct mab
{
    int offset;
    int size;
    int allocated;
    struct mab *next;
    struct mab *prev;
};
typedef struct mab Mab;
typedef Mab *MabPtr;

MabPtr last_allocated = NULL;
MabPtr memChk(MabPtr m, int size);               
MabPtr memAlloc(MabPtr m, int size, int policy); 
MabPtr memFree(MabPtr m);                        
MabPtr memMerge(MabPtr m);                       
MabPtr memSplit(MabPtr m, int size);             

MabPtr memChk(MabPtr m, int size)
{
    MabPtr current = m;
    while (current != NULL)
    {
        if (!current->allocated && current->size >= size)
        {
            return current; // Found a suitable block
        }
        current = current->next;
    }
    return NULL; 
}

MabPtr memAlloc(MabPtr m, int size, int policy)
{
    MabPtr selectedBlock = NULL;
    MabPtr current = m;

    switch (policy)
    {
    case 1: 
        while (current != NULL)
        {
            if (!current->allocated && current->size >= size)
            {
                selectedBlock = current;
                break;
            }
            current = current->next;
        }
        break;

    case 2: // Best Fit
        while (current != NULL)
        {
            if (!current->allocated && current->size >= size)
            {
                if (selectedBlock == NULL || current->size < selectedBlock->size)
                {
                    selectedBlock = current;
                }
            }
            current = current->next;
        }
        break;

    case 3: // Worst Fit
        while (current != NULL)
        {
            if (!current->allocated && current->size >= size)
            {
                if (selectedBlock == NULL || current->size > selectedBlock->size)
                {
                    selectedBlock = current;
                }
            }
            current = current->next;
        }
        break;

    case 4: // Next Fit
        if (last_allocated == NULL)
            last_allocated = m;
        current = last_allocated;
        while (current != NULL)
        {
            if (!current->allocated && current->size >= size)
            {
                selectedBlock = current;
                break;
            }
            current = current->next;
        }
        if (selectedBlock == NULL)
        {
            current = m; // Wrap around if no block found after last_allocated
            while (current != last_allocated)
            {
                if (!current->allocated && current->size >= size)
                {
                    selectedBlock = current;
                    break;
                }
                current = current->next;
            }
        }
        last_allocated = selectedBlock;
        break;

    default:
        printf("Unknown allocation policy\n");
        return NULL;
    }

    if (selectedBlock != NULL)
    {
        if (selectedBlock->size > size)
        {
            memSplit(selectedBlock, size);
        }
        selectedBlock->allocated = 1; 
    }
    else
    {
        printf("No suitable block available for allocation\n");
    }

    return selectedBlock;
}

MabPtr memFree(MabPtr m)
{
    m->allocated = 0; // Mark block as free
    // Merge adjacent free blocks
    if (m->prev && !m->prev->allocated)
    {
        m = memMerge(m->prev);
    }
    if (m->next && !m->next->allocated)
    {
        m = memMerge(m);
    }
    return m;
}

MabPtr memMerge(MabPtr m)
{
    if (m->next && !m->next->allocated)
    {
        MabPtr nextBlock = m->next;
        m->size += nextBlock->size; 
        m->next = nextBlock->next;  
        if (nextBlock->next)
        {
            nextBlock->next->prev = m;
        }
        free(nextBlock);
    }
    return m;
}

MabPtr memSplit(MabPtr m, int size)
{
    if (m->size > size)
    {
        MabPtr newBlock = (MabPtr)malloc(sizeof(Mab));
        newBlock->offset = m->offset + size;
        newBlock->size = m->size - size;
        newBlock->allocated = 0;
        newBlock->next = m->next;
        newBlock->prev = m;

        if (m->next)
        {
            m->next->prev = newBlock;
        }

        m->next = newBlock;
        m->size = size;
    }
    return m;
}

MabPtr createBlock(int offset, int size, int allocated)
{
    MabPtr newBlock = (MabPtr)malloc(sizeof(Mab));
    newBlock->offset = offset;
    newBlock->size = size;
    newBlock->allocated = allocated;
    newBlock->next = NULL;
    newBlock->prev = NULL;
    return newBlock;
}

void printMemory(MabPtr head)
{
    MabPtr current = head;
    while (current != NULL)
    {
        printf("Block offset: %d, size: %d, allocated: %d\n", current->offset, current->size, current->allocated);
        current = current->next;
    }
}

int main()
{
    MabPtr block1 = createBlock(0, 100, 0);
    MabPtr block2 = createBlock(100, 200, 0);
    MabPtr block3 = createBlock(300, 150, 0);

    // Link blocks
    block1->next = block2;
    block2->prev = block1;
    block2->next = block3;
    block3->prev = block2;

    printMemory(block1);

    printf("\nAllocating 50 bytes using Best Fit...\n");
    memAlloc(block1, 50, 2); // Best Fit
    printMemory(block1);

    printf("\nAllocating 100 bytes using Worst Fit...\n");
    memAlloc(block1, 100, 3); // Worst Fit
    printMemory(block1);

    printf("\nAllocating 80 bytes using First Fit...\n");
    memAlloc(block1, 80, 1); // First Fit
    printMemory(block1);

    printf("\nAllocating 30 bytes using Next Fit...\n");
    memAlloc(block1, 30, 4); // Next Fit
    printMemory(block1);

    printf("\nFreeing the first allocated block...\n");
    memFree(block1);
    printMemory(block1);

    return 0;
}