#include "include/raylib.h"
#include "vector_math.h"
#include <list>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

enum EntityType
{
    PLAYER,
    ENEMY,
    PLAYER_BULLET,
    ENEMY_BULLET,
};

enum EntityState
{
    ALIVE,
    DEAD,
};

struct Entity
{
    Vector2 pos;
    Vector2 size;
    Vector2 velocity;
    Vector2 acceleration;
    EntityType type;
    EntityState state;
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
        bullet.pos.x = player->pos.x + player->size.x / 2;
        bullet.pos.y = player->pos.y;
        bullet.acceleration = {0, -1};
        bullet.color = {255, 0, 0, 255};
        bullet.size = {10, 10};
        bullet.speed = 10000;
        bullets->push_back(bullet);
    }
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
    if (entity->pos.x < 0)
        entity->acceleration.x = 1;
    if (entity->pos.x > SCREEN_WIDTH - entity->size.x)
        entity->acceleration.x = -1;
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