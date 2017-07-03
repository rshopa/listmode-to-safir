#!/bin/bash
cd src/
mkdir build
cd build
cmake ../
make
cd ../../
export PATH=$PWD/src/build:$PATH
touch setup.sh
echo "export PATH=$PWD/src/build:$PATH" >> ~/.setup.sh

