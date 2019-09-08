#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>

//Structure for an NxN grid
typedef struct{
  int row;
  int col;
}vec2D_t;

typedef struct{
  int * board;
  int N;
  int subdivision;
  char BLANK;
} game_t;

typedef enum{
  HORIZONTAL,
  VERTICAL,
  GRID,
}check_e;

typedef struct{
  size_t len;
  int * buf;
  vec2D_t * posBuf;
} checker_t;

#if DEBUG
#define DLOG(fmt, ...) {printf(fmt, ##__VA_ARGS__);}
#else
#define DLOG(fmt, ...) {}
#endif

  
//Output functionality
int printBoard(game_t * game_, int format);
char * checkStr(check_e CHECK); //check enum to string

//Utility functionality
int initBoard(game_t * game);
int popRand(game_t * game);

int initChecker(game_t * game_, checker_t ** checker_);
int clearCheckArr(checker_t * checkArr_);

//Checking functionality
int checkGame(game_t * game_);
int checkHoriz(int row_, checker_t * checkArr_, game_t * game_); //deprecated
int checkVert(int col_, checker_t * checkArr_, game_t * game_); //deprecated
int checkGrid(vec2D_t * sPos, checker_t * checkArr_, game_t * game_); //deprecated

int checkFilled(checker_t * checkArr_, check_e CHECK);
int conditionCheck(int num_, vec2D_t cPos, checker_t * checkArr_);
int checkPosition(vec2D_t * pos_, checker_t * checkArgr_, game_t * game_, check_e CHECK_);

//Gameplay functionality



#endif
