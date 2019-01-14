//#include <stdio.h>
#include "rpi-uart.h"
#include "chess.h"
#include "defines.h"
#include <stdio.h>
#include <string.h>

int find_enum(char *sval)
{
    int result=A; /* value corresponding to columnLetters[0] */
    int i;
    for (i=0; columnLettersTable[i]!="Z"; ++i, ++result){
        if (sval == columnLettersTable[i]) {
            //uprintf("1");
            return result;
        }
    }
    return -1;
}



void init_grid(){
    int x,y;
    CHESSBOARD * board = &chessboard;
    
    for(x=0; x<BOARD_SIZE; x++) {
        for(y=0;y<BOARD_SIZE;y++) {
            if(x == 0){
                if (y == A || y== H){
                    board->grid[x][y] = ROOK_BLACK;
                }else if(y == B || y== G){
                    board->grid[x][y] = KNIGHT_BLACK;
                }else if(y == C || y== F){
                    board->grid[x][y] = BISHOP_BLACK;
                }else if(y == D){
                    board->grid[x][y] = QUEEN_BLACK;
                }else{
                    board->grid[x][y] = KING_BLACK;
                }
            }else if( x == 1) {
                board->grid[x][y] = PAWN_BLACK;
            }else if(x == 7){
                if (y == A || y== H){
                    board->grid[x][y] = ROOK_WHITE;
                }else if(y == B || y== G){
                    board->grid[x][y] = KNIGHT_WHITE;
                }else if(y == C || y== F){
                    board->grid[x][y] = BISHOP_WHITE;
                }else if(y == D){
                    board->grid[x][y] = QUEEN_WHITE;
                }else{
                    board->grid[x][y] = KING_WHITE;
                }
            }else if( x == 6) {
                board->grid[x][y] = PAWN_WHITE;
            }else{
                board->grid[x][y] = 0;
            }
            //printf("Enter value for board[%d][%d]:", i, j);
            //scanf("%d", &board[i][j]);
        }
    }
}

// returns 0 if both kings are alive
// returns 1 if white king is dead
// returns 2 if black king is dead
int checkGameOver(){
    int x,y;
    CHESSBOARD * board = &chessboard;
    int whiteKingExists = 0;
    int blackKingExists = 0;
    
    for(x=0; x<BOARD_SIZE; x++) {
        for(y=0;y<BOARD_SIZE;y++) {
            int chessman = board->grid[x][y];
            if (chessman == KING_WHITE){
                whiteKingExists = 1;
            } else if (chessman == KING_BLACK){
                blackKingExists = 1;
            }
        }
    }
    if (whiteKingExists && blackKingExists){
        return 0;
    } else if (!whiteKingExists){
        return 1;
    } else {
        return 2;
    }


}

int isEmpty(int x, int y){
    CHESSBOARD * board = &chessboard;
    int chessman = board->grid[y][x];
    uprintf("Target contains: %c\n\r", chessman+'0');
    int tim;
    for(tim = 0; tim < 500000; tim++);
    if (chessman == 0){
        return 1;
    } else {
        return 0;
    }
}

void move_int(int from_x, int from_y, int to_x, int to_y){
    CHESSBOARD * board = &chessboard;
    // x en y reversed: vraag aan George
    int chessman = board->grid[from_y][from_x];
    board -> grid[from_y][from_x] = 0;
    board -> grid[to_y][to_x] = chessman;

    //change turn information
    if ( chessman > 0 && chessman < 7 ){ //piece == black
        board->turn = WHITE;
    } else if (chessman > 6){ //piece == white
        board->turn = BLACK;
    }


}

void move_char(char* from_x, char* from_y, char* to_x, char* to_y){
    int from_x_int = find_enum(from_x);
    int from_y_int = (int) from_y - '0';
    int to_x_int = find_enum(to_x);
    int to_y_int = (int) to_y - '0';

    move_int(from_x_int, from_y_int, to_x_int, to_y_int);
}



int check_move_int(int from_x, int from_y, int to_x, int to_y){

    int tim;
    uprintf("Translated: from %c,%c to %c,%c\n\r", from_x +'0', from_y +'0', to_x +'0', to_y +'0');
    for (tim = 0; tim < 500000; tim++);
    // checks:
    //Depening on the type of chesspiece, we get different rules
    // can the piece move in this direction?
    // is there anything in the way?
    // Extra: special moves: 1)Castling 2)Pawn promotion 3)En Passant // only if we have extra time.
    CHESSBOARD * board = &chessboard;
    int piece = board->grid[from_y][from_x];
    int allowed = 0;

    //first check if the correct color is making a move, white or back

    if ( piece > 0 && piece < 7 ){ //piece == black
        if (board->turn != BLACK) {
            uprintf("please select a WHITE piece\n\r");
            return 0;
        }
    } else if (piece > 6){ //piece == white
        if (board->turn != WHITE) {
            uprintf("please select a BLACK piece\n\r");
            return 0;
        }
    } else {
        uprintf("UNKNOWN_PIECE selected.\n\r");
        return 0;
    }

    //PION 
    //can move forward if empty or diagonally to kill enemy
    //can move 2 spaces forward if first move
    if (piece == PAWN_WHITE){         
        uprintf("PAWN_WHITE\n\r");
        for(tim = 0; tim < 500000; tim++);
        // if it's the pawn's first move, it can move 2 spaces
        int first_move = 0;
        if (from_y == 6) first_move = 1;
        int diff = from_y - to_y;
        uprintf("first_move=%c diff=%c\n\r", first_move+'0',diff+'0');
        for(tim = 0; tim < 500000; tim++);

        if (from_x==to_x && diff==1){
            //FORWARD one space
            if (isEmpty(to_x,to_y)) return 1;
        } else if (from_x==to_x && diff == 2 && first_move){
            //FORWARD two space if its the pawn's first move
            if (isEmpty(to_x,to_y) && isEmpty(to_x,to_y+1)) return 1;
        } else if (to_x==from_x+1 && diff == 1){
            //KILL ENEMY diagonal to the right
            int chessman = board->grid[to_y][to_x];
            if (chessman !=0 && chessman < 7) return 1;
        } else if (to_x==from_x-1 && diff == 1){
            //KILL ENEMY diagonal to the left
            int chessman = board->grid[to_y][to_x];
            if (chessman !=0 && chessman < 7) return 1;
        }
        
    } else if (piece == PAWN_BLACK){
        uprintf("PAWN_BLACK\n\r");
        for(tim = 0; tim < 500000; tim++);
        // if it's the pawn's first move, it can move 2 spaces
        int first_move = 0;
        if (from_y == 1) first_move = 1;
        int diff = to_y - from_y;
        uprintf("first_move=%c diff=%c\n\r", first_move+'0',diff+'0');
        for(tim = 0; tim < 500000; tim++);

        if (from_x==to_x && diff==1){
            //FORWARD one space
            if (isEmpty(to_x,to_y)) return 1;
        } else if (from_x==to_x && diff == 2 && first_move){
            //FORWARD two space if its the pawn's first move
            if (isEmpty(to_x,to_y) && isEmpty(to_x,to_y-1)) return 1;
        } else if (to_x==from_x+1 && diff == 1){
            //KILL ENEMY diagonal to the right
            int chessman = board->grid[to_y][to_x];
            if (chessman > 6) return 1;
        } else if (to_x==from_x-1 && diff == 1){
            //KILL ENEMY diagonal to the left
            int chessman = board->grid[to_y][to_x];
            if (chessman > 6) return 1;
        }
    } 

    //TOREN 
    // can move vertical and horizontal unlimited
    // check if path to destination is clear
    // destination must be empty or contain enemy
    else if(piece == ROOK_WHITE || piece == ROOK_BLACK){   //TOREN
        uprintf("ROOK\n\r");
        for(tim = 0; tim < 500000; tim++);

        return 1;
        //Path has to be empty
        int roadblock = 0; // if we detect a piece in the path to the destination, roadblock will become 1
        if (to_x == from_x){
            //VERTICAL MOVEMENT
            int diff = to_y - from_y;
            // downwards
			int i;
            if (diff > 0){
                for (i = from_y+1; i<to_y ; i++){
                    if (!isEmpty(to_x, i)) roadblock = 1;
                }
            } 
            // upwards
            else if(diff < 0){
                for (i = from_y-1; i>to_y ; i--){
                    if (!isEmpty(to_x, i)) roadblock = 1;
                }
            }
        } else if (to_y == from_y){
            //HORIZONTAL MOVEMENT
            int diff = to_x - from_x;
			int i;
            // to the right
            if (diff > 0){
                for (i = from_x+1; i<to_x ; i++){
                    if (!isEmpty(i,to_y)) roadblock = 1;
                }
            } 
            // to the left
            else if(diff < 0){
                for (i = from_x-1; i>to_x ; i--){
                    if (!isEmpty(i, to_y)) roadblock = 1;
                }
            }
        } else {
            return 0;
        }
        //We have checked if the path is clear(with variable "roadblock"), now we check if the destination is empty OR contains enemy
        if (!roadblock){
            int chessman = board->grid[to_y][to_x];
            if(piece == ROOK_WHITE){
                if (chessman < 7) return 1;
            } else {
                if (chessman > 6 || chessman == 0) return 1;
            }
        } 
    } 

    // PAARD
    // can move in L shape
    // check if the move is L shaped
    // then check: destination must be empty or contain enemy
    else if(piece == KNIGHT_WHITE || piece == KNIGHT_BLACK){ //PAARD
        uprintf("KNIGHT\n\r");
        for(tim = 0; tim < 500000; tim++);
        // check if the knight is allowed to make a move to this location
        // afterwards we check if the destination is empty OR contains enemy
        int move = 0;
        
        // The allowed moves are the following:
        if (to_y == from_y +2){
            if (to_x == from_x+1 || to_x == from_x-1) move = 1;
        } else if (to_y == from_y-2){
            if (to_x == from_x+1 || to_x == from_x-1) move = 1;
        } else if (to_x == from_x+2){
            if (to_y == from_y+1 || to_y == from_y-1) move = 1;
        } else if (to_x == from_x-2){
            if (to_y == from_y+1 || to_y == from_y-1) move = 1;
        }
        // if we are allowed to move to this location, check the content if it is empty OR contains enemy
        if (move){
            int chessman = board->grid[to_y][to_x];
            if(piece == KNIGHT_WHITE){
                if (chessman < 7) return 1;
            } else {
                if (chessman > 6 || chessman == 0) return 1;
            }
        } 

    } 

    // LOPER
    // can move diagonally unlimited
    // check if path to destination is clear
    // destination must be empty or contain enemy
    else if(piece == BISHOP_WHITE || piece == BISHOP_BLACK){ //LOPER
        uprintf("BISHOP\n\r");
        
        //Path has to be empty
        int diff_x = to_x - from_x;
        int diff_y = to_y - from_y;
        int roadblock = 0; // if we detect a piece in the path to the destination, roadblock will become 1
		int i;
		int j;
        //
        if (abs(diff_x) == abs(diff_y) && diff_y != 0){
            //diagonaal naar rechtsonder
            if (diff_x > 0 && diff_y > 0){
                for (i = from_x+1; i<to_x ; i++){
                    for (j = from_y+1; j<to_y; j++){
                        if (!isEmpty(i, j)) roadblock = 1;
                    }
                }
            } 
            //diagonaal naar rechtsboven 
            else if (diff_x > 0 && diff_y < 0){
                for (i = from_x+1; i<to_x ; i++){
                    for ( j = from_y-1; j>to_y; j--){
                        if (!isEmpty(i, j)) roadblock = 1;
                    }
                }
            } 
            //diagonaal naar linksonder
            else if (diff_x < 0 && diff_y > 0){
                for (i = from_x-1; i>to_x ; i--){
                    for (j = from_y+1; j<to_y; j++){
                        if (!isEmpty(i, j)) roadblock = 1;
                    }
                }
            } 
            //diagonaal naar linksboven
            else if (diff_x > 0 && diff_y > 0){
                for (i = from_x-1; i>to_x ; i--){
                    for (j = from_y-1; j>to_y; j--){
                        if (!isEmpty(i, j)) roadblock = 1;
                    }
                }
            }
        } else {
            return 0;
        }
        //We have checked if the path is clear(with variable "roadblock"), now we check if the destination is empty OR contains enemy
        if (!roadblock){
            int chessman = board->grid[to_y][to_x];
            if(piece == BISHOP_WHITE){
                if (chessman < 7) return 1;
            } else {
                if (chessman > 6 || chessman == 0) return 1;
            }
        } 
    } else if(piece == QUEEN_WHITE || piece == QUEEN_BLACK){  //KONINGIN
        uprintf("QUEEN\n\r");
        for(tim = 0; tim < 500000; tim++);
        //Path has to be empty
        int diff_x = to_x - from_x;
        int diff_y = to_y - from_y;
        int roadblock = 0; // if we detect a piece in the path to the destination, roadblock will become 1
		int i;
		int j;
        if (to_x == from_x){
            //VERTICAL MOVEMENT
            // downwards
            if (diff_y > 0){
                for (i = from_y+1; i<to_y ; i++){
                    if (!isEmpty(to_x, i)) roadblock = 1;
                }
            } 
            // upwards
            else if(diff_y < 0){
                for (i = from_y-1; i>to_y ; i--){
                    if (!isEmpty(to_x, i)) roadblock = 1;
                }
            }
        } else if (to_y == from_y){
            //HORIZONTAL MOVEMENT
            // to the right
            if (diff_x > 0){
                for (i = from_x+1; i<to_x ; i++){
                    if (!isEmpty(i,to_y)) roadblock = 1;
                }
            } 
            // to the left
            else if(diff_x < 0){
                for (i = from_x-1; i>to_x ; i--){
                    if (!isEmpty(i, to_y)) roadblock = 1;
                }
            }
        } else if (abs(diff_x) == abs(diff_y)){
            //diagonaal naar rechtsonder
            if (diff_x > 0 && diff_y > 0){
                for (i = from_x+1; i<to_x ; i++){
                    for (j = from_y+1; j<to_y; j++){
                        if (!isEmpty(i, j)) roadblock = 1;
                    }
                }
            } 
            //diagonaal naar rechtsboven 
            else if (diff_x > 0 && diff_y < 0){
                for (i = from_x+1; i<to_x ; i++){
                    for (j = from_y-1; j>to_y; j--){
                        if (!isEmpty(i, j)) roadblock = 1;
                    }
                }
            } 
            //diagonaal naar linksonder
            else if (diff_x < 0 && diff_y > 0){
                for (i = from_x-1; i>to_x ; i--){
                    for (j = from_y+1; j<to_y; j++){
                        if (!isEmpty(i, j)) roadblock = 1;
                    }
                }
            } 
            //diagonaal naar linksboven
            else if (diff_x > 0 && diff_y > 0){
                for (i = from_x-1; i>to_x ; i--){
                    for (j = from_y-1; j>to_y; j--){
                        if (!isEmpty(i, j)) roadblock = 1;
                    }
                }
            }
        } else {
            return 0;
        }
        //We have checked if the path is clear(with variable "roadblock"), now we check if the destination is empty OR contains enemy
        if (!roadblock){
            int chessman = board->grid[to_y][to_x];
            if(piece == QUEEN_WHITE){
                if (chessman < 7) return 1;
            } else {
                if (chessman > 6 || chessman == 0) return 1;
            }
        } 
    } else if(piece == KING_WHITE || piece == KING_BLACK){   //KONING
        uprintf("KING\n\r");
        //Path has to be empty
        int diff_x = to_x - from_x;
        int diff_y = to_y - from_y;

        if (abs(diff_x) < 2 && abs(diff_y) < 2 ){
            int chessman = board->grid[to_y][to_x];
            if(piece == KING_WHITE){
                if (chessman < 7) return 1;
            } else {
                if (chessman > 6 || chessman == 0) return 1;
            }
        } else {
            return 0;
        }

    } else {
        uprintf("UNKNOWN_PIECE\n\r");
    }

    return allowed;
}   

int check_move_char(char* from_x, char* from_y, char* to_x, char* to_y){
    int from_x_int = find_enum(from_x);
    int from_y_int = (int) from_y - '0';
    int to_x_int = find_enum(to_x);
    int to_y_int = (int) to_y - '0';

    return check_move_int(from_x_int, from_y_int, to_x_int, to_y_int);
}   


void display_board_color(){
    int i, j;
    int tim;
    CHESSBOARD * board = &chessboard;
    if (board->turn == WHITE){
        uprintf("The next turn is for WHITE\n\r");
    } else {
        uprintf("The next turn is for BLACK\n\r");
    }
    
    for(tim = 0; tim < 50000; tim++);
    uprintf("  ABCDEFGH\n\r");
    for(tim = 0; tim < 50000; tim++);
    for(i=0; i<BOARD_SIZE; i++) {
        uprintf("%c ",i+'0');
        for(tim = 0; tim < 50000; tim++);
        for(j=0;j<BOARD_SIZE;j++) {
            switch(board->grid[i][j]){
                case PAWN_WHITE:
                    uprintf("P");
                    break;
                case ROOK_WHITE:
                    uprintf("R");
                    break;
                case KNIGHT_WHITE:
                    uprintf("H");
                    break;
                case BISHOP_WHITE:
                    uprintf("B");
                    break;
                case QUEEN_WHITE: 
                    uprintf("Q");
                    break;
                case KING_WHITE:
                    uprintf("K");
                    break;
                case PAWN_BLACK:
                    uprintf(KBLU "P" KNRM);
                    break;
                case ROOK_BLACK:
                    uprintf(KBLU "R" KNRM);
                    break;
                case KNIGHT_BLACK:
                    uprintf(KBLU "H" KNRM);
                    break;
                case BISHOP_BLACK:
                    uprintf(KBLU "B" KNRM);
                    break;
                case QUEEN_BLACK:
                    uprintf(KBLU "Q" KNRM);
                    break;
                case KING_BLACK:
                    uprintf(KBLU "K" KNRM);
                    break;
                default:
                    uprintf(KCYN "." KNRM);
                    break;
            }
            for(tim = 0; tim < 50000; tim++);
            
            if(j==BOARD_SIZE-1){
                uprintf("\n\r");
                for(tim = 0; tim < 50000; tim++);
            }
        }
    }
}

void display_board(){
    int i, j;
    CHESSBOARD * board = &chessboard;

    char str[256] = "  ABCDEFGH\n\r\n";
    for(i=0; i<BOARD_SIZE; i++) {
        str[ strlen(str) ] = i+'0';
        str[ strlen(str) ] = ' ';
        for(j=0;j<BOARD_SIZE;j++) {
            str[ strlen(str) ] = board->grid[i][j]+'0'; 
            
            if(j==BOARD_SIZE-1){
                str[ strlen(str) ] = '\n';
                str[ strlen(str) ] = '\r';
            }
            
        }
    }
    str[ strlen(str) ] = '\n';
    uprintf(str);

}


void init_board(){
    CHESSBOARD *board;
    board = &chessboard;
    board->turn = WHITE;
    
    init_grid();
    //display_board();

}

/*
void reset_board(){
    int i, j;
    for(i=0; i<BOARD_SIZE; i++) {
	for(j=0;j<BOARD_SIZE;j++) {
	    board[i][j] = 1;
	    //printf("Enter value for board[%d][%d]:", i, j);
	    //scanf("%d", &board[i][j]);
	}
    }
}



void display_board(UART* up){
    for(i=0; i<BOARD_SIZE; i++) {
	for(j=0;j<BOARD_SIZE;j++) {
	    uputc(up, board[i][j]+'0');
	    uprints(up, " ");
	    
	    if(j==BOARD_SIZE-1){
		uprints(up, "\n");
	    }
	}
    }
}
*/
