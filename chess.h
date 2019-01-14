#ifndef CHESS_H
#define CHESS_H

#include "defines.h"
#define BOARD_SIZE 8

//source numbers: https://www.chess.com/article/view/how-to-set-up-a-chessboard

enum chessmanTypes{
    PAWN_WHITE = 7,	//pion
    ROOK_WHITE = 8,	//toren
    KNIGHT_WHITE = 9,	//paard
    BISHOP_WHITE = 10,	//loper
    QUEEN_WHITE = 11,	//koningin
    KING_WHITE = 12,	    //koning
    
    PAWN_BLACK = 1,
    ROOK_BLACK = 2,
    KNIGHT_BLACK = 3,
    BISHOP_BLACK = 4,
    QUEEN_BLACK = 5,
    KING_BLACK = 6,

    // for turn select
    WHITE = 0,
    BLACK = 1
};

typedef enum columnLetters{
    A = 0,
    B = 1,
    C = 2,
    D = 3,
    E = 4,
    F = 5,
    G = 6,
    H = 7
}columnLetters; //y coord represent columns. x coord represent rows and each "sensor chip" contains all squares of one row.

const char columnLettersTable[] = {
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'Z'
};

/** @brief The CHESSBOARD variable set*/
typedef volatile struct board{
    int grid[BOARD_SIZE][BOARD_SIZE];
    char white_name[64];
    char black_name[64];
    int white_score;
    int black_score;
    int time;   //time left on current turn?
    int turn;   // which player is currently playing?
    
}CHESSBOARD;

CHESSBOARD chessboard;                                              // UART structure

extern void init_board();
extern void move_char(char* from_x, char* from_y, char* to_x, char* to_y);
extern int check_move_char(char* from_x, char* from_y, char* to_x, char* to_y);
extern void display_board();
extern int checkGameOver();


#endif