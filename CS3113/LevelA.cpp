#include "LevelA.h"

LevelA::LevelA()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

LevelA::LevelA(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

LevelA::~LevelA() { shutdown(); }

void LevelA::initialise()
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
        LEVEL_WIDTH, LEVEL_HEIGHT, // map grid cols & rows
        mLevelData,                // grid data
        "assets/game/tilemap_packed.png", // texture filepath
        TILE_DIMENSION,            // tile size
        16, 7,                      // texture cols & rows
        mOrigin                    // in-game origin
    );

    /*
        ----------- PROTAGONIST -----------
    */
    // std::map<Direction, std::vector<int>> xochitlAnimationAtlas = {
    //     { DOWN,  {  0,  1,  2,  3,  4,  5,  6,  7 } },
    //     { LEFT,  {  8,  9, 10, 11, 12, 13, 14, 15 } },
    //     { UP,    { 24, 25, 26, 27, 28, 29, 30, 31 } },
    //     { RIGHT, { 40, 41, 42, 43, 44, 45, 46, 47 } },
    // };

    std::map<Direction, std::vector<int>> xochitlAnimationAtlas = {
        { DOWN,  {  0,  0,  0,  0} },
        { LEFT,  {  8,  9, 10, 11} },
        { UP,    { 12,  13,  14,  15 } },
        { RIGHT, { 4, 5, 6, 7 } },
    };

    float sizeRatio = 1.0f; //48.0f / 64.0f;

    // Assets from @see https://sscary.itch.io/the-adventurer-female
    // mGameState.xochitl = new Entity(
    //     { mOrigin.x, 20.0f}, // position
    //     { 250.0f * sizeRatio * 0.3f , 250.0f * 0.5f},             // scale
    //     "assets/game/witch.png",                     // texture file address
    //     ATLAS,                                      // single image or atlas?
    //     { 1, 70 },                                   // atlas dimensions
    //     xochitlAnimationAtlas,                      // actual atlas
    //     PLAYER                                      // entity type
    // );
    const float SIZE = 100.0f;
    mGameState.xochitl = new Entity(
        { mOrigin.x - 1200.0f, mOrigin.y - 200.0f }, // position
        { SIZE, SIZE },             // scale
        "assets/game/plate.png",                     // texture file address
        ATLAS,                                      // single image or atlas?
        { 4, 4},                                   // atlas dimensions
        xochitlAnimationAtlas,                      // actual atlas
        PLAYER                                      // entity type
    );

    mGameState.xochitl->setJumpingPower(550.0f);
    mGameState.xochitl->setColliderDimensions({
        // mGameState.xochitl->getScale().x / 3.5f,
        // mGameState.xochitl->getScale().y / 3.0f
        SIZE * 0.8f,
        SIZE * 0.9f
    });
    mGameState.xochitl->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
}

void LevelA::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

    mGameState.xochitl->update(
        deltaTime,      // delta time / fixed timestep
        nullptr,        // player
        mGameState.map, // map
        nullptr,        // collidable entities
        0               // col. entity count
    );

    if (mGameState.xochitl->getPosition().y > END_GAME_THRESHOLD)
        mGameState.nextSceneID = LEVEL_B;
}

void LevelA::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.xochitl->render();
    mGameState.map->render();
}

void LevelA::shutdown()
{
    delete mGameState.xochitl;
    mGameState.xochitl = nullptr;
    delete mGameState.map;
    mGameState.map = nullptr;

    // UnloadMusicStream(mGameState.bgm);
    mGameState.bgm = {};
    UnloadSound(mGameState.jumpSound);
    mGameState.jumpSound = {};
}