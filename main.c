/*
 *  main.c
 *  C-2048
 *
 *  Created on: October 5, 2014
 *     Authors: shelbyspeegle & jesselink333
 */


#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

// TODO: Handle "end game" cases.
// TODO: User cannot spawn a new tile when shifting in a direction where movement isn't possible.
// TODO: See if terminal can handle colors.
// TODO: See if terminal can handle CHANGING colors.
// TODO: Change colors to match web version.
// TODO: Center game in terminal.
// TODO: Remove ncurses dependency.
// TODO: Check for memory leaks.

const int START_LINE = 3;
int grid[16];
int debug = false;
int score;
int highScore = 0;
FILE *f;

void loadScoreData();
char * intToDisplay( int input );
void newGame();
void printBoard();
int randomFreeSpace();
void setup();
void shift( int direction );
void shiftRight();
void showDebugInfo( int direction );
void rotateccw();
void finish();

int main(int argc, const char * argv[]) {
  srand( (unsigned) time(0) ); // Seed rand with this so it is more random

  setup();
  newGame();
  
  int playing = true;

  while( playing ) {
    int uInput = getch();

    switch (uInput) {
      case 'q':
        playing = false;
        break;
      case 'r':
        newGame();
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

  finish();

  exit( EXIT_SUCCESS );
}

void loadScoreData() {
  f = fopen(".scores", "r+");

  if (f) {
    char line[256];
    if ( fgets(line, sizeof(line), f) ) {
      highScore = atoi(line);
      fclose(f);
    }
  }

  f = fopen(".scores", "wb");
}

char * intToDisplay(int inputNumber) {
  char * returnString = malloc(4*sizeof(char*));

  returnString[0] = ' ';
  returnString[1] = ' ';
  returnString[2] = ' ';
  returnString[3] = ' ';

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
      case 4:
        returnString[0] = str[0];
        returnString[1] = str[1];
        returnString[2] = str[2];
        returnString[3] = str[3];
        break;
      case 3:
        returnString[3] = str[2];
      case 2:
        returnString[2] = str[1];
      case 1:
        returnString[1] = str[0];
        break;
      default:
        break;
    }
  }

  return returnString;
}

void newGame() {
  score = 0;

  int i;
  for (i = 0; i < 16; i++) {
    grid[i] = 0;
  }

  grid[randomFreeSpace()] = 2;
  grid[randomFreeSpace()] = 2;

  printBoard();
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
  loadScoreData();
  initscr(); // Start ncurses mode
  noecho(); // Silence user input
  curs_set(0); // Hide the cursor
  keypad( stdscr, TRUE ); // Converts arrow key input to usable chars
}

void shift( int direction ) {
  int numberOfRotationsBefore = 0;
  int numberOfRotationsAfter = direction;

  // Rotations before
  if (direction != 0) {
    numberOfRotationsBefore = 4-direction;
  }

  int i;
  for (i = 0; i < numberOfRotationsBefore; i++) {
    rotateccw();
  }

  shiftRight();

  // Rotations after
  for (i = 0; i < numberOfRotationsAfter; i++) {
    rotateccw();
  }

  // Set a random free square on the board to a 2 or 4.
  int newSquareValue = 2 * ((rand() % 2 ) + 1);
  grid[randomFreeSpace()] = newSquareValue;

  showDebugInfo( direction );
  printBoard();
}

void rotateccw() {
  int tempGrid[16];

  int i;
  for (i = 0; i < 16; i++) {
    tempGrid[i] = grid[i];
  }

  i = 0;
  int j;
  int countModifier;
  for ( countModifier = 0; countModifier < 4; countModifier++ ) {
    for ( j = 12+countModifier; j >= 0; j -= 4 ) {
      grid[j] = tempGrid[i++];
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

void showDebugInfo( int direction ) {
  if ( debug ) {
      mvprintw(0,25, "Direction = %i", direction);
  } else {
    mvprintw(0,25, "             ");
  }
}

void finish() {
  if ( score > highScore ) {
    fprintf(f, "%i", score);
  } else {
    fprintf(f, "%i", highScore);
  }

  fclose(f);
  endwin(); // End ncurses mode
}