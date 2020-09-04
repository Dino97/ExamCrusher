#include "Bullet.h"

Bullet::Bullet()
    : GameObject(), Radius(12.5f) {}

Bullet::Bullet(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity), Radius(radius), lifeTime(0.0f) {}

glm::vec2 Bullet::Move(GLfloat deltaTime)
{
    this->Position += this->Velocity * deltaTime;

    return this->Position;
}
