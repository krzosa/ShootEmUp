#include <vector>
#include "game.h"

namespace config
{

    constexpr static Vector2 player_position = {600, 600};
    constexpr static Vector2 player_size = {25, 25};
    constexpr static Color player_color = {0, 160, 200, 255};
    constexpr static float player_speed_multiplier = 8000;

    constexpr static Vector2 bullet_size = {10, 10};
    constexpr static Color bullet_color = {0, 120, 150, 255};
    constexpr static float bullet_speed_multiplier = 50000;

    constexpr static float time_to_win = 59.0f;

} // namespace config

internal void InitEvents(std::vector<Event> *events)
{
    double startTime = GetTime();
    /* {time, enemiesToSpawn, difficulty}*/
    for (int i = 2; i < 13; i = i + 2)
        events->push_back({(double)i + startTime, 7, 1.0f});
    for (int i = 13; i < 20; i = i + 2)
        events->push_back({(double)i + startTime, 12, 1.0f});
    for (int i = 20; i < 34; i = i + 2)
        events->push_back({(double)i + startTime, 12, 1.4f});
    for (int i = 34; i < 40; i++)
        events->push_back({(double)i + startTime, 12, 1.4f});
    for (int i = 40; i < 46; i++)
        events->push_back({(double)i + startTime, 16, 1.4f});
    for (int i = 47; i < 56; i++)
        events->push_back({(double)i + startTime, 16, 1.7f});
}
