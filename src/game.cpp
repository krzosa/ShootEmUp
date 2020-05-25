#include "include/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"
#include "vector_math.h"
#include "game.h"
#include "entity.cpp"

void UpdateAndRender(GameState *gameState)
{
    if (!gameState->initialized)
    {
        for (int i = 0; i < 20; i++)
        {
            gameState->events.push_back({(double)i, 5});
        }
        gameState->timeToWin = 23.0f;
        gameState->player = CreatePlayer();
        gameState->bullets = {};
        gameState->enemies = {};
        gameState->uicolor = {160, 160, 160, 200};
        gameState->initialized = 1;
        gameState->uiClosed = 0;
    }

    double time = GetTime();

    if (gameState->player.state != DEAD)
        EntityUpdate(&gameState->player, &gameState->bullets);

    // update enemies, check collisions with gameState->player, check if on screen
    for (std::list<Entity>::iterator enemy = gameState->enemies.begin(); enemy != gameState->enemies.end(); ++enemy)
    {
        EntityUpdate(&*enemy, &gameState->bullets);

        if (!EntityIsOnScreen(&*enemy, SCREEN_WIDTH, SCREEN_HEIGHT))
        {
            enemy->state = DEAD;
        }

        if (EntitiesCollide(*enemy, gameState->player) && gameState->player.state != DEAD)
        {
            gameState->player.state = DEAD;
            CreateDeathParticles(&gameState->bullets, gameState->player.pos.x - gameState->player.size.x, gameState->player.pos.y - gameState->player.size.y);
        }
        if (enemy->state == DEAD)
            gameState->enemies.erase(enemy);
    }

    // update gameState->enemies, check if out of bounds, check if collides with gameState->enemies
    for (std::list<Entity>::iterator bullet = gameState->bullets.begin(); bullet != gameState->bullets.end(); ++bullet)
    {
        EntityUpdate(&*bullet, &gameState->bullets);

        // delete out of bounds gameState->bullets
        if (!EntityIsOnScreen(&*bullet, SCREEN_WIDTH, SCREEN_HEIGHT))
        {
            bullet->state = DEAD;
        }

        // colliding with gameState->enemies
        for (std::list<Entity>::iterator enemy = gameState->enemies.begin(); enemy != gameState->enemies.end(); ++enemy)
        {
            if (EntitiesCollide(*bullet, *enemy))
            {
                gameState->enemies.erase(enemy);
                bullet->state = DEAD;
                gameState->score += 10;
            }
        }

        if (bullet->state == DEAD)
            gameState->bullets.erase(bullet);
    }

    // TraceLog(LOG_TRACE, "gameState->enemies size: %d", gameState->enemies.size());
    // TraceLog(LOG_TRACE, "gameState->bullets size: %d", gameState->bullets.size());

    BeginDrawing();
    {
        ClearBackground(RAYWHITE);

        if (gameState->player.state != DEAD)
            EntityDraw(&gameState->player);
        EntityListDraw(&gameState->bullets);
        EntityListDraw(&gameState->enemies);

        Rectangle scoreBox = {SCREEN_WIDTH / 2 - 50, 0, 300, 100};
        DrawTextRec(GetFontDefault(), TextFormat("%d", gameState->score), scoreBox, 100, 10, 0, gameState->uicolor);

        if (gameState->showFps)
        {
            DrawFPS(0, 0);
        }

        if (gameState->player.state == DEAD)
        {
            Rectangle scoreBox = {SCREEN_WIDTH / 2 - 500, 600, 1000, 100};
            DrawTextRec(GetFontDefault(), TextFormat("R to restart"), scoreBox, 100, 10, 0, gameState->uicolor);
        }
    }
    EndDrawing();

    // Debug Inputs
    {
        if (IsKeyPressed(KEY_F1))
        {
            gameState->uiClosed = !gameState->uiClosed;
        }
        if (IsKeyDown(KEY_ONE))
        {
            for (int i = 0; i < 10; i++)
            {
                CreateEnemyRandomized(&gameState->enemies);
            }
        }
        if (IsKeyDown(KEY_R))
        {
            gameState->player.state = ALIVE;
        }
    }

    // Events
    {
        if (time > gameState->timeToWin && gameState->player.state != DEAD)
        {
            Rectangle scoreBox = {SCREEN_WIDTH / 2 - 500, 600, 1000, 100};
            DrawTextRec(GetFontDefault(), TextFormat("You won!"), scoreBox, 100, 10, 0, gameState->uicolor);
        }

        for (std::vector<Event>::iterator event = gameState->events.begin(); event != gameState->events.end(); ++event)
        {
            if (time > event->time && !event->flag)
            {
                for (int i = 0; i < event->enemiesToSpawn; i++)
                {
                    CreateEnemyRandomized(&gameState->enemies);
                }
                event->flag = !event->flag;
            }
        }
    }

    if (!gameState->uiClosed)
    {
        gameState->uiClosed = GuiWindowBox({0, 0, 100, 100}, "DEBUG");
        GuiLabel({0, 0, 100, 100}, TextFormat("FPS: %d", GetFPS()));
        GuiLabel({0, 10, 100, 100}, TextFormat("projectiles: %d", gameState->bullets.size()));
        GuiLabel({0, 20, 100, 100}, TextFormat("enemies: %d", gameState->enemies.size()));
    }
}