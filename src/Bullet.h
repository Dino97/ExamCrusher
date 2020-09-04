#ifndef BULLET_H
#define BULLET_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "Texture2D.h"

class Bullet : public GameObject
{
public:
    GLfloat Radius;
    GLfloat lifeTime;

    Bullet();
    Bullet(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

    glm::vec2 Move(GLfloat deltaTime);
};

#endif
