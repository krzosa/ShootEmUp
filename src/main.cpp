#include "include/raylib.h"
#include "vector_math.h"
#include "entity.cpp"
#include <math.h>
#include <list>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

static void SpawnEnemy(std::list<Entity> *enemies)
{
    float posx = GetRandomValue(-200, SCREEN_WIDTH + 200);
    float speed = GetRandomValue(1000, 2000);

    Entity enemy = {};
    {
        enemy.size = {(float)GetRandomValue(50, 100), (float)GetRandomValue(25, 50)};
        enemy.color = {200, (unsigned char)GetRandomValue(50, 150), 0, 255};
        enemy.acceleration = {-1, 1};
        enemy.speed = speed;
        enemy.pos = {posx, 0};
    }

    enemies->push_front(enemy);
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Shoot'em up");
    SetTargetFPS(60);

    Entity player = {};
    {
        player.pos = {100, 100};
        player.size = {25, 25};
        player.color = {0, 150, 150, 255};
        player.speed = 7000.0f;
    }

    std::list<Entity> bullets = {};
    std::list<Entity> enemies = {};

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_R))
        {

            SpawnEnemy(&enemies);
            SpawnEnemy(&enemies);
            SpawnEnemy(&enemies);
            SpawnEnemy(&enemies);
            SpawnEnemy(&enemies);
            SpawnEnemy(&enemies);
            SpawnEnemy(&enemies);
            SpawnEnemy(&enemies);
            SpawnEnemy(&enemies);
        }

        if (player.state == ALIVE)
            PlayerUpdate(&player, &bullets);

        for (std::list<Entity>::iterator enemy = enemies.begin(); enemy != enemies.end(); ++enemy)
        {
            EntityUpdate(&*enemy);

            if (EntitiesCollide(*enemy, player))
            {
                player.state = DEAD;
            }
        }

        for (std::list<Entity>::iterator bullet = bullets.begin(); bullet != bullets.end(); ++bullet)
        {
            EntityUpdate(&*bullet);

            // delete out of bounds bullets
            if (!EntityIsOnScreen(&*bullet, SCREEN_WIDTH, SCREEN_HEIGHT))
            {
                bullets.erase(bullet);
            }

            // colliding with enemies
            for (std::list<Entity>::iterator enemy = enemies.begin(); enemy != enemies.end(); ++enemy)
            {
                if (EntitiesCollide(*bullet, *enemy))
                {
                    // TODO: Heap corruption I think this invalidates all the pointers
                    // it had, so we need to connect them to something else? or maybe use vectors
                    // dont know
                    bullets.erase(bullet);
                    enemies.erase(enemy);
                }
            }
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            if (player.state == ALIVE)
                EntityDraw(&player);
            EntityListDraw(&bullets);
            EntityListDraw(&enemies);
            // TraceLog(LOG_INFO, "cap: %d", bullets.size());

            DrawFPS(0, 0);
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
