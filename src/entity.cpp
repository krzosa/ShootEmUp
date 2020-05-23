#include "include/raylib.h"
#include "vector_math.h"
#include <list>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

enum EntityType
{
    ENEMY,
    PLAYER,
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

static Entity CreatePlayer()
{
    Entity player = {};
    {
        player.pos = {600, 600};
        player.size = {25, 25};
        player.color = {0, 160, 200, 255};
        player.speed = 7000.0f;
        player.type = PLAYER;
        player.state = ALIVE;
    }
    return player;
}

static Entity CreateBullet(float posx, float posy, EntityType type)
{
    unsigned char colorB = 120;
    unsigned char colorG = 150;

    Entity bullet = {};
    {
        bullet.pos.x = posx;
        bullet.pos.y = posy;
        bullet.acceleration = {0, -1};
        bullet.color = {0, colorG, colorB, 255};
        bullet.size = {10, 10};
        bullet.speed = 10000;
        bullet.type = type;
    }
    return bullet;
}

static void CreateEnemyRandomized(std::list<Entity> *enemies)
{
    float posx = GetRandomValue(-200, SCREEN_WIDTH + 200);
    float speed = GetRandomValue(1000, 2000);
    unsigned char color = GetRandomValue(50, 150);
    float accelerationX = GetRandomValue(-1, 1);

    Entity enemy = {};
    {
        enemy.size = {(float)GetRandomValue(50, 100), (float)GetRandomValue(25, 50)};
        enemy.color = {200, color, 0, 255};
        enemy.acceleration = {accelerationX, 1};
        enemy.speed = speed;
        enemy.pos = {posx, -enemy.size.y};
        enemy.type = ENEMY;
    }

    enemies->push_front(enemy);
}

static bool EntityIsOnScreen(Entity *entity, int screenWidth, int screenHeight)
{
    int padding = 200;

    if (entity->pos.x > screenWidth + padding || entity->pos.x < 0 - padding)
        return false;
    if (entity->pos.y > screenHeight + padding || entity->pos.y < 0 - padding)
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

static void EntityUpdate(Entity *entity, std::list<Entity> *bullets)
{
    if (entity->type == PLAYER)
    {
        Entity *player = entity;

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
            bullets->push_back(CreateBullet(player->pos.x + player->size.x / 2, player->pos.y, PLAYER_BULLET));
        }
    }
    else
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
}

static void EntitiesUpdate(std::list<Entity> *entities, std::list<Entity> *bullets)
{
    Entity *player = &*entities->begin();
    for (std::list<Entity>::iterator entity = entities->begin(); entity != entities->end(); ++entity)
    {
        EntityUpdate(&*entity, bullets);

        if (!EntityIsOnScreen(&*entity, SCREEN_WIDTH, SCREEN_HEIGHT))
        {
            entities->erase(entity);
        }

        if (EntitiesCollide(*entity, *player))
        {
            player->state = DEAD;
        }
    }

    for (std::list<Entity>::iterator bullet = bullets->begin(); bullet != bullets->end(); ++bullet)
    {
        EntityUpdate(&*bullet, bullets);
        bool erase = false;

        // delete out of bounds bullets
        if (!EntityIsOnScreen(&*bullet, SCREEN_WIDTH, SCREEN_HEIGHT))
        {
            erase = true;
        }

        // colliding with enemies
        for (std::list<Entity>::iterator entity = entities->begin(); entity != entities->end(); ++entity)
        {
            if (EntitiesCollide(*bullet, *entity))
            {
                entities->erase(entity);
                erase = true;
            }
        }

        if (erase == true)
            bullets->erase(bullet);
    }
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
