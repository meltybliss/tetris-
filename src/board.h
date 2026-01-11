#pragma once
#include "raylib.h"
#include "tetri.h"
#include "MINO_CONFIG.h"
#include <random>


struct BoardResult {
    int cleardThisTick = 0;
    bool gameOver = false;


};

class Board {

    public:
        Board();
        ~Board();
        
        BoardResult boardUpdate(float dt);
        void render();
        bool canCollide(const Tetrimino& tet) const;
        void init();

    private:

        const int cellSize = 40;
        const static int gridRow = 20;
        const static int gridCol = 10;
        
        int startX;
        int startY;

        Texture2D bg{};
        bool bgReady = false;


        bool outOfBounds(int x, int y) const;
        void instanceNewTet();
        void lockCurrToBoard();
        
        int checkLines();

        Tetrimino currTet;
        Tetrimino nextTet;

        std::array<std::array<int, gridCol>, gridRow> grid;
};
