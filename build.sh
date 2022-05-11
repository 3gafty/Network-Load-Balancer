#!/bin/bash
./clear.sh
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF .. && cmake --build . -j $(nproc)
