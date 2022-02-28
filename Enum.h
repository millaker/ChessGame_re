#ifndef ENUM_H
#define ENUM_H


typedef enum{
    WHITE = 0,
    BLACK = 8
} Color;

typedef enum{
    EMPTY = 0,
    W_KING = 1,
    W_QUEEN = 2,
    W_ROOK = 3,
    W_BISHOP = 4,
    W_KNIGHT = 5,
    W_PAWN = 6,
    B_KING = 9,
    B_QUEEN = 10,
    B_ROOK = 11,
    B_BISHOP = 12,
    B_KNIGHT = 13,
    B_PAWN = 14 
} Piece;

#endif