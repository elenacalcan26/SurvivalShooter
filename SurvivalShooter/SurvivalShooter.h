#pragma once

#include <vector>
#include <time.h>

#include "components/simple_scene.h"

#define RADIUS 30
#define SQUARE_SIDE 100
#define PLAYER_SPEED 50.f

using namespace std;

struct Proiectil {
    double translateX, translateY;
    float angle;
    bool canRender;
};

struct Obstacle {
    double x;
    double y;
    double width;
    double height;
};

struct MapBordersPos {
    double x;
    double y;
    double width;
    double height;
};

struct Enemy {
    double x;
    double y;
    double handX1;
    double handY1;
    double handX2;
    double handY2;
    double speed;
    bool isShot;
};

namespace m1
{
    class SurvivalShooter : public gfxc::SimpleScene
    {
     public:
         SurvivalShooter();
        ~SurvivalShooter();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
   
        bool playerObstacleCollision();
        bool proiectilBorderCollision(Proiectil proiectil);
        bool proiectilObstacleCollision(Proiectil proiectil);
        bool playerBorderCollision();
        bool shotEnemy(Enemy enemy);
        bool playerEnemyCollision();
        
        void placeEnemy();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
       
        glm::mat3 modelMatrix, move, move1stHand, move2ndHand, enemy1Hand, enemy2Hand;

        double posX, posY, mousePosX, mousePosY, posArmsX, posArmsY, posArmX2, posArmY2;
        double prevX, prevY, prevPosArmX1, prevPosArmX2, prevPosArmY1, prevPosArmY2, healthX, healthY;
        float angle;
        int lifes;

        int score;

        time_t proiectilStart, proiectilEnd, enemyStart, enemyEnd;
      
        vector<Proiectil> proiectils;
        int num_proiectils;

        vector<Obstacle> obstacles;
        vector<MapBordersPos> bordersPos;
        vector<Enemy> enemies;
        int numEnemies;

    };
}   // namespace m1
