/*
 *  main.c
 *  C-2048
 *
 *  Created on: October 5, 2014
 *     Author: shelbyspeegle & jesselink333
 */


#include <stdio.h>
#import <ncurses.h>
#import <stdlib.h>
#include <time.h>

int grid[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int debug = false;

void setup();
void initializeBoard();
char * intToDisplay( int input );
void printBoard();
void shift( int direction );
void shiftRight();
void shiftUp();
void shiftLeft();
void shiftDown();
void showDebugInfo();

int main(int argc, const char * argv[]) {
  srand( (unsigned) time(0) ); // Seed rand with this so it is more random

  initializeBoard();
  setup();
  printBoard();

  int flag = 1;

  while( flag ) {
    int uInput = getch();

    switch (uInput) {
      case 'q':
        flag = false;
        break;
      case 'd':
        debug = !debug;
        showDebugInfo();
        break;
      case KEY_RIGHT:
        shift(0);
        break;
      case KEY_UP:
        shift(1);
        break;
      case KEY_LEFT:
        shift(2);
        break;
      case KEY_DOWN:
        shift(3);
        break;
      default:
        break;
    }

    printBoard();
  }

  endwin(); // End ncurses mode
  exit( EXIT_SUCCESS );
}

void setup() {
  initscr(); // Start ncurses mode
  noecho(); // Silence user input
  curs_set(0); // Hide the cursor
  keypad( stdscr, TRUE ); // Converts arrow key input to usable chars
}

void initializeBoard() {
  //Pick two squares and set them to 2.

  int squareOne = rand() % 16;
  int squareTwo;

  do {
    squareTwo = rand() % 16;
  } while ( squareTwo == squareOne );

  grid[squareOne] = grid[squareTwo] = 2;
}

void printBoard() {
  clear();

  int i;
  int row =0;
  int col = 0;
  for (i=0; i < 16; i++) {

    int squareValue = grid[i];

    row = i/4;
    col = i%4;

    int squareStartY = row*3;
    int squareStartX = col*6;

    move(squareStartY, squareStartX);
    addch(ACS_ULCORNER);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_URCORNER);
    mvaddch(squareStartY +1, squareStartX, ACS_VLINE);
    mvprintw(squareStartY +1, squareStartX +1, "%s", intToDisplay(squareValue));
    mvaddch(squareStartY +1, squareStartX +5, ACS_VLINE);
    move(squareStartY +2, squareStartX);
    addch(ACS_LLCORNER);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_LRCORNER);
  }

  showDebugInfo();
  refresh();
}

void shift( int direction ) {
  switch ( direction ) {
    case 0:
      shiftRight();
      break;
    case 1:
      shiftUp();
      break;
    case 2:
      shiftLeft();
      break;
    case 3:
      shiftDown();
      break;
    default:
      break;
  }

  //TODO: add a random tile.

  if (debug) mvprintw(0,25, "Direction = %i", direction);
}

void shiftRight() {
  int row;
  for ( row = 0; row < 4; row++ ) {
    int i = ((row + 1) * 4) - 1; // 3, 7, 11, 15
    int j = i - 1;
    int rowLimit = (row * 4); // 0, 4, 8, 12
    while (i >= rowLimit && j >= rowLimit) { // 4, 8, 12, 16
      if (grid[i] == 0) {
        i--;
        j--;
      } else if (grid[j] == 0) {
        j--;
      } else if (grid[i] == grid[j]) {
        grid[i] *= 2;
        grid[j] = 0;
        i -= 2;
        j = i - 1;
      } else {
        i--;
        j = i - 1;
      }
    }

    j = ((row+1)* 4) - 1; // 3, 7, 11, 15

    for (i = j; i >= rowLimit; i--) { // 0, 4, 8, 12
      if (grid[i] != 0) {
        grid[j] = grid[i];
        j--;
      }
    }

    for (; j >= rowLimit; j--) {
      grid[j] = 0;
    }
  }
}

void shiftUp() {
  int col;
  for ( col = 0; col < 4; col++ ) {
    int i = col; // 0, 1, 2, 3
    int j = i + 4;
    int rowLimit = 12 + col;
    while (i <= rowLimit && j <= rowLimit) { // 12, 13, 14, 15
      if (grid[i] == 0) {
        i += 4;
        j += 4;
      } else if (grid[j] == 0) {
        j += 4;
      } else if (grid[i] == grid[j]) {
        grid[i] *= 2;
        grid[j] = 0;
        i += 8;
        j = i + 4;
      } else {
        i += 4;
        j = i + 4;
      }
    }

    j = col; // 0, 1, 2, 3

    for (i = j; i <= rowLimit; i += 4) { // 12, 13, 14, 15
      if (grid[i] != 0) {
        grid[j] = grid[i];
        j += 4;
      }
    }

    for (; j <= rowLimit; j += 4) {
      grid[j] = 0;
    }
  }
}

void shiftLeft() {
  int row;
  for ( row = 0; row < 4; row++ ) {
    int i = row * 4; // 0, 4, 8, 12
    int j = i + 1;
    int rowLimit = (row + 1) * 4;
    while (i < rowLimit && j < rowLimit) { // 4, 8, 12, 16
      if (grid[i] == 0) {
        i++;
        j++;
      } else if (grid[j] == 0) {
        j++;
      } else if (grid[i] == grid[j]) {
        grid[i] *= 2;
        grid[j] = 0;
        i += 2;
        j = i + 1;
      } else {
        i++;
        j = i + 1;
      }
    }

    j = row * 4; // 0, 4, 8, 12

    for (i = j; i < rowLimit; i++) { // 4, 8, 12, 16
      if (grid[i] != 0) {
        grid[j] = grid[i];
        j++;
      }
    }

    for (; j < rowLimit; j++) {
      grid[j] = 0;
    }
  }
}

void shiftDown() {
  int col;
  for ( col = 0; col < 4; col++ ) {
    int i = 12 + col; // 12, 13, 14, 15
    int j = i - 4;
    int rowLimit = col;
    while (i >= rowLimit && j >= rowLimit) { // 12, 13, 14, 15
      if (grid[i] == 0) {
        i -= 4;
        j -= 4;
      } else if (grid[j] == 0) {
        j -= 4;
      } else if (grid[i] == grid[j]) {
        grid[i] *= 2;
        grid[j] = 0;
        i -= 8;
        j = i - 4;
      } else {
        i -= 4;
        j = i - 4;
      }
    }

    j = 12 + col; // 12, 13, 14, 15

    for (i = j; i >= rowLimit; i -= 4) { // 0, 1, 2, 3
      if (grid[i] != 0) {
        grid[j] = grid[i];
        j -= 4;
      }
    }

    for (; j >= rowLimit; j -= 4) {
      grid[j] = 0;
    }
  }
}


char * intToDisplay(int inputNumber) {
  //TODO: Refactor this messy code.

  char * returnString = malloc(4*sizeof(char*));

  if (inputNumber != 0) {
    char str[4];
    sprintf(str, "%d", inputNumber);

    int length = 1;

    if (inputNumber > 9) {
      length++;
    }
    if (inputNumber > 99) {
      length++;
    }
    if (inputNumber > 999) {
      length++;
    }

    switch (length) {
      case 1:
        returnString[0] = ' ';
        returnString[1] = str[0];
        returnString[2] = ' ';
        returnString[3] = ' ';
        break;
      case 2:
        returnString[0] = ' ';
        returnString[1] = str[0];
        returnString[2] = str[1];
        returnString[3] = ' ';
        break;
      case 3:
        returnString[0] = ' ';
        returnString[1] = str[0];
        returnString[2] = str[1];
        returnString[3] = str[2];
        break;
      case 4:
        returnString[0] = str[0];
        returnString[1] = str[1];
        returnString[2] = str[2];
        returnString[3] = str[3];
        break;
      default:
        break;
    }
  } else {
    returnString[0] = ' ';
    returnString[1] = ' ';
    returnString[2] = ' ';
    returnString[3] = ' ';
  }

  return returnString;
}

void showDebugInfo() {
  if (debug) {
    mvprintw(0,25, "Direction = ");
  } else {
    mvprintw(0,25, "             ");
  }
}