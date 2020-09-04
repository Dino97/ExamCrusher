#include "Enemy.h"

Enemy::Enemy(glm::vec2 pos, GLfloat radius, Texture2D sprite, int style, int type)
    : GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite), Grade(10), MovementStyle(style), shouldGetDestroyed(false)
{
    gradeTexture = Resources::GetTexture("10");

    switch (type) {
        case 0:
            this->enemyType = ALG;
            break;
        case 1:
            this->enemyType = SAE;
            break;
        case 2:
            this->enemyType = PJSP;
            break;
        case 3:
            this->enemyType = ENG;
            break;
        case 4:
            this->enemyType = MA;
            break;
        case 5:
            this->enemyType = OET;
            break;
        case 6:
            this->enemyType = AR;
            break;
        case 7:
            this->enemyType = SATR;
            break;
    }
}

void Enemy::Draw(Renderer& renderer)
{
    GameObject::Draw(renderer);

    renderer.DrawSprite(gradeTexture, this->Position, this->Size);
}

void Enemy::ReduceGrade()
{
    if(this->Grade > 6)
    {
        this->Grade--;

        switch(this->Grade)
        {
            case 10:
                gradeTexture = Resources::GetTexture("10");
                break;
            case 9:
                gradeTexture = Resources::GetTexture("9");
                break;
            case 8:
                gradeTexture = Resources::GetTexture("8");
                break;
            case 7:
                gradeTexture = Resources::GetTexture("7");
                break;
            case 6:
                gradeTexture = Resources::GetTexture("6");
                break;
            case 5:
                gradeTexture = Resources::GetTexture("5");
                break;
        }
    }
    else
        this->shouldGetDestroyed = true;
}
