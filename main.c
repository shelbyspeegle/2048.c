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
void showDebugInfo( int debug );

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
        showDebugInfo(debug);
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
  refresh();
}

void shift( int direction ) {
  //TODO: implement

  if (debug) mvprintw(0,25, "Direction = %i", direction);
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

void showDebugInfo( int debug ) {
  if (debug) {
    mvprintw(0,25, "Direction = ");
  } else {
    mvprintw(0,25, "             ");
  }
}