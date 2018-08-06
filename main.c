#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h>
#include <math.h>
#include <termios.h>

#include "grid.h"
#include "block.h"

extern int height;
extern int width;

FILE* fp;
Block* b;
int level = 1;
char** grid;
struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };
struct termios orig_termios;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void disableRawMode();
void enableRawMode(int wait_time);
void processInput(char input, int* soft_drop);
int waitMillis(int ms);
void* gravity();
void printInformation(int score, int level, int lines, char* next_blocks);
void clearScreen();

int main(){

  fp = fopen("debug", "w");
  srand(time(NULL));
  pthread_t gravity_thread;

  int score = 0;
  int lines = 0;
  int soft_drop = 0;
  grid = createGrid(height, width);
  char* next_blocks = initializeNextBlocks();
  b = spawnBlock(next_blocks);
  pthread_create(&gravity_thread, NULL, gravity, NULL);

  do{
    char input = ' ';
    clearScreen();
    printInformation(score, level, lines, next_blocks);
    printGrid(height, width, grid, b);

    enableRawMode(10.5 - level * 0.5);
    read(STDIN_FILENO, &input, 1);
    disableRawMode();

    processInput(input, &soft_drop);
    moveBlock(b, input, grid, &score, &soft_drop);
    fprintf(fp, "%c (main)\n", input);

    if(blockHasCrashed(b)){
      waitMillis(150);
      clearLines(height, width, grid, &score, &level, &lines);
      free(b);

      b = spawnBlock(next_blocks);
    }

  }while(!playerLost(height, width, grid));

  printf("\nYou lost!\n");

  free(next_blocks);
  freeGrid(height, grid);
  fclose(fp);
  return 0;

}

void processInput(char input, int* soft_drop){
  switch(input){
    case 'q':
      fclose(fp);
      disableRawMode();
      exit(0);
    case 's':
      *soft_drop = 1;
      pthread_cond_signal(&cond);
      break;
    }
}

int waitMillis(int ms)
{
    int n;
    struct timeval tv;
    struct timespec ts;

    gettimeofday(&tv, NULL);
    ts.tv_sec = time(NULL) + ms / 1000;
    ts.tv_nsec = tv.tv_usec * 1000 + 1000 * 1000 * (ms % 1000);
    ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
    ts.tv_nsec %= (1000 * 1000 * 1000);

    pthread_mutex_lock(&mutex);
    n = pthread_cond_timedwait(&cond, &mutex, &ts);
    pthread_mutex_unlock(&mutex);

    if (n == 0) {
        return 0;
    } else if (n == ETIMEDOUT) {
        return 1;
    }
}

void* gravity(){

  char input = 's';
  int dummy_score = 0;
  int soft_drop = 0;

  while(1){
    do{
      if(waitMillis(1050 - level * 50)){
        moveBlock(b, input, grid, &dummy_score, &soft_drop);
        fprintf(fp, "down (thread)\n");
      }
    }while(!blockHasCrashed(b));
  }
}

void printInformation(int score, int level, int lines, char* next_blocks){
  printf("Score: %d\nLevel: %d\nLines: %d\n", score, level, lines);
  printf("Next blocks: %c, %c, %c\n", next_blocks[0], next_blocks[1], next_blocks[2]);
}

void clearScreen(){
  for(int i = 0; i < 35; i++){
    printf("\n");
  }
}

void disableRawMode(){
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
void enableRawMode(int wait_time){
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = wait_time;
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
