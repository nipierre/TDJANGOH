#!/bin/bash

rm -rf TGEANT_FILES
git clone --recursive https://user@gitlab.cern.ch/compass-tgeant/TGEANT.git  TGEANT_FILES
cd TGEANT_FILES
git checkout -b developement/djangoh origin/development/djangoh
cd ..
