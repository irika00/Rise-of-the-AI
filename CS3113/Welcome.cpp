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

    mGameState.bgm = LoadMusicStream("assets/game/04 - Silent Forest.wav");
    SetMusicVolume(mGameState.bgm, 0.33f);
    // PlayMusicStream(mGameState.bgm);

    mGameState.jumpSound = LoadSound("assets/game/Dirt Jump.wav");

    /*
        ----------- MAP -----------
    */
    mGameState.map = new Map(
        1, 1, // map grid cols & rows
        {0},                // grid data
        "assets/game/welcome.png", // texture filepath
        TILE_DIMENSION,            // tile size
        1, 1,                      // texture cols & rows
        mOrigin                    // in-game origin
    );




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


    
    mGameState.map->render();

}

void Welcome::shutdown()
{
    
    delete mGameState.map;
    mGameState.map = nullptr;
    // UnloadMusicStream(mGameState.bgm);
    mGameState.bgm = {};
    UnloadSound(mGameState.jumpSound);
    mGameState.jumpSound = {};
}