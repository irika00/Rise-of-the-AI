#include "LevelC.h"

LevelC::LevelC()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

LevelC::LevelC(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

LevelC::~LevelC() { shutdown(); }

void LevelC::initialise()
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
    const float SIZE = 140.0f;
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
        mGameState.xochitl->getScale().x / 2.0f ,
        mGameState.xochitl->getScale().y
        // SIZE * 0.8f,
        // SIZE * 0.9f
    });
    mGameState.xochitl->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });


    std::map<Direction, std::vector<int>> turnipAnimationAtlas = {
        { LEFT,  {  6,  7, 8} },
        { RIGHT, { 3, 4, 5 } },
    };

    mGameState.turnip = new Entity(
        {  mOrigin.x - 300.0f, mOrigin.y - 400.0f }, // position
        { 60.0f, 60.0f },             // scale
        "assets/game/turnip.png",                     // texture file address
        ATLAS,                                      // single image or atlas?
        { 4, 3},                                   // atlas dimensions
        turnipAnimationAtlas,                      // actual atlas
        NPC                                      // entity type
    );

    mGameState.turnip->setAIType(FOLLOWER);
    mGameState.turnip->setAIState(IDLE);
    mGameState.turnip->setSpeed(Entity::DEFAULT_SPEED * 0.50f);
    mGameState.turnip->setColliderDimensions({
        mGameState.turnip->getScale().x / 2.0f,
        mGameState.turnip->getScale().y 
    });
    mGameState.turnip->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    mGameState.turnip->setDirection(LEFT);

    std::map<Direction, std::vector<int>> breadAnimationAtlas = {
        { RIGHT, { 0, 1, 2, 2 } },
        { LEFT,  { 0, 1, 2, 2} },
    };

    mGameState.bread = new Entity(
        {  mOrigin.x - 1000.0f, mOrigin.y + 200.0f }, // position
        { 60.0f, 60.0f },             // scale
        "assets/game/bread.png",                     // texture file address
        ATLAS,                                      // single image or atlas?
        { 4, 4},                                   // atlas dimensions
        breadAnimationAtlas,                      // actual atlas
        NPC                                      // entity type
    );

    

    mGameState.bread->setAIType(WANDERER);
    mGameState.bread->setAIState(IDLE);
    mGameState.bread->setSpeed(Entity::DEFAULT_SPEED * 0.50f);
    mGameState.bread->setColliderDimensions({
        mGameState.bread->getScale().x / 2.0f,
        mGameState.bread->getScale().y 
    });
    mGameState.bread->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    mGameState.bread->setDirection(LEFT);

    


}

void LevelC::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

    mGameState.xochitl->update(
        deltaTime,      // delta time / fixed timestep
        nullptr,        // player
        mGameState.map, // map
        mGameState.turnip,        // collidable entities
        1              // col. entity count
    );

    mGameState.turnip->update(
        deltaTime,      // delta time / fixed timestep
        mGameState.xochitl,        // player
        mGameState.map, // map
        nullptr,        // collidable entities
        0               // col. entity count
    );

    mGameState.bread->update(
        deltaTime,      // delta time / fixed timestep
        mGameState.xochitl,        // player
        mGameState.map, // map
        nullptr,        // collidable entities
        0               // col. entity count
    );

    if (mGameState.xochitl->getPosition().y > END_GAME_THRESHOLD)
        mGameState.nextSceneID = LEVEL_B;
}


// void LevelC::updateLives(int lives)
// {
//     mLives = lives;

// }

void LevelC::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.xochitl->render();
    mGameState.xochitl->displayCollider();
    
    mGameState.map->render();
    mGameState.turnip->render();
    mGameState.turnip->displayCollider();
    mGameState.bread->render();
    mGameState.bread->displayCollider();
}

void LevelC::shutdown()
{
    delete mGameState.xochitl;
    mGameState.xochitl = nullptr;
    delete mGameState.map;
    mGameState.map = nullptr;
    delete mGameState.turnip;
    mGameState.turnip = nullptr;
    delete mGameState.bread;
    mGameState.bread = nullptr;
    // UnloadMusicStream(mGameState.bgm);
    mGameState.bgm = {};
    UnloadSound(mGameState.jumpSound);
    mGameState.jumpSound = {};
}