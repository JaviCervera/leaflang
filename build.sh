#!/bin/sh
cd `dirname $0`

echo "Creating library wrapper..."
cd corelib
g++ -o a.out gencorelib.cc core.cpp ../src/definitions.cpp ../src/generator.cpp ../src/lib.cpp ../src/parser.cpp ../src/token.cpp
./a.out
rm a.out
cd ..

echo "Creating folder for CMake..."
mkdir _CMAKE

echo "Building..."
cd _CMAKE
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make
cd ..

echo "Moving to _build/bin dir..."
mkdir -p _build/bin
mv _CMAKE/pico _build/bin/pico
