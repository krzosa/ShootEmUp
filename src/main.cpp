#include "include/raylib.h"
#include "vector_math.h"
#include "entity.cpp"
#include <math.h>
#include <list>
#include <vector>

/* DEBUGGING CHECKLIST
maybe pointer got erased twice? (especially in a container) 

 */
struct Event
{
    double time;
    bool flag;
};

struct GameState
{
    std::vector<Event> events;
    double timeToWin;
    bool showFps;
    unsigned int score;
};

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Shoot'em up");
    SetTargetFPS(60);

    GameState gameState = {};
    for (int i = 0; i < 20; i++)
    {
        gameState.events.push_back({(double)i});
    }
    gameState.timeToWin = 23.0f;

    Entity player = CreatePlayer();

    std::list<Entity> bullets = {};
    std::list<Entity> enemies = {};

    Color uicolor = {160, 160, 160, 200};

    while (!WindowShouldClose())
    {
        double time = GetTime();
        TraceLog(LOG_INFO, "Time: %f", time);

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
                    gameState.score += 10;
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

            Rectangle scoreBox = {SCREEN_WIDTH / 2 - 50, 0, 300, 100};
            DrawTextRec(GetFontDefault(), TextFormat("%d", gameState.score), scoreBox, 100, 10, 0, uicolor);

            if (gameState.showFps)
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
                gameState.showFps = !gameState.showFps;
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

            if (time > gameState.timeToWin && player.state != DEAD)
            {
                Rectangle scoreBox = {SCREEN_WIDTH / 2 - 500, 600, 1000, 100};
                DrawTextRec(GetFontDefault(), TextFormat("You won!"), scoreBox, 100, 10, 0, uicolor);
            }
        }

        // Events
        {
            for (std::vector<Event>::iterator event = gameState.events.begin(); event != gameState.events.end(); ++event)
            {
                if (time > event->time && !event->flag)
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
                    event->flag = !event->flag;
                }
            }
        }
    }
    CloseWindow();

    return 0;
}
