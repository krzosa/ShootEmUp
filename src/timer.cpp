static double global_timer;
static double renderTime = 0;
static double updateTime = 0;

static void StartTimer()
{
    global_timer = GetTime();
}

static void StopTimerGui(float y, char *text)
{
    GuiLabel({1100, y, 100, 100}, TextFormat("%s time: %f", text, (GetTime() - global_timer)));
}

static double StopTimer()
{
    return GetTime() - global_timer;
}
