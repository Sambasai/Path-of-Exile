#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game_level.h"

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_OVER
};

enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(20.0f, 20.0f);
const glm::vec2 MONSTER_SIZE(20.0f, 20.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(500.0f);
const float MONSTER_VELOCITY(700.0f);

class Game
{
public:
    GameState State;
    bool Keys[1024];
    unsigned int Width, Height;
    int light;
    glm::vec2 player_pos;
    int coins, score;
    std::vector<GameLevel> Levels;
    unsigned int Level;
    Game(unsigned int width, unsigned int height);
    ~Game();

    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void DoCollisions();
    void ChangeLevel(int lvl);
};

#endif