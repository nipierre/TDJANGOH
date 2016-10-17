// Djangoh interface
// Author: Nicolas PIERRE   14/10/16

/* \class TDjangoh
   \ingroup djangoh

TDjangoh is an interface class to Djangoh

To use this class you must install a version of djangoh.
See the installation instructions at :
  Spiesberger address
Or contact : nicolas.pierre@cern.ch

Djangoh event generator, written by H.Spiesberger.
For the details about the generator look at djangoh manual:

\verbatim
\endverbatim
*/

#include "TDjangoh.h"
#include "TClonesArray.h"
#include "TMCParticle.h"
#include "TParticle.h"

TDjangoh*  TDjangoh::fgInstance = 0;


#ifndef WIN32
# define lulist lulist_
# ifdef DJANGOH_DOUBLE_UNDERSCORE
#  define tdjangoh_open_fortran_file tdjangoh_open_fortran_file__
#  define tdjangoh_close_fortran_file tdjangoh_close_fortran_file__
#  define djangoh_common_address djangoh_common_address__
# elif DJANGOH_SINGLE_UNDERSCORE
#  define tdjangoh_open_fortran_file tdjangoh_open_fortran_file_
#  define tdjangoh_close_fortran_file tdjangoh_close_fortran_file_
#  define djangoh_common_address djangoh_common_address
# else
#  define djangoh_common_address djangoh_common_address
#  define tdjangoh_open_fortran_file tdjangoh_open_fortran_file_
#  define tdjangoh_close_fortran_file tdjangoh_close_fortran_file_
# endif
# define type_of_call
#else
# define lulist LULIST
# define tdjangoh_open_fortran_file TDJANGOH_OPEN_FORTRAN_FILE
# define tdjangoh_close_fortran_file TDJANGOH_CLOSE_FORTRAN_FILE
# define type_of_call _stdcall
#endif

using namespace std;

extern "C" void type_of_call lulist(int *key);

extern "C" {
void*  djangoh_common_address(const char*);
void   type_of_call tdjangoh_open_fortran_file(int* lun, char* name, int);
void   type_of_call tdjangoh_close_fortran_file(int* lun);
}

ClassImp(TDjangoh)

/** \class TDjangoh::TDjangohCleaner
   \ingroup djangoh

Utility class to manage the TDjangoh instance
*/

TDjangoh::TDjangohCleaner::TDjangohCleaner() {
}

////////////////////////////////////////////////////////////////////////////////
///delete the TDjangoh6 instance

TDjangoh::TDjangohCleaner::~TDjangohCleaner() {
  if (TDjangoh::fgInstance) {
     delete TDjangoh::fgInstance;
     TDjangoh::fgInstance = 0;
  }
}

//------------------------------------------------------------------------------
//  constructor is not supposed to be called from the outside - only
//  Initialize() method
////////////////////////////////////////////////////////////////////////////////
/// TDjangoh constructor: creates a TClonesArray in which it will store all
/// particles. Note that there may be only one functional TDjangoh object
/// at a time, so it's not use to create more than one instance of it.

TDjangoh::TDjangoh() : TGenerator("TDjangoh","TDjangoh") {
  // Protect against multiple objects.   All access should be via the
  // Instance member function.
  if (fgInstance)
     Fatal("TDjangoh", "There's already an instance of TDjangoh");

  delete fParticles; // was allocated as TObjArray in TGenerator

  fParticles = new TClonesArray("TMCParticle",50);

  // initialize common-blocks
  // the functions/subroutines referenced by TPythia6 can be found
  // FIND SUROUTINES IN DJANGOH

  fLujets = (Lujets_t*) pythia6_common_address("LUJETS");
  fLudat1 = (Ludat1_t*) pythia6_common_address("LUDAT1");
  fLudat2 = (Ludat2_t*) pythia6_common_address("LUDAT2");
  fLudat3 = (Ludat3_t*) pythia6_common_address("LUDAT3");
  fLudat4 = (Ludat4_t*) pythia6_common_address("LUDAT4");
  fLudatr = (Ludatr_t*) pythia6_common_address("LUDATR");
}

////////////////////////////////////////////////////////////////////////////////

TDjangoh::TDjangoh(const TDjangoh& dj) :
 TGenerator(dj),
 fLujets(p6.fLujets),
 fLudat1(p6.fLudat1),
 fLudat2(p6.fLudat2),
 fLudat3(p6.fLudat3),
 fLudat4(p6.fLudat4),
 fLudatr(p6.fLudatr)
{ }

////////////////////////////////////////////////////////////////////////////////
/// Destroys the object, deletes and disposes all TMCParticles currently on list.

TDjangoh::~TDjangoh()
{
  if (fParticles) {
     fParticles->Delete();
     delete fParticles;
     fParticles = 0;
  }
}

////////////////////////////////////////////////////////////////////////////////
/// model of automatic memory cleanup suggested by Jim Kowalkovski:
/// destructor for local static variable `cleaner' is  always called in the end
/// of the job thus deleting the only TPythia6 instance

TDjangoh* TDjangoh::Instance() {
  static TDjangoh::TDjangohCleaner cleaner;
  return fgInstance ? fgInstance : (fgInstance=new TDjangoh()) ;
}





////////////////////////////////////////////////////////////////////////////////
///  generate event and copy the information from /HEPEVT/ to fPrimaries

void TDjangoh::GenerateEvent() {
  pyevnt();
  ImportParticles();
}

////////////////////////////////////////////////////////////////////////////////
///interface with fortran i/o

void TDjangoh::OpenFortranFile(int lun, char* name) {
  tdjangoh_open_fortran_file(&lun, name, strlen(name));
}

////////////////////////////////////////////////////////////////////////////////
///interface with fortran i/o

void TDjangoh::CloseFortranFile(int lun) {
  tdjangoh_close_fortran_file(&lun);
}


////////////////////////////////////////////////////////////////////////////////
/// Fills TObjArray fParticles list with particles from common LUJETS
/// Old contents of a list are cleared. This function should be called after
/// any change in common LUJETS, however GetParticles() method  calls it
/// automatically - user don't need to care about it. In case you make a call
/// to LuExec() you must call this method yourself to transfer new data from
/// common LUJETS to the fParticles list.

TObjArray *TDjangoh::ImportParticles(Option_t *)
{
  fParticles->Clear();
  Int_t numpart   = fLujets->N;
  TClonesArray &a = *((TClonesArray*)fParticles);
  for (Int_t i = 0; i<numpart; i++) {
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

////////////////////////////////////////////////////////////////////////////////
///  Default primary creation method. It reads the /HEPEVT/ common block which
///  has been filled by the GenerateEvent method. If the event generator does
///  not use the HEPEVT common block, This routine has to be overloaded by
///  the subclasses.
///  The function loops on the generated particles and store them in
///  the TClonesArray pointed by the argument particles.
///  The default action is to store only the stable particles (ISTHEP = 1)
///  This can be demanded explicitly by setting the option = "Final"
///  If the option = "All", all the particles are stored.
///

Int_t TDjangoh::ImportParticles(TClonesArray *particles, Option_t *option)
{
  if (particles == 0) return 0;
  TClonesArray &clonesParticles = *particles;
  clonesParticles.Clear();
  Int_t numpart = fLujets->N;
  Int_t nparts=0;
  if (!strcmp(option,"") || !strcmp(option,"Final")) {
     for (Int_t i = 0; i<numpart; i++) {

     if (fLujets->K[0][i] == 1) {
        //
        //  Use the common block values for the TParticle constructor
        //
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

        //     if(gDebug) printf("%d %d %d! ",i,fLujets->K[1][i],numpart);
        nparts++;
        }
     }
  } else if (!strcmp(option,"All")) {
     for (Int_t i = 0; i<numpart; i++) {
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

////////////////////////////////////////////////////////////////////////////////
/// Calls DjInit with the same parameters after performing some checking,
/// sets correct title. This method should preferably be called instead of DjInit.
/// PURPOSE: to initialize the generation procedure.
/// ARGUMENTS: See documentation for details.
/// -  frame:  - specifies the frame of the experiment:
///                "CMS","FIXT","USER","FOUR","FIVE","NONE"
/// -  beam,
/// -  target: - beam and target particles (with additionaly charges, tildes or "bar":
///              e,nu_e,mu,nu_mu,tau,nu_tau,gamma,pi,n,p,Lambda,Sigma,Xi,Omega,
///              pomeron,reggeon
/// -  win:    - related to energy system:
///              - for frame=="CMS" - total energy of system
///              - for frame=="FIXT" - momentum of beam particle
///              - for frame=="USER" - dummy - see documentation.

void TDjangoh::Initialize(const char *frame, const char *beam, const char *target, float win, float pol=0)
{
  char  cframe[4];
  strlcpy(cframe,frame,4);
  char  cbeam[10];
  strlcpy(cbeam,beam,10);
  char  ctarget[10];
  strlcpy(ctarget,target,10);

  // For frames "3MOM", "4MOM" and "5MOM" see p. 181-182 of the version 6 manual,
  // http://home.thep.lu.se/~torbjorn/pythia/lutp0613man2.pdf
  // Maybe not useful in djangoh
  if ( (!strncmp(frame, "CMS"  ,3)) &&
       (!strncmp(frame, "FIXT" ,4)) &&
       (!strncmp(frame, "USER" ,4)) &&
       (!strncmp(frame, "FOUR" ,4)) &&
       (!strncmp(frame, "FIVE" ,4)) &&
       (!strncmp(frame, "3MOM" ,4)) &&
       (!strncmp(frame, "4MOM" ,4)) &&
       (!strncmp(frame, "5MOM" ,4)) &&
       (!strncmp(frame, "NONE" ,4)) ) {
     printf("WARNING! In TDjangoh:Initialize():\n");
     printf(" specified frame=%s is neither of CMS,FIXT,USER,FOUR,FIVE,NONE,3MOM,4MOM,5MOM\n",frame);
     printf(" resetting to \"CMS\" .");
     snprintf(cframe,4,"CMS");
  }
  // Djangoh accept only e and mu
  if ( (!strncmp(beam, "e"       ,1)) &&
       (!strncmp(beam, "nu_e"    ,4)) &&
       (!strncmp(beam, "mu"      ,2)) &&
       (!strncmp(beam, "nu_mu"   ,5)) &&
       (!strncmp(beam, "tau"     ,3)) &&
       (!strncmp(beam, "nu_tau"  ,6)) &&
       (!strncmp(beam, "gamma"   ,5)) &&
       (!strncmp(beam, "pi"      ,2)) &&
       (!strncmp(beam, "n"       ,1)) &&
       (!strncmp(beam, "p"       ,1)) &&
       (!strncmp(beam, "Lambda"  ,6)) &&
       (!strncmp(beam, "Sigma"   ,5)) &&
       (!strncmp(beam, "Xi"      ,2)) &&
       (!strncmp(beam, "Omega"   ,5)) &&
       (!strncmp(beam, "pomeron" ,7)) &&
       (!strncmp(beam, "reggeon" ,7)) ) {
     printf("WARNING! In TDjangoh:Initialize():\n");
     printf(" specified beam=%s is unrecognized .\n",beam);
     printf(" resetting to \"p+\" .");
     snprintf(cbeam,8,"p+");
  }
  // change it w/ A+Z like in djangoh
  if ( (!strncmp(target, "e"       ,1)) &&
       (!strncmp(target, "nu_e"    ,4)) &&
       (!strncmp(target, "mu"      ,2)) &&
       (!strncmp(target, "nu_mu"   ,5)) &&
       (!strncmp(target, "tau"     ,3)) &&
       (!strncmp(target, "nu_tau"  ,6)) &&
       (!strncmp(target, "gamma"   ,5)) &&
       (!strncmp(target, "pi"      ,2)) &&
       (!strncmp(target, "n"       ,1)) &&
       (!strncmp(target, "p"       ,1)) &&
       (!strncmp(target, "Lambda"  ,6)) &&
       (!strncmp(target, "Sigma"   ,5)) &&
       (!strncmp(target, "Xi"      ,2)) &&
       (!strncmp(target, "Omega"   ,5)) &&
       (!strncmp(target, "pomeron" ,7)) &&
       (!strncmp(target, "reggeon" ,7)) ){
     printf("WARNING! In TDjangoh:Initialize():\n");
     printf(" specified target=%s is unrecognized.\n",target);
     printf(" resetting to \"p+\" .");
     snprintf(ctarget,8,"p+");
  }

  Initialize_File(cbeam,ctarget,win,pol);

  Djinit();

  Clean_File();

  char atitle[32];
  snprintf(atitle,32," %s-%s at %g GeV",cbeam,ctarget,win);
  SetTitle(atitle);
}


void TDjangoh::Djinit()
{
  popen("$(DJANGOH)/djangoh < TDjangoh.in");
  pclose();
}


void TDjangoh::Initialize_File(const char *beam, const char *target, float win, float pol)
{
  ofstream ofs("TDjangoh.in",);

  ofs
  << "\nOUTFILENAM\n" << outfilename
  << "\nTITEL\nDJANGOH 4.6.10 for COMPASS for " << beam << " on " << target
      << " , NLO at " << win << " , pol at " << pol << " , Wmin = " << kinem_cut[6]
  << "\nEL-BEAM\n" << win << " 0.0D0 " << beam
  << "\nIOUNITS\n" << iounits[0] << " " << iounits[1] << " " << iounits[2]
  << "\nPR-BEAM\n" << pr-beam[0] << " " << pr-beam[1]
  << "\nGSW-PARA\n" <<  gsw-param[0] << " " <<  gsw-param[1] << " " <<  gsw-param[2]  << " "
      <<  gsw-param[3] << " " <<  gsw-param[4] << " " <<  gsw-param[5] << " " <<  gsw-param[6] << " "
      <<  gsw-param[7] << " " <<  gsw-param[8] << " " <<  gsw-param[9] << " " <<  gsw-param[10]
  << "\nKINECT-CUT\n" << kinem_cut_var << " " << kinem_cut[0] << " "  << kinem_cut[1] << " "
      << kinem_cut[2] << " " << kinem_cut[3] << " " << kinem_cut[4] << " "
      << kinem_cut[5] << " " << kinem_cut[6]
  << "\nEGAM-MIN\n" << egam_min
  << "\nINT-OPT-NC\n" << int_opt_nc[0] << " " << int_opt_nc[1] << " " << int_opt_nc[2] << " "
      << int_opt_nc[3] << " " << int_opt_nc[4] << " " << int_opt_nc[5] << " "
      << int_opt_nc[6] << " " << int_opt_nc[7] << " " << int_opt_nc[8]
  << "\nINT-OPT-CC\n" << int_opt_cc[0] << " " << int_opt_cc[1] << " "
      << int_opt_cc[2] << " " << int_opt_cc[3]
  << "\nINT-ONLY\n" << int_only
  << "\nINT-POINT\n" << int_point
  << "\nSAM-OPT-NC\n" << sam_opt_nc[0] << " " << sam_opt_nc[1] << " " << sam_opt_nc[2] << " "
      << sam_opt_nc[3] << " " << sam_opt_nc[4] << " " << sam_opt_nc[5] << " "
      << sam_opt_nc[6] << " " << sam_opt_nc[7] << " " << sam_opt_nc[8]
  << "\nSAM-OPT-CC\n" << sam_opt_cc[0] << " " << sam_opt_cc[1] << " " <<
      << sam_opt_cc[2] << " " << sam_opt_cc[3]
  << "\nNUCLEUS\n" << nucl_e << " " << A << " " << Z
  << "\nSTRUCTFUNC\n" << structfunc[0] << " " << structfunc[1] << " " << structfunc[2]
  << "\nLHAPTH\n" << getenv("LHAPATH")
  << "\nFLONG\n" << flong[0] << " " << flong[1] << " " << flong[2]
  << "\nALFAS\n" << alfas[0] << " " << alfas[1] << " " << alfas[2] << " " << alfas[3]
  << "\nNFLAVORS\n" << nflavors[0] << " " << nflavors[1]
  << "\nRNDM-SEEDS\n" << rndm_seeds[0] << " " << rndm_seeds[1]
  << "\nSOPHIA\n" << sophia
  << "\nOUT-LEP\n" << out_lep
  << "\nFRAG\n" << frag
  << "\nCASCADES\n" << cascades
  << "\nMAX-VIRT\n" << max_virt
  << "\nCONTINUE";

}

void TDjangoh::Clean_File()
{
  remove(TDjangoh_evt.dat);
  remove(TDjangoh_his.paw);
  remove(TDjangoh_out.dat);
  remove(TDjangoh_rnd.dat);
  remove(TDjangoh_smp.dat);
}

// see how well the F interface go
int TPythia6::Pycomp(int kf) {
  //interface with fortran routine pycomp
  return pycomp(&kf);
}

void TPythia6::Pyedit(int medit) {
  //interface with fortran routine pyedit
  pyedit(&medit);
  ImportParticles();
}

void TPythia6::Pydiff() {
  //interface with fortran routine pydiff
  pydiff();
}

void TPythia6::Pyevnt() {
  //interface with fortran routine pyevnt
  pyevnt();
}

void TPythia6::Pyexec() {
  //interface with fortran routine pyexec
  pyexec();
}

void TPythia6::Pygive(const char *param) {
  //interface with fortran routine pygive
  Long_t lparam = strlen(param);
  pygive(param,lparam);
}

void TPythia6::Pyhepc(int mconv) {
  //interface with fortran routine pyhepc
  pyhepc(&mconv);
}

void TDjangoh::Lulist(int flag) {
  //interface with fortran routine lulist
  lulist(&flag);
}

void TPythia6::Pyname(int kf, char* name) {
  //Note that the array name must be dimensioned in the calling program
  //to at least name[16]

  pyname(&kf,name,15);
  // cut trailing blanks to get C string
  name[15] = 0;
  //for (int i=15; (i>=0) && (name[i] == ' '); i--) {
  //  name[i] = 0;
  // }
}

double TPythia6::Pyr(int idummy) {
  //interface with fortran routine pyr
  return pyr(&idummy);
}

void TPythia6::Pyrget(int lun, int move) {
  //interface with fortran routine pyrget
  pyrget(&lun,&move);
}

void TPythia6::Pyrset(int lun, int move) {
  //interface with fortran routine pyrset
  pyrset(&lun,&move);
}

void TPythia6::Pystat(int flag) {
  //interface with fortran routine pystat
  pystat(&flag);
}

void TPythia6::Pytest(int flag) {
  //interface with fortran routine pytest
  pytest(&flag);
}

void TPythia6::Pytune(int itune) {
  //interface with fortran routine pytune
  pytune(&itune);
}

void TPythia6::Pyupda(int mupda, int lun) {
  //interface with fortran routine pyupda
  pyupda(&mupda,&lun);
}

double TPythia6::Pymass(int kf) {
  //interface with fortran routine pymass
  return pymass(&kf);
}

int TPythia6::Pychge(int kf) {
  //interface with fortran routine pychge
  return pychge(&kf);
}

////////////////////////////////////////////////////////////////////////////////
/// Add one entry to the event record, i.e. either a parton or a
/// particle.
///
/// - IP:   normally line number for the parton/particle. There are two
///         exceptions:
///         - If IP = 0: line number 1 is used and PYEXEC is called.
///         - If IP < 0: line -IP is used, with status code K(-IP,2)=2
///                   rather than 1; thus a parton system may be built
///                   up by filling all but the last parton of the
///                   system with IP < 0.
///  - KF:   parton/particle flavour code (PDG code)
///  - PE:   parton/particle energy. If PE is smaller than the mass,
///          the parton/particle is taken to be at rest.
///  - THETA:
///  - PHI:  polar and azimuthal angle for the momentum vector of the
///          parton/particle.

void TPythia6::Py1ent(Int_t ip, Int_t kf, Double_t pe, Double_t theta, Double_t phi)
{
  py1ent(ip, kf, pe, theta, phi);
}


////////////////////////////////////////////////////////////////////////////////
/// Exemplary setup of Pythia parameters:
/// Switches on processes 102,123,124 (Higgs generation) and switches off
/// interactions, fragmentation, ISR, FSR...

void TPythia6::SetupTest()
{
  SetMSEL(0);            // full user controll;

  SetMSUB(102,1);        // g + g -> H0
  SetMSUB(123,1);        // f + f' -> f + f' + H0
  SetMSUB(124,1);        // f + f' -> f" + f"' + H0


  SetPMAS(6,1,175.0);   // mass of TOP
  SetPMAS(25,1,300);    // mass of Higgs


  SetCKIN(1,290.0);     // range of allowed mass
  SetCKIN(2,310.0);

  SetMSTP(61,  0);      // switch off ISR
  SetMSTP(71,  0);      // switch off FSR
  SetMSTP(81,  0);      // switch off multiple interactions
  SetMSTP(111, 0);      // switch off fragmentation/decay
}
