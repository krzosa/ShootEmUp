#include "include/raylib.h"
#include "game.h"
/* WIN32 */
#define _X86_
#include "libloaderapi.h"
#include "WinDef.h"
#include "winbase.h"
/*  */

#define UPDATE_AND_RENDER(name) void name(GameState *gameState)
typedef UPDATE_AND_RENDER(UpdateAndRender);
UPDATE_AND_RENDER(UpdateAndRenderStub)
{
}

struct win32_game_code
{
    HMODULE library;
    long lastDllWriteTime;
    UpdateAndRender *UpdateAndRender;
    bool isValid;
};

static win32_game_code Win32LoadGameCode(char *mainDllPath, char *tempDllPath)
{
    win32_game_code Result;
    Result.lastDllWriteTime = GetFileModTime(tempDllPath);

    CopyFileA((LPCSTR)mainDllPath, (LPCSTR)tempDllPath, FALSE);
    Result.library = LoadLibraryA(tempDllPath);
    TraceLog(LOG_INFO, "Load game code");
    Result.isValid = 1;
    if (Result.library)
    {
        Result.UpdateAndRender = (UpdateAndRender *)
            GetProcAddress(Result.library, "UpdateAndRender");

        Result.isValid = (Result.UpdateAndRender != 0);
    }

    if (Result.isValid == 0)
    {
        Result.UpdateAndRender = UpdateAndRenderStub;
        TraceLog(LOG_INFO, "Failed to load library");
    }

    TraceLog(LOG_INFO, "GameCode valid? = %d", Result.isValid);
    return Result;
}

/* Unloads the dll and nulls the pointers to functions from the dll */
static void Win32UnloadGameCode(win32_game_code *GameCode)
{
    if (GameCode->library)
    {
        FreeLibrary(GameCode->library);
        GameCode->library = 0;
        GameCode->UpdateAndRender = UpdateAndRenderStub;
        TraceLog(LOG_INFO, "Unload game code");
    }

    GameCode->isValid = false;
}

int main(void)
{
    /* building paths to the dll files */
    const char *basePath = GetWorkingDirectory();
    char mainDllPath[MAX_PATH];
    char tempDllPath[MAX_PATH];
    {
        strcpy_s(mainDllPath, basePath);
        strcpy_s(tempDllPath, basePath);
        strcat_s(mainDllPath, "\\game.dll");
        strcat_s(tempDllPath, "\\game_temp.dll");
        TraceLog(LOG_INFO, basePath);
        TraceLog(LOG_INFO, mainDllPath);
        TraceLog(LOG_INFO, tempDllPath);
    }

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game");

    win32_game_code gameCode = {};
    gameCode = Win32LoadGameCode(mainDllPath, tempDllPath);

    GameState gameState = {};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        long dllFileWriteTime = GetFileModTime(mainDllPath);
        if (dllFileWriteTime != gameCode.lastDllWriteTime)
        {
            Win32UnloadGameCode(&gameCode);
            gameCode = Win32LoadGameCode(mainDllPath, tempDllPath);
        }

        gameCode.UpdateAndRender(&gameState);
    }
    CloseWindow();

    return 0;
}
