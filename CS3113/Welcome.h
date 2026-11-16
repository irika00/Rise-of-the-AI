#ifndef WELCOME_H
#define WELCOME_H

#include "Scene.h"



class Welcome : public Scene
{
private:
    Texture2D welcome;
public:
    static constexpr float TILE_DIMENSION          = 72.0f, 
                           ACCELERATION_OF_GRAVITY = 981.0f,
                           END_GAME_THRESHOLD      = 800.0f;

    Welcome();
    Welcome(Vector2 origin, const char *bgHexCode);
    ~Welcome();

    void initialise() override;
    void update(float deltaTime) override;
    // void updateLives(int lives) override;
    void render() override;
    void shutdown() override;
};

#endif // WELCOME_H