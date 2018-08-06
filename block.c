#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "block.h"
#include "grid.h"

extern int height;

extern char border;
extern char block;
extern char empty;

struct position{
  int x;
  int y;
};

struct block{
  Position p[4];
  char type;
  int crashed;
};

Block* spawnBlock(char* next_blocks){
  Block* b = malloc(sizeof(Block));
  b->type = next_blocks[0];
  b->crashed = 0;

  Position p;

  switch(b->type){
    case 'I':
      p.x = 1; p.y = 4;
      b->p[0] = p;

      p.y = 5;
      b->p[1] = p;

      p.y = 6;
      b->p[2] = p;

      p.y = 7;
      b->p[3] = p;
      break;

    case 'J':
      p.x = 1; p.y = 4;
      b->p[0] = p;

      p.x = 2;
      b->p[1] = p;

      p.y = 5;
      b->p[2] = p;

      p.y = 6;
      b->p[3] = p;
      break;

    case 'L':
      p.x = 1; p.y = 6;
      b->p[0] = p;

      p.x = 2; p.y = 4;
      b->p[1] = p;

      p.y = 5;
      b->p[2] = p;

      p.y = 6;
      b->p[3] = p;
      break;

    case 'O':
      p.x = 1; p.y = 6;
      b->p[0] = p;

      p.x = 2;
      b->p[1] = p;

      p.y = 5;
      b->p[2] = p;

      p.x = 1;
      b->p[3] = p;
      break;

    case 'S':
      p.x = 1; p.y = 6;
      b->p[0] = p;

      p.y = 5;
      b->p[1] = p;

      p.x = 2;
      b->p[2] = p;

      p.y = 4;
      b->p[3] = p;
      break;

    case 'T':
      p.x = 1; p.y = 6;
      b->p[0] = p;

      p.x = 2;
      b->p[2] = p;

      p.y = 7;
      b->p[3] = p;

      p.y = 5;
      b->p[1] = p;
      break;

    case 'Z':
      p.x = 1; p.y = 5;
      b->p[0] = p;

      p.y = 6;
      b->p[1] = p;

      p.x = 2; p.y = 5;
      b->p[2] = p;

      p.y = 6;
      b->p[3] = p;
  }

  next_blocks[0] = next_blocks[1];
  next_blocks[1] = next_blocks[2];
  next_blocks[2] = generateRandomType();

  return b;

}

void moveBlock(Block* b, char dir, char** grid, int* score, int* soft_drop){
  if(!isMoveAllowed(b, dir, grid))
    return;

  switch(dir){

    case 's':
      if(blockWillCrash(grid, b)){
        addBlockToGrid(grid, b);
        b->crashed = 1;
        return;
      }
      else{
        for(int i = 0; i < 4; i++)
          b->p[i].x++;

        if(*soft_drop){
          (*score)++;
          *soft_drop = 0;
        }
      }
      break;

    case 'a':
      for(int i = 0; i < 4; i++)
        b->p[i].y--;
      break;

    case 'd':
      for(int i = 0; i < 4; i++)
        b->p[i].y++;
      break;
    case 'w':
      turnBlock(grid, b);
    }

}

int blockInPosition(int x, int y, Block* b){
  for(int i = 0; i < 4; i++){
    if(b->p[i].x == x && b->p[i].y == y)
      return 1;
  }
  return 0;
}

int blockWillCrash(char** grid, Block* b){
  for(int i = 0; i < 4; i++)
      if(grid[b->p[i].x+1][b->p[i].y] == block || grid[b->p[i].x+1][b->p[i].y] == border)
        return 1;
  return 0;
}

void addBlockToGrid(char** grid, Block* b){
  for(int i = 0; i < 4; i++)
      grid[b->p[i].x][b->p[i].y] = block;
}

void turnBlock(char** grid, Block* b){
  if(b->type == 'O')
    return;

  Block* temp = malloc(sizeof(Block));

  Position center = b->p[2];
  int a = center.x;
  int bb = center.y;

  for(int i = 0; i < 4; i++){
    int x = b->p[i].x;
    int y = b->p[i].y;

    temp->p[i].x = -(y - bb) + a;
    temp->p[i].y = x - a + bb;

  }

  for(int i = 0; i < 4; i++){
    if(grid[temp->p[i].x][temp->p[i].y] == border || grid[temp->p[i].x][temp->p[i].y] == block){
      free(temp);
      return;
    }

  }

  for(int i = 0; i < 4; i++){
    b->p[i].x = temp->p[i].x;
    b->p[i].y = temp->p[i].y;
  }

  free(temp);
}

int isMoveAllowed(Block* b, char dir, char** grid){
  if(dir == 'a')
    for(int i = 0; i < 4; i++){
      if(grid[b->p[i].x][b->p[i].y-1] == border || grid[b->p[i].x][b->p[i].y-1] == block)
        return 0;
      }

  else if(dir == 'd')
    for(int i = 0; i < 4; i++){
      if(grid[b->p[i].x][b->p[i].y+1] == border || grid[b->p[i].x][b->p[i].y+1] == block)
        return 0;
      }

  return 1;
}

char* initializeNextBlocks(){
  char* next_blocks = malloc(4);

  for(int i = 0; i < 3; i++)
    next_blocks[i] = generateRandomType();

  return next_blocks;
}

char generateRandomType(){
  int type = rand()%7;

  switch(type){
    case 0:
      return 'I';

    case 1:
      return 'J';

    case 2:
      return 'L';

    case 3:
      return 'O';

    case 4:
      return 'S';

    case 5:
      return 'T';

    case 6:
      return 'Z';
  }
}

int blockHasCrashed(Block* b){
  return b->crashed;
}
