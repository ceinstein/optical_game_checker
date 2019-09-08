#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sudoku.h"

#if DEBUG
#define DLOG(fmt, ...) {printf(fmt, ##__VA_ARGS__);}
#else
#define DLOG(fmt, ...) {}
#endif

//Prints a formatted gameboard
int printBoard(game_t * game_, int format){
  int i,j;
  for(i = 0; i < game_->N; ++i){
    for(j = 0; j < game_->N; ++j){
      if((game_->board + ((game_->N * i) + j))){
	printf("%d", *(game_->board + ((game_->N * i) + j)));
      }
      if((j % (game_->N / game_->subdivision)) == game_->subdivision - 1 && format){
	printf(" |");
      }
      printf("\t");
    }
    if((i % (game_->N / game_->subdivision)) == game_->subdivision - 1 && format){
      printf("\n");
      int k;
      for(k = 0; k < 3; ++k){
	printf("- - - - - - - - - +\t");
      }
    }
    printf("\b\n\n");
    if((i % (game_->N / game_->subdivision)) == game_->subdivision - 1 && format){
      printf("\b");
    }
  }
  
  return 0;

}

//initialize an NxN grid in memory
//Give each space an empty character
int initBoard(game_t * game_){

  int i,j;
  game_->board = (int *)malloc(sizeof(int) * (game_->N * game_->N));

  for(i = 0; i < game_->N; ++i){
    for(j = 0; j < game_->N; ++j){
      *(game_->board + ((game_->N * i) + j)) = game_->BLANK;
    }
  }
  
  return 0;
}

//Initialize a checker array
int initChecker(game_t * game_, checker_t ** checkArr_){
  *checkArr_ = (checker_t *)malloc(sizeof(checker_t));
  (*checkArr_)->len = game_->N;
  (*checkArr_)->buf = (int *)malloc(sizeof(int) * (*checkArr_)->len);
  (*checkArr_)->posBuf = (vec2D_t *)malloc(sizeof(vec2D_t) * game_->N);
  return 0;
}

//Resets a checker array
int clearCheckArr(checker_t * checkArr_){
  int i;
  for(i = 0; i < checkArr_->len; ++i){
    *(checkArr_->buf + i) = 0;
  }
  return 0;
}

//There is a (1/chance)% chance that a random value between 1-9 (inclusive) will populate a certain square
int popRand(game_t * game_){
  int chance = 1; // 1/chance chance
  int i, j;
  for(i = 0; i < game_->N; ++i){
    for(j = 0;  j < game_->N; ++j){
      if(!(rand()%chance)){
	*(game_->board + ((game_->N * i) + j)) = (rand() % 9) + 1;
      }
    }
  }
  return 0;
} 

//Prints a string version of the given enum
char * checkStr(check_e CHECK){
  switch(CHECK){
  case HORIZONTAL:
    return "HORIZONTAL";
  case VERTICAL:
    return "VERTICAL";
  case GRID:
    return "GRID";
  default:
    DLOG("NOT A VALID CHECK!\n");
    return 0;
  }
}
      
//Check if a location has all required values (0-9)
int checkFilled(checker_t * checkArr_, check_e CHECK){
  int i;
  for(i = 0; i < checkArr_->len; ++i){
    if(!*(checkArr_->buf + i)){
      DLOG("%d IS NOT PRESENT IN %s\n", i, checkStr(CHECK));
      return 1;
    }
  }
  return 0;
}


//Checks if number is valid
//If it is, add it to the checker_t
int conditionCheck(int num_, vec2D_t cPos_, checker_t * checkArr_){
  if(num_ < 0){
    DLOG("SPACE %d,%d IS NOT FILLED IN!\n", cPos_.row, cPos_.col);
    return 1;
  }
  
  //Number has already been found
  if(*(checkArr_->buf + num_)){
    DLOG("POSITION %d,%d HAS FAILED WITH VALUE %d\n", cPos_.row,cPos_.col,num_+1);
    DLOG("%d HAS ALREADY BEEN FOUND at %d,%d!\n",
	 num_ + 1, (checkArr_->posBuf + num_)->row, (checkArr_->posBuf + num_)->col);
    return 1;
  }
  
  //Indicate that the number has been set
  *(checkArr_->buf + num_) = 1;
  (checkArr_->posBuf + num_)->row = cPos_.row;
  (checkArr_->posBuf + num_)->col = cPos_.col;

  return 0;
}

//Checks a position (horizontal,vertical,grid)
int checkPosition(vec2D_t * pos_, checker_t * checkArr_, game_t * game_, check_e CHECK_){

  int i,j, num;
  vec2D_t cPos;
  clearCheckArr(checkArr_);
  j = 0;
  for(i = 0; i < game_->N; ++i){
    switch(CHECK_){
    case HORIZONTAL:
      cPos.row = pos_->row;
      cPos.col = i;
      break;
    case VERTICAL:
      cPos.row = i;
      cPos.col = pos_->col;
      break;
    case GRID:
      cPos.row = ((game_->N * pos_->row) + (i / 3)); //grid row calc
      cPos.col = (pos_->col + (j++ % game_->subdivision)); //grid col calc
      break;
    default:
      DLOG("NOT A VALID CHECK\n");
      return 1;
    }

    num = (*(game_->board + ((game_->N * cPos.row)) + cPos.col)) - 1;

    if(conditionCheck(num, cPos, checkArr_)){
      return 1;
    }
  }
  return checkFilled(checkArr_, CHECK_);
}

//Evaluates a game
int checkGame(game_t * game_){
  int i, j;
  vec2D_t vec2D;
  checker_t * checker;
  check_e FAILED;
  initChecker(game_, &checker);
  j = 0;
  for(i = 0; i < game_->N; ++i){
    //check horizontal
    vec2D.row = i;
    vec2D.col = -1;
    if(checkPosition(&vec2D, checker, game_, HORIZONTAL)){
      FAILED = HORIZONTAL;
      goto FAILED;
    }

    //check vertical
    vec2D.row = -1;
    vec2D.col = i;
    if(checkPosition(&vec2D, checker, game_, VERTICAL)){
      FAILED = VERTICAL;
      goto FAILED;
    }
    
    //check grid
    vec2D.row = i / game_->subdivision;
    vec2D.col = i % game_->subdivision;
    if(checkPosition(&vec2D, checker, game_, GRID)){
      FAILED = GRID;
      goto FAILED;
    }
  }
  return 0;

 FAILED:
  DLOG("FAILED ON %s CHECK AT ", checkStr(FAILED));
  switch(FAILED){
  case HORIZONTAL:
    DLOG("ROW: %d\n", vec2D.row);
    break;
  case VERTICAL:
    DLOG("COL: %d\n", vec2D.col);
    break;
  case GRID:
    DLOG("GRID STARTING AT %d,%d\n",vec2D.row,vec2D.col);
    break;
  default:
    DLOG("UNKNOWN CHECK!\n");
  }
  return 1;
}

int main(int argc, char ** argv){
  srand(time(NULL));

  //Small testing of implemented functionality
  game_t s;
  s.N = 9;
  s.subdivision = 3;
  s.BLANK = 0;
  initBoard(&s);
  popRand(&s);
  printBoard(&s, 1);
  checkGame(&s) ? printf("FAILED\n") : printf("PASSED\n");
  return 0;
}
