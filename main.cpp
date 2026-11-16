#include "CS3113/LevelB.h"
#include "CS3113/LevelA.h"
#include "CS3113/LevelC.h"
#include "CS3113/Welcome.h"


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
int lives = 3;

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
    if (gCurrentScene) gCurrentScene->shutdown();
    gCurrentScene = scene;
    gCurrentScene->initialise();
    // gCamera.target = gCurrentScene->getState().xochitl->getPosition();
    Entity *player = gCurrentScene->getState().xochitl;
    if (player != nullptr) gCamera.target = player->getPosition();
    else                   gCamera.target = ORIGIN;
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Scenes");
    InitAudioDevice();


    gLevels[WELCOME] = new Welcome(ORIGIN, "#FFFFFF");
    gLevels[LEVEL_A] = new LevelA(ORIGIN, "#FFFFFF");
    // gLevels[LEVEL_A]->updateLives(lives);
    gLevels[LEVEL_B] = new LevelB(ORIGIN, "#FFFFFF");
    gLevels[LEVEL_C] = new LevelC(ORIGIN, "#FFFFFF");
    

    switchToScene(gLevels[WELCOME]);

    gCamera.offset   = { SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT / 2};
    gCamera.rotation = 0.0f;
    gCamera.zoom     = 1.0f;

    SetTargetFPS(FPS);
}

void processInput()
{
    // gCurrentScene->getState().xochitl->resetMovement();

    // if (IsKeyDown(KEY_D)) gCurrentScene->getState().xochitl->moveRight();
    // if (IsKeyDown(KEY_A)) gCurrentScene->getState().xochitl->moveLeft();

    // SceneID id = gCurrentScene->getState().nextSceneID;
    // switchToScene(gLevels[id]);

    if (IsKeyPressed(KEY_ONE)) switchToScene(gLevels[LEVEL_A]);
    if (IsKeyPressed(KEY_ENTER)) switchToScene(gLevels[LEVEL_A]);
    if (IsKeyPressed(KEY_TWO)) switchToScene(gLevels[LEVEL_B]);
    if (IsKeyPressed(KEY_THREE)) switchToScene(gLevels[LEVEL_C]);
    if (IsKeyPressed(KEY_FOUR)) switchToScene(gLevels[WELCOME]);


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
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;

        Entity *player = gCurrentScene->getState().xochitl;
        if (player != nullptr)
        {
            Vector2 currentPlayerPosition = {
            gCurrentScene->getState().xochitl->getPosition().x, gCurrentScene->getState().xochitl->getPosition().y
            };
            panCamera(&gCamera, &currentPlayerPosition);

        }  

       
    }

    gTimeAccumulator = deltaTime;
}

// void render()
// {
//     BeginDrawing();
//     BeginMode2D(gCamera);



//     if (gCurrentScene == gLevels[WELCOME])
//     {
//         gCurrentScene->render();          // no camera
//     }
//     else
//     {
//         BeginMode2D(gCamera);
//         gCurrentScene->render();
//         DrawText(TextFormat("Lives: %.0f", gCurrentScene->getState().xochitl->getLives()), 20, 20, 30, WHITE);
//         EndMode2D();
//     }

//     EndDrawing();
// }


void render()
{
    BeginDrawing();

    if (gCurrentScene == gLevels[WELCOME])
    {
        gCurrentScene->render();          // no camera
    }
    else
    {
        BeginMode2D(gCamera);
        gCurrentScene->render();
        EndMode2D();

        DrawText(TextFormat("Lives: %.0f",
                 gCurrentScene->getState().xochitl->getLives()),
                 20, 20, 30, WHITE);
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