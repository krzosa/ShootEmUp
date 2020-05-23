@echo off

set LIBS=/link ..\lib\raylib.lib kernel32.lib user32.lib shell32.lib winmm.lib gdi32.lib opengl32.lib
set FLAGS=/Od /Zi /nologo /std:c++17 /EHsc
set EXE_NAME=/Fe: "Game"

cd bin
cl ..\src\main.cpp %EXE_NAME% %FLAGS% %LIBS%
cd ..