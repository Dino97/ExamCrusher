#include "Game.h"

Renderer* renderer;
GameObject* player;
GameObject* background;
GameObject* difficulty;
GameObject* tutorial;
GameObject* quit;
GameObject* menuArrow;
GameObject* gameWin;
GameObject* gameOver;
GameObject* toMain;
GameObject* medianGrade;
GameObject* title;
GameObject* guide;
GameObject* play;

std::vector<Bullet*> bullets;
std::vector<Enemy*> enemies;
int grades[8];
int spawned[8];
int totalFailed = 0;

float timer = 0.0f;
float cdTimer = 0.0f;
float gradeTimer = 0.0f;
float reduceGradeTime = 3.0f;
float bulletSpeed = 250;
float spawnCooldown = 3.0f;

float spawnTimer = 3.0f;
int enemiesPerSubject = 1;

// Main menu
int selectedInMenu = 0;
int selectedDifficulty = 1;

Game::Game(GLuint width, GLuint height)
    : State(GAME_ACTIVE), Width(width), Height(height) { }

Game::~Game()
{
    delete renderer;
    delete player;

    int i;
    for(i = 0; i < bullets.size(); i++)
    {
        delete bullets[i];
    }

    for(i = 0; i < enemies.size(); i++)
    {
        delete enemies[i];
    }
}

void Game::Init()
{
    srand(time(NULL));
    this->shouldClose = false;

    Resources::LoadShader("shaders/Sprite.vert", "shaders/Sprite.frag", "sprite");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    Resources::GetShader("sprite").Use().SetInteger("image", 0);
    Resources::GetShader("sprite").SetMatrix4("projection", projection);

    // Load Textures
    Resources::LoadTexture("textures/player.png", GL_TRUE, "player");
    Resources::LoadTexture("textures/Bullet.png", GL_TRUE, "bullet");
    Resources::LoadTexture("textures/background_full.png", GL_FALSE, "background");

    Resources::LoadTexture("textures/ALG.png", GL_TRUE, "alg");
    Resources::LoadTexture("textures/SAE.png", GL_TRUE, "sae");
    Resources::LoadTexture("textures/PJSP.png", GL_TRUE, "pjsp");
    Resources::LoadTexture("textures/ENG.png", GL_TRUE, "eng");
    Resources::LoadTexture("textures/MA.png", GL_TRUE, "ma");
    Resources::LoadTexture("textures/OET.png", GL_TRUE, "oet");
    Resources::LoadTexture("textures/AR.png", GL_TRUE, "ar");
    Resources::LoadTexture("textures/SATR.png", GL_TRUE, "satr");

    Resources::LoadTexture("textures/5.png", GL_TRUE, "5");
    Resources::LoadTexture("textures/6.png", GL_TRUE, "6");
    Resources::LoadTexture("textures/7.png", GL_TRUE, "7");
    Resources::LoadTexture("textures/8.png", GL_TRUE, "8");
    Resources::LoadTexture("textures/9.png", GL_TRUE, "9");
    Resources::LoadTexture("textures/10.png", GL_TRUE, "10");

    Resources::LoadTexture("textures/Play.png", GL_TRUE, "play");
    Resources::LoadTexture("textures/Easy.png", GL_TRUE, "easy");
    Resources::LoadTexture("textures/Normal.png", GL_TRUE, "normal");
    Resources::LoadTexture("textures/Hard.png", GL_TRUE, "hard");
    Resources::LoadTexture("textures/Guide1.png", GL_TRUE, "tutorial");
    Resources::LoadTexture("textures/Quit.png", GL_TRUE, "quit");

    Resources::LoadTexture("textures/game_win.png", GL_TRUE, "win");
    Resources::LoadTexture("textures/game_over.png", GL_TRUE, "over");
    Resources::LoadTexture("textures/back_to_main.png", GL_TRUE, "to_main");

    Resources::LoadTexture("textures/HowTo.png", GL_TRUE, "guide");
    Resources::LoadTexture("textures/Title.png", GL_TRUE, "title");

    // Set up renderer
    Shader shader;
    shader = Resources::GetShader("sprite");
    renderer = new Renderer(shader);

    // GameObjects
    glm::vec2 playerSize(128, 128);
    glm::vec2 playerPos = glm::vec2(this->Width / 2 - playerSize.x/2, this->Height/2 - playerSize.y/2);
    Texture2D texture = Resources::GetTexture("player");
    player = new GameObject(playerPos, playerSize, texture);

    glm::vec2 bgdPos = glm::vec2(0.0f, 0.0f);
    glm::vec2 bgdSize = glm::vec2(this->Width, this->Height);
    Texture2D bgdTexture = Resources::GetTexture("background");
    background = new GameObject(bgdPos, bgdSize, bgdTexture);

    // Main menu
    glm::vec2 buttonSize = glm::vec2(256, 64);

    glm::vec2 playPos = glm::vec2(this->Width/2 - buttonSize.x/2, this->Height/2);
    Texture2D playTexture = Resources::GetTexture("play");
    play = new GameObject(playPos, buttonSize, playTexture);

    glm::vec2 diffPos = glm::vec2(this->Width/2 - buttonSize.x/2, this->Height/2 + 69);
    Texture2D diffTexture = Resources::GetTexture("normal");
    difficulty = new GameObject(diffPos, buttonSize, diffTexture);

    glm::vec2 tutPos = glm::vec2(this->Width/2 - buttonSize.x/2, this->Height/2 + 133);
    Texture2D tutTexture = Resources::GetTexture("tutorial");
    tutorial = new GameObject(tutPos, buttonSize, tutTexture);

    glm::vec2 quitPos = glm::vec2(this->Width/2 - buttonSize.x/2, this->Height/2 + 202);
    Texture2D quitTexture = Resources::GetTexture("quit");
    quit = new GameObject(quitPos, buttonSize, quitTexture);

    // Menu arrow
    glm::vec2 arrowPos = glm::vec2(this->Width/2 - buttonSize.x/2 - 69, this->Height/2);
    glm::vec2 arrowSize = glm::vec2(64, 64);
    Texture2D arrowTexture = Resources::GetTexture("bullet");
    menuArrow = new GameObject(arrowPos, arrowSize, arrowTexture);

    // Game win and over
    glm::vec2 textSize = glm::vec2(512, 256);
    glm::vec2 textPos = glm::vec2(this->Width/2 - textSize.x/2, this->Height/2 - textSize.y/2 - 69);

    Texture2D winTexture = Resources::GetTexture("win");
    gameWin = new GameObject(textPos, textSize, winTexture);

    Texture2D overTexture = Resources::GetTexture("over");
    gameOver = new GameObject(textPos, textSize, overTexture);

    glm::vec2 toMainSize = glm::vec2(512, 64);
    glm::vec2 toMainPos = glm::vec2(this->Width/2 - toMainSize.x/2, this->Height/2);
    Texture2D toMainTexture = Resources::GetTexture("to_main");
    toMain = new GameObject(toMainPos, toMainSize, toMainTexture);

    glm::vec2 gradeSize = glm::vec2(512, 512);
    glm::vec2 gradePos = glm::vec2(this->Width/2 - gradeSize.x/2, this->Height/2 - 197);
    Texture2D gradeTexture = Resources::GetTexture("10");
    medianGrade = new GameObject(gradePos, gradeSize, gradeTexture);

    glm::vec2 titleSize = glm::vec2(512, 256);
    glm::vec2 titlePos = glm::vec2(this->Width/2 - titleSize.x/2, this->Height/2 - 250);
    Texture2D titleTexture = Resources::GetTexture("title");
    title = new GameObject(titlePos, titleSize, titleTexture);

    glm::vec2 guidePos = glm::vec2(this->Width/2 - gradeSize.x/2, this->Height/2 - gradeSize.y/2);
    Texture2D guideTexture = Resources::GetTexture("guide");
    guide = new GameObject(guidePos, gradeSize, guideTexture);
}

void Game::ProcessInput(float deltaTime)
{
    // Game input logic
    if(this->State == GAME_ACTIVE)
    {
        if(this->Keys[GLFW_KEY_LEFT])
            player->Rotation -= deltaTime * rotationSpeed;
        else if(this->Keys[GLFW_KEY_RIGHT])
            player->Rotation += deltaTime * rotationSpeed;

        if(this->Keys[GLFW_KEY_SPACE] && cdTimer <= 0 && !buttonDown)
        {
            double xVel, yVel;

            xVel = std::cos(player->Rotation - M_PI/2);
            yVel = std::sin(player->Rotation - M_PI/2);

            GLfloat bulletSize = 10.0f;

            glm::vec2 bulletPos((this->Width/2 - bulletSize) + xVel * player->Size.x/2, (this->Height/2 - bulletSize) + yVel * player->Size.y/2);
            glm::vec2 velocity(xVel * bulletSpeed, yVel * bulletSpeed);
            Texture2D bulletTexture = Resources::GetTexture("bullet");

            bullets.push_back(new Bullet(bulletPos, bulletSize, velocity, bulletTexture));
            cdTimer = bulletCooldown;
            buttonDown = GL_TRUE;
        }

        if(this->Keys[GLFW_KEY_ESCAPE] && !buttonDown)
        {
            this->State = GAME_MENU;
            buttonDown = GL_TRUE;
        }
    }
    else if(this->State == GAME_MENU)
    {
        if(selectedInMenu == 0)
        {
            // Start the game
            if(this->Keys[GLFW_KEY_ENTER] && !buttonDown)
            {
                StartGame();
                buttonDown = GL_TRUE;
            }
        }

        // Select difficulty
        else if(selectedInMenu == 1)
        {
            if(this->Keys[GLFW_KEY_LEFT] && selectedDifficulty > 0 && !buttonDown)
            {
                selectedDifficulty--;
                buttonDown = GL_TRUE;
            }
            else if(this->Keys[GLFW_KEY_RIGHT] && selectedDifficulty < 2 && !buttonDown)
            {
                selectedDifficulty++;
                buttonDown = GL_TRUE;
            }
        }
        // Show tutorial
        else if(selectedInMenu == 2)
        {
            if(this->Keys[GLFW_KEY_ENTER] && !buttonDown)
            {
                this->State = GAME_GUIDE;
                buttonDown = GL_TRUE;
            }
        }

        // Exit application
        else if(selectedInMenu == 3)
        {
            if(this->Keys[GLFW_KEY_ENTER])
            {
                this->shouldClose = true;
            }
        }

        // Up/down navigation
        if(this->Keys[GLFW_KEY_UP] && selectedInMenu > 0 && !buttonDown)
        {
            selectedInMenu--;
            buttonDown = GL_TRUE;
        }
        else if(this->Keys[GLFW_KEY_DOWN] && selectedInMenu < 3 && !buttonDown)
        {
            selectedInMenu++;
            buttonDown = GL_TRUE;
        }
    }
    else if(this->State == GAME_WIN)
    {
        if(this->Keys[GLFW_KEY_ENTER] && !buttonDown)
        {
            this->State = GAME_MENU;
            buttonDown = GL_TRUE;
        }
    }
    else if(this->State == GAME_OVER)
    {
        if(this->Keys[GLFW_KEY_ENTER] && !buttonDown)
        {
            this->State = GAME_MENU;
            buttonDown = GL_TRUE;
        }
    }
    else if(this->State == GAME_GUIDE)
    {
        if(this->Keys[GLFW_KEY_ENTER] && !buttonDown)
        {
            this->State = GAME_MENU;
            buttonDown = GL_TRUE;
        }
    }
}

void Game::Update(float deltaTime)
{
    // Game logic
    if(this->State == GAME_ACTIVE)
    {
        this->DoCollisions();
        timer += deltaTime;

        int totalSpawnedEnemies = 0;
        int i;

        // Check how many enemies from each subject were spawned
        for(i = 0; i < 8; i++)
        {
            totalSpawnedEnemies += spawned[i];
        }

        // Spawn enemies
        if(totalSpawnedEnemies < enemiesPerSubject * 8) {
            if(spawnTimer > 0)
                spawnTimer -= deltaTime;
            else if(spawnTimer <= 0)
            {
                spawnTimer = spawnCooldown;

                glm::vec2 enemyPos(rand() % this->Width, rand() % this->Height);
                Texture2D enemyTexture;

                int enemyType;

                do {
                    enemyType = rand() % 8;
                } while(spawned[enemyType] >= enemiesPerSubject);

                switch (enemyType) {
                    case 0:
                        enemyTexture = Resources::GetTexture("alg");
                        spawned[0]++;
                        break;
                    case 1:
                        enemyTexture = Resources::GetTexture("sae");
                        spawned[1]++;
                        break;
                    case 2:
                        enemyTexture = Resources::GetTexture("pjsp");
                        spawned[2]++;
                        break;
                    case 3:
                        enemyTexture = Resources::GetTexture("eng");
                        spawned[3]++;
                        break;
                    case 4:
                        enemyTexture = Resources::GetTexture("ma");
                        spawned[4]++;
                        break;
                    case 5:
                        enemyTexture = Resources::GetTexture("oet");
                        spawned[5]++;
                        break;
                    case 6:
                        enemyTexture = Resources::GetTexture("ar");
                        spawned[6]++;
                        break;
                    case 7:
                        enemyTexture = Resources::GetTexture("satr");
                        spawned[7]++;
                        break;
                }

                int movementStyle;
                if(this->Difficulty == Easy)
                    movementStyle = 0;
                else
                    movementStyle = rand() % 4 + 1;

                enemies.push_back(new Enemy(enemyPos, 32, enemyTexture, movementStyle, enemyType));
                enemies.back()->gradeTimer = reduceGradeTime;
            }
        }

        // Move enemies
        for(i = 0; i < enemies.size(); i++)
        {
            float movementSpeed;

            movementSpeed = 1.0f;

            if(enemies[i]->enemyType == ALG || enemies[i]->enemyType == MA || enemies[i]->enemyType == OET)
                movementSpeed = 2.0f;

            switch(enemies[i]->MovementStyle) {
                // Static
                case 0:
                    break;
                // Horizontal movement
                case 1:
                    enemies[i]->Position.x += sin(timer * movementSpeed) * 100 * movementSpeed* deltaTime;
                    break;
                // Vertical movement
                case 2:
                    enemies[i]->Position.y += sin(timer * movementSpeed) * 100 * movementSpeed * deltaTime;
                    break;
                // Clockwise
                case 3:
                    enemies[i]->Position.y -= sin(timer * movementSpeed) * 100 * movementSpeed * deltaTime;
                    enemies[i]->Position.x -= cos(timer * movementSpeed) * 100 * movementSpeed * deltaTime;
                    break;
                // Counter-clockwise
                case 4:
                    enemies[i]->Position.y += sin(timer * movementSpeed) * 100 * movementSpeed * deltaTime;
                    enemies[i]->Position.x += cos(timer * movementSpeed) * 100 * movementSpeed * deltaTime;
                    break;
            }

            enemies[i]->gradeTimer -= deltaTime;

            if(enemies[i]->gradeTimer <= 0)
            {
                enemies[i]->ReduceGrade();
                enemies[i]->gradeTimer = reduceGradeTime;

                if(enemies[i]->shouldGetDestroyed)
                    totalFailed++;
            }

            if(enemies[i]->shouldGetDestroyed)
            {
                delete enemies[i];
                enemies.erase(enemies.begin() + i);
            }
        }

        // Handle shooting cooldown and moving bullets
        for(i = 0; i < bullets.size(); i++)
        {
            bullets[i]->Move(deltaTime);

            bullets[i]->lifeTime += deltaTime;

            if(bullets[i]->lifeTime > bulletLife)
            {
                delete bullets[i];
                bullets.erase(bullets.begin() + i);
            }
        }

        // Shooting cooldown reduction
        if(cdTimer > 0)
            cdTimer -= deltaTime;

        // Check if the game ended
        if(totalSpawnedEnemies >= enemiesPerSubject * 8 && enemies.size() == 0)
        {
            if(totalFailed <= (enemiesPerSubject * 4))
            {
                this->State = GAME_WIN;

                float middleGrade = 0;

                for(i = 0; i < 8; i++)
                    middleGrade += grades[i];

                middleGrade /= (enemiesPerSubject * 8 - totalFailed);
                middleGrade += 0.5f;

                int mg = (int)middleGrade;

                switch(mg)
                {
                case 10:
                    medianGrade->Sprite = Resources::GetTexture("10");
                    break;
                case 9:
                    medianGrade->Sprite = Resources::GetTexture("9");
                    break;
                case 8:
                    medianGrade->Sprite = Resources::GetTexture("8");
                    break;
                case 7:
                    medianGrade->Sprite = Resources::GetTexture("7");
                    break;
                case 6:
                    medianGrade->Sprite = Resources::GetTexture("6");
                    break;
                }
            }
            else
                this->State = GAME_OVER;
        }
    }

    else if(this->State == GAME_MENU)
    {
        switch(selectedDifficulty)
        {
        case 0:
            difficulty->Sprite = Resources::GetTexture("easy");
            break;
        case 1:
            difficulty->Sprite = Resources::GetTexture("normal");
            break;
        case 2:
            difficulty->Sprite = Resources::GetTexture("hard");
            break;
        }

        // Position of arrow
        switch(selectedInMenu)
        {
        case 0:
            menuArrow->Position.y = this->Height/2;
            break;
        case 1:
            menuArrow->Position.y = this->Height/2 + 69;
            break;
        case 2:
            menuArrow->Position.y = this->Height/2 + 133;
            break;
        case 3:
            menuArrow->Position.y = this->Height/2 + 202;
            break;
        }
    }
}

void Game::Render()
{
    background->Draw(*renderer);
    if(this->State == GAME_ACTIVE) {
        player->Draw(*renderer);

        int i;
        for(i = 0; i < bullets.size(); i++)
            bullets[i]->Draw(*renderer);

        for(i = 0; i < enemies.size(); i++)
            enemies[i]->Draw(*renderer);
    }
    else if(this->State == GAME_MENU)
    {
        difficulty->Draw(*renderer);
        tutorial->Draw(*renderer);
        quit->Draw(*renderer);
        menuArrow->Draw(*renderer);
        title->Draw(*renderer);
        play->Draw(*renderer);
    }
    else if(this->State == GAME_WIN)
    {
        gameWin->Draw(*renderer);
        toMain->Draw(*renderer);
        medianGrade->Draw(*renderer);
    }
    else if(this->State == GAME_OVER)
    {
        gameOver->Draw(*renderer);
        toMain->Draw(*renderer);
    }
    else if(this->State == GAME_GUIDE)
    {
        guide->Draw(*renderer);
    }
}

GLboolean CheckCollision(const Bullet& bullet, const GameObject& enemy)
{
    glm::vec2 center(bullet.Position + bullet.Radius);
    glm::vec2 aabb_half_extents(enemy.Size.x / 2, enemy.Size.y / 2);
    glm::vec2 aabb_center(
        enemy.Position.x + aabb_half_extents.x,
        enemy.Position.y + aabb_half_extents.y);

    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    glm::vec2 closest = aabb_center + clamped;

    difference = closest - center;
    return glm::length(difference) < bullet.Radius;
}

void Game::DoCollisions()
{
    int i,j,k;
    for(i = 0; i < bullets.size(); i++)
    {
        for(j = 0; j < enemies.size(); j++)
        {
            if(CheckCollision(*bullets[i], *enemies[j]))
            {
                // Add grade points
                switch (enemies[j]->enemyType) {
                    case ALG:
                        grades[0] += enemies[j]->Grade;
                        break;
                    case SAE:
                        grades[1] += enemies[j]->Grade;
                        break;
                    case PJSP:
                        grades[2] += enemies[j]->Grade;
                        break;
                    case ENG:
                        grades[3] += enemies[j]->Grade;
                        break;
                    case MA:
                        grades[4] += enemies[j]->Grade;
                        break;
                    case OET:
                        grades[5] += enemies[j]->Grade;
                        break;
                    case AR:
                        grades[6] += enemies[j]->Grade;
                        break;
                    case SATR:
                        grades[7] += enemies[j]->Grade;
                        break;
                }

                // Destroy bullet object
                delete bullets[i];
                bullets.erase(bullets.begin() + i);

                // Destroy enemy object
                delete enemies[j];
                enemies.erase(enemies.begin() + j);
            }
        }
    }
}

void Game::StartGame()
{
    // Set the game difficulty
    switch(selectedDifficulty) {
    case 0:
        this->Difficulty = Easy;
        break;
    case 1:
        this->Difficulty = Normal;
        break;
    case 2:
        this->Difficulty = Hard;
        break;
    }

    // Set difficulty rules
    if(this->Difficulty == Easy)
    {
        reduceGradeTime = 3.0f;
        bulletSpeed = 700.0f;
        spawnCooldown = 1.5f;
        enemiesPerSubject = 1;
    }
    else if(this->Difficulty == Normal)
    {
        reduceGradeTime = 1.5f;
        bulletSpeed = 450.0f;
        spawnCooldown = 1.5f;
        enemiesPerSubject = 2;
    }
    else
    {
        reduceGradeTime = 1.0f;
        bulletSpeed = 250.0f;
        spawnCooldown = 1.0f;
        enemiesPerSubject = 3;
    }

    int i;
    for(i = 0; i < 8; i++)
        grades[i] = 0;

    for(i = 0; i < 8; i++)
        spawned[i] = 0;

    totalFailed = 0;

    timer = 0.0f;
    cdTimer = 0.0f;
    gradeTimer = 0.0f;
    spawnTimer = 3.0f;

    player->Rotation = 0.0f;


    for(i = 0; i < bullets.size(); i++)
    {
        delete bullets[i];
    }

    bullets.clear();

    for(i = 0; i < enemies.size(); i++)
    {
        delete enemies[i];
    }

    enemies.clear();

    this->State = GAME_ACTIVE;
}
