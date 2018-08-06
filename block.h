#ifndef BLOCK_H
#define BLOCK_H

typedef struct position Position;
typedef struct block Block;

Block* spawnBlock(char* next_blocks);
void moveBlock(Block* b, char dir, char** grid, int* score, int* soft_drop);
int blockInPosition(int x, int y, Block* b);
int blockWillCrash(char** grid, Block* b);
void addBlockToGrid(char** grid, Block* b);
void addBlockToPrintableGrid(char** grid_with_falling_block, Block* b);
void turnBlock(char** grid, Block* b, char input);
int isMoveAllowed(Block* b, char dir, char** grid);
char* initializeNextBlocks();
char generateRandomType();
int blockHasCrashed(Block* b);

#endif
