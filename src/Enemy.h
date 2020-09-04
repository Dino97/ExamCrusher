#ifndef ENEMY_H
#define ENEMY_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "Texture2D.h"
#include "Resources.h"

enum EnemyType
{
    ALG,
    SAE,
    PJSP,
    ENG,
    MA,
    OET,
    AR,
    SATR
};

class Enemy : public GameObject
{
public:
    EnemyType enemyType;
    int Grade;
    int MovementStyle;
    Texture2D gradeTexture;
    float gradeTimer;

    bool shouldGetDestroyed;

    Enemy(glm::vec2 pos, GLfloat radius, Texture2D sprite, int style, int type);
    void Draw(Renderer& renderer);
    void ReduceGrade();
};

#endif
