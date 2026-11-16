#include "Win.h"

Win::Win()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

Win::Win(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

Win::~Win() { shutdown(); }

void Win::initialise()
{
    mGameState.nextSceneID = NO_SCENE;

    mGameState.bgm = LoadMusicStream("assets/game/04 - Silent Forest.wav");
    SetMusicVolume(mGameState.bgm, 0.33f);
    // PlayMusicStream(mGameState.bgm);

    mGameState.jumpSound = LoadSound("assets/game/Dirt Jump.wav");
    win = LoadTexture("assets/game/Win.png");


}

void Win::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

  
}


// void Win::updateLives(int lives)
// {
//     mLives = lives;

// }

void Win::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    DrawTexturePro(win,
    { 0, 0, (float) win.width, (float) win.height },
    { 0, 0, (float) 1152, (float) 792 },
    { 0, 0 }, 0.0f, WHITE);


}

void Win::shutdown()
{
    
    UnloadMusicStream(mGameState.bgm);
    mGameState.bgm = {};
    UnloadSound(mGameState.jumpSound);
    mGameState.jumpSound = {};
    UnloadTexture(win);
}