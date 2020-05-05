#pragma once
#define internal static
#define global_variable static
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


typedef struct Bullets
{
    Rectangle *list;
    int maxAmount;
    int index;
    Color color;
} Bullets;

internal Bullets GetBullets(int size)
{
    Bullets bullets = {0};
    bullets.index = -1;
    bullets.maxAmount = size;
    bullets.color = (Color){150, 0, 50, 255};
    bullets.list = (Rectangle*)calloc(size, sizeof(Rectangle));

    return bullets;
}

internal void PushBullet(Bullets *bullets, int x, int y, int width, int height)
{
    bullets->index++;
    if(bullets->index == bullets->maxAmount)
    {
        bullets->index = 0;
    }
    int index = bullets->index;

    Rectangle bullet = {x, y, width, height};
    bullets->list[index] = bullet;
}

internal void clampFloat(float *val, float min, float max)
{
    if (*val > max) *val = max;
    else if (*val < min) *val = min;
}

internal void clampInt(int *val, int min, int max)
{
    if (*val > max) *val = max;
    else if (*val < min) *val = min;
}
