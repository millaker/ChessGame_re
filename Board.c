#include "Board.h"
#include "FEN.h"
#include "Enum.h"
#include "misc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Global variables */
int row_buf = 0;
int col_buf = 0;
List *modified_buf = NULL;
List *promotion_buf = NULL;
List *removed_buf = NULL;
List *added_buf = NULL;

void print_grid(Position* pos)
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            putchar(get_piece_name(pos->grid[i][j]));
            putchar(' ');
        }
        putchar('\n');
    }
}

void init_pos(Position *pos)
{
    pos->active_color = WHITE;
    pos->castling = 0;
    pos->en_passant = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            pos->grid[i][j] = 0;
        }
    }
}

void gen_pseudo_legal(Position *pos, List *legal, List *active)
{
    List *curr = active->next;
    while (curr) {
        switch (curr->piece) {
        case W_KING:
        case B_KING:
            search_king_legal(pos, legal, curr);
            break;
        case W_ROOK:
        case B_ROOK:
            search_rook_legal(pos, legal, curr);
            break;
        case W_QUEEN:
        case B_QUEEN:
            search_rook_legal(pos, legal, curr);
            search_bishop_legal(pos, legal, curr);
            break;
        case W_BISHOP:
        case B_BISHOP:
            search_bishop_legal(pos, legal, curr);
            break;
        case W_KNIGHT:
        case B_KNIGHT:
            search_knight_legal(pos, legal, curr);
            break;
        case W_PAWN:
            search_white_pawn_legal(pos, legal, curr);
            break;
        case B_PAWN:
            search_black_pawn_legal(pos, legal, curr);
            break;
        default:
            break;
        }
        curr = curr->next;
    }
}

void init_list_node(List* node)
{
    node->piece = EMPTY;
    node->row = 0;
    node->col = 0;
    node->next = NULL;
}

void list_insert_tail(List* head, List*node)
{
    List *curr = head;
    while (curr->next)
        curr = curr->next;
    curr->next = node;
}

void list_remove_node(List* head, List *target)
{
    List *curr = head, *prev = NULL;
    while (curr != NULL && curr != target) {
        prev = curr;
        curr = curr->next;
    }
    prev->next = curr->next;
}

List *list_alloc()
{
    List *temp = malloc(sizeof(List));
    init_list_node(temp);
    if(!temp) {
        printf("Allocate fail\n");
        return NULL;
    }
    return temp;
}

void list_print(List* head)
{
    List *curr = head->next;
    while (curr) {
        printf("Node: %c, (%d, %d)\n", get_piece_name(curr->piece), curr->row, curr->col);
        curr = curr->next;
    }
}

void get_active(Position* pos, List *white, List *black)
{
    List *temp;
    for (int i = 0; i < 8; i++) {
        for (int j = 0 ;j < 8; j++) {
            if (pos->grid[i][j] != EMPTY) {
                List *new = list_alloc();
                new->piece = pos->grid[i][j];
                new->row = i;
                new->col = j;
                temp = (pos->grid[i][j] & 8) ? black: white;
                list_insert_tail(temp, new);
            }
        }
    }
}

void free_list(List* head)
{
    List *curr = head->next;
    while (curr) {
        free(curr);
        curr = curr->next;
    }
}

void search_rook_legal(Position *pos, List *legal, List *piece)
{
    if (!piece || !pos || !legal) {
        printf("Search rook legal move fail: Passed Null pointer\n");
        exit(-1);
    }
    int curr_row = piece->row, curr_col = piece->col;
    Color curr_color = (piece->piece & 8);
    //Up
    for (int i = curr_row - 1; i >= 0; i--) {
        List *temp = list_alloc();
        temp->col = curr_col;
        temp->row = i;
        temp->piece = piece->piece;
        if (pos->grid[i][curr_col] != EMPTY) {
            if ((pos->grid[i][curr_col] & 8) == curr_color) {
                free(temp);
                break;
            }
            list_insert_tail(legal, temp);
            break;
        }
        list_insert_tail(legal, temp);
    }
    //Down
    for (int i = curr_row + 1; i < 8; i++) {
        List *temp = list_alloc();
        temp->col = curr_col;
        temp->row = i;
        temp->piece = piece->piece;
        if (pos->grid[i][curr_col] != EMPTY) {
            if ((pos->grid[i][curr_col] & 8) == curr_color) {
                free(temp);
                break;
            }
            list_insert_tail(legal, temp);
            break;
        }
        list_insert_tail(legal, temp);
    }
    //Right
    for (int i = curr_col + 1; i < 8; i++) {
        List *temp = list_alloc();
        temp->col = i;
        temp->row = curr_row;
        temp->piece = piece->piece;
        if (pos->grid[curr_row][i] != EMPTY) {
            if ((pos->grid[curr_row][i] & 8) == curr_color) {
                free(temp);
                break;
            }
            list_insert_tail(legal, temp);
            break;
        }
        list_insert_tail(legal, temp);
    }
    //Left
    for (int i = curr_col - 1; i >= 0; i--) {
        List *temp = list_alloc();
        temp->col = i;
        temp->row = curr_row;
        temp->piece = piece->piece;
        if (pos->grid[curr_row][i] != EMPTY) {
            if ((pos->grid[curr_row][i] & 8) == curr_color) {
                free(temp);
                break;
            }
            list_insert_tail(legal, temp);
            break;
        }
        list_insert_tail(legal, temp);
    }
}

void search_bishop_legal(Position *pos, List *legal, List *piece)
{
    if (!piece || !pos || !legal) {
        printf("Search rook legal move fail: Passed Null pointer\n");
        exit(-1);
    }
    int curr_row = piece->row, curr_col = piece->col;
    Color curr_color = (piece->piece & 8);
    //Up Right
    for (int i = curr_row - 1, j = curr_col + 1; i >= 0 && j < 8; i--, j++) {
        List *temp = list_alloc();
        temp->col = j;
        temp->row = i;
        temp->piece = piece->piece;
        if (pos->grid[i][j] != EMPTY) {
            if ((pos->grid[i][j] & 8) == curr_color) {
                free(temp);
                break;
            }
            list_insert_tail(legal, temp);
            break;
        }
        list_insert_tail(legal, temp);
    }
    //Up Left
    for (int i = curr_row - 1, j = curr_col - 1; i >= 0 && j >= 0; i--, j--) {
        List *temp = list_alloc();
        temp->col = j;
        temp->row = i;
        temp->piece = piece->piece;
        if (pos->grid[i][j] != EMPTY) {
            if ((pos->grid[i][j] & 8) == curr_color) {
                free(temp);
                break;
            }
            list_insert_tail(legal, temp);
            break;
        }
        list_insert_tail(legal, temp);
    }
    //Down Left
    for (int i = curr_row + 1, j = curr_col - 1; i < 8 && j >= 0; i++, j--) {
        List *temp = list_alloc();
        temp->col = j;
        temp->row = i;
        temp->piece = piece->piece;
        if (pos->grid[i][j] != EMPTY) {
            if ((pos->grid[i][j] & 8) == curr_color) {
                free(temp);
                break;
            }
            list_insert_tail(legal, temp);
            break;
        }
        list_insert_tail(legal, temp);
    }
    //Down Right
    for (int i = curr_row + 1, j = curr_col + 1; i < 8 && j < 8; i++, j++) {
        List *temp = list_alloc();
        temp->col = j;
        temp->row = i;
        temp->piece = piece->piece;
        if (pos->grid[i][j] != EMPTY) {
            if ((pos->grid[i][j] & 8) == curr_color) {
                free(temp);
                break;
            }
            list_insert_tail(legal, temp);
            break;
        }
        list_insert_tail(legal, temp);
    }
}

void search_knight_legal(Position *pos, List *legal, List *piece)
{
    if (!piece || !pos || !legal) {
        printf("Search rook legal move fail: Passed Null pointer\n");
        exit(-1);
    }
    int curr_row = piece->row, curr_col = piece->col;
    int row_offset[8] = {-1, 1, -1, 1, -2 , 2, -2, 2};
    int col_offset[8] = {-2, -2, 2, 2, -1, -1, 1, 1};
    int row, col;
    for (int i = 0; i < 8; i++) {
        row = curr_row + row_offset[i];
        col = curr_col + col_offset[i];
        move_check(pos, legal, piece, row, col);
    }
}

void move_check(Position *pos, List *legal, List *piece, int row, int col)
{
    Color color = (piece->piece & 8);
    if (row >= 0 && col >= 0 && row < 8 && col < 8) {
        List *temp = list_alloc();
        temp->row = row;
        temp->col = col;
        temp->piece = piece->piece;
        if ((pos->grid[row][col] & 8) == color && 
            pos->grid[row][col] != EMPTY)
            free(temp);
        else
            list_insert_tail(legal, temp);
    }
}

void search_king_legal(Position *pos, List *legal, List *piece)
{
    if (!piece || !pos || !legal) {
        printf("Search rook legal move fail: Passed Null pointer\n");
        exit(-1);
    }
    int curr_row = piece->row, curr_col = piece->col;
    int row_offset[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
    int col_offset[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
    int row, col;
    for (int i = 0; i < 8; i++) {
        row = curr_row + row_offset[i];
        col = curr_col + col_offset[i];
        move_check(pos, legal, piece, row, col);
    }
    //Castle
    int KingSide = (piece->piece & 8) ? 8: 2;
    int QueenSide = (piece->piece & 8) ? 4: 1;
    if ((pos->castling & KingSide) == KingSide &&
        pos->grid[curr_row][curr_col + 1] == EMPTY &&
        pos->grid[curr_row][curr_col + 2] == EMPTY) {
        row = curr_row;
        col = curr_col + 2;
        List *temp = list_alloc();
        temp->row = row;
        temp->col = col;
        temp->piece = piece->piece;
        list_insert_tail(legal, temp);
    }
    if ((pos->castling & QueenSide) == QueenSide &&
        pos->grid[curr_row][curr_col - 1] == EMPTY &&
        pos->grid[curr_row][curr_col - 2] == EMPTY) {
        row = curr_row;
        col = curr_col - 2;
        List *temp = list_alloc();
        temp->row = row;
        temp->col = col;
        temp->piece = piece->piece;
        list_insert_tail(legal, temp);
    }
}


void search_white_pawn_legal(Position *pos, List *legal, List *piece)
{
    if (!piece || !pos || !legal) {
        printf("Search rook legal move fail: Passed Null pointer\n");
        exit(-1);
    }
    int curr_row = piece->row, curr_col = piece->col;
    List *temp;
    Color curr_color = WHITE;
    //First move
    if (curr_row == 6) {
        if (pos->grid[curr_row - 1][curr_col] == EMPTY && 
            pos->grid[curr_row - 2][curr_col] == EMPTY ) {
            temp = list_alloc();
            temp->row = curr_row - 2;
            temp->col = curr_col;
            temp->piece = piece->piece;
            list_insert_tail(legal, temp);
        }
    }
    if (curr_row - 1 >= 0 && pos->grid[curr_row - 1][curr_col] == EMPTY) {
        temp = list_alloc();
        temp->row = curr_row - 1;
        temp->col = curr_col;
            temp->piece = piece->piece;
        list_insert_tail(legal, temp);
    }
    //Pawn attacks
    if (curr_row - 1 >= 0) {
        if (curr_col - 1 >= 0 && 
            pos->grid[curr_row - 1][curr_col - 1] != EMPTY &&
            (pos->grid[curr_row - 1][curr_col - 1] & 8) != curr_color) {
            temp = list_alloc();
            temp->row = curr_row - 1;
            temp->col = curr_col - 1;
            temp->piece = piece->piece;
            list_insert_tail(legal, temp);
        }
        if (curr_col + 1 < 8 && 
            pos->grid[curr_row - 1][curr_col + 1] != EMPTY &&
            (pos->grid[curr_row - 1][curr_col + 1] & 8) != curr_color) {
            temp = list_alloc();
            temp->row = curr_row - 1;
            temp->col = curr_col + 1;
            temp->piece = piece->piece;
            list_insert_tail(legal, temp);
        }
    }
    //En passant
    int p_col = pos->en_passant;
    if ((curr_col == p_col - 1 || curr_col == p_col + 1) && curr_row == 3) {
        temp = list_alloc();
        temp->row = curr_row - 1;
        temp->col = p_col;
            temp->piece = piece->piece;
        list_insert_tail(legal, temp);
    }
}

void search_black_pawn_legal(Position *pos, List *legal, List *piece)
{
    if (!piece || !pos || !legal) {
        printf("Search rook legal move fail: Passed Null pointer\n");
        exit(-1);
    }
    int curr_row = piece->row, curr_col = piece->col;
    List *temp;
    Color curr_color = BLACK;
    //First move
    if (curr_row == 1) {
        if (pos->grid[curr_row + 1][curr_col] == EMPTY && 
            pos->grid[curr_row + 2][curr_col] == EMPTY ) {
            temp = list_alloc();
            temp->row = curr_row + 1;
            temp->col = curr_col;
            temp->piece = piece->piece;
            list_insert_tail(legal, temp);
        }
    }
    if (curr_row + 1 < 8 && pos->grid[curr_row + 1][curr_col] == EMPTY) {
        temp = list_alloc();
        temp->row = curr_row + 1;
        temp->col = curr_col;
            temp->piece = piece->piece;
        list_insert_tail(legal, temp);
    }
    //Pawn attacks
    if (curr_row + 1 < 8) {
        if (curr_col - 1 >= 0 && 
            pos->grid[curr_row + 1][curr_col - 1] != EMPTY &&
            (pos->grid[curr_row + 1][curr_col - 1] & 8) != curr_color) {
            temp = list_alloc();
            temp->row = curr_row + 1;
            temp->col = curr_col - 1;
            temp->piece = piece->piece;
            list_insert_tail(legal, temp);
        }
        if (curr_col + 1 < 8 && 
            pos->grid[curr_row + 1][curr_col + 1] != EMPTY &&
            (pos->grid[curr_row + 1][curr_col + 1] & 8) != curr_color) {
            temp = list_alloc();
            temp->row = curr_row + 1;
            temp->col = curr_col + 1;
            temp->piece = piece->piece;
            list_insert_tail(legal, temp);
        }
    }
    //En passant
    int p_col = pos->en_passant;
    if ((curr_col == p_col - 1 || curr_col == p_col + 1) && curr_row == 4) {
        temp = list_alloc();
        temp->row = curr_row - 1;
        temp->col = p_col;
            temp->piece = piece->piece;
        list_insert_tail(legal, temp);
    }
}

int list_count(List *head) {
    List *curr = head->next;
    int num = 0;
    while (curr) {
        num++;
        curr = curr->next;
    }
    return num;
}

int make_move(Position *pos, List *white, List *black, List *piece, int row, int col)
{
    removed_buf = NULL;
    modified_buf = NULL;
    added_buf = NULL;
    promotion_buf = NULL;
    int promotion = 0, king_move = 0, pawn_2= 0, rook_move = 0, castle = 0;
    Color curr_color = (piece->piece & 8);
    List *attack = curr_color == WHITE ? white: black;
    List *defense = curr_color == WHITE ? black: white;
    //Check promotion
    if ((piece->piece == W_PAWN && row == 0) || 
        (piece->piece == B_PAWN && row == 7)) {
        promotion = 8;
        promotion_buf = piece;
        list_remove_node(attack, piece);
        List *new = list_alloc();
        new->piece = curr_color? B_QUEEN: W_QUEEN;
        new->row = row;
        new->col = col;
        added_buf = new;
        list_insert_tail(attack, new);
    }   
    //Check en passant
    if ((piece->piece == W_PAWN && piece->row - row == 2) ||
        (piece->piece == B_PAWN && piece->row - row == -2)) {
            pawn_2 = 4;
        }
    //Check rook
    if (piece->piece == W_ROOK || piece->piece == B_ROOK) {
        rook_move = piece->col == 0 ? 32: 2;
    }
    //Check castle
    if ((piece->piece == W_KING || piece->piece == B_KING) && 
    (piece->col - col > 1 || piece->col - col < -1)) {
        castle = 16;
    }
    //Check destination square
    Piece target = pos->grid[row][col];
    if (target == W_ROOK || target == B_ROOK) {
        rook_move = col == 0? 32: 2;
    }
    //Find destination square pointer and remove it from active
    List *captured = NULL;
    if (target != EMPTY) {
        list_for_each(captured, defense) {
            if (captured->row == row && captured->col == col)
                break;
        }
    }
    //Find en passant captured
    if ((piece->piece == W_PAWN || piece->piece == B_PAWN) &&
        col == pos->en_passant && (row == 2 || row == 5)) {
            list_for_each(captured, defense) {
                if ((captured->piece & 6) == 6)
                    break;
            }
        }
    removed_buf = captured;
    list_remove_node(defense, captured);
    //Update moving node, store in buffer
    modified_buf = piece;
    row_buf = piece->row;
    col_buf = piece->col;
    piece->row = row;
    piece->col = col;
    return (promotion & king_move & rook_move & pawn_2 & castle);
}

void unmake_move(Position *pos, List *white, List *black)
{
    Color color = pos->active_color;
    List *attack = color ? black: white;
    List *defense = color ? white: black;
    if (added_buf) {
        list_remove_node(attack, added_buf);
        free(added_buf);
        added_buf = NULL;
    }
    if (removed_buf) {
        list_insert_tail(defense, removed_buf);
        removed_buf = NULL;
    }
    if (modified_buf) {
        modified_buf->row = row_buf;
        modified_buf->col = col_buf;
        modified_buf = NULL;
    }
    if (promotion_buf) {
        list_insert_tail(attack, promotion_buf);
        promotion_buf = NULL;
    }
}

int is_checked(Position *pos, List *white, List *black)
{
    int res = 0;
    Color color = pos->active_color;
    List *attack = color == WHITE? black: white;
    List *defense = color == WHITE ? white: black;
    List *king;
    update_grid(pos, white, black);
    list_for_each(king, defense){
        if (king->piece == W_KING || king->piece == B_KING)
            break;
    }
    List *temp = list_alloc();
    gen_pseudo_legal(pos, temp, attack);
    //Is checked
    List *node;
    list_for_each(node, temp){
        if (node->col == king->col && node->row == king->row) {
            res = 1;
            break;
        }
    }
    free_list(temp);
    free(temp);
    return res;
}

void update_grid(Position *pos, List *white, List *black)
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            pos->grid[i][j] = 0;
        }
    }
    List *node;
    list_for_each(node, white){
        pos->grid[node->row][node->col] = node->piece;
    }
    list_for_each(node, black){
        pos->grid[node->row][node->col] = node->piece;
    }
}

void clear_buffer()
{
    modified_buf = NULL;
    added_buf = NULL;
    if (promotion_buf)
        free(promotion_buf);
    if (removed_buf)
        free(removed_buf);
}