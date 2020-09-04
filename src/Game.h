#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Resources.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Bullet.h"
#include "Enemy.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

const float bulletLife = 5.0f;
const float bulletCooldown = 0.40f;
const float rotationSpeed = 3.0f;

enum GameState
{
    GAME_MENU,
    GAME_ACTIVE,
    GAME_OVER,
    GAME_WIN,
    GAME_GUIDE
};

enum GameDifficulty
{
    Easy,
    Normal,
    Hard
};

class Game
{
public:
    GameState State;
    GameDifficulty Difficulty;
    GLboolean Keys[1024];
    GLboolean buttonDown;
    GLint MouseX, MouseY;
    GLint Width, Height;

    bool shouldClose;

    Game(GLuint width, GLuint height);
    ~Game();

    void Init();

    void ProcessInput(float deltaTime);
    void Update(float deltaTime);
    void Render();
    void DoCollisions();
    void StartGame();
};

#endif
