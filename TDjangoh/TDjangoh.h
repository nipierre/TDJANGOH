//==============================================================================
/*!

    \file                         TDjangoh.h
    \brief                        Interface class to Djangoh generator
    \author                       Nicolas PIERRE
    \version                      0.1
    \date                         28/10/16
    Support :                     mail to : nicolas.pierre@cern.ch

    TDjangoh is an interface class to DJANGOH

    To use this class you must install a modified version of djangoh.
    Find the modified version at :
      Github/GitLab
    Or contact : nicolas.pierre@cern.ch

    Djangoh event generator, written by H.Spiesberger.
    For the details about the generator look at djangoh manual :
      Spiesberger address

*/
//==============================================================================

#ifndef DJANGOH_TDjangoh
#define DJANGOH_TDjangoh

#ifndef ROOT_TDjangohCalls
#include "TDjangohCalls.h"
#endif

#ifndef ROOT_TGenerator
#include "TGenerator.h"
#endif

#ifndef ROOT_TObjArray
#include "TObjArray.h"
#endif

#include <string>

using namespace std;


/*!
* \class TDjangoh
* \brief C/C++ Interface to Djangoh
*/
class TDjangoh : public TGenerator {

protected:

  static  TDjangoh* fgInstance; /*!< Instance of TDjangoh. Only one at a time permitted */

  Lujets_t*  fLujets; /*!< Container of the lujets_ common block */
  Ludat1_t*  fLudat1; /*!< Container of the ludat1_ common block. For the moment not used due to 32/64 incomp */
  Ludat2_t*  fLudat2; /*!< Container of the ludat2_ common block. For the moment not used due to 32/64 incomp */

  /*!
  * \class TDjangohCleaner
  * \brief A cleaner for TDjangoh
  */
  class  TDjangohCleaner {
  public:
    /*!
    * \brief Constructor of the TDjangohCleaner class
    */
    TDjangohCleaner();
    /*!
    * \brief Destructor of the TDjangohCleaner class
    */
    ~TDjangohCleaner();
  };
  friend class TDjangohCleaner;

  /*!
  * \brief Copy Constructor of the TDjangoh class
  * \param TDjangoh : TDjangoh object
  */
  TDjangoh(const TDjangoh&);            // Cannot be copied
  /*!
  * \brief = operator overload
  * \param TDjangoh : TDjangoh object
  */
  TDjangoh& operator=(const TDjangoh&); // Cannot be copied


public:

  //----------------------------------------------------------------------------
  // All variables hard-coded for the Djangoh input file -> copy djangoh man
  //----------------------------------------------------------------------------

  string outfilename = "TDjangoh";/*!< Instance of TDjangoh. Only one at a time permitted */
  int iounits[3] = {1,1,1};/*!< Instance of TDjangoh. Only one at a time permitted */
  string pr_beam[2] = {"0D0","0D0"};/*!< Instance of TDjangoh. Only one at a time permitted */
  int gsw_param[11] = {2,1,3,1,0,0,2,1,1,1,1};/*!< Instance of TDjangoh. Only one at a time permitted */
  int kinem_cut_var = 3;/*!< Instance of TDjangoh. Only one at a time permitted */
  string kinem_cut[7] = {"0.0001D0","1.00D0","0.01D0","0.95D0","1.0D0","1D5","1.4D0"};/*!< Instance of TDjangoh. Only one at a time permitted */
  string egam_min = "0D0";/*!< Instance of TDjangoh. Only one at a time permitted */
  int int_opt_nc[9] = {1, 18, 18, 18, 0, 0, 0, 0, 0};/*!< Instance of TDjangoh. Only one at a time permitted */
  int int_opt_cc[4] = {0, 0, 0, 0};/*!< Instance of TDjangoh. Only one at a time permitted */
  int int_only = 0;/*!< Instance of TDjangoh. Only one at a time permitted */
  int int_point = 3000;/*!< Instance of TDjangoh. Only one at a time permitted */
  int sam_opt_nc[9] = {1, 1, 1, 1, 0, 0, 0, 0, 0};/*!< Instance of TDjangoh. Only one at a time permitted */
  int sam_opt_cc[4] = {0, 0, 0, 0};/*!< Instance of TDjangoh. Only one at a time permitted */
  string nucl_e = "0.0D0";/*!< Instance of TDjangoh. Only one at a time permitted */
  int structfunc[3] = {0, 2, 10150};/*!< Instance of TDjangoh. Only one at a time permitted */
  float flong[3] = {111, 0.01, 0.03};/*!< Instance of TDjangoh. Only one at a time permitted */
  float alfas[4] = {1, 1, 0.20, 0.235};/*!< Instance of TDjangoh. Only one at a time permitted */
  int nflavors[2] = {0, 3};/*!< Instance of TDjangoh. Only one at a time permitted */
  int rndm_seeds[2] = {-1, -1};/*!< Instance of TDjangoh. Only one at a time permitted */
  int start = 1;/*!< Instance of TDjangoh. Only one at a time permitted */
  float sophia = 1.5;/*!< Instance of TDjangoh. Only one at a time permitted */
  int out_lep = 1;/*!< Instance of TDjangoh. Only one at a time permitted */
  int frag = 1;/*!< Instance of TDjangoh. Only one at a time permitted */
  int cascades = 12;/*!< Instance of TDjangoh. Only one at a time permitted */
  int max_virt = 5;/*!< Instance of TDjangoh. Only one at a time permitted */


  /*!
  * \brief Constructor of the TDjangoh class
  */
  TDjangoh();
  /*!
  * \brief Destructor of the TDjangoh class
  */
  virtual ~TDjangoh();
  /*!
  * \brief Instance creation
  */
  static TDjangoh *Instance();
  /*!
  * \brief Generation of an event
  */
  void             GenerateEvent();
  /*!
  * \brief Initialization of an event
  * \param beam : Type of particle for the beam
  * \param nuc_A : A number of the target nucleus
  * \param nuc_Z : Z number of the target nucleus
  * \param beam_e : Energy of the beam (in GeV)
  * \param nuc_e : Energy of the target (in GeV, 0 for fixed target)
  * \param pol : Polarization of the target
  */
  void             Initialize(const char *beam, int nuc_A, int nuc_Z, float beam_e, float nuc_e, float pol=0);
  /*!
  * \brief Initialization of the input file
  * \param beam : Type of particle for the beam
  * \param PID : PID of beam particle
  * \param nuc_A : A number of the target nucleus
  * \param nuc_Z : Z number of the target nucleus
  * \param beam_e : Energy of the beam (in GeV)
  * \param nuc_e : Energy of the target (in GeV, 0 for fixed target)
  * \param pol : Polarization of the target
  */
  void             Initialize_File(const char *beam, int PID, int nuc_A, int nuc_Z, float beam_e, float nuc_e, float pol);
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \param particles : Array of particles
  * \param option :
  * \return nparts : number of particles
  */
  Int_t            ImportParticles(TClonesArray *particles, Option_t *option="");
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \param option :
  * \return : TObjArray w/ particles
  */
  TObjArray       *ImportParticles(Option_t *option="");
  /*!
  * \brief Clean files created by djangoh
  */
  void             Clean_File();
  /*!
  * \brief lulist_ routine
  */
  void             Lulist(int flag);


  // ---------------------------------------------------------------------------
  // ACCESSORS
  // FORTRAN indexing in accessing the arrays, indices start from 1 !!
  // ---------------------------------------------------------------------------


  // ---------------------------------------------------------------------------
  // /LUJETS/
  /*!
  * \brief Recover the actual Lujets_t structure
  * \return : fLujets
  */
  Lujets_t*   GetLujets        ()           { return fLujets; }
  /*!
  * \brief Recover the number of particles
  * \return : N
  */
  int         GetN             ()           { return fLujets->N; }
  /*!
  * \brief Recover
  * \return : NPAD
  */
  int         GetNPAD          ()           { return fLujets->NPAD; }
  /*!
  * \brief Recover the K[i-1][ip-1] element
  * \param ip : column index
  * \param i : line index
  * \return : K[i-1][ip-1]
  */
  int         GetK(int ip, int i)           { return fLujets->K[i-1][ip-1]; }
  /*!
  * \brief Recover the P[i-1][ip-1] element
  * \param ip : column index
  * \param i : line index
  * \return : P[i-1][ip-1]
  */
  double      GetP(int ip, int i)           { return fLujets->P[i-1][ip-1]; }
  /*!
  * \brief Recover the V[i-1][ip-1] element
  * \param ip : column index
  * \param i : line index
  * \return : V[i-1][ip-1]
  */
  double      GetV(int ip, int i)           { return fLujets->V[i-1][ip-1]; }
  /*!
  * \brief Set the number of particles
  * \param n : number of particles
  */
  void        SetN     (int n)              { fLujets->N = n;    }
  /*!
  * \brief Recover
  * \param n : NPAD
  */
  void        SetNPAD  (int n)              { fLujets->NPAD = n;    }
  /*!
  * \brief Recover the K[i-1][ip-1] element
  * \param ip : column index
  * \param i : line index
  * \param k : value of K[i-1][ip-1]
  */
  void        SetK(int ip, int i, int k)    { fLujets->K[i-1][ip-1] = k; }
  /*!
  * \brief Recover the P[i-1][ip-1] element
  * \param ip : column index
  * \param i : line index
  * \param k : value of P[i-1][ip-1]
  */
  void        SetP(int ip, int i, double p) { fLujets->P[i-1][ip-1] = p;    }
  /*!
  * \brief Recover the V[i-1][ip-1] element
  * \param ip : column index
  * \param i : line index
  * \param k : value of V[i-1][ip-1]
  */
  void        SetV(int ip, int i, double v) { fLujets->V[i-1][ip-1] = v;    }


  // ---------------------------------------------------------------------------
  // /LUDAT1/ (NOT USED)
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \return : TObjArray w/ particles
  */
  Ludat1_t*   GetLudat1   () { return fLudat1; }
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \return : TObjArray w/ particles
  */
  int         GetMSTU(int i) { return fLudat1->MSTU[i-1]; }
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \return : TObjArray w/ particles
  */
  double      GetPARU(int i) { return fLudat1->PARU[i-1]; }
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \return : TObjArray w/ particles
  */
  int         GetMSTJ(int i) { return fLudat1->MSTJ[i-1]; }
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \return : TObjArray w/ particles
  */
  double      GetPARJ(int i) { return fLudat1->PARJ[i-1]; }
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \return : TObjArray w/ particles
  */
  void        SetMSTU(int i, int m   ) { fLudat1->MSTU[i-1] = m; }
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \return : TObjArray w/ particles
  */
  void        SetPARU(int i, double p) { fLudat1->PARU[i-1] = p; }
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \return : TObjArray w/ particles
  */
  void        SetMSTJ(int i, int m   ) { fLudat1->MSTJ[i-1] = m; }
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \return : TObjArray w/ particles
  */
  void        SetPARJ(int i, double p) { fLudat1->PARJ[i-1] = p; }


  // ---------------------------------------------------------------------------
  // /LUDAT2/ (NOT USED)
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \return : TObjArray w/ particles
  */
  Ludat2_t*   GetLudat2           () { return fLudat2; }
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \return : TObjArray w/ particles
  */
  int         GetKCHG(int ip, int i) { return fLudat2->KCHG[i-1][ip-1]; }
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \return : TObjArray w/ particles
  */
  double      GetPMAS(int ip, int i) { return fLudat2->PMAS[i-1][ip-1]; }
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \return : TObjArray w/ particles
  */
  double      GetPARF        (int i) { return fLudat2->PARF[i-1]; }
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \return : TObjArray w/ particles
  */
  double      GetVCKM(int i,  int j) { return fLudat2->VCKM[j-1][i-1]; }
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \return : TObjArray w/ particles
  */
  void        SetKCHG(int ip, int i, int k   ) { fLudat2->KCHG[i-1][ip-1] = k; }
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \return : TObjArray w/ particles
  */
  void        SetPMAS(int ip, int i, double m) { fLudat2->PMAS[i-1][ip-1] = m; }
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \return : TObjArray w/ particles
  */
  void        SetPARF        (int i, double p) { fLudat2->PARF[i-1]       = p; }
  /*!
  * \brief Import particles from lujets_ subroutine and copy it in TClonesArray*
  * \return : TObjArray w/ particles
  */
  void        SetVCKM (int i, int j, double v) { fLudat2->VCKM[j-1][i-1]  = v; }

};

#endif
