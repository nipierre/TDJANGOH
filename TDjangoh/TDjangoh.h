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

  char* lhapathi;
  int lhalen;

  //----------------------------------------------------------------------------
  // All variables hard-coded for the Djangoh input file -> copy djangoh man
  //----------------------------------------------------------------------------

  string outfilename = "TDjangoh";/*!< User defined heading of the first output page */
  int iounits[3] = {1,1,1};/*!< Logical numbers for in- and output */
  string pr_beam[2] = {"0D0","0D0"};/*!< Properties of the target/other beam */
  int gsw_param[11] = {2,1,3,1,0,0,2,1,1,1,1};/*!< Monitoring the definition of EW parameters and inclusion of virtual corrections :
                                                   - LPARIN(1) = 1 : EW param set w/ fixed values for bosons masses M_W, M_Z.
                                                               = 2 : EW param calculated from fixed M_W, M_Z (DEFAULT).
                                                   - LPARIN(2) = 0 : only Born cross-section w/o EM or W corrections is integrated.
                                                               = 1 : Born cross-section includes corrections.
                                                   - LPARIN(3)     : flag for inclusion of higher order contributions
                                                               = 0 : no higher order corrections.
                                                               ≥ 1 : terms of O(a^2.m_t^2) included.
                                                               ≥ 2 : terms of O(a.a_s.m_t^2) included.
                                                               ≥ 3 : running a(Q2) used for the radiative cross-section.
                                                   - LPARIN(4)     : leptonic QED corrections.
                                                   - LPARIN(5)     : quarkonic QED cprrections.
                                                   - LPARIN(6)     : lepton-quark interference.
                                                   - LPARIN(7)     : fermionic contributions to the photon self energy sigma^gamma.
                                                               = 0 : not included.
                                                               = 1 : parametrization w/ the help of the quark masses.
                                                               = 2 : parametrization from H. Burkhardt et al. (Polarization at LEP, CERN 88-06).
                                                   - LPARIN(8)     : fermionic contribution to the gamma-Z mixing.
                                                   - LPARIN(9)     : fermionic contribution to the self energy of the Z bosons.
                                                   - LPARIN(10)    : fermionic contribution to the self energy of the W bosons.
                                                   - LPARIN(11)    : purely weak contributions to self energy, vertex corrections and boxes. */
  int kinem_cut_var = 3;/*!< Define the number of kinematical cuts:
                             - 1 : cuts in x_l and lower cut in Q2_l (GeV^2), cuts in y_l ignored
                             - 2 : cuts in x_l, lower cut in Q2_l and lower cut in W_l, cuts in y_l ignored
                             - 3 : cuts in x_l, y_l, Q2_l and W_l */
  string kinem_cut[7] = {"0.0001D0","1.00D0","0.01D0","0.95D0",
                         "1.0D0","1D5","1.4D0"}; /*!< Kinematical cuts : XMIN, XMAX, YMIN, YMAX, Q2MIN, Q2MAX, WMIN*/
  string egam_min = "0D0";/*!< Definition of lower cutoff energy for bremsstrahlung photons (GeV) :
                               - =0.0 : Both radiative and non-radiative events are generated. The separation of the
                               the cross-section into rad/non-rad parts w/ the help of a soft-photon cutoff is determined
                               internally as a function of x and y. Used if you want both rad/non-rad integrated.
                               - >0.0 : Only hard-photon bremsstrahlung is considered in event sampling.*/
  int int_opt_nc[9] = {1, 18, 18, 18, 0, 0, 0, 0, 0};/*!< Instance of TDjangoh. Only one at a time permitted */
  int int_opt_cc[4] = {0, 0, 0, 0};/*!< Instance of TDjangoh. Only one at a time permitted */
  int int_only = 0;/*!< Instance of TDjangoh. Only one at a time permitted */
  int int_point = 3000;/*!< Number of integration points. */
  int sam_opt_nc[9] = {1, 1, 1, 1, 0, 0, 0, 0, 0};/*!< Instance of TDjangoh. Only one at a time permitted */
  int sam_opt_cc[4] = {0, 0, 0, 0};/*!< Instance of TDjangoh. Only one at a time permitted */
  string nucl_e = "0.0D0";/*!< Energy per nucleon in the target/beam (GeV). */
  int structfunc[3] = {0, 2, 10150};/*!< Defines the parametrization of parton densities or structure fonctions applied in the calculation.
                                         ILQMOD = 0 : use unmodified parton distribution functions for all Q2.
                                         ILQMOD = 1 : apply exponential low Q2 suppression factor to PDF. (DEFAULT)
                                         ILQMOD = 2 : for low Q2 (< 6 GeV^2) use F1/F2 parametrization from Brasse (W < 2 GeV) and Stein (W > 2 GeV).
                                                      For larger Q2, PDF from ICODE are used.
                                         ILQMOD = 3 : for Q2 < 6 GeV^2 use parametrization from Abramowicz, Levy, Levin and Maor (W > 2 GeV) and Brasse (W < 2 GeV).
                                                      If ICODE = 0, the ALLM param is used for high Q2. Else, PDF from ICODE are combined with ALLM param.
                                         ILQMOD = 4 : For x < 0.1 and nu > 10 GeV, use of Badelek and Kwiecinski parametrization.
                                                      For nu < 10 GeV and x < 0.1, use of Brass and Stein parametrizations.
                                                      At larger x, the PDF from MRS D'_ (RECOMMENDED USE W/ ILQMOD = 4 : ALWAYS USE W/ ICODE = 0).
                                         ILQMOD = 5 : Use of Donnachie-Landshoff parametrization for Q2 < 10 GeV^2 w/ PDF.
                                                      If ICODE = 0, use of MRS B235 (gives smallest discontinuities of the cross-section).
                                         ILIB = 1 : Choice of PDF from PYTHIA routine PYSTFU
                                         In that case :
                                           ICODE = 1 : Eichten et al. set 1.
                                                 = 2 : Eichten et al. set 2.
                                                 = 3 : Duke/Owens set 1.
                                                 = 4 : Duke/Owens set 2.
                                                 = 5 : CTEQ2M (best MSB fit).
                                                 = 6 : CTEQ2MS (singular at small x).
                                                 = 7 : CTEQ2MF (flat at small x).
                                                 = 8 : CTEQ2ML (large lambda).
                                                 = 9 : CTEQ2L (best LO fit).
                                                 = 10: CTEQ2D (best DIS fit).
                                         ILIB = 2 : Use of PDFLIB. (DEFAULT)
                                        In that case :
                                           ICODE = 1000*NGROUP+NSET, the identification code for the parametrization of PDF for PDFLIB (DEFAULT = 3041)*/
  float flong[3] = {111, 0.01, 0.03};/*!< For the inclusion of the longitudinal structure fonction F_l.
                                          IFLOPT = LQCD + LTM*10 + LHT*100 (QCD contributions, target mass effects and higher twists resp.).
                                          PARL11 : accuracy for the integration needed to calculate F_L.
                                          PARL19 : kappa^2, scale param for higher twists contributions in F_l.*/
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
  void             Data_default();
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
  /*!
  * \brief lulist_ routine
  */
  void             Lulist(int flag) {}


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
