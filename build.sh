#!/bin/sh
cd `dirname $0`

echo "Creating folder for CMake..."
mkdir _CMAKE

echo "Building..."
cd _CMAKE
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make
cd ..

echo "Moving to _build/bin dir..."
mkdir -p _build/bin
mv _CMAKE/leaf _build/bin/leaf
