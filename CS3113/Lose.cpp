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


    lose = LoadTexture("assets/game/Lose.png");


}

void Lose::update(float deltaTime)
{
  
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
    

    UnloadTexture(lose);
}