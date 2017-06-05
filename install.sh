#!/bin/bash
cd src/
mkdir build
cd build
cmake ../
make
cd ..
export PATH=$PWD:$PATH
