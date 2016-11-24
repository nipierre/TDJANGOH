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

#include "TDjangoh.h"
#include "TClonesArray.h"
#include "TMCParticle.h"
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
  void hsmain_(char* clhapathi,int* clhalen);
}

extern "C" struct hselab
{
  double sp;
  double eele = 160.0;
  double pele;
  double epro = 0.0;
  double ppro;
} hselab_;

extern "C" struct hscuts
{
  double xmin = 0.0001;
  double xmax = 1.00;
  double q2min = 1.0;
  double q2max = 10^5;
  double ymin = 0.01;
  double ymax = 0.95;
  double wmin = 1.4;
  double gmin;
} hscuts_;

extern "C" struct hstcut
{
  double themin = 0.0;
  double themax = 180.0;
  double cthmin = 1.0;
  double cthcon = 10^15;
} hstcut_;

extern "C" struct hspcut
{
  double ptmin = 0.0;
  double ptxm0 = 0.0;
} hspcut_;

extern "C" struct hsisgm
{
  double tcutq = 0.25;
  double tcutqs = 0.25;
} hsisgm_;

extern "C" struct hsparl
{
  int lpar[20];
  int lparin[12] = {2,1,3,1,0,0,2,1,1,1,1,1};
} hsparl_;

extern "C" struct hsstrp
{
  int icode = 3041;
  int ilib = 2;
  int ilqmod = 1;
  int idpvr = 100;
} hsstrp_;

extern "C" struct hspdfo
{
  int ipdfop;
  int iflopt = 111;
  int lqcd = 0;
  int ltm = 0;
  int lht = 0;
} hspdfo_;

extern "C" struct hselep
{
  double idipol = 0;
} hselep_;

extern "C" struct hsnucl
{
  double hna = 1;
  double hnz = 1;
  double inumod = 0;
} hsnucl_;

extern "C" struct hsparm
{
  double polari = 0.0;
  double hpolar = 0.0;
  int llept = -3;
  int lqua = 0;
} hsparm_;

extern "C" struct hswgtc
{
  int iweigs;
  int iweigr = 0;
} hswgtc_;

extern "C" struct hsirct
{
  double deleps;
  double delta;
  double egmin = 0.0;
  double iopegm;
} hsirct_;

extern "C" struct hsalfs
{
  double par111;
  double par112;
  double parl11;
  double parl19;
  double mst111;
  double mst115;
} hsalfs_;

extern "C" struct hsintnc
{
  int inc2 = 1;
  int inc31 = 18;
  int inc32 = 18;
  int inc33 = 18;
  int inc34 = 0;
  int iel2 = 0;
  int iel31 = 0;
  int iel32 = 0;
  int iel33 = 0;
} hsintnc_;

extern "C" struct hsintcc
{
  int icc2 = 0;
  int icc31 = 0;
  int icc32 = 0;
  int icc33 = 0;
} hsintcc_;

extern "C" struct hssamnc
{
  int isnc2 = 1;
  int isnc31 = 1;
  int isnc32 = 1;
  int isnc33 = 1;
  int isnc34 = 0;
  int isel2 = 0;
  int isel31 = 0;
  int isel32 = 0;
  int isel33 = 0;
} hssamnc_;

extern "C" struct hssamcc
{
  int iscc2 = 0;
  int iscc31 = 0;
  int iscc32 = 0;
  int iscc33 = 0;
} hssamcc_;

extern "C" struct hsrdio
{
  int isdinp = -1;
  int isdout = -1;
} hsrdio_;

extern "C" struct hsvglp
{
  int npoveg = 3000;
  int numint;
  int nphyp = 20;
} hsvglp_;

extern "C" struct hystfu
{
  int pystop;
  int pyslam;
  int npymax = 3;
  int npymin = 0;
} hystfu_;

extern "C" struct hsoptn
{
  int int2[5];
  int int3[15];
  int isam2[5];
  int isam3[15];
  int ioplot = 0;
  int iprint = 0;
  int icut = 3;
} hsoptn_;

extern "C" struct hsnume
{
  double sigtot;
  double sigerr;
  double sigg[20];
  double siggrr[20];
  int nevent = 1;
  int neve[20];
} hsnume_;

extern "C" struct lhapdfc
{
  char* lhapath;
} lhapdfc_;

extern "C" struct hsoutf
{
  char* outfilenam;
  int ich;
} hsoutf_;

using namespace std;


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
  //char* cfile = "TDjangoh.in";
  //int len_cfile = strlen(cfile);

  cout << lhapathi << " " << lhalen;


  hsmain_(lhapathi,&lhalen);

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
  char atitle[32];

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

  Data_default();

  // Initialize_File(name,beam,PID,nuc_A,nuc_Z,beam_e,nuc_e,pol);
  // cout << "File initialized !" << endl;

  snprintf(atitle,32," %s-N(%d,%d) at %g GeV",beam,nuc_A,nuc_Z,sqrt(pow(beam_e,2)+pow(nuc_e,2)));
  SetTitle(atitle);
}

void TDjangoh::Data_default()
{

  // OUTFILENAME
  hsoutf_.outfilenam = "TDjangoh";
  // outlen = int(strlen("TDjangoh"));

  // EL-BEAM
  hselab_.eele = 130.0;
  hsparm_.polari = 0;
  hsparm_.llept = -3;

  // KINEM-CUTS
  hsoptn_.icut = 3;
  hscuts_.xmin = 0.0001;
  hscuts_.xmax = 1.00;
  hscuts_.ymin = 0.01;
  hscuts_.ymax = 0.95;
  hscuts_.q2min = 1.0;
  hscuts_.q2max = 1e5;
  hscuts_.wmin = 1.40;

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
  // hsalfs_.parl11 = 0.01;
  // hsalfs_.parl19 = 0.03;

  // ALFAS
  // hsalfs_.mst111 = 1.0;
  // hsalfs_.mst115 = 1.0;
  // hsalfs_.par111 = 0.20;
  // hsalfs_.par112 = 0.235;

  // EP-DIPOLE
  hselep_.idipol = 0;

  // NUCLEUS
  hselab_.epro = 0;
  hsparm_.hpolar = 0;
  hsnucl_.hna = 1.0;
  hsnucl_.hnz = 1.0;

  // NUCL-MOD
  hsnucl_.inumod = 0;

  // LHAPATH
  lhapathi = getenv("LHAPATH");
  lhalen = strlen(lhapathi);

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
}

void TDjangoh::Clean_File()
{
  remove("TDjangoh_evt.dat");
  remove("TDjangoh_out.dat");
  remove("TDjangoh_rnd.dat");
  remove("TDjangoh_smp.dat");
}
