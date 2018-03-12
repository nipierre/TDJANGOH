#!/bin/bash

function usage
{
    echo "Usage:"
    echo "$1 0 (Default) or 1 (DJANGOH)"
    exit
}

if [[ $# != 1 ]]; then
 usage
fi

rm -rf build
mkdir build
cd build
if [[ $1 = 1 ]]; then
	cmake -DCMAKE_INSTALL_PREFIX=../TGEANT -DCLHEP_ROOT_DIR=$CLHEP -DTGEANT_EXTERNAL_LHAPDF=YES -DTGEANT_DISABLE_PYTHIA=YES ../TGEANT_FILES/
else
	cmake -DCMAKE_INSTALL_PREFIX=../TGEANT -DCLHEP_ROOT_DIR=$CLHEP -DTGEANT_EXTERNAL_LHAPDF=YES -DTGEANT_DISABLE_DJANGOH=YES ../TGEANT_FILES/
fi

make -j4 install; make -j4 install
