//==============================================================================
/*!

    \file                         TDjangoh.cc
    \brief                        Interface class to Djangoh generator
    \author                       Nicolas PIERRE
    \version                      0.1
    \date                         28/10/16
    Support :                     mail to : nicolas.pierre@cern.ch

\class TDjangoh
\ingroup djangoh
*/
//==============================================================================

#include <iostream>
#include <fstream>
#include <cstring>

#include "../include/TDjangoh.h"
#include "TClonesArray.h"
#include "../include/TMCParticle.h"
#include "TParticle.h"
#include "TString.h"

TDjangoh*  TDjangoh::fgInstance = 0;

# define type_of_call _stdcall

Lujets_t lujets_;
// Ludat1_t ludat1_;
// Ludat2_t ludat2_;
Djkin_t djkin_;

extern "C"
{
  void hsmain_();
}

extern "C" struct ihscw
{
  char inputcodewd[46][10];
  int itcw;
} ihscw_;

extern "C" struct isdebug
{
  int isdbg;
} isdebug_;

extern "C" struct hselab
{
  double sp;
  double eele;
  double pele;
  double epro;
  double ppro;
} hselab_;

extern "C" struct ihscut
{
  float ixmin;
  float ixmax;
  float iq2min;
  float iq2max;
  float iymin;
  float iymax;
  float iwmin;
} ihscut_;

extern "C" struct hstcut
{
  double themin;
  double themax;
  double cthmin;
  double cthcon;
} hstcut_;

extern "C" struct hspcut
{
  double ptmin = 0.0;
  double ptxm0 = 0.0;
} hspcut_;

extern "C" struct hsisgm
{
  double tcutq;
  double tcutqs;
} hsisgm_;

extern "C" struct hsparl
{
  int lpar[20];
  int lparin[12];
} hsparl_;

extern "C" struct hsstrp
{
  int icode;
  int ilib;
  int ilqmod;
  int idpvr;
} hsstrp_;

extern "C" struct hspdfo
{
  int ipdfop;
  int iflopt;
  int lqcd;
  int ltm;
  int lht;
} hspdfo_;

extern "C" struct hselep
{
  double idipol;
} hselep_;

extern "C" struct hsnucl
{
  int hna;
  int hnz;
  double inumod;
} hsnucl_;

extern "C" struct hsparm
{
  double polari;
  double hpolar;
  int llept;
  int lqua;
} hsparm_;

extern "C" struct hswgtc
{
  int iweigs;
  int iweigr;
} hswgtc_;

extern "C" struct hsirct
{
  double deleps;
  double delta;
  double egmin;
  double iopegm;
} hsirct_;

extern "C" struct hsalfs
{
  float par111;
  float par112;
  float parl11;
  float parl19;
  int mst111;
  int mst115;
} hsalfs_;

extern "C" struct hsintnc
{
  int inc2;
  int inc31 ;
  int inc32 ;
  int inc33 ;
  int inc34;
  int iel2;
  int iel31;
  int iel32;
  int iel33;
} hsintnc_;

extern "C" struct hsintcc
{
  int icc2;
  int icc31;
  int icc32;
  int icc33;
} hsintcc_;

extern "C" struct hssamnc
{
  int isnc2;
  int isnc31;
  int isnc32;
  int isnc33;
  int isnc34;
  int isel2;
  int isel31;
  int isel32;
  int isel33;
} hssamnc_;

extern "C" struct hssamcc
{
  int iscc2;
  int iscc31;
  int iscc32;
  int iscc33;
} hssamcc_;

extern "C" struct hsrdio
{
  int isdinp;
  int isdout;
} hsrdio_;

extern "C" struct hsvglp
{
  int npoveg;
  int numint;
  int nphyp;
} hsvglp_;

extern "C" struct hystfu
{
  float pystop;
  float pyslam;
  int npymax;
  int npymin;
} hystfu_;

extern "C" struct hsoptn
{
  int int2[5];
  int int3[15];
  int isam2[5];
  int isam3[15];
  int ioplot;
  int iprint;
  int icut;
} hsoptn_;

extern "C" struct hsnume
{
  double sigtot;
  double sigerr;
  double sigg[20];
  double siggrr[20];
  int nevent;
  int neve[20];
} hsnume_;

extern "C" struct lhapdfc
{
  char lhapath[232];
} lhapdfc_;

extern "C" struct hsoutf
{
  char outfilenam[80];
} hsoutf_;


using namespace std;

void ConvertToFortran(char* fstring, std::size_t fstring_len,
                      const char* cstring)
{
    size_t inlen = strlen(cstring);
    size_t cpylen = min(inlen, fstring_len);

    if (inlen > fstring_len)
    {
        cout << "ERROR" << endl;
    }

    copy(cstring, cstring + cpylen, fstring);
    fill(fstring + cpylen, fstring + fstring_len, ' ');
}


TDjangoh::TDjangohCleaner::TDjangohCleaner() {}


TDjangoh::TDjangohCleaner::~TDjangohCleaner()
{
  if (TDjangoh::fgInstance) {
     delete TDjangoh::fgInstance;
     TDjangoh::fgInstance = 0;
  }
}


TDjangoh::TDjangoh() : TGenerator("TDjangoh","TDjangoh")
{
  if (fgInstance)
     Fatal("TDjangoh", "There's already an instance of TDjangoh");

  delete fParticles;

  fParticles = new TClonesArray("TMCParticle",50);
}


TDjangoh::TDjangoh(const TDjangoh& dj) :
 TGenerator(dj),
 fLujets(dj.fLujets),
 fLudat1(dj.fLudat1),
 fLudat2(dj.fLudat2),
 fDjkin(dj.fDjkin)

{}

TDjangoh::~TDjangoh()
{
  if (fParticles) {
     fParticles->Delete();
     delete fParticles;
     fParticles = 0;
  }
}


TDjangoh* TDjangoh::Instance()
{
  static TDjangoh::TDjangohCleaner cleaner;
  return fgInstance ? fgInstance : (fgInstance=new TDjangoh()) ;
}


void TDjangoh::GenerateEvent()
{
  hsmain_();
  // cout << lujets_.N << endl;
  fLujets = &lujets_;
  // fLudat1 = &ludat1_;
  // fLudat2 = &ludat2_;
  fDjkin = &djkin_;
  ImportParticles();
}


TObjArray *TDjangoh::ImportParticles(Option_t *)
{
  fParticles->Clear();
  Int_t numpart   = fLujets->N;
  TClonesArray &a = *((TClonesArray*)fParticles);

  for (Int_t i = 0; i<numpart; i++)
  {
     new(a[i]) TMCParticle(fLujets->K[0][i] ,
                           fLujets->K[1][i] ,
                           fLujets->K[2][i] ,
                           fLujets->K[3][i] ,
                           fLujets->K[4][i] ,
                           fLujets->P[0][i] ,
                           fLujets->P[1][i] ,
                           fLujets->P[2][i] ,
                           fLujets->P[3][i] ,
                           fLujets->P[4][i] ,
                           fLujets->V[0][i] ,
                           fLujets->V[1][i] ,
                           fLujets->V[2][i] ,
                           fLujets->V[3][i] ,
                           fLujets->V[4][i]);
  }
  return fParticles;
}


Int_t TDjangoh::ImportParticles(TClonesArray *particles, Option_t *option)
{
  if (particles == 0) return 0;
  TClonesArray &clonesParticles = *particles;
  clonesParticles.Clear();
  Int_t numpart = fLujets->N;
  Int_t nparts=0;

  if (!strcmp(option,"") || !strcmp(option,"Final"))
  {
     for (Int_t i = 0; i<numpart; i++)
     {
       if (fLujets->K[0][i] == 1)
       {
          new(clonesParticles[nparts]) TParticle(
                             fLujets->K[1][i] ,
                             fLujets->K[0][i] ,
                             fLujets->K[2][i] ,
                             -1,
                             fLujets->K[3][i] ,
                             fLujets->K[4][i] ,
                             fLujets->P[0][i] ,
                             fLujets->P[1][i] ,
                             fLujets->P[2][i] ,
                             fLujets->P[3][i] ,
                             fLujets->V[0][i] ,
                             fLujets->V[1][i] ,
                             fLujets->V[2][i] ,
                             fLujets->V[3][i]);

          //  printf("%d %d %d! ",i,fLujets->K[1][i],numpart);
          nparts++;
        }
     }
  }
  else if (!strcmp(option,"All"))
  {
   for (Int_t i = 0; i<numpart; i++)
   {
      new(clonesParticles[i]) TParticle(
                         fLujets->K[1][i] ,
                         fLujets->K[0][i] ,
                         fLujets->K[2][i] ,
                         -1,
                         fLujets->K[3][i] ,
                         fLujets->K[4][i] ,
                         fLujets->P[0][i] ,
                         fLujets->P[1][i] ,
                         fLujets->P[2][i] ,
                         fLujets->P[3][i] ,
                         fLujets->V[0][i] ,
                         fLujets->V[1][i] ,
                         fLujets->V[2][i] ,
                         fLujets->V[3][i]);
   }
   nparts=numpart;
 }
  return nparts;
}


void TDjangoh::Initialize(const char *beam, int nuc_A, int nuc_Z, float beam_e, float nuc_e, float pol)
{
  int PID;
  char* inputcw[46];

  // Djangoh accept only e and mu
  if      (!strcmp(beam, "e-" )) PID = -1;
  else if (!strcmp(beam, "e+" )) PID = 1;
  else if (!strcmp(beam, "mu-")) PID = -3;
  else if (!strcmp(beam, "mu+")) PID = 3;
  else
  {
     printf("WARNING! In TDjangoh:Initialize():\n");
     printf(" specified beam=%s is unrecognized .\n",beam);
     printf(" resetting to \"e+\" .");
     PID = 11;
  }

  inputcw[0] = "OUTFILENAM";
  inputcw[1] = "TITLE     ";
  inputcw[2] = "EL-BEAM   ";
  inputcw[3] = "IOUNITS   ";
  inputcw[4] = "GSW-PARAM ";
  inputcw[5] = "KINEM-CUTS";
  inputcw[6] = "EGAM-MIN  ";
  inputcw[7] = "INT-OPT-NC";
  inputcw[8] = "INT-OPT-CC";
  inputcw[9] = "INT-ONLY  ";
  inputcw[10] = "INT-POINTS";
  inputcw[11] = "SAM-OPT-NC";
  inputcw[12] = "SAM-OPT-CC";
  inputcw[13] = "NUCLEUS   ";
  inputcw[14] = "STRUCTFUNC";
  inputcw[15] = "LHAPATH   ";
  inputcw[16] = "FLONG     ";
  inputcw[17] = "ALFAS     ";
  inputcw[18] = "NFLAVORS  ";
  inputcw[19] = "RNDM-SEEDS";
  inputcw[20] = "START     ";
  inputcw[21] = "SOPHIA    ";
  inputcw[22] = "OUT-LEP   ";
  inputcw[23] = "FRAG      ";
  inputcw[24] = "CASCADES  ";
  inputcw[25] = "MAX-VIRT  ";
  inputcw[26] = "CONTINUE  ";

  for(int i=0; i<27; i++)
    ConvertToFortran(ihscw_.inputcodewd[i], sizeof ihscw_.inputcodewd[i], inputcw[i]);

  // OUTFILENAME
  char* outfilenamei = "TDjangoh";
  ConvertToFortran(hsoutf_.outfilenam, sizeof hsoutf_.outfilenam, outfilenamei);

  // EL-BEAM
  hselab_.eele = beam_e;
  hsparm_.polari = pol;
  hsparm_.llept = PID;

  // KINEM-CUTS
  hsoptn_.icut = 3;
  ihscut_.ixmin = 0.0001;
  ihscut_.ixmax = 1.00;
  ihscut_.iymin = 0.01;
  ihscut_.iymax = 0.95;
  ihscut_.iq2min = 1.0;
  ihscut_.iq2max = 1e5;
  ihscut_.iwmin = 1.40;

  // EGAM-MIN
  hsirct_.egmin = 0.0;

  // INT-OPT-NC
  hsintnc_.inc2 = 1;
  hsintnc_.inc31 = 18;
  hsintnc_.inc32 = 18;
  hsintnc_.inc33 = 18;
  hsintnc_.inc34 = 0;
  hsintnc_.iel2 = 0;
  hsintnc_.iel31 = 0;
  hsintnc_.iel32 = 0;
  hsintnc_.iel33 = 0;

  // INT-OPT-CC
  hsintcc_.icc2 = 0;
  hsintcc_.icc31 = 0;
  hsintcc_.icc32 = 0;
  hsintcc_.icc33 = 0;

  // INT-POINTS
  hsvglp_.npoveg = 3000;

  // HYP-CUBES
  hsvglp_.nphyp = 20;

  // GSW-PARAM
  hsparl_.lparin[0] = 2;
  hsparl_.lparin[1] = 1;
  hsparl_.lparin[2] = 3;
  hsparl_.lparin[3] = 1;
  hsparl_.lparin[4] = 0;
  hsparl_.lparin[5] = 0;
  hsparl_.lparin[6] = 2;
  hsparl_.lparin[7] = 1;
  hsparl_.lparin[8] = 1;
  hsparl_.lparin[9] = 1;
  hsparl_.lparin[10] = 1;
  hsparl_.lparin[10] = 1;

  // STRUCTFUNC
  hsstrp_.ilqmod = 1;
  hsstrp_.ilib = 2;
  hsstrp_.icode = 10150;

  // NFLAVORS
  hystfu_.npymin = 0;
  hystfu_.npymax = 3;

  // SAM-OPT-NC
  hssamnc_.isnc2 = 1;
  hssamnc_.isnc31 = 1;
  hssamnc_.isnc32 = 1;
  hssamnc_.isnc33 = 1;
  hssamnc_.isnc34 = 0;
  hssamnc_.isel2 = 0;
  hssamnc_.isel31 = 0;
  hssamnc_.isel32 = 0;
  hssamnc_.isel33 = 0;

  // SAM-OPT-CC
  hssamcc_.iscc2 = 0;
  hssamcc_.iscc31 = 0;
  hssamcc_.iscc32 = 0;
  hssamcc_.iscc33 = 0;

  // RNDM-SEEDS
  hsrdio_.isdinp = -1;
  hsrdio_.isdout = -1;

  // THMIN-QRAD
  hsisgm_.tcutq = 0.25;
  hsisgm_.tcutqs = 0.25;

  // FLONG
  hspdfo_.iflopt = 111;
  hsalfs_.parl11 = 0.01;
  hsalfs_.parl19 = 0.03;

  // ALFAS
  hsalfs_.mst111 = 1;
  hsalfs_.mst115 = 1;
  hsalfs_.par111 = 0.20;
  hsalfs_.par112 = 0.235;

  // EP-DIPOLE
  hselep_.idipol = 0;

  // NUCLEUS
  hselab_.epro = nuc_e;
  hsparm_.hpolar = 0;
  hsnucl_.hna = nuc_A;
  hsnucl_.hnz = nuc_Z;

  // NUCL-MOD
  hsnucl_.inumod = 0;

  // LHAPATH
  char* lhapathi;
  lhapathi = getenv("LHAPATH");
  ConvertToFortran(lhapdfc_.lhapath, sizeof lhapdfc_.lhapath, lhapathi);

  // THETA-CUT
  hstcut_.themin = 0.0;
  hstcut_.themax = 180.0;

  // PT-CUT
  hspcut_.ptmin = 0.0;

  // POLPDF
  hsstrp_.idpvr = 100;

  // WEIGHTS
  hswgtc_.iweigr = 0;

  // INT-ONLY
  hsoptn_.ioplot = 0;

  // START
  hsnume_.nevent = 1;

  // DEBUG_MODE
  isdebug_.isdbg = 1;

}


void TDjangoh::Configure(const char *beam, int nuc_A, int nuc_Z, float beam_e, float nuc_e, float pol)
{
  int PID;

  // Djangoh accept only e and mu
  if      (!strcmp(beam, "e-" )) PID = -1;
  else if (!strcmp(beam, "e+" )) PID = 1;
  else if (!strcmp(beam, "mu-")) PID = -3;
  else if (!strcmp(beam, "mu+")) PID = 3;
  else
  {
     printf("WARNING! In TDjangoh:Initialize():\n");
     printf(" specified beam=%s is unrecognized .\n",beam);
     printf(" resetting to \"e+\" .");
     PID = 11;
  }

// EL-BEAM
  hselab_.eele = beam_e;
  hsparm_.polari = pol;
  hsparm_.llept = PID;

// NUCLEUS
  hselab_.epro = nuc_e;
  hsnucl_.hna = nuc_A;
  hsnucl_.hnz = nuc_Z;
}

void TDjangoh::Clean_File()
{
  remove("TDjangoh_evt.dat");
  remove("TDjangoh_out.dat");
  remove("TDjangoh_rnd.dat");
  remove("TDjangoh_smp.dat");
}
