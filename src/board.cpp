#include "board.h"



static std::mt19937 rng(std::random_device{}());



Board::Board() : currTet(0, 0, 0), nextTet(0, 0, 0)
{
    init();
}

Board::~Board()
{
    if (bgReady) UnloadTexture(bg);
}

BoardResult Board::boardUpdate(float dt)
{

    BoardResult result;
    result.cleardThisTick = 0;
    result.gameOver = false;
  
    int key = GetKeyPressed();

    if (GetTime() - currTet.lastFallTime >= currTet.fallInterval) {
        float oldY = currTet.getY();
        currTet.move(currTet.getX(), oldY + cellSize);
        if (canCollide(currTet)) {
            currTet.move(currTet.getX(), oldY);
            currTet.landed = true;
        }

        currTet.lastFallTime = GetTime();
    }

    if (key == KEY_S) {
        float oldY = currTet.getY();
        currTet.move(currTet.getX(), oldY + cellSize);
        if (canCollide(currTet)) {
            currTet.move(currTet.getX(), oldY);
            currTet.landed = true;
        }
    }

    if (key == KEY_A || key == KEY_D) {
        float oldX = currTet.getX();
        float dx = (key == KEY_A) ? -cellSize : cellSize;

        currTet.move(oldX + dx, currTet.getY());
        if (canCollide(currTet)) {
            currTet.move(oldX, currTet.getY());
            
        }
    }

    int oldRot = currTet.getRotationState();
    currTet.rotate();
    if (canCollide(currTet)) {
        currTet.setRotationState(oldRot);
    }

    if (currTet.landed) {


        lockCurrToBoard();
        result.cleardThisTick = this->checkLines();

        instanceNewTet();

        if (canCollide(currTet)) {
            // Game over　生成した瞬間に衝突したらゲームオーバー

            result.gameOver = true;
        }
    }

    return result;
}

void Board::render()
{


    currTet.tetriRender(cellSize);
    for (int row = 0; row < gridRow; ++row) {
        for (int col = 0; col < gridCol; ++col) {
            if (grid[row][col] != -1) {
                float drawX = startX + col * cellSize;
                float drawY = startY + row * cellSize;
                DrawRectangle(drawX, drawY, cellSize, cellSize, Tetrimino::colors[grid[row][col]]);

                DrawRectangleLines((int)drawX, (int)drawY, cellSize, cellSize, Fade(BLACK, 0.5f));

            }
        }
    }

    int w = gridCol * cellSize;
    int h = gridRow * cellSize;

    DrawRectangleLines(startX, startY, w, h, RAYWHITE);

    // ===== NEXT BOX =====
    int boxCell = cellSize / 2;              // プレビューは小さめが見やすい
    int boxSize = boxCell * 4;               // 4x4分
    int pad = 16;

    int boxX = startX + gridCol * cellSize + 40; // 盤面の右に余白
    int boxY = startY + 40;                      // 上からちょい下

    DrawText("NEXT", boxX, boxY - 30, 24, RAYWHITE);
    DrawRectangleLines(boxX - 6, boxY - 6, boxSize + 12, boxSize + 12, Fade(RAYWHITE, 0.8f));

    // nextTet をプレビュー描画（回転0）
    nextTet.renderPreview(boxX, boxY, boxCell, 0);

}


bool Board::canCollide(const Tetrimino& tet) const
{
    const auto& a = tet.getCells(cellSize, startX, startY);

    for (const auto& cell : a) {
        if (outOfBounds(cell.x, cell.y)) return true;
    }

    for (int row = 0; row < gridRow; ++row) {//why did i make such shit code
        for (int col = 0; col < gridCol; ++col) {
            if (grid[row][col] != -1) {
                for (const auto& cellA : a) {
                    if (cellA.x == col && cellA.y == row) return true;
                }
            }
            
        }
    }
        
    


    return false;
}

void Board::init()
{

    startX = GetScreenWidth() / 2 - (gridCol * cellSize) / 2;
    startY = cellSize;

    for (auto& row : grid) {
        row.fill(-1);
    }

    std::uniform_int_distribution<int> dist(0, MINO_CONFIG::MINO_DATA.size() - 1);
    int shapeId = dist(rng);

    int spawnX = startX + (gridCol / 2 - 2) * cellSize; // だいたい中央
    int spawnY = startY;
    nextTet = Tetrimino(dist(rng), spawnX, spawnY);

    instanceNewTet();
}

bool Board::outOfBounds(int x, int y) const
{
    if (x < 0 || x >= gridCol) return true;
    if (y >= gridRow) return true;

    return false;
}

void Board::instanceNewTet()
{
    std::uniform_int_distribution<int> dist(0, MINO_CONFIG::MINO_DATA.size() - 1);
    int shapeId = dist(rng);

    int spawnX = startX + (gridCol / 2 - 2) * cellSize; 
    int spawnY = startY;

    currTet = nextTet;
    nextTet = Tetrimino(dist(rng), spawnX, spawnY);
}

void Board::lockCurrToBoard()
{
    auto cells = currTet.getCells(cellSize, startX, startY);
    int id = currTet.getShapeId();

    for (const auto& cell : cells) {
        if (cell.y >= 0 && cell.y < gridRow && cell.x >= 0 && cell.x < gridCol) {
            grid[cell.y][cell.x] = id;
        }
    }
}


int Board::checkLines()
{
    int cleard = 0;


    for (int row = gridRow - 1; row >= 0; --row) {
        bool fullLine = true;

        for (int col = 0; col < gridCol; ++col) {
            if (grid[row][col] == -1) {fullLine = false; break;}
        }
        
        //move down
        if (fullLine) {
            for (int r = row; r > 0; --r) {
                for (int col = 0; col < gridCol; ++col) {
                    grid[r][col] = grid[r-1][col];
                }
            }

            for (int col = 0; col < gridCol; ++col) {
                grid[0][col] = -1;
            }
            
            cleard++;
            row++;
        }
    }
    return cleard;
}
