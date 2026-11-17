#include "CS3113/LevelB.h"
#include "CS3113/LevelA.h"
#include "CS3113/LevelC.h"
#include "CS3113/Welcome.h"
#include "CS3113/Win.h"
#include "CS3113/Lose.h"

/**

* Author: Irika Aggarwal

* Assignment: Rise of the AI

* Date due: Monday, Nov 16th, 11:59PM

* I pledge that I have completed this assignment without

* collaborating with anyone else, in conformance with the

* NYU School of Engineering Policies and Procedures on

* Academic Misconduct.

**/

// Global Constants
constexpr int SCREEN_WIDTH  = 1152,
              SCREEN_HEIGHT = 792,
              FPS           = 120;
              

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus       = RUNNING;
float     gPreviousTicks   = 0.0f,
          gTimeAccumulator = 0.0f;

Camera2D gCamera = {};
int gLives = 3;

Scene                     *gCurrentScene = nullptr;
std::map<SceneID, Scene *> gLevels       = {};

// Function Declarations
void switchToScene(Scene *scene);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene *scene)
{

    if (gCurrentScene && gCurrentScene->getState().xochitl != nullptr)
        gLives = gCurrentScene->getState().xochitl->getLives();
    if (gCurrentScene) gCurrentScene->shutdown();
    gCurrentScene = scene;
    gCurrentScene->initialise();
    if (scene == gLevels[LEVEL_A]) gLives = 3.0f;
    // gCamera.target = gCurrentScene->getState().xochitl->getPosition();
    Entity *player = gCurrentScene->getState().xochitl;
    if (player != nullptr)
    {
        player->setLives(gLives);
        gCamera.target = player->getPosition();
    } 
    
    else   
    {
        gCamera.target = ORIGIN;

    }                
    
    
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Scenes");
    InitAudioDevice();


    gLevels[WELCOME] = new Welcome(ORIGIN, "#FFFFFF");
    gLevels[LEVEL_A] = new LevelA(ORIGIN, "#FFFFFF");
    gLevels[LEVEL_B] = new LevelB(ORIGIN, "#FFFFFF");
    gLevels[LEVEL_C] = new LevelC(ORIGIN, "#FFFFFF");
    gLevels[WIN] = new Win(ORIGIN, "#FFFFFF");
    gLevels[LOSE] = new Lose(ORIGIN, "#FFFFFF");
    
    
    
    gLives = 3;
    switchToScene(gLevels[WELCOME]);

    gCamera.offset   = { SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT / 2};
    gCamera.rotation = 0.0f;
    gCamera.zoom     = 1.0f;

    SetTargetFPS(FPS);
}

void processInput()
{


    //for debugging

    if (IsKeyPressed(KEY_ONE)) switchToScene(gLevels[LEVEL_A]);

    
    if (IsKeyPressed(KEY_TWO)) switchToScene(gLevels[LEVEL_B]);
    if (IsKeyPressed(KEY_THREE)) switchToScene(gLevels[LEVEL_C]);
    if (IsKeyPressed(KEY_FOUR)) switchToScene(gLevels[WELCOME]);
    if (IsKeyPressed(KEY_FIVE)) switchToScene(gLevels[WIN]);
    if (IsKeyPressed(KEY_SIX)) switchToScene(gLevels[LOSE]);


    if (IsKeyPressed(KEY_ENTER) && gCurrentScene == gLevels[WELCOME])
    {
        switchToScene(gLevels[LEVEL_A]);
    }
    else if (IsKeyPressed(KEY_ENTER)) 
    {
        switchToScene(gLevels[WELCOME]);
    
    }


    // if (IsKeyPressed(KEY_W) &&
    //     gCurrentScene->getState().xochitl->isCollidingBottom())
    // {
    //     gCurrentScene->getState().xochitl->jump();
    //     PlaySound(gCurrentScene->getState().jumpSound);
    // }

    // if (GetLength(gCurrentScene->getState().xochitl->getMovement()) > 1.0f)
    //     gCurrentScene->getState().xochitl->normaliseMovement();

    Entity *player = gCurrentScene->getState().xochitl;
    if (player != nullptr)
    {
        player->resetMovement();

        if (IsKeyDown(KEY_D)) player->moveRight();
        if (IsKeyDown(KEY_A)) player->moveLeft();

        if (IsKeyPressed(KEY_W) && player->isCollidingBottom())
        {
            player->jump();
            PlaySound(gCurrentScene->getState().jumpSound);
        }

        // to avoid faster diagonal speed
        if (GetLength(player->getMovement()) > 1.0f)
            player->normaliseMovement();
    }

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    // Delta time
    float ticks     = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    // Fixed timestep
    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        Entity *player = gCurrentScene->getState().xochitl;
        float livesBefore = (player != nullptr) ? player->getLives() : 0.0f;

        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;

        player = gCurrentScene->getState().xochitl;
        if (player != nullptr)
        {
            float livesNow = player->getLives();

            if (livesNow < livesBefore && livesNow > 0.0f)
                PlaySound(gCurrentScene->getState().hitSound);

            Vector2 currentPlayerPosition = {
                player->getPosition().x, player->getPosition().y
            };
            panCamera(&gCamera, &currentPlayerPosition);
        }
    }

    gTimeAccumulator = deltaTime;
}


void render()
{
    BeginDrawing();

    if (gCurrentScene == gLevels[WELCOME] || gCurrentScene == gLevels[WIN] || gCurrentScene == gLevels[LOSE])
    {
        gCurrentScene->render();          // no camera
    }
    else
    {
        BeginMode2D(gCamera);
        gCurrentScene->render();
        EndMode2D();

        Entity *player = gCurrentScene->getState().xochitl;
        DrawText(TextFormat("Lives: %.0f", player->getLives()),
                 20, 20, 30, PINK);


        //to be deleted
        // Entity *player = gCurrentScene->getState().xochitl;
        float   playerX = player->getPosition().x;
        float playerY = player->getPosition().y;

        // DrawText(TextFormat("player x: %.0f", playerX), 20, 60, 24, BLACK);
        DrawText(TextFormat("player y: %.0f", playerY), 20, 60, 24, BLACK);



    }

    EndDrawing();
}

void shutdown()
{
    for (std::pair<const SceneID, Scene *> &entry : gLevels)
        delete entry.second;

    gLevels.clear();

    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();

        if (gCurrentScene->getState().nextSceneID != NO_SCENE)
        {
            SceneID id = gCurrentScene->getState().nextSceneID;
            switchToScene(gLevels[id]);
        }

        render();
    }

    shutdown();

    return 0;
}