@echo off

set LIBS=/link ..\lib\raylib.lib kernel32.lib user32.lib shell32.lib winmm.lib gdi32.lib opengl32.lib
set FLAGS=/Od /Zi /nologo /std:c++17 /EHsc
set EXE_NAME=/Fe: "ShootEmUp"

cd bin
del *.pdb > NUL 2> NUL
cl %FLAGS% -LD ..\src\game.cpp %LIBS% -incremental:no -opt:ref /EXPORT:UpdateAndRender
cl ..\src\main.cpp %EXE_NAME% %FLAGS% %LIBS%
cd ..