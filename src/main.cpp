#include "raylib.h"
#include "game.h"

int main() {

    int width = GetMonitorWidth(0);
    int height = GetMonitorHeight(0);

    InitAudioDevice();
    InitWindow(width, height, "Tetris++");

    SetTargetFPS(60);

    Game game;

    game.Run();



    return 0;
}
