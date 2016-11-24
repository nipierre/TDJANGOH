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
  void hsmain_(const char* inputfile, int* len, int* nbf);
}

struct myabc
{
  int mya;
  int myb;
  int myc;
} myabc_;

struct hselab
{
  float sp;
  float eele;
  float pele;
  float epro;
  float ppro;
} hselab_;

struct hscuts
{
  float xmin;
  float xmax;
  float q2min;
  float q2max;
  float ymin;
  float ymax;
  float wmin;
  float wmax;
  float gmin;
} hscuts_;

struct hstcut
{
  float themin;
  float themax;
  float cthmin;
  float cthcon;
} hstcut_;

struct hspcut
{
  float ptimin;
  float ptxm0;
} hspcut_;

struct hsisgm
{
  float tcutq;
  float tcutqs;
} hsisgm_;

struct hsparl
{
  float lpar[20];
  float lparin[12];
} hsparl_;

struct hsstrp
{
  float icode;
  float ilib;
  float ilqmod;
  float idpvr;
} hsstrp_

struct HSPDFO
{
  float ipdfop;
  float iflopt;
  float lqcd;
  float ltm;
  float lht;
} hspdfo_;

struct hselep
{
  float idipol;
} hselep_;

struct hsnucl
{
  int hna;
  int hnz;
  float inumod;
} hsnucl_;

struct hsparm
{
  float polari;
  float hpolar;
  float llept;
  float lqua;
} hsparm_;

struct hswgtc
{
  float iweigs;
} hswgtc_;

struct hsonly
{
  float ihsonl;
} hsonly_;

struct hsltyp
{
  float lepin1;
} hsltyp_;

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


void TDjangoh::GenerateEvent(const char* cfile, int nbf)
{
  //char* cfile = "TDjangoh.in";
  int len_cfile = strlen(cfile);

  myabc_.mya=1;
  myabc_.myb=2;
  myabc_.myc=3;

  hsmain_(cfile,&len_cfile,&nbf);
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


void TDjangoh::Initialize(const char *name,const char *beam, int nuc_A, int nuc_Z, float beam_e, float nuc_e, float pol)
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

  Initialize_File(name,beam,PID,nuc_A,nuc_Z,beam_e,nuc_e,pol);
  cout << "File initialized !" << endl;

  snprintf(atitle,32," %s-N(%d,%d) at %g GeV",beam,nuc_A,nuc_Z,sqrt(pow(beam_e,2)+pow(nuc_e,2)));
  SetTitle(atitle);
}

void TDjangoh::Initialize_File(const char *name, const char *beam, int PID, int nuc_A, int nuc_Z, float beam_e, float nuc_e, float pol)
{
  ofstream ofs("TDjangoh.in", std::ofstream::out | std::ofstream::trunc);

  ofs
  << "OUTFILENAM\n" << outfilename
  << "\nTITLE\nDJANGOH 4.6.10 for COMPASS for " << beam << " on N(" << nuc_A << "," << nuc_Z
      << ") , NLO at " << beam_e << " , pol at " << pol << " , Wmin = " << kinem_cut[6]
  << "\nEL-BEAM\n\t" << beam_e << "D0 0.0D0 " << PID
  << "\nIOUNITS\n\t" << iounits[0] << " " << iounits[1] << " " << iounits[2]
  << "\nPR-BEAM\n\t" << pr_beam[0] << " " << pr_beam[1]
  << "\nGSW-PARAM\n\t" <<  gsw_param[0] << " " <<  gsw_param[1] << " " <<  gsw_param[2]  << " "
      <<  gsw_param[3] << " " <<  gsw_param[4] << " " <<  gsw_param[5] << " " <<  gsw_param[6] << " "
      <<  gsw_param[7] << " " <<  gsw_param[8] << " " <<  gsw_param[9] << " " <<  gsw_param[10]
  << "\nKINEM-CUTS\n\t" << kinem_cut_var << " " << kinem_cut[0] << " "  << kinem_cut[1] << " "
      << kinem_cut[2] << " " << kinem_cut[3] << " " << kinem_cut[4] << " "
      << kinem_cut[5] << " " << kinem_cut[6]
  << "\nEGAM-MIN\n\t" << egam_min
  << "\nINT-OPT-NC\n\t" << int_opt_nc[0] << " " << int_opt_nc[1] << " " << int_opt_nc[2] << " "
      << int_opt_nc[3] << " " << int_opt_nc[4] << " " << int_opt_nc[5] << " "
      << int_opt_nc[6] << " " << int_opt_nc[7] << " " << int_opt_nc[8]
  << "\nINT-OPT-CC\n\t" << int_opt_cc[0] << " " << int_opt_cc[1] << " "
      << int_opt_cc[2] << " " << int_opt_cc[3]
  << "\nINT-ONLY\n\t" << int_only
  << "\nINT-POINTS\n\t" << int_point
  << "\nSAM-OPT-NC\n\t" << sam_opt_nc[0] << " " << sam_opt_nc[1] << " " << sam_opt_nc[2] << " "
      << sam_opt_nc[3] << " " << sam_opt_nc[4] << " " << sam_opt_nc[5] << " "
      << sam_opt_nc[6] << " " << sam_opt_nc[7] << " " << sam_opt_nc[8]
  << "\nSAM-OPT-CC\n\t" << sam_opt_cc[0] << " " << sam_opt_cc[1] << " "
      << sam_opt_cc[2] << " " << sam_opt_cc[3]
  << "\nNUCLEUS\n\t" << nuc_e << " " << nuc_A << " " << nuc_Z
  << "\nSTRUCTFUNC\n\t" << structfunc[0] << " " << structfunc[1] << " " << structfunc[2]
  << "\nLHAPATH\n" << getenv("LHAPATH")
  << "\nFLONG\n\t" << flong[0] << " " << flong[1] << " " << flong[2]
  << "\nALFAS\n\t" << alfas[0] << " " << alfas[1] << " " << alfas[2] << " " << alfas[3]
  << "\nNFLAVORS\n\t" << nflavors[0] << " " << nflavors[1]
  << "\nRNDM-SEEDS\n\t" << rndm_seeds[0] << " " << rndm_seeds[1]
  << "\nSTART\n\t" << start
  << "\nSOPHIA\n\t" << sophia
  << "\nOUT-LEP\n\t" << out_lep
  << "\nFRAG\n\t" << frag
  << "\nCASCADES\n\t" << cascades
  << "\nMAX-VIRT\n\t" << max_virt
  << "\nCONTINUE";

  ofs.close();
}

void TDjangoh::Clean_File()
{
  remove("TDjangoh_evt.dat");
  remove("TDjangoh_out.dat");
  remove("TDjangoh_rnd.dat");
  remove("TDjangoh_smp.dat");
}
