#include "Welcome.h"

Welcome::Welcome()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

Welcome::Welcome(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

Welcome::~Welcome() { shutdown(); }

void Welcome::initialise()
{
    mGameState.nextSceneID = NO_SCENE;

    mGameState.bgm = LoadMusicStream("assets/game/bgmusic.mp3");
    SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(mGameState.bgm);

    welcome = LoadTexture("assets/game/welcome.png");


}

void Welcome::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

  
}


// void Welcome::updateLives(int lives)
// {
//     mLives = lives;

// }

void Welcome::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    DrawTexturePro(welcome,
    { 0, 0, (float) welcome.width, (float) welcome.height },
    { 0, 0, (float) 1152, (float) 792 },
    { 0, 0 }, 0.0f, WHITE);


}

void Welcome::shutdown()
{
    
    UnloadMusicStream(mGameState.bgm);
    mGameState.bgm = {};
    UnloadTexture(welcome);
}