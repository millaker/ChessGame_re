#include <stdio.h>
#include "Board.h"
#include "FEN.h"
#include "Enum.h"

int main()
{
    Position pos;
    List *active = list_alloc();
    List *legal = list_alloc();
    init_pos(&pos);
    char* s = "rnbqkbnr/ppp1pppp/3p4/8/4N3/8/PPPPPPPP/R1BQKBNR w KQkq - 1 2";
    FEN_Parser(&pos, s);
    print_grid(&pos);
    get_active(&pos, active);
    printf("Number of active pieces: %d\n", list_count(active));

    List *pawn = list_alloc();
    pawn->piece = W_PAWN;
    pawn->row = 3;
    pawn->col = 1;
    search_white_pawn_legal(&pos, legal, pawn);
    printf("Start testing pawn legal moves, enpassant:\n");
    list_print(legal);
    
    return 0;
}