#include "include/raylib.h"
#include "vector_math.h"
#include <math.h>
#include <list>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

enum EntityType
{
    Player,
    Enemy,
    PlayersBullet,
    EnemyBullet,
};

struct Entity
{
    Vector2 pos;
    Vector2 size;
    Vector2 velocity;
    Vector2 acceleration;
    EntityType type;
    float speed;
    Color color;
};

static void PlayerUpdate(Entity *player, std::list<Entity> *bullets)
{
    player->acceleration = {0, 0};
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        player->acceleration.y = -1.0f;
    }
    else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        player->acceleration.y = 1.0f;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        player->acceleration.x = -1.0f;
    }
    else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        player->acceleration.x = 1.0f;
    }
    player->acceleration = player->acceleration * player->speed;
    float dtime = GetFrameTime();

    player->acceleration = player->acceleration - player->velocity * 8;

    player->pos = player->acceleration * (dtime * dtime) + player->velocity * dtime + player->pos;
    player->velocity = player->acceleration * dtime + player->velocity;

    player->pos.x = clampFloat(player->pos.x, 0, SCREEN_WIDTH - player->size.x);
    player->pos.y = clampFloat(player->pos.y, 0, SCREEN_HEIGHT - player->size.y);

    if (IsKeyDown(KEY_O))
    {
        Entity bullet = {};
        bullet.pos = player->pos;
        bullet.acceleration = {0, -1};
        bullet.color = {255, 0, 0, 255};
        bullet.size = {10, 10};
        bullet.speed = 1000;
        bullets->push_back(bullet);
    }
}

static void EnemyUpdate(Entity *enemy)
{
    if (enemy->pos.x < 0)
        enemy->acceleration = {1, 0};
    if (enemy->pos.x > SCREEN_WIDTH - enemy->size.x)
        enemy->acceleration = {-1, 0};
    Vector2 acceleration = enemy->acceleration * enemy->speed;
    float dtime = GetFrameTime();

    acceleration = acceleration - enemy->velocity * 4;

    enemy->pos = acceleration * (dtime * dtime) + enemy->velocity * dtime + enemy->pos;
    enemy->velocity = acceleration * dtime + enemy->velocity;
}

static bool EntityIsOnScreen(Entity *entity, int screenWidth, int screenHeight)
{
    if (entity->pos.x > screenWidth || entity->pos.x < 0)
        return false;
    if (entity->pos.y > screenHeight || entity->pos.y < 0)
        return false;
    return true;
}

bool EntitiesCollide(Entity entity1, Entity entity2)
{
    bool collision = false;

    if ((entity1.pos.x < (entity2.pos.x + entity2.size.x) && (entity1.pos.x + entity1.size.x) > entity2.pos.x) &&
        (entity1.pos.y < (entity2.pos.y + entity2.size.y) && (entity1.pos.y + entity1.size.y) > entity2.pos.y))
        collision = true;

    return collision;
}

static void EntityUpdate(Entity *entity)
{
    Vector2 acceleration = entity->acceleration * entity->speed;
    float dtime = GetFrameTime();

    acceleration = acceleration - entity->velocity * 4;

    entity->pos = acceleration * (dtime * dtime) + entity->velocity * dtime + entity->pos;
    entity->velocity = acceleration * dtime + entity->velocity;
}

static void EntityDraw(Entity *entity)
{
    DrawRectangle(entity->pos.x, entity->pos.y, entity->size.x, entity->size.y, entity->color);
}

static void EntityListDraw(std::list<Entity> *entities)
{
    for (std::list<Entity>::iterator it = entities->begin(); it != entities->end(); ++it)
    {
        EntityDraw(&*it);
    }
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Shoot'em up");
    SetTargetFPS(60);

    Entity player = {};
    {
        player.pos = {100, 100};
        player.size = {100, 100};
        player.color = {0, 150, 150, 255};
        player.speed = 7000.0f;
    }

    Entity enemy = {};
    {
        enemy.pos = {200, 200};
        enemy.size = {100, 100};
        enemy.color = {150, 150, 0, 255};
        enemy.acceleration.x = -1;
        enemy.speed = 2000.0f;
    }

    std::list<Entity> bullets = {};
    std::list<Entity> enemies = {};
    enemies.push_front(enemy);

    while (!WindowShouldClose())
    {
        PlayerUpdate(&player, &bullets);

        for (std::list<Entity>::iterator bullet = enemies.begin(); bullet != enemies.end(); ++bullet)
        {
            EnemyUpdate(&*bullet);
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
                    bullets.erase(bullet);
                }
            }
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

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
