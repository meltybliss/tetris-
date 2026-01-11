#pragma once
#include <vector>
#include <string>

#include "raylib.h"
#include "board.h"

class Game {

    public:
        Game();
        ~Game();

        void Run();

    private:

        int score;
        bool gameOver = false;

        Music bgm;
        bool bgmReady = false;

        Board board;

        void Tick();
        void Render();
};
