#include "tetri.h"


Color Tetrimino::colors[7] = {RED, BLUE, GREEN, YELLOW, PURPLE, ORANGE, SKYBLUE};



Tetrimino::Tetrimino(int shapeId, int startX, int startY)
{
    this->shapeId = shapeId;
    this->x = static_cast<float>(startX);
    this->y = static_cast<float>(startY);
    this->rotationState = 0;

    this->fallInterval = 0.5f;
    this->lastFallTime = 0.0;
}

Tetrimino::~Tetrimino()
{
}

void Tetrimino::move(float x, float y)
{
    this->x = x;
    this->y = y;  
}

void Tetrimino::rotate()
{
    if (IsKeyPressed(KEY_Q)) {
    
        rotationState = (rotationState - 1 == -1) ? MINO_CONFIG::MINO_DATA[shapeId].size() - 1 : rotationState - 1;

    }    
    else if (IsKeyPressed(KEY_E)) {
         rotationState = (rotationState + 1 == MINO_CONFIG::MINO_DATA[shapeId].size()) ? 0 : rotationState + 1;
    }
    
}

void Tetrimino::tetriRender(int cellSize)
{
   
    auto& shape = MINO_CONFIG::MINO_DATA[shapeId][rotationState];
    for (int row = 0; row < shape.size(); ++row) {
        for (int col = 0; col < shape[row].size(); ++col) {
            if (shape[row][col] == 1) {

                int drawX = static_cast<int>(x) + col * cellSize;
                int drawY = static_cast<int>(y) + row * cellSize;

                DrawRectangle(drawX, drawY, cellSize, cellSize, colors[shapeId]);

                DrawRectangleLines(drawX, drawY, cellSize, cellSize, Fade(BLACK, 0.5f));
            }
        }
    }
    
}

void Tetrimino::renderPreview(int px, int py, int cellSize, int rot) const
{
    int r = rot;
    auto& shape = MINO_CONFIG::MINO_DATA[shapeId][r];

    for (int row = 0; row < (int)shape.size(); ++row) {
        for (int col = 0; col < (int)shape[row].size(); ++col) {
            if (shape[row][col] == 1) {
                int x = px + col * cellSize;
                int y = py + row * cellSize;

                DrawRectangle(x, y, cellSize, cellSize, colors[shapeId]);
                DrawRectangleLines(x, y, cellSize, cellSize, Fade(BLACK, 0.5f));
            }
        }
    }
}

std::array<Cell, 4> Tetrimino::getCells(int cellSize, int boardX, int boardY) const
{

    std::array<Cell, 4> out{};
    int n = 0;

    int baseX = (int)std::floor((this->x - boardX) / (float)cellSize);
    int baseY = (int)std::floor((this->y - boardY) / (float)cellSize);


    auto& shape = MINO_CONFIG::MINO_DATA[shapeId][rotationState];
    for (int row = 0; row < shape.size(); ++row) {
        for (int col = 0; col < shape[row].size(); ++col) {
            if (shape[row][col] == 1) {
                if (n < 4) out[n++] = {baseX + col, baseY + row};
            }
        }
    }


    return out;
}
