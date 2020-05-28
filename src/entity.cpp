internal Entity EntityPlayerCreate()
{
    Entity player = {};
    {
        player.pos = config::player_position;
        player.size = config::player_size;
        player.color = config::player_color;
        player.speed = config::player_speed_multiplier;
        player.type = PLAYER;
        player.state = ALIVE;
    }
    return player;
}

internal Entity EntityBulletCreate(Vector2 pos)
{
    unsigned char colorB = 120;
    unsigned char colorG = 150;

    Entity bullet = {};
    {
        bullet.pos = pos;
        bullet.acceleration = {0, -1};
        bullet.color = config::bullet_color;
        bullet.size = config::bullet_size;
        bullet.speed = config::bullet_speed_multiplier;
        bullet.state = ALIVE;
        bullet.type = PLAYER_BULLET;
    }
    return bullet;
}

internal void EntityEnemyCreate(std::list<Entity> *enemies)
{
    Entity enemy = {};
    {
        enemy.size = {(float)GetRandomValue(50, 100), (float)GetRandomValue(25, 50)};
        enemy.color = {200, (uchar)GetRandomValue(50, 150), 0, 255};
        enemy.acceleration = {(float)GetRandomValue(-1, 1), 1};
        enemy.speed = 2000;
        enemy.pos = {50, -enemy.size.y - 50};
        enemy.type = ENEMY;
    }

    enemies->push_front(enemy);
}

internal void EntityEnemyCreateRandomized(std::list<Entity> *enemies, float difficulty)
{
    Entity enemy = {};
    {
        enemy.size = {(float)GetRandomValue(50, 100), (float)GetRandomValue(25, 50)};
        enemy.color = {200, (uchar)GetRandomValue(50, 150), 0, 255};
        enemy.acceleration = {(float)GetRandomValue(-1, 1), 1};
        enemy.speed = (float)GetRandomValue(1000, 2000 * difficulty);
        enemy.pos = {(float)GetRandomValue(-200, SCREEN_WIDTH + 200), -enemy.size.y - 50};
        enemy.type = ENEMY;
    }

    enemies->push_front(enemy);
}

internal void EntitiesDeathParticlesCreate(std::list<Entity> *list, int posx, int posy)
{
    for (int i = 1; i < 11; i++)
    {
        float accelerationy = -1 * i;
        if (i % 2)
        {
            accelerationy = 1 * i;
        }
        float accelerationx = GetRandomValue(-1, 1) * i;
        Entity bullet = {};
        {
            bullet.pos.x = posx;
            bullet.pos.y = posy;
            bullet.acceleration = {accelerationx, accelerationy};
            bullet.color = {200, 100, 0, 255};
            bullet.size = {(float)GetRandomValue(5, 25), (float)GetRandomValue(5, 25)};
            bullet.speed = 7000.0f;
            bullet.state = ALIVE;
            bullet.type = PLAYER_BULLET;
        }
        list->push_back(bullet);
    }
}

internal bool EntityIsOnScreen(Entity *entity, int screenWidth, int screenHeight)
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

internal void EntityUpdate(Entity *entity, std::list<Entity> *bullets)
{
    float dtime = GetFrameTime();
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
        if (IsKeyDown(KEY_O))
        {
            bullets->push_back(EntityBulletCreate({player->pos.x + player->size.x / 2, player->pos.y}));
        }
        player->acceleration = player->acceleration * player->speed;

        player->acceleration = player->acceleration - player->velocity * 8;

        player->pos = player->acceleration * (dtime * dtime) + player->velocity * dtime + player->pos;
        player->velocity = player->acceleration * dtime + player->velocity;

        player->pos.x = clampFloat(player->pos.x, 0, SCREEN_WIDTH - player->size.x);
        player->pos.y = clampFloat(player->pos.y, 0, SCREEN_HEIGHT - player->size.y);
    }
    else
    {
        if (entity->pos.x < 0)
            entity->acceleration.x = 1;
        if (entity->pos.x > SCREEN_WIDTH - entity->size.x)
            entity->acceleration.x = -1;
        Vector2 acceleration = entity->acceleration * entity->speed;

        acceleration = acceleration - entity->velocity * 4;

        entity->pos = acceleration * (dtime * dtime) + entity->velocity * dtime + entity->pos;
        entity->velocity = acceleration * dtime + entity->velocity;
    }
}

internal void EntityDraw(Entity *entity)
{
    DrawRectangle(entity->pos.x, entity->pos.y, entity->size.x, entity->size.y, entity->color);
}

internal void EntityListDraw(std::list<Entity> *entities)
{
    for (std::list<Entity>::iterator it = entities->begin(); it != entities->end(); ++it)
    {
        EntityDraw(&*it);
    }
}
