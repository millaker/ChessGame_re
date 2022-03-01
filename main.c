#include <stdio.h>
#include "Board.h"
#include "FEN.h"
#include "Enum.h"
#include "GUI.h"

int main(int argc, char *argv[])
{
    init_game();
    printf("Enter game loop");
    game_loop();
    printf("Exit game loop");
    close_game();
    return 0;
}