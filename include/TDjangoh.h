//==============================================================================
/*!

    \file                         TDjangoh.h
    \brief                        Interface class to Djangoh generator
    \author                       Nicolas PIERRE
    \version                      0.1
    \date                         28/10/16
    Support :                     mail to : nicolas.pierre@cern.ch

    TDjangoh is an interface class to DJANGOH

    Contact : nicolas.pierre@cern.ch

    Djangoh event generator, written by H.Spiesberger.
    For the details about the generator look at djangoh manual :
      http://wwwthep.physik.uni-mainz.de/~hspiesb/djangoh/djangoh.html

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
  Djkin_t*   fDjkin;

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
  * \brief Initialization of DJANGOH
  * \param beam : Type of particle for the beam
  * \param nuc_A : A number of the target nucleus
  * \param nuc_Z : Z number of the target nucleus
  * \param beam_e : Energy of the beam (in GeV)
  * \param nuc_e : Energy of the target (in GeV, 0 for fixed target)
  * \param pol : Polarization of the target
  */
  void             Initialize(const string pFilename);
  /*!
  * \brief Configuration of DJANGOH
  * \param beam_e : Energy of the beam (in GeV)
  */
  void             Configure(float beam_e, float pol);
  /*!
  * \brief Print a recap of the generation made
  */
  void             EndRecap();
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
  * \return TObjArray w/ particles
  */
  TObjArray       *ImportParticles(Option_t *option="");
  /*!
  * \brief Clean files created by djangoh
  */
  void             Clean_File();


  // ---------------------------------------------------------------------------
  // ACCESSORS
  // FORTRAN indexing in accessing the arrays, indices start from 1 !!
  // ---------------------------------------------------------------------------


  // ---------------------------------------------------------------------------
  // /LUJETS/
  /*!
  * \brief Recover the actual Lujets_t structure
  * \return fLujets
  */
  Lujets_t*   GetLujets        ()           { return fLujets; }
  /*!
  * \brief Recover the number of particles
  * \return N
  */
  int         GetN             ()           { return fLujets->N; }
  /*!
  * \brief Recover
  * \return NPAD
  */
  int         GetNPAD          ()           { return fLujets->NPAD; }
  /*!
  * \brief Recover the K[i-1][ip-1] element
  * \param ip : column index
  * \param i : line index
  * \return K[i-1][ip-1]
  */
  int         GetK(int ip, int i)           { return fLujets->K[i-1][ip-1]; }
  /*!
  * \brief Recover the P[i-1][ip-1] element
  * \param ip : column index
  * \param i : line index
  * \return P[i-1][ip-1]
  */
  double      GetP(int ip, int i)           { return fLujets->P[i-1][ip-1]; }
  /*!
  * \brief Recover the V[i-1][ip-1] element
  * \param ip : column index
  * \param i : line index
  * \return V[i-1][ip-1]
  */
  double      GetV(int ip, int i)           { return fLujets->V[i-1][ip-1]; }
  /*!
  * \brief Set the number of particles
  * \param n : number of particles
  */
  void        SetN     (int n)              { fLujets->N = n;    }
  /*!
  * \brief Set NPAD
  * \param n : NPAD
  */
  void        SetNPAD  (int n)              { fLujets->NPAD = n;    }
  /*!
  * \brief Set the K[i-1][ip-1] element
  * \param ip : column index
  * \param i : line index
  * \param k : value of K[i-1][ip-1]
  */
  void        SetK(int ip, int i, int k)    { fLujets->K[i-1][ip-1] = k; }
  /*!
  * \brief Set the P[i-1][ip-1] element
  * \param ip : column index
  * \param i : line index
  * \param p : value of P[i-1][ip-1]
  */
  void        SetP(int ip, int i, double p) { fLujets->P[i-1][ip-1] = p;    }
  /*!
  * \brief Set the V[i-1][ip-1] element
  * \param ip : column index
  * \param i : line index
  * \param v : value of V[i-1][ip-1]
  */
  void        SetV(int ip, int i, double v) { fLujets->V[i-1][ip-1] = v;    }


  // ---------------------------------------------------------------------------
  // /LUDAT1/ (NOT USED)
  /*!
  * \brief Recover the actual Ludat1_t structure
  * \return fLudat1
  */
  Ludat1_t*   GetLudat1   () { return fLudat1; }
  /*!
  * \brief Recover the i element of MSTU
  * \param i : element index
  * \return MSTU[i-1]
  */
  int         GetMSTU(int i) { return fLudat1->MSTU[i-1]; }
  /*!
  * \brief Recover the i element of PARU
  * \param i : element index
  * \return PARU[i-1]
  */
  double      GetPARU(int i) { return fLudat1->PARU[i-1]; }
  /*!
  * \brief Recover the i element of MSTJ
  * \param i : element index
  * \return MSTJ[i-1]
  */
  int         GetMSTJ(int i) { return fLudat1->MSTJ[i-1]; }
  /*!
  * \brief Recover the i element of PARJ
  * \param i : element index
  * \return PARJ[i-1]
  */
  double      GetPARJ(int i) { return fLudat1->PARJ[i-1]; }
  /*!
  * \brief Set the i element of MSTU
  * \param i : element index
  * \param m : value of MSTU[i-1]
  */
  void        SetMSTU(int i, int m   ) { fLudat1->MSTU[i-1] = m; }
  /*!
  * \brief Set the i element of PARU
  * \param i : element index
  * \param p : value of PARU[i-1]
  */
  void        SetPARU(int i, double p) { fLudat1->PARU[i-1] = p; }
  /*!
  * \brief Set the i element of MSTJ
  * \param i : element index
  * \param m : value of MSTJ[i-1]
  */
  void        SetMSTJ(int i, int m   ) { fLudat1->MSTJ[i-1] = m; }
  /*!
  * \brief Set the i element of PARJ
  * \param i : element index
  * \param p : value of PARJ[i-1]
  */
  void        SetPARJ(int i, double p) { fLudat1->PARJ[i-1] = p; }


  // ---------------------------------------------------------------------------
  // /LUDAT2/ (NOT USED)
  /*!
  * \brief Recover the actual Ludat1_t structure
  * \return fLudat2
  */
  Ludat2_t*   GetLudat2           () { return fLudat2; }
  /*!
  * \brief Recover the KCHG[i-1][ip-1] element
  * \param ip : column index
  * \param i : line index
  * \return KCHG[i-1][ip-1]
  */
  int         GetKCHG(int ip, int i) { return fLudat2->KCHG[i-1][ip-1]; }
  /*!
  * \brief Recover the PMAS[i-1][ip-1] element
  * \param ip : column index
  * \param i : line index
  * \return PMAS[i-1][ip-1]
  */
  double      GetPMAS(int ip, int i) { return fLudat2->PMAS[i-1][ip-1]; }
  /*!
  * \brief Recover the i element of PARF
  * \param i : element index
  * \return PARF[i-1]
  */
  double      GetPARF        (int i) { return fLudat2->PARF[i-1]; }
  /*!
  * \brief Recover the VCKM[i-1][j-1] element
  * \param j : column index
  * \param i : line index
  * \return VCKM[i-1][j-1]
  */
  double      GetVCKM(int i,  int j) { return fLudat2->VCKM[j-1][i-1]; }
  /*!
  * \brief Set the KCHG[i-1][ip-1] element
  * \param ip : column index
  * \param i : line index
  * \param k : value of KCHG[i-1][ip-1]
  */
  void        SetKCHG(int ip, int i, int k   ) { fLudat2->KCHG[i-1][ip-1] = k; }
  /*!
  * \brief Set the PMAS[i-1][ip-1] element
  * \param ip : column index
  * \param i : line index
  * \param m : value of PMAS[i-1][ip-1]
  */
  void        SetPMAS(int ip, int i, double m) { fLudat2->PMAS[i-1][ip-1] = m; }
  /*!
  * \brief Set the i element of PARF
  * \param i : element index
  * \param p : value of PARF[i-1]
  */
  void        SetPARF        (int i, double p) { fLudat2->PARF[i-1]       = p; }
  /*!
  * \brief Set the VCKM[i-1][j-1] element
  * \param j : column index
  * \param i : line index
  * \param v : value of VCKM[i-1][j-1]
  */
  void        SetVCKM (int i, int j, double v) { fLudat2->VCKM[j-1][i-1]  = v; }


  // ---------------------------------------------------------------------------
  // /DJKIN/

  Djkin_t*    GetDjkin           () { return fDjkin; }

  double      GetX() { return fDjkin->DJX; }

  double      GetY() { return fDjkin->DJY; }

  double      GetW2() { return fDjkin->DJW2; }

  double      GetQ2() { return fDjkin->DJQ2; }

  double      GetU() { return fDjkin->DJU; }

  double      GetXHAD() { return fDjkin->DJXHAD; }

  double      GetYHAD() { return fDjkin->DJYHAD; }

  double      GetQ2HAD() { return fDjkin->DJQ2HAD; }

  void        SetX(double x) { fDjkin->DJX = x; }

  void        SetY(double y) { fDjkin->DJY = y; }

  void        SetW2(double w) { fDjkin->DJW2 = w; }

  void        SetQ2(double q) { fDjkin->DJQ2 = q; }

  void        SetU(double u) { fDjkin->DJU = u; }

  void        SetXHAD(double x) { fDjkin->DJXHAD = x; }

  void        SetYHAD(double y) { fDjkin->DJYHAD = y; }

  void        SetQ2HAD(double q) { fDjkin->DJQ2HAD = q; }
};

#endif
