#include "include/raylib.h"
#include "vector_math.h"

struct Entity
{
    Vector2 pos;
    Vector2 size;
    Vector2 velocity;
    Vector2 acceleration;
    float speed;
    Color color;
};

static void PlayerUpdate(Entity *player)
{
    player->acceleration = {0,0};
    if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        player->acceleration.y = -1.0f;
    }   
    else if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        player->acceleration.y = 1.0f;
    }
    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        player->acceleration.x = -1.0f;
    }   
    else if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        player->acceleration.x = 1.0f;
    }
    player->acceleration = player->acceleration * player->speed;
    float dtime = GetFrameTime();

    player->acceleration = player->acceleration - player->velocity * 8;

    player->pos = player->acceleration * (dtime*dtime) + player->velocity * dtime + player->pos;
    player->velocity = player->acceleration * dtime + player->velocity;

}

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Shoot'em up");
    SetTargetFPS(60);

    Entity player = {};
    {
        player.pos = {100,100};
        player.size = {100,100};
        player.color =  {0,150,150,255};
        player.speed = 7000.0f;
    }

    Entity enemy = {};
    {
        enemy = {200,200};

    }

    while (!WindowShouldClose()) 
    {
        PlayerUpdate(&player);

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawRectangle(player.pos.x, player.pos.y, player.size.x, player.size.y, player.color);
        }
        EndDrawing();
    }
    CloseWindow();     

    return 0;
}
