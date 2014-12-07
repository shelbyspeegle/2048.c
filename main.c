/*
 *  main.c
 *  C-2048
 *
 *  Created on: October 5, 2014
 *     Authors: shelbyspeegle & jesselink333
 */


#include <stdio.h>
#import <ncurses.h>
#import <stdlib.h>
#include <time.h>

// TODO: Write highScore to disk.
// TODO: Read in highScore from disk.
// TODO: Handle "end game" cases.
// TODO: Use the r key to restart the game.

const int START_LINE = 3;
int grid[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int debug = false;
int score = 0;
int highScore = 0;

char * intToDisplay( int input );
void newGame();
void printBoard();
int randomFreeSpace();
void setup();
void shift( int direction );
void shiftDown();
void shiftLeft();
void shiftRight();
void shiftUp();
void showDebugInfo( int direction );

int main(int argc, const char * argv[]) {
  srand( (unsigned) time(0) ); // Seed rand with this so it is more random

  setup();
  newGame();
  printBoard();

  int flag = true;

  while( flag ) {
    int uInput = getch();

    switch (uInput) {
      case 'q':
        flag = false;
        break;
      case 'd':
        debug = !debug;
        showDebugInfo( 0 );
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
  }

  endwin(); // End ncurses mode
  exit( EXIT_SUCCESS );
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

void newGame() {
  grid[randomFreeSpace()] = 2;
  grid[randomFreeSpace()] = 2;
}

void printBoard() {
  int i;
  for ( i = 0; i < START_LINE + 12; i++ ) {
    mvprintw( i, 0, "                    ");
  }

  mvprintw( 1, 1, "2048");

  // TODO: Make scores align right.
  mvprintw( 0, 10, "Score: %i", score);
  mvprintw( 1, 11, "Best: %i", highScore);

  int row = 0;
  int col = 0;
  for (i=0; i < 16; i++) {

    int squareValue = grid[i];

    row = i/4;
    col = i%4;

    int squareStartY = START_LINE + row*3;
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

  refresh();
}

int randomFreeSpace() {
  int emptySquare;
  int searchCount = 0;

  while ( 1 ) {
    emptySquare = rand() % 16;

    if (grid[emptySquare] == 0) {
      return emptySquare;
    }
    if (searchCount++ == 16) {
      return -1;
    }
  }
}

void setup() {
  initscr(); // Start ncurses mode
  noecho(); // Silence user input
  curs_set(0); // Hide the cursor
  keypad( stdscr, TRUE ); // Converts arrow key input to usable chars
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

  // Set a random free square on the board to a 2 or 4.
  int newSquareValue = 2 * ((rand() % 2 ) + 1);
  grid[randomFreeSpace()] = newSquareValue;

  showDebugInfo( direction );
  printBoard();
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
        score += grid[i] *= 2;
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
        score += grid[i] *= 2;
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
        score += grid[i] *= 2;
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
        score += grid[i] *= 2;
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

void showDebugInfo( int direction ) {
  if ( debug ) {
      mvprintw(0,25, "Direction = %i", direction);
  } else {
    mvprintw(0,25, "             ");
  }

//  refresh();
}