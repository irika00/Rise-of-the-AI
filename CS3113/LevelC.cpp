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

    mGameState.bgm = LoadMusicStream("assets/game/bgmusic.mp3");
    mGameState.hitSound  = LoadSound("assets/game/hit.wav"); 
    mGameState.deathSound = LoadSound("assets/game/death.wav");
    SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(mGameState.bgm);

    mGameState.jumpSound = LoadSound("assets/game/slime_jump.wav");

    /*
        ----------- MAP -----------
    */

   //from https://kenney.nl/assets/pixel-platformer-food-expansion
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


    const float SIZE = 140.0f;
    mGameState.xochitl = new Entity(
        { - 1080.0f, mOrigin.y - 200.0f }, // position
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

    });
    mGameState.xochitl->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });


    std::map<Direction, std::vector<int>> turnipAnimationAtlas = {
        { LEFT,  {  6,  7, 8} },
        { RIGHT, { 3, 4, 5 } },
    };

    mGameState.turnip = new Entity(
        {  210.0f, mOrigin.y - 400.0f }, // position
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
    mGameState.turnip->setPatrolRange(64.0f, 650.0f); 

    std::map<Direction, std::vector<int>> breadAnimationAtlas = {
        { RIGHT, { 0, 1, 2, 2 } },
        { LEFT,  { 0, 1, 2, 2} },
    };

    mGameState.bread = new Entity(
         { 200.0f, 125.0f}, // position
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
    mGameState.bread->setPatrolRange(110.0f, 290.0f);;
    mGameState.bread->moveLeft();


    std::map<Direction, std::vector<int>> donutAnimationAtlas = {
        { RIGHT, { 10, 11, 12, 13, 14 } },
        { LEFT,  { 23, 28, 28, 28, 28} },
    };


    mGameState.donut = new Entity(
        { - 1080.0f, mOrigin.y - 200.0f}, // position
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
        mGameState.donut->getScale().y 
    });
    mGameState.donut->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
    mGameState.donut->setDirection(LEFT);
    mGameState.donut->setLerpFactor(2.0f);

    


}

void LevelC::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

    Entity *enemies[] = { mGameState.turnip, mGameState.bread };

    
    mGameState.xochitl->update(
        deltaTime,      // delta time / fixed timestep
        nullptr,        // player
        mGameState.map, // map
        enemies,        // collidable entities
        2             // col. entity count
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

    mGameState.donut->update(
        deltaTime,      // delta time / fixed timestep
        mGameState.xochitl,        // player
        mGameState.map, // map
        nullptr,        // collidable entities
        0               // col. entity count
    );

    if (mGameState.xochitl->getPosition().x > 1800.0f && mGameState.xochitl->getPosition().y > 35)
        mGameState.nextSceneID = WIN;

    if (mGameState.xochitl->getLives() <= 0) mGameState.nextSceneID = LOSE;
}


void LevelC::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.xochitl->render();
    // mGameState.xochitl->displayCollider();
    
    mGameState.map->render();
    mGameState.turnip->render();
    // mGameState.turnip->displayCollider();
    mGameState.bread->render();
    // mGameState.bread->displayCollider();
    
    mGameState.donut->render();
    // mGameState.donut->displayCollider();
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