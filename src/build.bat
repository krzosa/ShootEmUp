@echo off

set LIBS=/link ..\lib\raylib.lib kernel32.lib user32.lib shell32.lib winmm.lib gdi32.lib opengl32.lib
set FLAGS=/Od /Zi /DEBUG /nologo
set EXE_NAME=/Fe: "Game"

cd ..\bin
cl ..\src\main.c %EXE_NAME% %FLAGS% %LIBS%
cd ..\src