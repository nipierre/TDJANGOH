/* @(#)root/djangoh:$Id$ */

#ifndef ROOT_TDjangohCalls
#define ROOT_TDjangohCalls


int   const KSUSY1  =  1000000;
int   const KSUSY2  =  2000000;
int   const KEXCIT  =  4000000;
int   const KNDCAY  =  8000; //should be 4000 for 1

/// Djangoh common block Lujets
struct Lujets_t {
 int    N;
 int    NPAD;
 int    K[5][4000];
 double P[5][4000];
 double V[5][4000];
};

/// Djangoh common block Ludat1
struct Ludat1_t {
 int    MSTU[200];
 double PARU[200];
 int    MSTJ[200];
 double PARJ[200];
};

/// Djangoh common block Ludat2
struct Ludat2_t {
 int    KCHG[4][500];
 double PMAS[4][500];
 double PARF[2000];
 double VCKM[4][4];
};

/// Djangoh common block Ludat3
struct Ludat3_t {
 int    MDCY[3][500];
 int    MDME[2][KNDCAY];
 double BRAT[KNDCAY];
 int    KFDP[5][KNDCAY];
};

/// Djangoh common block Ludat4
struct Ludat4_t {
 char  CHAF[2][500][16]; // here I needed manual intervention
};

/// Djangoh common block Ludatr
struct Ludatr_t {
 int    MRLU[6];
 double RRLU[100];
};

#endif