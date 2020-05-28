static double global_timer;
static double renderTime = 0;
static double updateTime = 0;

internal void StartTimer()
{
    global_timer = GetTime();
}

internal void StopTimerGui(float y, char *text)
{
    GuiLabel({1100, y, 100, 100}, TextFormat("%s time: %f", text, (GetTime() - global_timer)));
}

internal double StopTimer()
{
    return GetTime() - global_timer;
}
