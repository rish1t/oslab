#include <stdio.h>
#include <stdlib.h>

#define TOTAL_BLOCKS 20
#define SPARE_BLOCKS 5

typedef struct
{
    int bad_block;
    int spare_block;
} BadBlockEntry;

BadBlockEntry bad_blocks_table[TOTAL_BLOCKS];
int spare_blocks[SPARE_BLOCKS];
int bad_block_count = 0;

void initialize_spare_blocks()
{
    for (int i = 0; i < SPARE_BLOCKS; i++)
    {
        spare_blocks[i] = TOTAL_BLOCKS - SPARE_BLOCKS + i;
    }
}

int allocate_spare_block(int bad_block)
{
    if (bad_block_count >= SPARE_BLOCKS)
    {
        printf("No spare blocks available.\n");
        return -1;
    }
    int spare_block = spare_blocks[bad_block_count];
    bad_blocks_table[bad_block_count].bad_block = bad_block;
    bad_blocks_table[bad_block_count].spare_block = spare_block;
    bad_block_count++;
    return spare_block;
}

void display_access_status(int block)
{
    int replaced = 0;
    for (int i = 0; i < bad_block_count; i++)
    {
        if (bad_blocks_table[i].bad_block == block)
        {
            printf("Block %d: Replaced with spare block %d\n", block, bad_blocks_table[i].spare_block);
            replaced = 1;
            break;
        }
    }
    if (!replaced)
    {
        printf("Block %d: Access successful\n", block);
    }
}

int main()
{
    int bad_block;
    initialize_spare_blocks();

    printf("Enter bad blocks (enter -1 to end):\n");
    while (1)
    {
        printf("Bad Block Number: ");
        scanf("%d", &bad_block);
        if (bad_block == -1)
        {
            break;
        }
        if (bad_block < 0 || bad_block >= TOTAL_BLOCKS)
        {
            printf("Invalid block number.\n");
            continue;
        }
        allocate_spare_block(bad_block);
    }

    printf("Accessing all blocks:\n");
    for (int i = 0; i < TOTAL_BLOCKS; i++)
    {
        display_access_status(i);
    }

    return 0;
}