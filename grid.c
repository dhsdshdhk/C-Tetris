#include <stdio.h>
#include <stdlib.h>

#include "grid.h"
#include "block.h"

int height = 22;
int width = 12;

char border = '-';
char block = 'o';
char empty = ' ';

char** createGrid(int h, int w){
  char** grid = malloc(h*sizeof(char*));
  for(int i = 0; i < h; i++)
    grid[i] = malloc(w);

  for(int i = 0; i < h; i++)
    for(int j = 0; j < h; j++)
      if(i == 0 || j == 0 || i == h-1 || j == w-1)
        grid[i][j] = border;
      else
        grid[i][j] = empty;

  return grid;
}

void freeGrid(int h, char** grid){
  for(int i = 0; i < h; i++)
    free(grid[i]);

  free(grid);
}

void printGrid(int h, int w, char** grid, Block* b){
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++)
      if(blockInPosition(i, j, b))
        printf("%c", block);
      else
        printf("%c", grid[i][j]);
    printf("\n");
    }
}

void clearLines(int h, int w, char** grid, int* score, int* level, int* lines){
  int i;
  int j;
  int k;

  int lines_to_clean = 0;

  for(i = h - 2; i > 0; i--)
    if(isLineOnlyChar(grid[i], block))
      lines_to_clean++;

  switch(lines_to_clean){
    case 0:
      return;
    case 1:
      *score += 40 * (*level + 1);
      break;
    case 2:
      *score += 100 * (*level + 1);
      break;
    case 3:
      *score += 300 * (*level + 1);
      break;
    case 4:
      *score += 1200 * (*level + 1);
  }

  (*lines) += lines_to_clean;
  (*level) = (*lines / 10) + 1;

  for(int j = 0; j < lines_to_clean; j++){
    for(i = h - 2; i > 0; i--)
      if(isLineOnlyChar(grid[i], block)){
        for(k = i - 1; k > 0; k--, i--)
          copyLineAtoB(grid[k], grid[i]);
        }
  }

}

int isLineOnlyChar(char* line, char c){
  for(int i = 1; i < width - 1; i++)
    if(line[i] != c)
      return 0;
    return 1;
}

void copyLineAtoB(char* a, char* b){
  for(int i = 1; i < width - 1; i++)
    b[i] = a[i];
}

int playerLost(int h, int w, char** grid){
  for(int i = 1; i < h-1; i++)
    if(grid[1][i] == block)
      return 1;
  return 0;
}
