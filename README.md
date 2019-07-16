# TDJANGOH v. 1.4

## Patchnote

[TDJANGOH v. 1.4 Patchnote](PATCHNOTE.md)

## Requirements

GCC > 4.4.7 with build-in GFORTRAN

ROOT > 5.34/32

LHAPDF 5.9.1 (in any case lower than LHAPDF 6.x.x) [Download LHAPDF 5.x.x](https://www.hepforge.org/downloads/lhapdf)

CERNLIB (2006) [Download CERNLIB](http://cernlib.web.cern.ch/cernlib/version.html)

(Tested on LXPLUS and CCAGE)


## Environment variables

After recovering the package, export the following variables (better in .bashrc or similar) :

  `export TDJANGOH=/path/to/TDJANGOH/`
  `export LHAPDF5=/path/to/LHAPDF5/`

## Setup & Build

To setup :
  `make setup`

To build :
  `make`

## Test Programs

See more on [test programs](test/TESTPROGRAMS.md)..

## Utilities Programs

See more on [utilities programs](utils/UTILSPROGRAMS.md)..

## More informations on Djangoh
One way to learn more on TDJANGOH is to look at the doxygen documentation.

The [DJANGOH manual Version 1.6](http://wwwthep.physik.uni-mainz.de/~hspiesb/djangoh/djangoh_m.4.6.6.ps.gz) contains the description of input options and includes short lists of most important changes with respect to previous versions.


## Contact

For any problem, input, questions : nicolas.pierre@cern.ch
