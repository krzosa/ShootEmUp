#include "include/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"

#include "main.h"
#include "player.c"

/* IDEA: 
    - shmup in which you can become invisible and have some kind of bonus for it
        but ofc you cant see yourself */

/* SHMUP TODO:
    - some kind of enemy 
    - scrolling maps
    - some kind of format to read maps and write maps to */

typedef struct GuiProperties
{
    Rectangle frame;
    bool isOpen;
    bool isDragged;
} GuiProperties;

internal void updateGui(GuiProperties* gui, Vector2 mousePosition)
{
    // TODO: Add mouse position offset
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(mousePosition, gui->frame))
        {
            gui->isDragged = true;
        }
    }
    if(IsKeyPressed(KEY_F1))
    {
        gui->isOpen = !gui->isOpen;
    }

    if (gui->isDragged)
    {            
        gui->frame.x = (mousePosition.x);
        gui->frame.y = (mousePosition.y);
        
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) gui->isDragged = false;
    }
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "title");
    SetTargetFPS(60);
    GuiProperties gui = {0};
    gui.frame.width = 200;
    gui.frame.height = 200;

    Vector2 mousePosition = { 0 };
    float dtime = 0;
    Input input = {0};
    
    Player player = {{100, 500, 25, 25}};
    player.speed = 5.0f;
    player.dash.speed = 2.5f;
    player.dash.frameDuration = 12;
    player.shootDelay = 0.1f;
    player.shootTimer = 0;
    player.bullets = GetBullets(50);

    while(!WindowShouldClose())
    {
        dtime = GetFrameTime();
        mousePosition = GetMousePosition();

        handleInput(&input);
        updatePlayer(&input, &player, dtime);
        
        updateGui(&gui, mousePosition);
        BeginDrawing();

            ClearBackground((Color){0, 0, 0, 255});

            // Draw GUI ----------------------------------------------
            if(!gui.isOpen)
            {
                GuiPanel(gui.frame);
                DrawText(TextFormat("FPS: %2i", GetFPS()), gui.frame.x, gui.frame.y, 10, DARKGRAY);
            }

            DrawRectangle(player.hitbox.x, player.hitbox.y, player.hitbox.width, player.hitbox.height, (Color){150, 0, 150, 255});
            Rectangle *bullet;
            for(int i = 0; i < player.bullets.maxAmount; i++)
            {
                bullet = &player.bullets.list[i];
                DrawRectangle(bullet->x, bullet->y, bullet->width, bullet->height, player.bullets.color);
            }


            

        EndDrawing();

    }
    CloseWindow();
}