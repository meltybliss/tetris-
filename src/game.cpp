#include "game.h"

Game::Game()
{
    score = 0;
    gameOver = false;
    bgm = LoadMusicStream("assets/A-Type.mp3"); 
    bgmReady = (bgm.stream.buffer != nullptr); 
    if (bgmReady) {
        SetMusicVolume(bgm, 0.5f);
        PlayMusicStream(bgm);
    }
}


Game::~Game() {

    if (bgmReady) UnloadMusicStream(bgm);
    CloseAudioDevice();


}

void Game::Run()
{
    while (!WindowShouldClose()) {
        //update
        this->Tick();
    

        BeginDrawing();

         ClearBackground(BLANK);

            this->Render();

        EndDrawing();

    }

    CloseWindow();
}

void Game::Tick() {
    
    BoardResult result = board.boardUpdate(GetFrameTime());

    if (bgmReady) UpdateMusicStream(bgm);

    if (result.cleardThisTick > 0) {
        static int table[5] = {0, 100, 300, 500, 800};
        score += table[result.cleardThisTick];
    }

    if (result.gameOver) {
        gameOver = true;
        
        StopMusicStream(bgm);

        if (IsKeyPressed(KEY_ENTER)) {
            score = 0;
            gameOver = false;
            
            PlayMusicStream(bgm);

            board.init();
        }
    }
}


void Game::Render() {

    board.render();

    std::string s = "Score: " + std::to_string(score);
    DrawText(s.c_str(), 20, 20, 30, RAYWHITE);

    if (gameOver) {
        const char* msg = "GAME OVER";
        int fontSize = 80;

        int textW = MeasureText(msg, fontSize);
        int x = GetScreenWidth()/2 - textW/2;
        int y = GetScreenHeight()/2 - fontSize/2;

        DrawText(msg, x, y, fontSize, RED);

        // ついでに再開案内も出すなら
        const char* sub = "Press Enter to restart";
        int subSize = 30;
        int subW = MeasureText(sub, subSize);
        DrawText(sub, GetScreenWidth()/2 - subW/2, y + 90, subSize, RAYWHITE);
    }
}
