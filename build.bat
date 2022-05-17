@echo off

echo Creating folder for CMake...
mkdir _CMAKE

echo Building leaf...
cd _CMAKE
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
mingw32-make
cd ..

echo Moving leaf to _build/bin dir...
mkdir _build\bin
move "_CMAKE\leaf.exe" "_build/bin/leaf.exe"


echo Building lfmake...
_build\bin\leaf lfmake.lf
gcc -o _build/bin/lfmake lfmake.c _build/libs/core/core.c -I_build/libs -w -lm -O2 -s
del lfmake.c

pause
