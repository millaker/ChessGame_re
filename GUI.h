#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "Enum.h"
#include "FEN.h"
#include "Board.h"

/* Initialize SDL */
void init_game();
void init_texture();

/* Main loop of the window */
void game_loop();

/* Close SDL */
void close_game();

/*  Input handler */
void handle_input();

/* Draw chess board */
void draw_chessboard();

/* Draw chess piece */
void draw_chesspiece();

/* Draw chess piece on cursor */
void draw_chesspiece_on_cursor();

/*  Pick piece using mouse position */
void pick_piece();



#endif