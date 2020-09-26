#!/bin/bash 2>nul || goto :windows

# linux ------------------------------------------------------------------------
cd `dirname $0`

sudo apt-get install xorg-dev libglfw3-dev libassimp-dev clang ffmpeg

clang   3rd/3rd_tools/ass2iqe.c   -I. -g -w -lm -ldl -lpthread -lassimp -o 3rd/3rd_tools/ass2iqe
clang++ 3rd/3rd_tools/iqe2iqm.cpp -I. -g -w -lm -ldl -lpthread          -o 3rd/3rd_tools/iqe2iqm
clang   3rd/3rd_tools/mid2wav.c   -I. -g -w -lm -ldl -lpthread          -o 3rd/3rd_tools/mid2wav
clang   3rd/3rd_tools/xml2json.c  -I. -g -w -lm -ldl -lpthread          -o 3rd/3rd_tools/xml2json

clang test.c         -g -w -lm -ldl -lpthread -o test
clang test_cubemap.c -g -w -lm -ldl -lpthread -o test_cubemap
clang test_collide.c -g -w -lm -ldl -lpthread -o test_collide
clang test_model.c   -g -w -lm -ldl -lpthread -o test_model
clang test_scene.c   -g -w -lm -ldl -lpthread -o test_scene
clang test_sprite.c  -g -w -lm -ldl -lpthread -o test_sprite
clang test_video.c   -g -w -lm -ldl -lpthread -o test_video
clang test_script.c  -g -w -lm -ldl -lpthread -o test_script

exit

:windows -----------------------------------------------------------------------
@echo off
if not "%Platform%"=="x64" (
           if exist "%VS160COMNTOOLS%\..\..\VC\Auxiliary\Build\vcvarsx86_amd64.bat" (
              @call "%VS160COMNTOOLS%\..\..\VC\Auxiliary\Build\vcvarsx86_amd64.bat"
    ) else if exist "%VS150COMNTOOLS%\..\..\VC\Auxiliary\Build\vcvarsx86_amd64.bat" (
              @call "%VS150COMNTOOLS%\..\..\VC\Auxiliary\Build\vcvarsx86_amd64.bat"
    ) else if exist "%VS140COMNTOOLS%\..\..\VC\bin\x86_amd64\vcvarsx86_amd64.bat" (
              @call "%VS140COMNTOOLS%\..\..\VC\bin\x86_amd64\vcvarsx86_amd64.bat"
    ) else if exist "%VS120COMNTOOLS%\..\..\VC\bin\x86_amd64\vcvarsx86_amd64.bat" (
              @call "%VS120COMNTOOLS%\..\..\VC\bin\x86_amd64\vcvarsx86_amd64.bat"
    ) else if exist "%ProgramFiles(x86)%\microsoft visual studio\2017\community\VC\Auxiliary\Build\vcvarsx86_amd64.bat" (
              @call "%ProgramFiles(x86)%\microsoft visual studio\2017\community\VC\Auxiliary\Build\vcvarsx86_amd64.bat"
    ) else if exist "%ProgramFiles(x86)%\microsoft visual studio\2019\community\VC\Auxiliary\Build\vcvarsx86_amd64.bat" (
              @call "%ProgramFiles(x86)%\microsoft visual studio\2019\community\VC\Auxiliary\Build\vcvarsx86_amd64.bat"
    ) else (
        echo Error: Could not find x64 environment variables for Visual Studio 2019/2017/2015/2013
        exit /b
    )
    set Platform=x64
)

cd "%~dp0"

cl 3rd\3rd_tools\ass2iqe.c   /Fe3rd\3rd_tools\ass2iqe.exe  -I. /nologo /openmp /O2 /Oy /MT /DNDEBUG /DFINAL
cl 3rd\3rd_tools\iqe2iqm.cpp /Fe3rd\3rd_tools\iqe2iqm.exe  -I. /nologo /openmp /O2 /Oy /MT /DNDEBUG /DFINAL
cl 3rd\3rd_tools\mid2wav.c   /Fe3rd\3rd_tools\mid2wav.exe  -I. /nologo /openmp /O2 /Oy /MT /DNDEBUG /DFINAL
cl 3rd\3rd_tools\xml2json.c  /Fe3rd\3rd_tools\xml2json.exe -I. /nologo /openmp /O2 /Oy /MT /DNDEBUG /DFINAL

cl test.c         /nologo /openmp /Zi
cl test_cubemap.c /nologo /openmp /Zi
cl test_collide.c /nologo /openmp /Zi
cl test_model.c   /nologo /openmp /Zi
cl test_scene.c   /nologo /openmp /Zi
cl test_sprite.c  /nologo /openmp /Zi
cl test_video.c   /nologo /openmp /Zi
cl test_script.c  /nologo /openmp /Zi

pause
exit /b

rem emscripten
rem emcc test_ems.c -o test_ems.html -w -s TOTAL_MEMORY=268435456 -s USE_GLFW=3 -s USE_WEBGL2=1 -s LINKABLE=1
