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

#include <iostream>
#include <fstream>

#include "TDjangoh.h"
#include "TClonesArray.h"
#include "TMCParticle.h"
#include "TParticle.h"
#include "TDjangoh_inputfile.h"
#include "TString.h"

TDjangoh*  TDjangoh::fgInstance = 0;

# define type_of_call _stdcall

Lujets_t lujets_;

extern "C" {
void hsmain_(char* inputfile, int* len);
}

using namespace std;

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

  // fLujets = (Lujets_t*) djangoh_common_address("LUJETS");
  // fLudat1 = (Ludat1_t*) djangoh_common_address("LUDAT1");
  // fLudat2 = (Ludat2_t*) djangoh_common_address("LUDAT2");
}

////////////////////////////////////////////////////////////////////////////////

TDjangoh::TDjangoh(const TDjangoh& dj) :
 TGenerator(dj),
 fLujets(dj.fLujets),
 fLudat1(dj.fLudat1),
 fLudat2(dj.fLudat2)
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
  char* cfile = "TDjangoh.in";
  int len_cfile = strlen(cfile);
  //Ludat1_t* ludat1_;
  //Ludat2_t* ludat2_;
  hsmain_(cfile,&len_cfile);
  cout << lujets_.N << endl;
  fLujets = &lujets_;
  //fLudat1 = ludat1_;
  //fLudat2 = ludat2_;
  ImportParticles();
}
/*
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
*/

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
/// -  beam,
/// -  nuc_A,
/// -  nuc_Z:   - Beam particle and nucleus
///               Specify the lepton for beam and the (A,Z) pair for the nucleus
/// -  beam_e:  - Energy of the beam
/// -  nuc_e:   - Energy of the nucleus (0 for FIXT)

void TDjangoh::Initialize(const char *beam, int nuc_A, int nuc_Z, float beam_e, float nuc_e, float pol)
{
  int PID;

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

  Initialize_File(beam,PID,nuc_A,nuc_Z,beam_e,nuc_e,pol);

  char atitle[32];
  snprintf(atitle,32," %s-N(%d,%d) at %g GeV",beam,nuc_A,nuc_Z,sqrt(pow(beam_e,2)+pow(nuc_e,2)));
  SetTitle(atitle);
}


void TDjangoh::Djrun()
{
  FILE *in;

  if(!(in=popen(Form("%s/djangoh < TDjangoh.in",getenv("DJANGOH")),"w")))
  {
    printf("WARNING! In TDjangoh::Djinit :\n");
    printf("POSIX popen() couldn't launch/find Djangoh !\n");
  }
  pclose(in);
}


void TDjangoh::Initialize_File(const char *beam, int PID, int nuc_A, int nuc_Z, float beam_e, float nuc_e, float pol)
{
  ofstream ofs("TDjangoh.in", std::ofstream::out);

  ofs
  << "OUTFILENAM\n" << outfilename
  << "\nTITLE\nDJANGOH 4.6.10 for COMPASS for " << beam << " on N(" << nuc_A << "," << nuc_Z
      << ") , NLO at " << beam_e << " , pol at " << pol << " , Wmin = " << kinem_cut[6]
  << "\nEL-BEAM\n\t" << beam_e << ".0D0 0.0D0 " << PID
  << "\nIOUNITS\n" << iounits[0] << " " << iounits[1] << " " << iounits[2]
  << "\nPR-BEAM\n" << pr_beam[0] << " " << pr_beam[1]
  << "\nGSW-PARAM\n" <<  gsw_param[0] << " " <<  gsw_param[1] << " " <<  gsw_param[2]  << " "
      <<  gsw_param[3] << " " <<  gsw_param[4] << " " <<  gsw_param[5] << " " <<  gsw_param[6] << " "
      <<  gsw_param[7] << " " <<  gsw_param[8] << " " <<  gsw_param[9] << " " <<  gsw_param[10]
  << "\nKINEM-CUTS\n" << kinem_cut_var << " " << kinem_cut[0] << " "  << kinem_cut[1] << " "
      << kinem_cut[2] << " " << kinem_cut[3] << " " << kinem_cut[4] << " "
      << kinem_cut[5] << " " << kinem_cut[6]
  << "\nEGAM-MIN\n" << egam_min
  << "\nINT-OPT-NC\n" << int_opt_nc[0] << " " << int_opt_nc[1] << " " << int_opt_nc[2] << " "
      << int_opt_nc[3] << " " << int_opt_nc[4] << " " << int_opt_nc[5] << " "
      << int_opt_nc[6] << " " << int_opt_nc[7] << " " << int_opt_nc[8]
  << "\nINT-OPT-CC\n" << int_opt_cc[0] << " " << int_opt_cc[1] << " "
      << int_opt_cc[2] << " " << int_opt_cc[3]
  << "\nINT-ONLY\n" << int_only
  << "\nINT-POINTS\n" << int_point
  << "\nSAM-OPT-NC\n" << sam_opt_nc[0] << " " << sam_opt_nc[1] << " " << sam_opt_nc[2] << " "
      << sam_opt_nc[3] << " " << sam_opt_nc[4] << " " << sam_opt_nc[5] << " "
      << sam_opt_nc[6] << " " << sam_opt_nc[7] << " " << sam_opt_nc[8]
  << "\nSAM-OPT-CC\n" << sam_opt_cc[0] << " " << sam_opt_cc[1] << " "
      << sam_opt_cc[2] << " " << sam_opt_cc[3]
  << "\nNUCLEUS\n" << nuc_e << " " << nuc_A << " " << nuc_Z
  << "\nSTRUCTFUNC\n" << structfunc[0] << " " << structfunc[1] << " " << structfunc[2]
  << "\nLHAPATH\n" << getenv("LHAPATH")
  << "\nFLONG\n" << flong[0] << " " << flong[1] << " " << flong[2]
  << "\nALFAS\n" << alfas[0] << " " << alfas[1] << " " << alfas[2] << " " << alfas[3]
  << "\nNFLAVORS\n" << nflavors[0] << " " << nflavors[1]
  << "\nRNDM-SEEDS\n" << rndm_seeds[0] << " " << rndm_seeds[1]
  << "\nSTART\n" << start
  << "\nSOPHIA\n" << sophia
  << "\nOUT-LEP\n" << out_lep
  << "\nFRAG\n" << frag
  << "\nCASCADES\n" << cascades
  << "\nMAX-VIRT\n" << max_virt
  << "\nCONTINUE";

}

void TDjangoh::Clean_File()
{
  remove("TDjangoh_evt.dat");
  remove("TDjangoh_out.dat");
  remove("TDjangoh_rnd.dat");
  remove("TDjangoh_smp.dat");
}
