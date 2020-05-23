#include "include/raylib.h"
#include "vector_math.h"
#include "entity.cpp"
#include <math.h>
#include <list>

/* DEBUGGING CHECKLIST
maybe pointer got erased twice? (especially in a container) 

 */

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Shoot'em up");
    SetTargetFPS(60);

    bool showFps = 0;
    unsigned int score = 0;

    Entity player = CreatePlayer();

    std::list<Entity> bullets = {};
    std::list<Entity> enemies = {};

    while (!WindowShouldClose())
    {
        if (player.state != DEAD)
            EntityUpdate(&player, &bullets);

        // update enemies, check collisions with player, check if on screen
        for (std::list<Entity>::iterator enemy = enemies.begin(); enemy != enemies.end(); ++enemy)
        {
            EntityUpdate(&*enemy, &bullets);

            if (!EntityIsOnScreen(&*enemy, SCREEN_WIDTH, SCREEN_HEIGHT))
            {
                enemy->state = DEAD;
            }

            if (EntitiesCollide(*enemy, player) && player.state != DEAD)
            {
                player.state = DEAD;
                CreateDeathParticles(&bullets, player.pos.x - player.size.x, player.pos.y - player.size.y);
            }
            if (enemy->state == DEAD)
                enemies.erase(enemy);
        }

        // update enemies, check if out of bounds, check if collides with enemies
        for (std::list<Entity>::iterator bullet = bullets.begin(); bullet != bullets.end(); ++bullet)
        {
            EntityUpdate(&*bullet, &bullets);

            // delete out of bounds bullets
            if (!EntityIsOnScreen(&*bullet, SCREEN_WIDTH, SCREEN_HEIGHT))
            {
                bullet->state = DEAD;
            }

            // colliding with enemies
            for (std::list<Entity>::iterator enemy = enemies.begin(); enemy != enemies.end(); ++enemy)
            {
                if (EntitiesCollide(*bullet, *enemy))
                {
                    enemies.erase(enemy);
                    bullet->state = DEAD;
                    score += 10;
                }
            }

            if (bullet->state == DEAD)
                bullets.erase(bullet);
        }

        TraceLog(LOG_TRACE, "enemies size: %d", enemies.size());
        TraceLog(LOG_TRACE, "bullets size: %d", bullets.size());

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            if (player.state != DEAD)
                EntityDraw(&player);
            EntityListDraw(&bullets);
            EntityListDraw(&enemies);

            Color uicolor = {160, 160, 160, 200};
            Rectangle scoreBox = {SCREEN_WIDTH / 2 - 50, 0, 300, 100};
            DrawTextRec(GetFontDefault(), TextFormat("%d", score), scoreBox, 100, 10, 0, uicolor);

            if (showFps)
            {
                DrawFPS(0, 0);
            }

            if (player.state == DEAD)
            {
                Rectangle scoreBox = {SCREEN_WIDTH / 2 - 500, 600, 1000, 100};
                DrawTextRec(GetFontDefault(), TextFormat("R to restart"), scoreBox, 100, 10, 0, uicolor);
            }
        }
        EndDrawing();

        // check for inputs and show debug stuff
        {
            if (IsKeyPressed(KEY_F1))
            {
                SetTraceLogLevel(LOG_DEBUG);
            }
            else if (IsKeyPressed(KEY_F2))
            {
                SetTraceLogLevel(LOG_TRACE);
            }
            else if (IsKeyPressed(KEY_F3))
            {
                SetTraceLogLevel(LOG_INFO);
            }

            if (IsKeyPressed(KEY_F4))
            {
                showFps = !showFps;
            }

            if (IsKeyDown(KEY_ONE))
            {
                CreateEnemyRandomized(&enemies);
                CreateEnemyRandomized(&enemies);
                CreateEnemyRandomized(&enemies);
                CreateEnemyRandomized(&enemies);
                CreateEnemyRandomized(&enemies);
                CreateEnemyRandomized(&enemies);
                CreateEnemyRandomized(&enemies);
                CreateEnemyRandomized(&enemies);
                CreateEnemyRandomized(&enemies);
            }

            if (IsKeyDown(KEY_R))
            {
                player.state = ALIVE;
            }
        }
    }
    CloseWindow();

    return 0;
}
