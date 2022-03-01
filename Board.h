#ifndef BOARD_H
#define BOARD_H

#include "Enum.h"

/*
 *  Position stores the position of the board,
 *  @castling uses bits to represent castling rights for either side:
 *      KQkq
 *  @en_passant stores the possible en passant file, the color of the piece
 *  is determined by active color. A~H file is 0~7
 *  a1 is 0,0 for example
 */
struct Position{
    Piece grid[8][8];
    int castling;
    int en_passant;
    Color active_color;
};
typedef struct Position Position;

/*
 *  Data structure for storing moves
 *  Singly-linked list, NULL terminated
 */
struct List{
    Piece piece;
    int row;
    int col;
    struct List* next;
};
typedef struct List List;

/*  Using the same linked list to store moves and active piece
 *  Active piece will store in two separate lists
*/

/*  Macro for traversing the whole list */
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != NULL; pos = pos->next)

/*
 *  Initialize the list node pointed by node
 */
void init_list_node(List*);

/*  Insert the node to the tail of the list */
void list_insert_tail(List* head, List *node);

/*  Remove the node target node from the list */
void list_remove_node(List* head, List *target);

/*  For debugging, traverse the list and print it */
void list_print(List* head);

/*  Allocate a new node */
List *list_alloc();

/*  Free the whole list except the head */
void free_list(List* head);

/* Count number of nodes in list */
int list_count(List *head);

/*
 *  Print the grid in the given position
*/
void print_grid(Position* pos);

/* 
 *   Initialize the given position
*/
void init_pos(Position* pos);

/*
 *  Generate pseudo-legal moves
 *  Use a singly-linked list to store the moves
*/
void gen_pseudo_legal(Position *pos, List *legal, List *active);

/* Generate legal moves for specific piece */
void gen_legal_from_piece(Position *pos, List *legal, List *active, List *piece);


/* 
 *  Search legal moves for horizontal and vertical
 */
void search_rook_legal(Position *pos, List *legal, List *piece);

/*
 *  Search legal moves for diagonals
 */
void search_bishop_legal(Position *pos, List *legal, List *piece);

/*
 *  Search legal moves for knight
 */
void search_knight_legal(Position *pos, List *legal, List *piece);
void move_check(Position *pos, List *legal, List *piece, int row, int col);

/*
 * Search legal moves for king
 */
void search_king_legal(Position *pos, List *legal, List *piece);

/*
 *  Search legal moves for pawn
 */
void search_white_pawn_legal(Position *pos, List *legal, List *piece);
void search_black_pawn_legal(Position *pos, List *legal, List *piece);


/*  Initialize active piece from position */
void get_active(Position* pos, List *white, List *black);

/*  Make the specified move 
 *  Return value is the bit mask of the move
 *  1: King ordinary moves
 *  2: Rook moves King
 *  4: pawn moves two square
 *  8:  King castle King
 *  16: King castle Queen
 *  32: Rook moves Queen
 *  64: captured something
 *  0: other moves
 * 
 *  For convinience, modify castling rook position after validating moves
 *  State of position is left unchanged as well. Change states after true
 *  legal move.
 */
int make_move(Position *pos, List *white, List *black, List *piece, int row, int col);

/*  Unmake the specified move and restore active piece */
void unmake_move(Position *pos, List *white, List *black);

/*  Clear buffer */
void clear_buffer();

/*  Is king checked */
int is_checked(Position *pos, List *white, List *black);

/*  A check function for castling specifically, check if the square is check */
int is_square_checked(Position *pos, List *white, List *black, int row, int col);

/* Update position grid using active pieces */
void update_grid(Position *pos, List *white, List *black);

/*  Update game state 
 *  Update rook position and active list if castle
 */
void update_state(Position *pos, List *piece, int move_type, List *active);
/* side = 0 for king side, = 1 for queen side */
void update_rook_castling(Position *pos, int side, List *active);

#endif