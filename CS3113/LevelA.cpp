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

    mGameState.bgm = LoadMusicStream("assets/game/bgmusic.mp3");
    SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(mGameState.bgm);

    mGameState.jumpSound = LoadSound("assets/game/slime_jump.wav");
    mGameState.hitSound  = LoadSound("assets/game/hit.wav"); 
    mGameState.deathSound = LoadSound("assets/game/death.wav");
    

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


    // std::map<Direction, std::vector<int>> turnipAnimationAtlas = {
    //     { LEFT,  {  6,  7, 8} },
    //     { RIGHT, { 3, 4, 5 } },
    // };

    // mGameState.turnip = new Entity(
    //     {  mOrigin.x - 1000.0f, mOrigin.y - 200.0f }, // position
    //     { 60.0f, 60.0f },             // scale
    //     "assets/game/turnip.png",                     // texture file address
    //     ATLAS,                                      // single image or atlas?
    //     { 4, 3},                                   // atlas dimensions
    //     turnipAnimationAtlas,                      // actual atlas
    //     NPC                                      // entity type
    // );

    // mGameState.turnip->setAIType(WANDERER);
    // mGameState.turnip->setAIState(IDLE);
    // mGameState.turnip->setSpeed(Entity::DEFAULT_SPEED * 0.50f);
    // mGameState.turnip->setColliderDimensions({
    //     mGameState.turnip->getScale().x / 2.0f,
    //     mGameState.turnip->getScale().y 
    // });
    // mGameState.turnip->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    // mGameState.turnip->setDirection(LEFT);


    std::map<Direction, std::vector<int>> breadAnimationAtlas = {
        { RIGHT, { 0, 1, 2, 2 } },
        { LEFT,  { 0, 1, 2, 2} },
    };

    mGameState.bread = new Entity(
        {  mOrigin.x - 900.0f, mOrigin.y + 200.0f }, // position
        { 100.0f, 100.0f },             // scale
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
        mGameState.bread->getScale().y / 2.0f
    });
    mGameState.bread->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    mGameState.bread->setDirection(LEFT);
    mGameState.bread->setPatrolRange(-300, 400);



    //5 * 10
    std::map<Direction, std::vector<int>> donutAnimationAtlas = {
        { RIGHT, { 10, 11, 12, 13, 14 } },
        { LEFT,  { 23, 28, 28, 28, 28} },
    };


    mGameState.donut = new Entity(
        { mOrigin.x - 1200.0f, mOrigin.y - 200.0f}, // position
        { 50.0f, 50.0f },             // scale
        "assets/game/donut.png",                     // texture file address
        ATLAS,                                      // single image or atlas?
        { 5, 10},                                   // atlas dimensions
        donutAnimationAtlas,                      // actual atlas
        NPC                                      // entity type
    );


    mGameState.donut->setAIType(LERPER);
    mGameState.donut->setAIState(IDLE);
    mGameState.donut->setSpeed(Entity::DEFAULT_SPEED * 0.50f);
    mGameState.donut->setColliderDimensions({
        mGameState.donut->getScale().x / 2.0f,
        mGameState.donut->getScale().y / 2.0f
    });
    mGameState.donut->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    mGameState.donut->setDirection(LEFT);
    mGameState.donut->setLerpFactor(2.0f);

    


}

void LevelA::update(float deltaTime)
{


    if (mGameState.xochitl->getPosition().x > 1750.0f && mGameState.xochitl->getPosition().y > 35) mGameState.nextSceneID = LEVEL_B;
    if (mGameState.xochitl->getLives() <= 0) mGameState.nextSceneID = LOSE;

    UpdateMusicStream(mGameState.bgm);

    mGameState.xochitl->update(
        deltaTime,      // delta time / fixed timestep
        nullptr,        // player
        mGameState.map, // map
        &mGameState.bread,        // collidable entities
        1              // col. entity count
    );

    // mGameState.turnip->update(
    //     deltaTime,      // delta time / fixed timestep
    //     mGameState.xochitl,        // player
    //     mGameState.map, // map
    //     nullptr,        // collidable entities
    //     0               // col. entity count
    // );

    // if (mGameState.xochitl->getPosition().y > END_GAME_THRESHOLD)
    //     mGameState.nextSceneID = LEVEL_B;


    mGameState.bread->update(
        deltaTime,      // delta time / fixed timestep
        mGameState.xochitl,        // player
        mGameState.map, // map
        nullptr,        // collidable entities
        0               // col. entity count
    );


     mGameState.donut->update(
        deltaTime,      // delta time / fixed timestep
        mGameState.xochitl,        // player
        mGameState.map, // map
        nullptr,        // collidable entities
        0               // col. entity count
    );

}



void LevelA::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));


    mGameState.xochitl->render();
    // mGameState.xochitl->displayCollider();
    
    mGameState.map->render();
    // mGameState.turnip->render();
    // mGameState.turnip->displayCollider();
    mGameState.bread->render();
    // mGameState.bread->displayCollider();

    mGameState.donut->render();
    // mGameState.donut->displayCollider();

   

}

void LevelA::shutdown()
{
    delete mGameState.xochitl;
    mGameState.xochitl = nullptr;
    delete mGameState.map;
    mGameState.map = nullptr;
    // delete mGameState.turnip;
    // mGameState.turnip = nullptr;
    delete mGameState.bread;
    mGameState.bread = nullptr;

    delete mGameState.donut;
    mGameState.donut = nullptr;

    UnloadMusicStream(mGameState.bgm);
    mGameState.bgm = {};
    UnloadSound(mGameState.jumpSound);
    UnloadSound(mGameState.hitSound);
    UnloadSound(mGameState.deathSound);
    mGameState.jumpSound = {};
    mGameState.hitSound  = {};
    mGameState.deathSound = {};
}