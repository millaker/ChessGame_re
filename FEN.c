#include "FEN.h"
#include "Enum.h"
#include "string.h"
#include "Board.h"
#include <ctype.h>
#include <stdio.h>
#include "stdlib.h"

int parse_number(char* s, int *index)
{
    int temp = 0;
    while (s[*index] != ' ') {
        if (!isdigit(s[*index])) {
            return -1;
        }
        temp *= 10;
        temp += s[(*index)++] - '0';
    }
    return temp;
}

int FEN_Parser(Position* pos, char* fen_string)
{
    int index = 0, row = 0, col = 0;
    while (fen_string[index] != ' ') {
        if (isdigit(fen_string[index])) {
            for (int i = 0; i < fen_string[index] - '0'; i++) {
                //Error check
                if (col >= 8) {
                    printf("FEN string error: Invalid Board representation\n");
                    return -1;
                }
                pos->grid[row][col++] = EMPTY;
            }
            index++;
            continue;
        }
        switch (fen_string[index]) {
        case '/':
            row++;
            col = 0;
            break;
        case 'K':
            pos->grid[row][col] = W_KING;
            col++;
            break;
        case 'Q':
            pos->grid[row][col] = W_QUEEN;
            col++;
            break;
        case 'R':
            pos->grid[row][col] = W_ROOK;
            col++;
            break;
        case 'B':
            pos->grid[row][col] = W_BISHOP;
            col++;
            break;
        case 'N':
            pos->grid[row][col] = W_KNIGHT;
            col++;
            break;
        case 'P':
            pos->grid[row][col] = W_PAWN;
            col++;
            break;
        case 'k':
            pos->grid[row][col] = B_KING;
            col++;
            break;
        case 'q':
            pos->grid[row][col] = B_QUEEN;
            col++;
            break;
        case 'r':
            pos->grid[row][col] = B_ROOK;
            col++;
            break;
        case 'b':
            pos->grid[row][col] = B_BISHOP;
            col++;
            break;
        case 'n':
            pos->grid[row][col] = B_KNIGHT;
            col++;
            break;
        case 'p':
            pos->grid[row][col] = B_PAWN;
            col++;
            break;
        default:
            printf("FEN string error: Invalid Board representation\n");
            return -1;
        }
        if (row > 8 || col > 8) {
            printf("FEN string error: Invalid Board representation\n");
            return -1;
        }
        index++;
    }
    index++;
    //Parse active color
    if (fen_string[index] == 'w')
        pos->active_color = WHITE;
    else if (fen_string[index] == 'b')
        pos->active_color = BLACK;
    else {
        printf("FEN string error: Invalid active color\n");
        return -1;
    }
    index += 2;
    //Parse Castling right
    while (fen_string[index] != ' ') {
        switch (fen_string[index]) {
        case 'K':
            pos->castling |= 8;
            break;
        case 'Q':
            pos->castling |= 4;
            break;
        case 'k':
            pos->castling |= 2;
            break;
        case 'q':
            pos->castling |= 1;
            break;
        default:
            printf("FEN string error: Invalid castling right\n");
            return -1;
        }
        index++;
    }
    index++;
    //Parse en passant
    if (fen_string[index] != '-') {
        if (!isalpha(fen_string[index]) || fen_string[index] < 'a' || fen_string[index] > 'h') {
            printf("FEN string error: Invalid en passant\n");
            return -1;
        }
        pos->en_passant = fen_string[index] - 'a';
        index++;
        if (!isdigit(fen_string[index]) || fen_string[index] != '3' || fen_string[index] != '6') {
            printf("FEN string error: Invalid en passant\n");
            return -1;
        }
        index++;
    }
    index++;
    //Parse halfmove
    int t;
    if ((t = parse_number(fen_string, &index)) < 0) {
        printf("FEN string error: Invalid halfmove\n");
        return -1;
    }
    //Set halfmove here
    index++;
    //Parse fullmove
    if ((t = parse_number(fen_string, &index)) < 0) {
        printf("FEN string error: Invalid halfmove\n");
        return -1;
    }
    //Set fullmove here
    return 0;
}

