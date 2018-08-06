#ifndef __GRID_H__
#define __GRID_H__

#include "block.h"

char** createGrid(int h, int w);
void freeGrid(int h, char** grid);
void printGrid(int h, int w, char** grid, Block* b);
void clearLines(int h, int w, char** grid, int* score, int* level, int* lines);
int isLineOnlyChar(char* line, char c);
void copyLineAtoB(char* a, char* b);
int playerLost(int h, int w, char** grid);

#endif
