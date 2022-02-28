#include "GUI.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "Enum.h"
#include "FEN.h"
#include "Board.h"
#include "misc.h"
#include <string.h>

/*  Global variables for SDL window, textures */
#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720
#define INTERVAL (WINDOW_HEIGHT/8)
#define START_X ((WINDOW_WIDTH-WINDOW_HEIGHT)/2)
#define START_Y 0

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
SDL_Texture *white_tex[6];
SDL_Texture *black_tex[6];
Piece selected = EMPTY;
int selected_row = -1;
int selected_col = -1;
int mouse_x = 0;
int mouse_y = 0;
int is_mouse_down = 0;
SDL_Color White_Board_Color = {
    .r = 252,
    .g = 216,
    .b = 223,
    .a = 255
};
SDL_Color Black_Board_Color = {
    .r = 147,
    .g = 88,
    .b = 135,
    .a = 255
};
int is_running = 1;
Position *pos = NULL;


void init_game()
{
    //Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL init failed\n");
        exit(1);
    }
    //Init window
    if (!(win = SDL_CreateWindow("Chess", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN))) {
        printf("SDL create window error\n");
        exit(1);
    }
    //Init renderer
    if (!(ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))) {
        printf("SDL create renderer error\n");
        SDL_DestroyWindow(win);
        exit(1);
    }
    //Init image
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL image init failed\n");
        SDL_DestroyWindow(win);
        SDL_DestroyRenderer(ren);
        exit(1);
    }
    init_texture();
}

void game_loop()
{
    /*for testing */
    pos = malloc(sizeof(Position));  
    char *s = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    FEN_Parser(pos, s);
    is_running = 1;
    while (is_running) {
        SDL_SetRenderDrawColor(ren, 255, 134, 214, 255);
        SDL_RenderClear(ren);
        //Handle input
        handle_input();

        //Draw 
        draw_chessboard();
        draw_chesspiece();
        draw_chesspiece_on_cursor();

        SDL_RenderPresent(ren);
    }
    free(pos);
}

void handle_input()
{
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        switch(e.type){
            case SDL_QUIT:
                is_running = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (!is_mouse_down && e.button.button == SDL_BUTTON_LEFT) {
                    is_mouse_down = 1;
                    pick_piece();
                    printf("Mouse button down\n");
                    printf("Picked %c, (%d, %d)\n", get_piece_name(selected), selected_row, selected_col);
                }
                break;
            case SDL_MOUSEMOTION:
                mouse_x = e.motion.x;
                mouse_y = e.motion.y;
                //printf("Mouse: %d, %d\n", mouse_x, mouse_y);
                break;
            case SDL_MOUSEBUTTONUP:
                if (is_mouse_down && e.button.button == SDL_BUTTON_LEFT) {
                    is_mouse_down = 0;
                    selected = EMPTY;
                    printf("Dropped\n");
                    printf("Mouse up\n");
                }
                break;
            default:
                break;
        }
    }
    return;
}

void draw_chessboard()
{
    SDL_Rect block = {
        .x = START_Y,
        .y = START_X,
        .w = INTERVAL,
        .h = INTERVAL
    };
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            block.x = START_X + j * INTERVAL;
            block.y = START_Y + i * INTERVAL;
            if((i + j)% 2 == 0){
                SDL_SetRenderDrawColor(ren, White_Board_Color.r, White_Board_Color.g, White_Board_Color.b, White_Board_Color.a);
            }else{
                SDL_SetRenderDrawColor(ren, Black_Board_Color.r, Black_Board_Color.g, Black_Board_Color.b, Black_Board_Color.a);
            }
            SDL_RenderFillRect(ren, &block);
        }
    }
}

void close_game()
{
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

void draw_chesspiece()
{
    SDL_Rect block;
    SDL_Texture* tex;
    int offset = INTERVAL * 0.1;
    float mul = 0.8;

    block.h = block.w = INTERVAL * mul;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            //Move block
            if (selected != EMPTY && i == selected_row && j == selected_col)
                continue;
            block.x = START_X + j * INTERVAL + offset;
            block.y = i * INTERVAL + offset;
            Piece p = pos->grid[i][j];
            if (p == EMPTY)
                continue;
            tex = (p & 8) == 8 ? black_tex[p - 9]: white_tex[p - 1];
            if(tex)
                SDL_RenderCopy(ren, tex, NULL, &block);
        }
    }
}

void init_texture()
{
    char *w[6] = {"King", "Queen", "Rook", "Bishop", "Knight", "Pawn"};
    char *b[6] = {"King", "Queen", "Rook", "Bishop", "Knight", "Pawn"};
    SDL_Texture *tex;
    SDL_Surface *sur;
    for (int i = 0; i < 6; i++) {
        char imagePath[64] = "./assets/white";
        strncat(imagePath, w[i], 64);
        strncat(imagePath, ".png", 64);
        printf("Load %s\n", imagePath);
        sur = IMG_Load(imagePath);
        if (!sur) {
            printf("Load %s error sur\n", w[i]);
        }
        tex = SDL_CreateTextureFromSurface(ren, sur);
        if (!sur) {
            printf("Load %s error\n", w[i]);
        }
        SDL_FreeSurface(sur);
        white_tex[i] = tex;
    }
    for (int i = 0; i < 6; i++) {
        char imagePath[64] = "./assets/black";
        strncat(imagePath, b[i], 64);
        strncat(imagePath, ".png", 64);
        printf("Load %s\n", imagePath);
        sur = IMG_Load(imagePath);
        if (!sur) {
            printf("Load %s error sur\n", w[i]);
        }
        tex = SDL_CreateTextureFromSurface(ren, sur);
        if (!sur) {
            printf("Load %s error\n", w[i]);
        }
        SDL_FreeSurface(sur);
        black_tex[i] = tex;
    }
}

void draw_chesspiece_on_cursor()
{
    if (selected == EMPTY)
        return;
    SDL_Texture **piece_tex = selected & 8 ? black_tex: white_tex;
    int offset = selected & 8 ? -9: -1;
    SDL_Texture *tex = piece_tex[selected + offset];
    SDL_Rect b = {mouse_x - INTERVAL/2, mouse_y - INTERVAL/2, INTERVAL, INTERVAL};
    SDL_RenderCopy(ren, tex, NULL, &b);
}

void pick_piece()
{
    int row = (mouse_y - START_Y) / INTERVAL;
    int col = (mouse_x - START_X) / INTERVAL;
    printf("Mouse_x : %d, Mouse_y: %d, choose %d, %d\n", mouse_x, mouse_y, row, col);
    if (row >= 8 || row < 0 || col >= 8 || col < 0) {
        selected = EMPTY;
        return;
    }
    selected_row = row;
    selected_col = col;
    selected = pos->grid[row][col];
}