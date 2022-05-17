#!/bin/sh
cd `dirname $0`

echo "Creating folder for CMake..."
mkdir _CMAKE

echo "Building leaf..."
cd _CMAKE
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make
cd ..

echo "Moving leaf to _build/bin dir..."
mkdir -p _build/bin
mv _CMAKE/leaf _build/bin/leaf

echo "Building lfmake..."
_build/bin/leaf lfmake.lf
gcc -o _build/bin/lfmake lfmake.c _build/libs/core/core.c -I_build/libs -w -lm -O2 -s
rm lfmake.c
