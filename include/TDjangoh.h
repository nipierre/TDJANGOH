//==============================================================================
/*!

    \file                         TDjangoh.h
    \brief                        Interface class to Djangoh generator
    \author                       Nicolas PIERRE
    \version                      1.1
    \date                         14/06/17
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
  Djkin_t*   fDjkin; /*!< Container of the djkin_ common block */

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
  */
  void             Initialize();
  /*!
  * \brief Reading of parameters in XML file
  * \param pFilename : XML file path
  */
  void             ReadXMLFile(const string pFilename);

  void             WriteXMLFile(const string pFilename);
  /*!
  * \brief Modification of kinematical cuts
  * \param pcut : types of cut (see djangoh manual for further infos)
  * \param pxmin : x lower cut
  * \param pxmax : x higher cut
  * \param pymin : y lower cut
  * \param pymax : y higher cut
  * \param pq2min : Q2 lower cut
  * \param pq2max : Q2 higher cut
  * \param pwmin : W lower cut
  */
  void             ModKineCuts(int pcut, double pxmin, double pxmax,
                               double pymin, double pymax, double pq2min,
                               double pq2max, double pwmin);
  /*!
  * \brief Configuration for Born w/o quasielastic contribution generation
  */
  void             BornWOqelNC();
  /*!
  * \brief Configuration for leptonic RC w/o quasielastic contribution generation
  */
  void             RClepWOqelNC();
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

  // ---------------------------------------------------------------------------
  // Cross-Section

  // Total Cross-Section
  double      GetSigtot();

  // Error on total Cross-Section
  double      GetSigtrr();

  // ---------------------------------------------------------------------------
  // Djangoh inputs accessorsmodifiers

  int         GetBeamType();
  void        SetBeamType(int pvalue);
  void        SetBeamType(const char* pvalue);
  void        SetBeam(double pBeamE, double pPol);

  double      GetBeamPolar();
  double      SetBeamPolar(double pvalue);

  double      GetKinemCut(int i);
  void        SetKinemCut(double pvalue, int i);

  double      GetGdOpt(int i);
  void        SetGdOpt(double pvalue, int i);

  int         GetGsw(int i);
  void        SetGsw(int pvalue, int i);

  double      GetEgamMin();
  void        SetEgamMin(double pvalue);

  int         GetIntOptNC(int i);
  void        SetIntOptNC(int pvalue, int i);

  void        SetSamOptNC(int pvalue, int i);

  int         GetStructFunc(int i);
  void        SetStructFunc(int pvalue, int i);

  double      GetSophia();
  void        SetSophia(double pvalue);

  int         GetVerbose();
  void        SetVerbose(int pvalue);

  int         GetIntOptCC(int i);
  void        SetIntOptCC(int pvalue, int i);

  void        SetSamOptCC(int pvalue, int i);

  void        SetNucleus(double pHpolar, int pHna, int pHnz, double pEpro=0);

  // ---------------------------------------------------------------------------
  // Save final state infos in file
  void        WriteFSInFile();
  void        CleanFSFile() { remove("finalState.txt"); }

  // ---------------------------------------------------------------------------
  // Accessors for MC for (quasi)elastic case (for more infos, see manual)
  double      GetPHEP(int ip, int i);
  double      GetVHKK(int ip, int i);
  int         GetIDPHEP(int i);
  int         GetChannel();

  // ---------------------------------------------------------------------------
  // Cleaner
  /*!
  * \brief Clean files created by djangoh
  */
  void        Clean_File();


  // ---------------------------------------------------------------------------
  // FORTRAN ACCESSORS
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

  // ---------------------------------------------------------------------------

};

#endif
