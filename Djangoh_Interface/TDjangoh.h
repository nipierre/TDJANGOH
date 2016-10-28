// Djangoh interface
// Author: Nicolas PIERRE   14/10/16

#ifndef DJANGOH_TDjangoh
#define DJANGOH_TDjangoh

/*

TDjangoh is an interface class to Djangoh

To use this class you must install a version of djangoh.
See the installation instructions at :
  Spiesberger address
Or contact : nicolas.pierre@cern.ch

Djangoh event generator, written by H.Spiesberger.
For the details about the generator look at djangoh manual:

*/
//#ifdef __GNUG__
//#pragma interface
//#endif

#ifndef ROOT_TDjangohCalls
#include "TDjangohCalls.h"
#endif

#ifndef ROOT_TGenerator
#include "TGenerator.h"
#endif

#ifndef ROOT_TObjArray
#include "TObjArray.h"
#endif

class TDjangoh : public TGenerator {

protected:
  static  TDjangoh* fgInstance;
  // Fortran common blocks
  // int lujets_[2+5*4000+2*2*5*4000];
  // int ludat1_[200+2*200+200+2*200];
  // int ludat2_[4*500+2*4*500+2*2000+2*4*4];
  // Djangoh common-blocks
  Lujets_t*  fLujets;
  Ludat1_t*  fLudat1;
  Ludat2_t*  fLudat2;
  // Cleanup
  class  TDjangohCleaner {
  public:
     TDjangohCleaner();
     ~TDjangohCleaner();
  };
  friend class TDjangohCleaner;

  TDjangoh(const TDjangoh&);            // Cannot be copied
  TDjangoh& operator=(const TDjangoh&); // Cannot be copied

// void *djangoh_common_address(const char* name)
// {
//    if      (!strcmp(name,"LUJETS")) return lujets_;
//    else if (!strcmp(name,"LUDAT1")) return ludat1_;
//    else if (!strcmp(name,"LUDAT2")) return ludat2_;
//    return 0;
// }


public:
  // ****** constructors and destructor
  TDjangoh();
  virtual ~TDjangoh();

  static TDjangoh *Instance();

  // ****** accessors
  // FORTRAN indexing in accessing the arrays,
  // indices start from 1 !!!!!

  // ****** access to Djangoh common-blocks

  // ****** /LUJETS/

  Lujets_t*   GetLujets        ()           { return fLujets; }
  int         GetN             ()           { return fLujets->N; }
  int         GetNPAD          ()           { return fLujets->NPAD; }
  int         GetK(int ip, int i)           { return fLujets->K[i-1][ip-1]; }
  double      GetP(int ip, int i)           { return fLujets->P[i-1][ip-1]; }
  double      GetV(int ip, int i)           { return fLujets->V[i-1][ip-1]; }

  void        SetN     (int n)              { fLujets->N = n;    }
  void        SetNPAD  (int n)              { fLujets->NPAD = n;    }
  void        SetK(int ip, int i, int k)    { fLujets->K[i-1][ip-1] = k; }
  void        SetP(int ip, int i, double p) { fLujets->P[i-1][ip-1] = p;    }
  void        SetV(int ip, int i, double v) { fLujets->V[i-1][ip-1] = v;    }

  // ****** /LUDAT1/

  Ludat1_t*   GetLudat1   () { return fLudat1; }
  int         GetMSTU(int i) { return fLudat1->MSTU[i-1]; }
  double      GetPARU(int i) { return fLudat1->PARU[i-1]; }
  int         GetMSTJ(int i) { return fLudat1->MSTJ[i-1]; }
  double      GetPARJ(int i) { return fLudat1->PARJ[i-1]; }

  void        SetMSTU(int i, int m   ) { fLudat1->MSTU[i-1] = m; }
  void        SetPARU(int i, double p) { fLudat1->PARU[i-1] = p; }
  void        SetMSTJ(int i, int m   ) { fLudat1->MSTJ[i-1] = m; }
  void        SetPARJ(int i, double p) { fLudat1->PARJ[i-1] = p; }

  // ****** /LUDAT2/

  Ludat2_t*   GetLudat2           () { return fLudat2; }
  int         GetKCHG(int ip, int i) { return fLudat2->KCHG[i-1][ip-1]; }
  double      GetPMAS(int ip, int i) { return fLudat2->PMAS[i-1][ip-1]; }
  double      GetPARF        (int i) { return fLudat2->PARF[i-1]; }
  double      GetVCKM(int i,  int j) { return fLudat2->VCKM[j-1][i-1]; }

  void        SetKCHG(int ip, int i, int k   ) { fLudat2->KCHG[i-1][ip-1] = k; }
  void        SetPMAS(int ip, int i, double m) { fLudat2->PMAS[i-1][ip-1] = m; }
  void        SetPARF        (int i, double p) { fLudat2->PARF[i-1]       = p; }
  void        SetVCKM (int i, int j, double v) { fLudat2->VCKM[j-1][i-1]  = v; }

  // ****** TDJANGOH routines

  void             GenerateEvent();
  void             Djrun();

  void             Initialize(const char *beam, int nuc_A, int nuc_Z, float beam_e, float nuc_e, float pol=0);
  void             Initialize_File(const char *beam, int PID, int nuc_A, int nuc_Z, float beam_e, float nuc_e, float pol);

  Int_t            ImportParticles(TClonesArray *particles, Option_t *option="");
  TObjArray       *ImportParticles(Option_t *option="");

  //void             OpenFortranFile(int lun, char* name);
  //void             CloseFortranFile(int lun);
  void             Clean_File();

  void             Lulist(int flag);
};

#endif
