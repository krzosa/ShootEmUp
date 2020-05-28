#pragma once
#include <vector>
#include <list>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
typedef uint32_t uint32;
typedef unsigned char uchar;
// function internal to the file, obj
#define internal static

enum Direction
{
    UP,
    DOWN,
    RIGHT,
    LEFT
};

struct Event
{
    double time;
    int enemiesToSpawn;
    float difficulty;
    bool happened;
};

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
    Color color;
    float speed;
    EntityType type;
    EntityState state;
};

struct GameState
{
    std::vector<Event> events;
    std::list<Entity> bullets;
    std::list<Entity> enemies;
    Entity player;

    double timeToWin;
    int score;
    int highScore;

    Color uiColor;
    bool uiClosed;
    bool initialized;
};