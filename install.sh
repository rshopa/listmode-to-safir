#!/bin/bash
cd src/
mkdir build
cd build
cmake ../
make
cd ../../
export PATH=$PWD/src/build:$PATH
echo "export PATH=$PWD/src/build:$PATH" >> ~/.profile

