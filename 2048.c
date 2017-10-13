/*
 *  2048.c
 *
 *  Created on: October 5, 2014
 *     Authors: shelbyspeegle & jesselink333
 */


#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define NUM_TILES 16

// TODO: Handle win case.
// TODO: See if terminal can handle colors.
// TODO: See if terminal can handle CHANGING colors.
// TODO: Change colors to match web version.

const int START_LINE = 3;
int grid[NUM_TILES];
int score;
int highScore = 0;
FILE *f;

char * intToDisplay( int input );
void newGame();
void printBoard();
void printGameOverMessage();
int randomFreeSpace();
void setup();
int gameOver();
void shift( int direction );
int shiftRight();
void rotateccw();
int tilePairsExist();
int checkNeighborsForMatch( int tileIndex );
int tileEast( int tileIndex );
int tileNorth( int tileIndex );
int tileWest( int tileIndex );
int tileSouth( int tileIndex );

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

    if ( gameOver() ) {
      printGameOverMessage();

      while (true) {
        uInput = getch();

        if (uInput == 'r') {
          newGame();
          break;
        } else if (uInput == 'q') {
          playing = false;
          break;
        } else {
          ; // Invalid user input. Ignore.
        }
      }
    }
  }

  // finish.
  highScore = score > highScore ? score : highScore;
  
  f = fopen(".scores", "w");
  fprintf(f, "%i", highScore);
  fclose(f);

  endwin(); // End ncurses mode

  exit( EXIT_SUCCESS );
}

void printGameOverMessage() {
  int boardStartX = getmaxx(stdscr)/2 - 12;

  move( START_LINE + 3, boardStartX );
  addch(ACS_ULCORNER);
  int i;
  for (i = 0; i < 22; i++) {
    addch(ACS_HLINE);
  }
  addch(ACS_URCORNER);

  move( START_LINE + 4, boardStartX );
  addch(ACS_VLINE);
  mvprintw(START_LINE + 4, boardStartX + 1, "      Game over!      ");
  addch(ACS_VLINE);

  move( START_LINE + 5, boardStartX );
  addch(ACS_VLINE);
  mvprintw(START_LINE + 5, boardStartX + 1, "                      ");
  addch(ACS_VLINE);

  move( START_LINE + 6, boardStartX );
  addch(ACS_VLINE);
  mvprintw(START_LINE + 6, boardStartX + 1, " Press r for new game ");
  addch(ACS_VLINE);

  move( START_LINE + 7, boardStartX );
  addch(ACS_VLINE);
  mvprintw(START_LINE + 7, boardStartX + 1, " or q to quit.        ");
  addch(ACS_VLINE);

  move( START_LINE + 8, boardStartX );
  addch(ACS_LLCORNER);
  for (i = 0; i < 22; i++) {
    addch(ACS_HLINE);
  }
  addch(ACS_LRCORNER);

  refresh();
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
  for (i = 0; i < NUM_TILES; i++) {
    grid[i] = 0;
  }

  grid[randomFreeSpace()] = 2;
  grid[randomFreeSpace()] = 2;

  printBoard();
}

void printBoard() {
  clear();

  int boardStartX = getmaxx(stdscr)/2 - 12;

  mvprintw( 0, boardStartX + 10, "Score: %6i", score);
  mvprintw( 1, boardStartX + 1, "2048      Best: %6i", highScore);

  int row = 0;
  int col = 0;
  int i;
  for (i = 0; i < NUM_TILES; i++) {
    row = i/4;
    col = i%4;

    int squareStartY = START_LINE + row*3;
    int squareStartX = boardStartX + col*6;

    move(squareStartY, squareStartX);
    addch(ACS_ULCORNER);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_HLINE);
    addch(ACS_URCORNER);
    mvaddch(squareStartY +1, squareStartX, ACS_VLINE);
    char * gridLabel = intToDisplay( grid[i] );
    mvprintw(squareStartY +1, squareStartX +1, "%s", gridLabel);
    free(gridLabel);
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
    emptySquare = rand() % NUM_TILES;

    if (grid[emptySquare] == 0) {
      return emptySquare;
    }
    if (searchCount++ == NUM_TILES) {
      return -1;
    }
  }
}

void setup() {
  // load score data
  f = fopen(".scores", "r");
  if (f) {
    char line[256];
    if ( fgets(line, sizeof(line), f) ) {
      highScore = atoi(line);
      fclose(f);
      f = NULL;
    }
  }

  initscr(); // Start ncurses mode
  noecho(); // Silence user input
  curs_set(0); // Hide the cursor
  keypad( stdscr, TRUE ); // Converts arrow key input to usable chars
}

int gameOver() {
  if ( randomFreeSpace() == -1 ) {
    if ( !tilePairsExist() ) {
      return TRUE;
    }
  }
  return FALSE;
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

  int changeHappened = shiftRight();

  // Rotations after
  for (i = 0; i < numberOfRotationsAfter; i++) {
    rotateccw();
  }

  if (changeHappened) {
    // Set a random free square on the board to a 2 or 4.
    int newSquareValue = 2 * ((rand() % 2 ) + 1);
    grid[randomFreeSpace()] = newSquareValue;
  }
}

void rotateccw() {
  int tempGrid[NUM_TILES];

  int i;
  for (i = 0; i < NUM_TILES; i++) {
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

int shiftRight() {
  int row;
  int changeHappened = false;

  int tempGrid[NUM_TILES];

  int i;
  for (i = 0; i < NUM_TILES; i++) {
    tempGrid[i] = grid[i];
  }

  for ( row = 0; row < 4; row++ ) {
    i = ((row + 1) * 4) - 1; // 3, 7, 11, 15
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

  for (i = 0; i < NUM_TILES; i++) {
    if (tempGrid[i] != grid[i]) {
      changeHappened = true;
    }
  }

  return changeHappened;
}

int tilePairsExist() {
  int i;
  for ( i = 0; i < NUM_TILES; i++ ) {
    if (checkNeighborsForMatch(i)) {
      return true;
    }
  }

  return false;
}

int checkNeighborsForMatch(int tileIndex) {
  if (tileEast(tileIndex) > 0) {
    if ( grid[tileIndex] == grid[tileEast(tileIndex)]) {
      return true;
    }
  }

  if (tileNorth(tileIndex) > 0) {
    if ( grid[tileIndex] == grid[tileNorth(tileIndex)]) {
      return true;
    }
  }

  if (tileWest(tileIndex) > 0) {
    if ( grid[tileIndex] == grid[tileWest(tileIndex)]) {
      return true;
    }
  }

  if (tileSouth(tileIndex) > 0) {
    if ( grid[tileIndex] == grid[tileSouth(tileIndex)]) {
      return true;
    }
  }

  return false;
}

int tileEast(int tileIndex){
  int canCheckE = tileIndex != 3 && tileIndex != 7 && tileIndex != 11 && tileIndex != 15;

  return canCheckE ? tileIndex + 1 : -1;
}

int tileNorth(int tileIndex){
  int canCheckN = tileIndex > 3;

  return canCheckN ? tileIndex - 4 : -1;
}

int tileWest(int tileIndex){
  int canCheckW = tileIndex != 0 && tileIndex != 4 && tileIndex != 8 && tileIndex != 12;

  return canCheckW ? tileIndex - 1 : -1;
}

int tileSouth(int tileIndex){
  int canCheckS = tileIndex < 12;

  return canCheckS ? tileIndex + 4 : -1;
}
