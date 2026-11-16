#ifndef LOSE_H
#define LOSE_H

#include "Scene.h"


class Lose : public Scene
{
private:
    Texture2D lose;
public:
    static constexpr float TILE_DIMENSION          = 72.0f, 
                           ACCELERATION_OF_GRAVITY = 981.0f,
                           END_GAME_THRESHOLD      = 800.0f;

    Lose();
    Lose(Vector2 origin, const char *bgHexCode);
    ~Lose();

    void initialise() override;
    void update(float deltaTime) override;
    // void updateLives(int lives) override;
    void render() override;
    void shutdown() override;
};

#endif // Lose_H