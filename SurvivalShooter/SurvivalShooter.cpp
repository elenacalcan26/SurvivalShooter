#include "lab_m1/Tema1/SurvivalShooter.h"

#include <vector>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include "SurvivalShooter/transform2D.h"
#include "SurvivalShooter/createObjects2D.h"

using namespace std;
using namespace m1;


SurvivalShooter::SurvivalShooter()
{
}


SurvivalShooter::~SurvivalShooter()
{
}


void SurvivalShooter::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(150, 150, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);

    posX = 750;
    posY = 450;

    posArmsX = posX - 30;
    posArmsY = posY + 30;

    posArmX2 = posX + 30;
    posArmY2 = posY + 30;

    mousePosX = 0;
    mousePosY = 0;

    healthX = 1200;
    healthY = 800;

    num_proiectils = 0;
    numEnemies = 0;
    score = 0;

    proiectilStart = time(NULL);
    enemyStart = time(NULL);

    obstacles = {
        { 1150, 150, 100, 450 },
        { 250, 450, 250, 50 },
        { 350, 100, 300, 150 },
        { 750, 550, 100, 100 },
        { 400, 700, 100, 100 },
        { 800, 150, 75, 250 },
        { 950, 700, 400, 50 }
    };

    bordersPos = {
        { 200, 15, 50, 800 },
        { 200, 815, 1300, 50},
        { 1450, 15, 50, 850},
        { 200, 0, 1300, 16}
    };

    lifes = 140;

    Mesh* mapBorder = createObjects2D::CreateSquare("border", corner, SQUARE_SIDE * 8, glm::vec3(0.39f, 0.45f, 0.52f), true);
    AddMeshToList(mapBorder);

    Mesh* body = createObjects2D::CreateCircle("circle", corner, RADIUS, glm::vec3(1, 0.6f, 0), true);
    AddMeshToList(body);

    Mesh* arms = createObjects2D::CreateCircle("arms", corner, RADIUS / 3, glm::vec3(1, 0.6f, 0), true);
    AddMeshToList(arms);

    Mesh* map = createObjects2D::CreateSquare("map", corner, SQUARE_SIDE * 8, glm::vec3(0.5f, 0.5f, 0.5f), true);
    AddMeshToList(map);

    Mesh* obstacle = createObjects2D::CreateSquare("obstacle", corner, SQUARE_SIDE, glm::vec3(0.19f, 0.52f, 0.09f), true);
    AddMeshToList(obstacle);

    Mesh* proiectil = createObjects2D::CreateSquare("proiectil", corner, SQUARE_SIDE / 5, glm::vec3(0, 0, 0), true);
    AddMeshToList(proiectil);

    Mesh* enemy = createObjects2D::CreateSquare("enemy", corner, SQUARE_SIDE / 2, glm::vec3(1, 0, 0), true);
    AddMeshToList(enemy);

    Mesh* healthBarBorder = createObjects2D::CreateHealthBar("healthBarBorder", corner, lifes, glm::vec3(1, 0, 0), false);
    AddMeshToList(healthBarBorder);
}

void SurvivalShooter::placeEnemy() 
{
    // se plaseaza un inamic random pe harta la fieacre 5 secunde

    enemyEnd = time(NULL);

    if (enemyEnd - enemyStart >= 5) 
    {
        Enemy enemy;
        enemy.x = rand() % (1100 - 250) + 250;
        enemy.y = rand() % (800 - 10 + 1) + 10;
        enemy.speed = rand() % (75 - 25) + 25;
        enemy.handX1 = enemy.x - 20;
        enemy.handY1 = enemy.y - 20;
        enemy.handX2 = enemy.x + 45;
        enemy.handY2 = enemy.y - 20;
        enemy.isShot = false;

        enemies.push_back(enemy);
        numEnemies++;

        enemyStart = enemyEnd;
    }
}

void SurvivalShooter::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
   
    glViewport(0, 0, resolution.x, resolution.y);
}

bool SurvivalShooter::playerObstacleCollision() {

    for (int i = 0; i < obstacles.size(); i++) {
        float edgeX = posX, edgeY = posY;
        float obstacleX = obstacles[i].x;
        float obstacleY = obstacles[i].y;

        if (posX < obstacleX) {
            edgeX = obstacleX;
        }
        else if (posX > obstacleX + obstacles[i].width) {
            edgeX = obstacleX + obstacles[i].width;
        }

        if (posY < obstacleY) {
            edgeY = obstacleY;
        }
        else if (posY > obstacleY + obstacles[i].height) {
            edgeY = obstacleY + obstacles[i].height;
        }

        float distX = posX - edgeX;
        float distY = posY - edgeY;
        float dist = sqrt(pow(distX, 2) + pow(distY, 2));

        if (dist <= RADIUS) {
            return true;
        }
    }

    return false;
}

bool SurvivalShooter::playerBorderCollision() {

    // coliziune jucator - perete harta
    for (int i = 0; i < 4; i++) {
        float edgeX = posX, edgeY = posY;
        float obstacleX = bordersPos[i].x;
        float obstacleY = bordersPos[i].y;

        if (posX < obstacleX) {
            edgeX = obstacleX;
        }
        else if (posX > obstacleX + bordersPos[i].width) {
            edgeX = obstacleX + bordersPos[i].width;
        }

        if (posY < obstacleY) {
            edgeY = obstacleY;
        }
        else if (posY > obstacleY + bordersPos[i].height) {
            edgeY = obstacleY + bordersPos[i].height;
        }

        float distX = posX - edgeX;
        float distY = posY - edgeY;
        float dist = sqrt(pow(distX, 2) + pow(distY, 2));

        if (dist <= RADIUS) return true;
    }
    
    return false;
}

bool SurvivalShooter::shotEnemy(Enemy enemy) 
{
    // coliziune inamic - proiectile
    for (int i = 0; i < num_proiectils; i++) {
        if (proiectils[i].canRender == true) {

            if (proiectils[i].translateX < enemy.handX1 + (SQUARE_SIDE / 4) &&
                proiectils[i].translateX + (SQUARE_SIDE / 5) > enemy.handX1 &&
                proiectils[i].translateY < enemy.handY1 + (SQUARE_SIDE / 4) &&
                proiectils[i].translateY + (SQUARE_SIDE / 5) > enemy.handY1) 
            {
                proiectils[i].canRender = false;
                return true;
            }

            if (proiectils[i].translateX < enemy.handX2 + (SQUARE_SIDE / 4) &&
                proiectils[i].translateX + (SQUARE_SIDE / 5) > enemy.handX2 &&
                proiectils[i].translateY < enemy.handY2 + (SQUARE_SIDE / 4) &&
                proiectils[i].translateY + (SQUARE_SIDE / 5) > enemy.handY2)
            {
                proiectils[i].canRender = false;
                return true;
            }

            if (proiectils[i].translateX < enemy.x + (SQUARE_SIDE / 2) &&
                proiectils[i].translateX + (SQUARE_SIDE / 5) > enemy.x &&
                proiectils[i].translateY < enemy.y + (SQUARE_SIDE / 2) &&
                proiectils[i].translateY + (SQUARE_SIDE / 5) > enemy.y) {

                proiectils[i].canRender = false;
                return true;
            }
        }
    }
    return false;
}

bool SurvivalShooter::playerEnemyCollision() 
{

    // coliziune jucator - inamic
    for (int i = 0; i < numEnemies; i++) 
    {
        if (!enemies[i].isShot) {

            float edgeX = posX, edgeY = posY;
            float obstacleX = enemies[i].x;
            float obstacleY = enemies[i].y;

            if (posX < obstacleX) {
                edgeX = obstacleX;
            }
            else if (posX > obstacleX + (SQUARE_SIDE / 5)) {
                edgeX = obstacleX + (SQUARE_SIDE / 5);
            }

            if (posY < obstacleY) {
                edgeY = obstacleY;
            }
            else if (posY > obstacleY + (SQUARE_SIDE / 5)) {
                edgeY = obstacleY + (SQUARE_SIDE / 5);
            }

            float distX = posX - edgeX;
            float distY = posY - edgeY;
            float dist = sqrt(pow(distX, 2) + pow(distY, 2));

            if (dist <= RADIUS) {
                enemies[i].isShot = true;
                return true;
            }
        }
    }

    return false;
}

bool SurvivalShooter::proiectilBorderCollision(Proiectil proiectil)
{
    // coliziune proiectil - perete
    for (int j = 0; j < 4; j++)
    {
        if (proiectil.translateX < bordersPos[j].x + bordersPos[j].width &&
            proiectil.translateX + (SQUARE_SIDE / 5) > bordersPos[j].x &&
            proiectil.translateY < bordersPos[j].y + bordersPos[j].height &&
            proiectil.translateY + (SQUARE_SIDE / 5) > bordersPos[j].y)
        {
            return true;
        }
    }
     return false;
}

bool SurvivalShooter::proiectilObstacleCollision(Proiectil proiectil)
{
    // coliziune proiectil - obstacole
    for (int j = 0; j < obstacles.size(); j++)
    {
        if (proiectil.translateX < obstacles[j].x + obstacles[j].width &&
            proiectil.translateX + (SQUARE_SIDE / 5) > obstacles[j].x &&
            proiectil.translateY < obstacles[j].y + obstacles[j].height &&
            proiectil.translateY + (SQUARE_SIDE / 5) > obstacles[j].y)
        {
            return true;
        }
    }
    return false;
}

void SurvivalShooter::Update(float deltaTimeSeconds)
{

    // Player
    move = glm::mat3(1);
    move1stHand = glm::mat3(1);
    move2ndHand = glm::mat3(1);

    if (!playerObstacleCollision() && !playerBorderCollision()) {
       // daca nu jucatorul nu intra in coliziuni continua sa mearga pe harta
        prevX = posX;
        prevY = posY;
        prevPosArmX1 = posArmsX;
        prevPosArmY1 = posArmsY;
        prevPosArmX2 = posArmX2;
        prevPosArmY2 = posArmY2;
    }
    else {
        // jucatorul sta pe loc deoarece a intrat intr-o coliziune
        posX = prevX;
        posY = prevY;
        posArmsX = prevPosArmX1;
        posArmsY = prevPosArmY1;
        posArmX2 = prevPosArmX2;
        posArmY2 = prevPosArmY2;
    }
    
    move *= transform2D::Translate(posX, posY);

    move1stHand *= transform2D::Translate(posArmsX, posArmsY);
    move1stHand *= transform2D::Translate(RADIUS, -RADIUS);
    move1stHand *= transform2D::Rotate(angle);
    move1stHand *= transform2D::Translate(-RADIUS,  RADIUS);

    move2ndHand *= transform2D::Translate(posArmX2, posArmY2);
    move2ndHand *= transform2D::Translate(-RADIUS, -RADIUS);
    move2ndHand *= transform2D::Rotate(angle);
    move2ndHand *= transform2D::Translate(RADIUS, RADIUS);
    
    RenderMesh2D(meshes["arms"], shaders["VertexColor"], move1stHand);
    RenderMesh2D(meshes["arms"], shaders["VertexColor"], move2ndHand);
    RenderMesh2D(meshes["circle"], shaders["VertexColor"], move);

    if (playerEnemyCollision()) 
    {
        // daca jucatorul intra in coliziune cu inamicul se scade viata
        lifes -= 20;
    }
   
    // HealthBar

    Mesh* healthBar = createObjects2D::CreateHealthBar("healthBar", glm::vec3(0), lifes, glm::vec3(1, 0, 0), true);
    AddMeshToList(healthBar);
    RenderMesh2D(meshes["healthBar"], shaders["VertexColor"], transform2D::Translate(healthX, healthY));
    RenderMesh2D(meshes["healthBarBorder"], shaders["VertexColor"], transform2D::Translate(healthX, healthY));

    if (lifes == 0) 
    {
        // jucatorul nu ai are viata => GAME OVER
        cout << "GAME OVER" << endl;
        window->Close();
    }

    for (int i = 0; i < num_proiectils; i++) {
                 
        if (proiectilBorderCollision(proiectils[i]) || proiectilObstacleCollision(proiectils[i])) 
        {
            // proiectilul a intrat intr-o coliziune
            proiectils[i].canRender = false;
        }

        if (proiectils[i].canRender) {
            // proiectilul nu a disparut de pe ecran
            double d = 100 * deltaTimeSeconds;
            move = glm::mat3(1);
            proiectils[i].translateX += d * sin(proiectils[i].angle);
            proiectils[i].translateY += d * cos(proiectils[i].angle);

            move *= transform2D::Translate(proiectils[i].translateX, proiectils[i].translateY);

            RenderMesh2D(meshes["proiectil"], shaders["VertexColor"], move);
                   
        }

    }

    // Enemy

    placeEnemy();

    for (int i = 0; i < numEnemies; i++) 
    {
        // verific ca inamicul sa nu fie eliminat
        if (!enemies[i].isShot) 
        {

            // verific distantele intre inamic si player

            if (enemies[i].x - posX < 0) 
            {
                enemies[i].x += enemies[i].speed * deltaTimeSeconds;
                enemies[i].handX1 += enemies[i].speed * deltaTimeSeconds;
                enemies[i].handX2 += enemies[i].speed * deltaTimeSeconds;
            }
            else {
                enemies[i].x -= enemies[i].speed * deltaTimeSeconds;
                enemies[i].handX1 -= enemies[i].speed * deltaTimeSeconds;
                enemies[i].handX2 -= enemies[i].speed * deltaTimeSeconds;
            }

            if (enemies[i].y - posY < 0) {
                enemies[i].y += enemies[i].speed * deltaTimeSeconds;
                enemies[i].handY1 += enemies[i].speed * deltaTimeSeconds;
                enemies[i].handY2 += enemies[i].speed * deltaTimeSeconds;
            }
            else
            {
                enemies[i].y -= enemies[i].speed * deltaTimeSeconds;
                enemies[i].handY1 -= enemies[i].speed * deltaTimeSeconds;
                enemies[i].handY2 -= enemies[i].speed * deltaTimeSeconds;
            }

            enemies[i].isShot = shotEnemy(enemies[i]);

            if (!enemies[i].isShot) {

                // inamicul nu este lovit, atunci el apare pe ecran
                enemy1Hand = glm::mat3(1);
                enemy1Hand *= transform2D::Translate(enemies[i].handX1, enemies[i].handY1);
                enemy1Hand *= transform2D::Scale(0.5, 0.5);

                enemy2Hand = glm::mat3(1);
                enemy2Hand *= transform2D::Translate(enemies[i].handX2, enemies[i].handY2);
                enemy2Hand *= transform2D::Scale(0.5, 0.5);

                RenderMesh2D(meshes["enemy"], shaders["VertexColor"], enemy1Hand);
                RenderMesh2D(meshes["enemy"], shaders["VertexColor"], enemy2Hand);
                RenderMesh2D(meshes["enemy"], shaders["VertexColor"], transform2D::Translate(enemies[i].x, enemies[i].y));

            }
            else {

                // inamic lovit => creste scorul cu 1 pct

                score++;
                cout << "SCORE:" << ' ' << score << endl;
            }

        }
    }
   
    // Obstacles
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(1150, 150);
    modelMatrix *= transform2D::Scale(1.f, 4.5f);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(250, 450);
    modelMatrix *= transform2D::Scale(2.5f, 0.5f);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(350, 100);
    modelMatrix *= transform2D::Scale(3.f, 1.5f);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(800, 150);
    modelMatrix *= transform2D::Scale(0.75f, 2.5f);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(950, 700);
    modelMatrix *= transform2D::Scale(4.f, 0.5f);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);

    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], transform2D::Translate(750, 550));
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], transform2D::Translate(400, 700));

    // Map
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(250, 15);
    modelMatrix *= transform2D::Scale(1.5f, 1.f);
    RenderMesh2D(meshes["map"], shaders["VertexColor"], modelMatrix);

    // map borders
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(200, 15);
    modelMatrix *= transform2D::Scale(0.0625f, 1.f);
    RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(200, 815);
    modelMatrix *= transform2D::Scale(1.5625f, 0.0625f);
    RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(1450, 15);
    modelMatrix *= transform2D::Scale(0.0625f, 1.0625f);
    RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(200, 0);
    modelMatrix *= transform2D::Scale(1.625f, 0.02f);
    RenderMesh2D(meshes["border"], shaders["VertexColor"], modelMatrix);

}


void SurvivalShooter::FrameEnd()
{
}


void SurvivalShooter::OnInputUpdate(float deltaTime, int mods)
{

    auto camera = GetSceneCamera();
    glm::ivec2 resolution = window->GetResolution();
    if (window->KeyHold(GLFW_KEY_W)) {
        posY += PLAYER_SPEED * deltaTime;
        posArmsY += PLAYER_SPEED * deltaTime;
        posArmY2 += PLAYER_SPEED * deltaTime;
        
          if (!playerObstacleCollision() && !playerBorderCollision())
          {
            healthY += PLAYER_SPEED * deltaTime;
            camera->MoveUp((PLAYER_SPEED / 2) * deltaTime);
          }

    }

    if (window->KeyHold(GLFW_KEY_S)) {
        posY -= PLAYER_SPEED * deltaTime;
        posArmsY -= PLAYER_SPEED * deltaTime;
        posArmY2 -= PLAYER_SPEED * deltaTime;
        
        if (!playerObstacleCollision() && !playerBorderCollision()) {
            healthY -= PLAYER_SPEED * deltaTime;
            camera->MoveDown((PLAYER_SPEED / 2) * deltaTime);
        }

    } 

    if (window->KeyHold(GLFW_KEY_D)) {
        posX += PLAYER_SPEED * deltaTime;
        posArmsX += PLAYER_SPEED * deltaTime;
        posArmX2 += PLAYER_SPEED * deltaTime;

        if (!playerObstacleCollision() && !playerBorderCollision()) {
            healthX += PLAYER_SPEED * deltaTime;
            camera->MoveRight((PLAYER_SPEED / 2) * deltaTime);
        }
    } 

    if (window->KeyHold(GLFW_KEY_A)) {
        posX -= PLAYER_SPEED * deltaTime;
        posArmsX -= PLAYER_SPEED * deltaTime;
        posArmX2 -= PLAYER_SPEED * deltaTime;
        
        if (!playerObstacleCollision() && !playerBorderCollision()) {
            healthX -= PLAYER_SPEED * deltaTime;
            camera->MoveLeft((PLAYER_SPEED / 2) * deltaTime);
        }

    }

    camera->Update();
}


void SurvivalShooter::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void SurvivalShooter::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void SurvivalShooter::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    glm::ivec2 resolution = window->GetResolution();
    mousePosX = resolution.x / 2 - mouseX;
    mousePosY = resolution.y / 2 -  mouseY;
    angle = atan2(mousePosX, mousePosY);
}


void SurvivalShooter::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void SurvivalShooter::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    glm::ivec2 resolution = window->GetResolution();

    proiectilEnd = time(NULL);

    if (proiectilEnd - proiectilStart >= 5) {
        struct Proiectil proiectil;
        proiectil.canRender = true;
        proiectil.translateX = posX;
        proiectil.translateY = posY;
        proiectil.angle = atan2(mouseX - resolution.x / 2, resolution.y / 2 - mouseY);
        num_proiectils++;
        proiectils.push_back(proiectil);
        proiectilStart = proiectilEnd;
    }
}


void SurvivalShooter::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void SurvivalShooter::OnWindowResize(int width, int height)
{
}
