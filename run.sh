#!/bin/bash
cd "$(dirname "$0")"
qmake
make
./target/MazeGame