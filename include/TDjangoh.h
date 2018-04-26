//==============================================================================
/*!

    \file                         TDjangoh.h
    \brief                        Interface class to Djangoh generator
    \author                       Nicolas PIERRE
    \version                      1.3
    \date                         09/04/18
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
  int        fUnfragSave; /*!< Flag enabling saving of unfragmented final state */

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
  * \return Number of particles
  */
  Int_t            ImportParticles(TClonesArray *particles, Option_t *option="");
  /*!
  * \brief Patchy method to recover final state for (quasi)elastic events
  */
  void             ElasFS();
  /*!
  * \brief Patchy method to recover final state for radiative (quasi)elastic events
  */
  void             RadElasFS();

  // ---------------------------------------------------------------------------
  // Cross-Section

  /*!
  * \brief Access total Cross-Section
  * \return Total Cross-Section
  */
  double      GetSigtot();

  /*!
  * \brief Access error on Cross-Section
  * \return Error on Cross-Section
  */
  double      GetSigtrr();

  // ---------------------------------------------------------------------------
  // Djangoh inputs accessorsmodifiers

  /*!
  * \brief Access beam particle type
  * \return Beam particle type
  */
  int         GetBeamType();
  /*!
  * \brief Set beam particle type
  * \param pvalue : Particle ID
  */
  void        SetBeamType(int pvalue);
  /*!
  * \brief Set beam particle type
  * \param pvalue : Particle name (eg. e-)
  */
  void        SetBeamType(const char* pvalue);
  /*!
  * \brief Set beam energy and polarization
  * \param pvalue : Beam energy
  * \param pvalue : Beam polarization
  */
  void        SetBeam(double pBeamE, double pPol);
  /*!
  * \brief Access beam polarization
  * \return Beam polarization
  */
  double      GetBeamPolar();
  /*!
  * \brief Set beam polarization
  * \param pvalue : Beam polarization
  */
  double      SetBeamPolar(double pvalue);
  /*!
  * \brief Access kinematical cuts
  * \param i : Kinematical cut number (in the order as in the manual)
  * \return Cut value
  */
  double      GetKinemCut(int i);
  /*!
  * \brief Set kinematical cuts
  * \param pvalue : Cut value
  * \param i : Kinematical cut number (in the order as in the manual)
  */
  void        SetKinemCut(double pvalue, int i);
  /*!
  * \brief Access cross-section grid values
  * \param i : Cross-section grid value number (in the order as in the manual)
  * \return Cross-section grid value
  */
  double      GetGdOpt(int i);
  /*!
  * \brief Set cross-section grid values
  * \param pvalue : Cross-section grid value
  * \param i : Cross-section grid value number (in the order as in the manual)
  */
  void        SetGdOpt(double pvalue, int i);
  /*!
  * \brief Access EW parameters values
  * \param i : EW parameter value number (in the order as in the manual)
  * \return EW parameter value
  */
  int         GetGsw(int i);
  /*!
  * \brief Set EW parameters values
  * \param pvalue : EW parameter value
  * \param i : EW parameter value number (in the order as in the manual)
  */
  void        SetGsw(int pvalue, int i);
  /*!
  * \brief Access cutoff energy value
  * \return Cutoff energy value
  */
  double      GetEgamMin();
  /*!
  * \brief Set cutoff energy value
  * \param pvalue : Cutoff energy value
  */
  void        SetEgamMin(double pvalue);
  /*!
  * \brief Access contribution to NC interactions values
  * \param i : Contribution value number (in the order as in the manual)
  * \return Contribution value
  */
  int         GetIntOptNC(int i);
  /*!
  * \brief Set contribution to NC interactions values
  * \param pvalue : Contribution value
  * \param i : Contribution value number (in the order as in the manual)
  */
  void        SetIntOptNC(int pvalue, int i);
  /*!
  * \brief Set inclusion of NC cross-section to sampling values
  * \param pvalue : Inclusion value
  * \param i : Inclusion value number (in the order as in the manual)
  */
  void        SetSamOptNC(int pvalue, int i);
  /*!
  * \brief Access parametrization of structure functions values
  * \param i : Parameter value number (in the order as in the manual)
  * \return Parameter value
  */
  int         GetStructFunc(int i);
  /*!
  * \brief Set parametrization of structure functions values
  * \param pvalue : Parameter value
  * \param i : Parameter value number (in the order as in the manual)
  */
  void        SetStructFunc(int pvalue, int i);
  /*!
  * \brief Access low W limit for Sophia value
  * \return Low W limit value
  */
  double      GetSophia();
  /*!
  * \brief Set low W limit for Sophia value
  * \param pvalue : Low W limit value
  */
  void        SetSophia(double pvalue);
  /*!
  * \brief Get verbose value
  * \return Verbose value
  */
  int         GetVerbose();
  /*!
  * \brief Set verbose value
  * \param pvalue : Verbose value
  */
  void        SetVerbose(int pvalue);
  /*!
  * \brief Access contribution to CC interactions values
  * \param i : Contribution value number (in the order as in the manual)
  * \return Contribution value
  */
  int         GetIntOptCC(int i);
  /*!
  * \brief Set contribution to CC interactions values
  * \param pvalue : Contribution value
  * \param i : Contribution value number (in the order as in the manual)
  */
  void        SetIntOptCC(int pvalue, int i);
  /*!
  * \brief Set inclusion of CC cross-section to sampling values
  * \param pvalue : Inclusion value
  * \param i : Inclusion value number (in the order as in the manual)
  */
  void        SetSamOptCC(int pvalue, int i);
  /*!
  * \brief Set nucleus properties values
  * \param pHpolar : Polarisation of the nucleus
  * \param pHna : A of the nucleus
  * \param pHnz : Z of the nucleus
  * \param pEpro : Energy of the nucleus
  */
  void        SetNucleus(double pHpolar, int pHna, int pHnz, double pEpro=0);
  /*!
  * \brief Get nucleus ID (PDG numbering scheme)
  * \return Nucleus ID
  */
  int         GetNucleusID();

  // ---------------------------------------------------------------------------
  // Final State utilitaries
  /*!
  * \brief Save final state infos in file
  */
  void        WriteFSInFile();
  /*!
  * \brief Save unfragmented final state infos in file
  */
  void        SaveUnfragState();
  /*!
  * \brief Remove final state file
  */
  void        CleanFSFile() { remove("finalState.txt"); }
  /*!
  * \brief Remove final state file
  */
  void        CleanUSFile() { remove("unfragState.txt"); }

  // ---------------------------------------------------------------------------
  // Accessors for MC for (quasi)elastic case (for more infos, see manual)
  /*!
  * \brief Access PHEP content of event
  * \param ip : Column index
  * \param i : row index
  * \return PHEP(ip,i)
  */
  double      GetPHEP(int ip, int i);
  /*!
  * \brief Access VHKK content of event
  * \param ip : Column index
  * \param i : row index
  * \return VHKK(ip,i)
  */
  double      GetVHKK(int ip, int i);
  /*!
  * \brief Access IDPHEP content of event
  * \param i : index
  * \return IDPHEP(i)
  */
  int         GetIDPHEP(int i);
  /*!
  * \brief Access sampling channel from Djangoh
  * \return Sampling channel number
  */
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
  * \brief Access K content of event
  * \param ip : column index
  * \param i : row index
  * \return K(ip,i)
  */
  int         GetK(int ip, int i)           { return fLujets->K[i-1][ip-1]; }
  /*!
  * \brief Access P content of event
  * \param ip : column index
  * \param i : row index
  * \return P(ip,i)
  */
  double      GetP(int ip, int i)           { return fLujets->P[i-1][ip-1]; }
  /*!
  * \brief Access V content of event
  * \param ip : column index
  * \param i : row index
  * \return V(ip,i)
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
  * \brief Set K content of event
  * \param ip : column index
  * \param i : row index
  * \param k : value of K(ip,i)
  */
  void        SetK(int ip, int i, int k)    { fLujets->K[i-1][ip-1] = k; }
  /*!
  * \brief Set P content of event
  * \param ip : column index
  * \param i : row index
  * \param p : value of P(ip,i)
  */
  void        SetP(int ip, int i, double p) { fLujets->P[i-1][ip-1] = p;    }
  /*!
  * \brief Set V content of event
  * \param ip : column index
  * \param i : row index
  * \param v : value of V(ip,i)
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
