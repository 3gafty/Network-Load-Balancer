#!/bin/bash
./clear.sh
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build . -j $(nproc)
