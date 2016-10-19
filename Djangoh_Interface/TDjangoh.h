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
  // Djangoh common-blocks
  Lujets_t*  fLujets;
  Ludat1_t*  fLudat1;
  Ludat2_t*  fLudat2;
  Ludat3_t*  fLudat3;
  Ludat4_t*  fLudat4;
  Ludatr_t*  fLudatr;
  // Cleanup
  class  TDjangohCleaner {
  public:
     TDjangohCleaner();
     ~TDjangohCleaner();
  };
  friend class TDjangohCleaner;

  TDjangoh(const TDjangoh&);            // Cannot be copied
  TDjangoh& operator=(const TDjangoh&); // Cannot be copied

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

  // ****** /LUDAT3/

  Ludat3_t*   GetLudat3() { return fLudat3; }
  int         GetMDCY(int i, int j) { return fLudat3->MDCY[j-1][i-1]; }
  int         GetMDME(int i, int j) { return fLudat3->MDME[j-1][i-1]; }
  double      GetBRAT       (int i) { return fLudat3->BRAT[i-1]; }
  int         GetKFDP(int i, int j) { return fLudat3->KFDP[j-1][i-1]; }

  void        SetMDCY(int i, int j, int m) { fLudat3->MDCY[j-1][i-1] = m; }
  void        SetMDME(int i, int j, int m) { fLudat3->MDME[j-1][i-1] = m; }
  void        SetBRAT(int i, double b)     { fLudat3->BRAT[i-1]      = b; }
  void        SetKFDP(int i, int j, int k) { fLudat3->KFDP[j-1][i-1] = k; }

  // ****** /LUDAT4/

  Ludat4_t*   GetLudat4() { return fLudat4; }

  // ****** /LUDATR/ - random number generator info

  Ludatr_t*   GetLudatr   () { return fLudatr; }
  int         GetMRLU(int i) { return fLudatr->MRLU[i-1]; }
  double      GetRRLU(int i) { return fLudatr->RRLU[i-1]; }

  void        SetMRLU(int i, int m)    { fLudatr->MRLU[i-1] = m; }
  void        SetRRLU(int i, double r) { fLudatr->RRLU[i-1] = r; }

  // ****** TDJANGOH routines

  void             GenerateEvent();
  void             Djrun();

  void             Initialize(const char *beam, int nuc_A, int nuc_Z, float beam_e, float nuc_e, float pol=0);
  void             Initialize_File(int PID, int nuc_A, int nuc_Z, float beam_e, float nuc_e, float pol);

  Int_t            ImportParticles(TClonesArray *particles, Option_t *option="");
  TObjArray       *ImportParticles(Option_t *option="");

  //void             OpenFortranFile(int lun, char* name);
  //void             CloseFortranFile(int lun);
  void             Clean_File();

  void             Lulist(int flag);

  ClassDef(TDjangoh,0)  //Interface to Djangoh Event Generator
};

#endif
