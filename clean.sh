#!/bin/bash
cd "$(dirname "$0")"
qmake
make clean
rm .qmake.stash