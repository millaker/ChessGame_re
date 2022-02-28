#include "Enum.h"

char get_piece_name(Piece p)
{
    switch (p) { 
    case EMPTY:
        return '.';
    case W_KING:
        return 'K';
    case W_QUEEN:
        return 'Q';
    case W_ROOK:
        return 'R';
    case W_BISHOP:
        return 'B';
    case W_KNIGHT:
        return 'N';
    case W_PAWN:
        return 'P';
    case B_KING:
        return 'k';
    case B_QUEEN:
        return 'q';
    case B_ROOK:
        return 'r';
    case B_BISHOP:
        return 'b';
    case B_KNIGHT:
        return 'n';
    case B_PAWN:
        return 'p';
    default:
        return '\0';
    }
}