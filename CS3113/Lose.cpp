#include "Lose.h"

Lose::Lose()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

Lose::Lose(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

Lose::~Lose() { shutdown(); }

void Lose::initialise()
{
    mGameState.nextSceneID = NO_SCENE;

    mGameState.bgm = LoadMusicStream("assets/game/04 - Silent Forest.wav");
    SetMusicVolume(mGameState.bgm, 0.33f);
    // PlayMusicStream(mGameState.bgm);

    mGameState.jumpSound = LoadSound("assets/game/Dirt Jump.wav");
    lose = LoadTexture("assets/game/Lose.png");


}

void Lose::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

  
}


// void Lose::updateLives(int lives)
// {
//     mLives = lives;

// }

void Lose::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    DrawTexturePro(lose,
    { 0, 0, (float) lose.width, (float) lose.height },
    { 0, 0, (float) 1152, (float) 792 },
    { 0, 0 }, 0.0f, WHITE);


}

void Lose::shutdown()
{
    
    UnloadMusicStream(mGameState.bgm);
    mGameState.bgm = {};
    UnloadSound(mGameState.jumpSound);
    mGameState.jumpSound = {};
    UnloadTexture(lose);
}