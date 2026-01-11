#pragma once
#include <vector>
#include <unordered_map>
#include <array>
#include <cmath>
#include "raylib.h"
#include "MINO_CONFIG.h"

struct Cell {
    int x;
    int y;
};

class Tetrimino {

    public:

        Tetrimino(int type, int startX, int startY);
        ~Tetrimino();

        void move(float x, float y);
        void rotate();
        void tetriRender(int cellSize);

        int getX() const {return x;}
        int getY() const {return y;}
        int getRotationState() const {return rotationState;}
        int getShapeId() const {return shapeId;}
        void setRotationState(int state) {rotationState = state;}
        void renderPreview(int px, int py, int cellSize, int rot = 0) const;
        
        std::array<Cell, 4> getCells(int cellSize, int boardX, int boardY) const;
      

        float fallInterval;
        double lastFallTime;

        bool landed = false;

        static Color colors[7];
    private:

        int x;
        int y;
        int rotationState;
        int shapeId;
        

};
