#pragma once
#include "main.h"
/* TODO: delete this */
#include "include/raylib.h"

typedef struct Dash
{
    bool flag;
    int frameDuration;
    float speed;
    float timer;
} Dash;

typedef struct Player
{
    Rectangle hitbox;
    float speed;
    float shootDelay;
    float shootTimer;
    Bullets bullets;
    Dash dash;

} Player;

typedef struct Input
{
    bool a;
    bool b;
    Vector2 move;
    Vector2 axis;
} Input;


internal void updatePlayer(Input *input, Player *player, float dtime)
{
    /* DASH */
    float dash = 1;
    
    // Start timer if dashed
    if(player->dash.flag != 0) player->dash.timer += dtime;

    int dashFrames = player->dash.frameDuration;
    if(input->b == 1 && player->dash.flag < dashFrames || 
        player->dash.flag < dashFrames && player->dash.flag != 0)
    {
        dash = player->dash.speed;
        player->dash.timer = 0;
        player->dash.flag++;
    }

    /* Reset flag to 0 when dash timer reaches 0.2 */
    if(player->dash.flag != 0 && player->dash.timer > 0.2f && input->b != 1)
    {
        player->dash.timer = 0;
        player->dash.flag = 0;
    }

    /* MOVEMENT */
    float multiplier = player->speed * dash * (dtime*100);

    player->hitbox.x += (int)(input->axis.x * multiplier);
    player->hitbox.y += (int)(input->axis.y * multiplier);

    player->hitbox.y += (int)(input->move.y * multiplier);
    player->hitbox.x += (int)(input->move.x * multiplier);

    // making sure that player wont go out of bounds
    clampFloat(&player->hitbox.x, 0, SCREEN_WIDTH - player->hitbox.width);
    clampFloat(&player->hitbox.y, 0, SCREEN_HEIGHT - player->hitbox.height);

    // SHOOTING
    if(input->a == 1 && player->shootTimer <= 0)
    {
        PushBullet(&player->bullets, player->hitbox.x, player->hitbox.y, 10, 10);
        player->shootTimer = player->shootDelay;
    }
    if (player->shootTimer > 0) player->shootTimer -= dtime;

    // BULLET UPDATE ---------------------------------------------
    for(int i = 0; i < player->bullets.maxAmount; i++)
    {
        #define outofboundsoffset 50
        if(player->bullets.list[i].y > 0 - outofboundsoffset)
            player->bullets.list[i].y -= 7;
    }

}


internal void handleInput(Input *input)
{
    if(IsKeyPressed(KEY_UP))
    {
        input->move.y += -1;
    }
    else if(IsKeyPressed(KEY_DOWN))
    {
        input->move.y += 1;
    }
    if(IsKeyPressed(KEY_LEFT))
    {
        input->move.x += -1;
    }
    else if(IsKeyPressed(KEY_RIGHT))
    {
        input->move.x += 1;
    }

    if(IsKeyPressed(KEY_W))
    {
        input->move.y += -1;
    }
    else if(IsKeyPressed(KEY_S))
    {
        input->move.y += 1;
    }
    if(IsKeyPressed(KEY_A))
    {
        input->move.x += -1;
    }
    else if(IsKeyPressed(KEY_D))
    {
        input->move.x += 1;
    }

    if(IsKeyPressed(KEY_O))
    {
        input->a = 1;
    }
    if(IsKeyPressed(KEY_P))
    {
        input->b = 1;
    }

    if(IsKeyReleased(KEY_UP))
    {
        input->move.y += 1;
    }
    else if(IsKeyReleased(KEY_DOWN))
    {
        input->move.y += -1;
    }
    if(IsKeyReleased(KEY_LEFT))
    {
        input->move.x += 1;
    }
    else if(IsKeyReleased(KEY_RIGHT))
    {
        input->move.x += -1;
    }

    if(IsKeyReleased(KEY_W))
    {
        input->move.y += 1;
    }
    else if(IsKeyReleased(KEY_S))
    {
        input->move.y += -1;
    }
    if(IsKeyReleased(KEY_A))
    {
        input->move.x += 1;
    }
    else if(IsKeyReleased(KEY_D))
    {
        input->move.x += -1;
    }

    if(IsKeyReleased(KEY_O))
    {
        input->a = 0;
    }
    if(IsKeyReleased(KEY_P))
    {
        input->b = 0;
    }
    if(IsGamepadAvailable(0))
    {
        if(IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
        {
            input->a = 1;
        }
        if(IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_1))
        {
            input->b = 1;
        }
        if(IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
        {
            input->a = 0;
        }
        if(IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_1))
        {
            input->b = 0;
        }
        input->axis.x = GetGamepadAxisMovement(0, 1);
        if(input->axis.x < 0.2 && input->axis.x > -0.2) input->axis.x = 0;
        input->axis.y = GetGamepadAxisMovement(0, 2);
        if(input->axis.y < 0.2 && input->axis.y > -0.2) input->axis.y = 0;
    }
}