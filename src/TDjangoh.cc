//==============================================================================
/*!

    \file                         TDjangoh.cc
    \brief                        Interface class to Djangoh generator
    \author                       Nicolas PIERRE
    \version                      1.3
    \date                         09/04/18
    Support :                     mail to : nicolas.pierre@cern.ch

    \class TDjangoh
    \ingroup djangoh
*/
//==============================================================================

#include <iostream>
#include <fstream>
#include <cstring>

#include "../include/TDjangoh.h"
#include "TClonesArray.h"
#include "../include/TMCParticle.h"
#include "TParticle.h"
#include "TString.h"
#include "pugixml.hpp"

TDjangoh*  TDjangoh::fgInstance = 0;

# define type_of_call _stdcall
# define VERSION 1
# define SUBVERSION 3

// COLORS

#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST

#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST


using namespace std;


//------------------------------------------------------------------------------
// Dumping of Fortran COMMON Blocks
//------------------------------------------------------------------------------

Lujets_t lujets_;
Djkin_t djkin_;

extern "C"
{
  void hsinpt_();
}

extern "C"
{
  void hsegen_();
}

extern "C"
{
  void hsrcap_();
}

extern "C" struct ihscw
{
  char inputcodewd[46][10];
  int itcw;
} ihscw_;

extern "C" struct hsvrbz
{
  int verboz;
} hsvrbz_;

extern "C" struct hselab
{
  double sp;
  double eele;
  double pele;
  double epro;
  double ppro;
} hselab_;

extern "C" struct ihscut
{
  double ixmin;
  double ixmax;
  double iq2min;
  double iq2max;
  double iymin;
  double iymax;
  double iwmin;
} ihscut_;

extern "C" struct hstcut
{
  double themin;
  double themax;
  double cthmin;
  double cthcon;
} hstcut_;

extern "C" struct hspcut
{
  double ptmin = 0.0;
  double ptxm0 = 0.0;
} hspcut_;

extern "C" struct hsisgm
{
  double tcutq;
  double tcutqs;
} hsisgm_;

extern "C" struct hsparl
{
  int lpar[20];
  int lparin[12];
} hsparl_;

extern "C" struct hsstrp
{
  int icode;
  int ilib;
  int ilqmod;
  int idpvr;
} hsstrp_;

extern "C" struct hspdfo
{
  int ipdfop;
  int iflopt;
  int lqcd;
  int ltm;
  int lht;
} hspdfo_;

extern "C" struct hselep
{
  double idipol;
} hselep_;

extern "C" struct hsnucl
{
  int hna;
  int hnz;
  double inumod;
} hsnucl_;


extern "C" struct hsparm
{
  double polari;
  double hpolar;
  int llept;
  int lqua;
} hsparm_;

extern "C" struct hswgtc
{
  int iweigs;
  int iweigr;
} hswgtc_;

extern "C" struct hsirct
{
  double deleps;
  double delta;
  double egmin;
  double iopegm;
} hsirct_;

extern "C" struct hsalfs
{
  float par111;
  float par112;
  float parl11;
  float parl19;
  int mst111;
  int mst115;
} hsalfs_;

extern "C" struct hsintnc
{
  int inc2;
  int inc31;
  int inc32;
  int inc33;
  int inc34;
  int iel2;
  int iel31;
  int iel32;
  int iel33;
} hsintnc_;

extern "C" struct hsintcc
{
  int icc2;
  int icc31;
  int icc32;
  int icc33;
} hsintcc_;

extern "C" struct hssamnc
{
  int isnc2;
  int isnc31;
  int isnc32;
  int isnc33;
  int isnc34;
  int isel2;
  int isel31;
  int isel32;
  int isel33;
} hssamnc_;

extern "C" struct hssamcc
{
  int iscc2;
  int iscc31;
  int iscc32;
  int iscc33;
} hssamcc_;

extern "C" struct hslptu
{
  int hslst[40];
  double hsparl[30];
} hslptu_;

extern "C" struct hsrdio
{
  int isdinp;
  int isdout;
} hsrdio_;

extern "C" struct hsvglp
{
  int npoveg;
  int numint;
  int nphyp;
} hsvglp_;

extern "C" struct hystfu
{
  double pystop;
  double pyslam;
  int npymax;
  int npymin;
} hystfu_;

extern "C" struct hsoptn
{
  int int2[5];
  int int3[15];
  int isam2[5];
  int isam3[15];
  int ioplot;
  int iprint;
  int icut;
} hsoptn_;

extern "C" struct hsnume
{
  double sigtot[100];
  double sigtrr[100];
  double sigg[20][100];
  double siggrr[20][100];
  int nevent;
  int neve[20];
} hsnume_;

extern "C" struct hsgrid
{
  int gdsize;
  int gdindx;
  double gdmean;
  double gdsddv;
  double gdscle;
} hsgrid_;

extern "C" struct sophct
{
  double wsophia;
} sophct_;

extern "C" struct lhapdfc
{
  char lhapath[232];
} lhapdfc_;

extern "C" struct hsoutf
{
  char outfilenam[80];
} hsoutf_;

extern "C" struct chnumb
{
  int ichngl;
} chnumb_;

extern "C" struct hepsav
{
  int idhsv[3];
  double phepsv[3][5];
  double vhkksv[3][4];
} hepsav_;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Function to convert string to Fortran char
//------------------------------------------------------------------------------

void ConvertToFortran(char* fstring, std::size_t fstring_len,
                      const char* cstring)
{
    size_t inlen = strlen(cstring);
    size_t cpylen = min(inlen, fstring_len);

    if (inlen > fstring_len)
    {
        cout << "ERROR" << endl;
    }

    copy(cstring, cstring + cpylen, fstring);
    fill(fstring + cpylen, fstring + fstring_len, ' ');
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


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

  // LOGO display.
  cout
  << "\n\n\n"
  << BOLD(FBLU("             *//---------------------------------------------------------------------------------------//*\n"))
  << BOLD(FBLU("            *//"))<<BOLD(FMAG(" ___________ _____  ___________ _______ _____      ___ _______   _______   ___   ___"))<<BOLD(FBLU("  //*\n"))
  << BOLD(FBLU("           *// "))<<BOLD(FMAG("/____    __/  __  \\/____   ___/  ___   |     |    /  /"))<<BOLD(FCYN("*******"))<<BOLD(FMAG("/  /"))
  << BOLD(FCYN("*******"))<<BOLD(FMAG("\\ /  /  /"))<<BOLD(FCYN("**"))<<BOLD(FMAG("/"))<<BOLD(FBLU(" //*\n"))
  << BOLD(FBLU("          *//      "))<<BOLD(FMAG("/  /  /  /  \\  \\   /  /  /  /   |  | /|  |   /  /"))<<BOLD(FCYN("*"))<<BOLD(FMAG("/        /"))
  << BOLD(FCYN("**"))<<BOLD(FMAG("/   \\"))<<BOLD(FCYN("**"))<<BOLD(FMAG("\\  /__/"))<<BOLD(FCYN("**"))<<BOLD(FMAG("/"))<<BOLD(FBLU(" //*\n"))
  << BOLD(FBLU("         *//      "))<<BOLD(FMAG("/  /  /  /   /  /  /  /  /  /____|  |/ |  |  /  /"))<<BOLD(FCYN("*"))<<BOLD(FMAG("/    ____/"))
  << BOLD(FCYN("**"))<<BOLD(FMAG("/     \\"))<<BOLD(FCYN("**"))<<BOLD(FMAG("\\___ "))<<BOLD(FCYN("**"))<<BOLD(FMAG("/"))<<BOLD(FBLU(" //*\n"))
  << BOLD(FBLU("        *//      "))<<BOLD(FMAG("/  /  /  /   /  /  /  /  /  ______   |  |  | /  /\\"))<<BOLD(FCYN("**"))<<BOLD(FMAG("\\  |"))
  << BOLD(FCYN("****"))<<BOLD(FMAG("\\"))<<BOLD(FCYN("**"))<<BOLD(FMAG("\\     /"))<<BOLD(FCYN("**"))<<BOLD(FMAG("/  /"))<<BOLD(FCYN("**"))<<BOLD(FMAG("/"))<<BOLD(FBLU(" //*\n"))
  << BOLD(FBLU("       *//      "))<<BOLD(FMAG("/  /  /  /___/  /__/  /  /  /      |  |  |  |/  /  \\"))<<BOLD(FCYN("**"))<<BOLD(FMAG("\\___/"))
  << BOLD(FCYN("**"))<<BOLD(FMAG("/\\"))<<BOLD(FCYN("**"))<<BOLD(FMAG("\\___/"))<<BOLD(FCYN("**"))<<BOLD(FMAG("/  ___"))<<BOLD(FBLU("  //*\n"))
  << BOLD(FBLU("      *//      "))<<BOLD(FMAG("/__/  /_________/_____/  /__/       |__|  |_____/    \\"))<<BOLD(FCYN("*******"))
  << BOLD(FMAG("/  \\"))<<BOLD(FCYN("*******"))<<BOLD(FMAG("/  /"))<<BOLD(FCYN("**"))<<BOLD(FMAG("/"))<<BOLD(FBLU(" //*\n"))
  << BOLD(FBLU("     *//-----------------------------------------------------------------------------"))<< FCYN(" v. ") << VERSION << FWHT(".") << SUBVERSION << BOLD(FBLU(" -//*\n"))
  << BOLD(FBLU("    *//-----"))<<BOLD(FCYN(" TDjangoh : An Interface to Djangoh"))<<BOLD(FBLU(" ----- "))<<BOLD(FCYN("N. PIERRE, nicolas.pierre@cern.ch"))<<BOLD(FBLU(" -----//*\n"))
  << BOLD(FBLU("   *//--------------------------------------------------------------------------------------//*\n\n\n"));

  delete fParticles;

  fParticles = new TClonesArray("TMCParticle",50);
}


TDjangoh::TDjangoh(const TDjangoh& dj) :
 TGenerator(dj),
 fLujets(dj.fLujets),
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


void TDjangoh::GenerateEvent()
{
  hsegen_();
  fLujets = &lujets_;
  fDjkin = &djkin_;
  // Patch in order to obtain the final state in case of (quasi-)elastic events.
  if(chnumb_.ichngl == 15)
  {
    ElasFS();
  }
  else if(chnumb_.ichngl == 16 || chnumb_.ichngl == 17)
  {
    RadElasFS();
  }
  ImportParticles();
}


void TDjangoh::EndRecap()
{
  hsrcap_();
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

#ifdef DEBUG
          printf("%d %d %d! ",i,fLujets->K[1][i],numpart);
#endif
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

void TDjangoh::ElasFS()
{
  fLujets->N = 2;
  fLujets->K[0][-1] = 1; fLujets->K[1][-1] = GetIDPHEP(1); fLujets->K[2][-1] = 0;
  fLujets->K[3][-1] = 0; fLujets->K[4][-1] = 0;
  fLujets->P[0][-1] = GetPHEP(1,1); fLujets->P[1][-1] = GetPHEP(2,1); fLujets->P[2][-1] = GetPHEP(3,1);
  fLujets->P[3][-1] = GetPHEP(4,1); fLujets->P[4][-1] = GetPHEP(5,1);
  fLujets->V[0][-1] = GetVHKK(1,1); fLujets->V[1][-1] = GetVHKK(2,1); fLujets->V[2][-1] = GetVHKK(3,1);
  fLujets->V[3][-1] = GetVHKK(4,1); fLujets->V[4][-1] = GetVHKK(5,1);
  fLujets->K[0][0] = 1; fLujets->K[1][0] = GetNucleusID(); fLujets->K[2][0] = 0;
  fLujets->K[3][0] = 0; fLujets->K[4][0] = 0;
  fLujets->P[0][0] = GetPHEP(1,2); fLujets->P[1][0] = GetPHEP(2,2); fLujets->P[2][0] = GetPHEP(3,2);
  fLujets->P[3][0] = GetPHEP(4,2); fLujets->P[4][0] = GetPHEP(5,2);
  fLujets->V[0][0] = GetVHKK(1,2); fLujets->V[1][0] = GetVHKK(2,2); fLujets->V[2][0] = GetVHKK(3,2);
  fLujets->V[3][0] = GetVHKK(4,2); fLujets->V[4][0] = GetVHKK(5,2);
}

void TDjangoh::RadElasFS()
{
  fLujets->N = 3;
  fLujets->K[0][-1] = 1; fLujets->K[1][-1] = GetIDPHEP(1); fLujets->K[2][-1] = 0;
  fLujets->K[3][-1] = 0; fLujets->K[4][-1] = 0;
  fLujets->P[0][-1] = GetPHEP(1,1); fLujets->P[1][-1] = GetPHEP(2,1); fLujets->P[2][-1] = GetPHEP(3,1);
  fLujets->P[3][-1] = GetPHEP(4,1); fLujets->P[4][-1] = GetPHEP(5,1);
  fLujets->V[0][-1] = GetVHKK(1,1); fLujets->V[1][-1] = GetVHKK(2,1); fLujets->V[2][-1] = GetVHKK(3,1);
  fLujets->V[3][-1] = GetVHKK(4,1); fLujets->V[4][-1] = GetVHKK(5,1);
  fLujets->K[0][0] = 1; fLujets->K[1][0] = GetNucleusID(); fLujets->K[2][0] = 0;
  fLujets->K[3][0] = 0; fLujets->K[4][0] = 0;
  fLujets->P[0][0] = GetPHEP(1,2); fLujets->P[1][0] = GetPHEP(2,2); fLujets->P[2][0] = GetPHEP(3,2);
  fLujets->P[3][0] = GetPHEP(4,2); fLujets->P[4][0] = GetPHEP(5,2);
  fLujets->V[0][0] = GetVHKK(1,2); fLujets->V[1][0] = GetVHKK(2,2); fLujets->V[2][0] = GetVHKK(3,2);
  fLujets->V[3][0] = GetVHKK(4,2); fLujets->V[4][0] = GetVHKK(5,2);
  fLujets->K[0][1] = 1; fLujets->K[1][1] = GetIDPHEP(3); fLujets->K[2][1] = 0;
  fLujets->K[3][1] = 0; fLujets->K[4][1] = 0;
  fLujets->P[0][1] = GetPHEP(1,3); fLujets->P[1][1] = GetPHEP(2,3); fLujets->P[2][1] = GetPHEP(3,3);
  fLujets->P[3][1] = GetPHEP(4,3); fLujets->P[4][1] = GetPHEP(5,3);
  fLujets->V[0][1] = GetVHKK(1,3); fLujets->V[1][1] = GetVHKK(2,3); fLujets->V[2][1] = GetVHKK(3,3);
  fLujets->V[3][1] = GetVHKK(4,3); fLujets->V[4][1] = GetVHKK(5,3);
}

void TDjangoh::ReadXMLFile(const string pFilename)
{
  int PID;
  string cbeam;
  char* inputcw[46];
  pugi::xml_document doc;

  if(pFilename.find ( ".xml" ) != std::string::npos)
  {
    pugi::xml_parse_result result = doc.load_file(pFilename.c_str());
  }
  else
  {
    cout << "Could not parse settings file " << pFilename << " - it is not an .xml file !" ;
    return;
  }

  inputcw[0] = "OUTFILENAM";
  inputcw[1] = "TITLE     ";
  inputcw[2] = "EL-BEAM   ";
  inputcw[3] = "GD-OPT    ";
  inputcw[4] = "GSW-PARAM ";
  inputcw[5] = "KINEM-CUTS";
  inputcw[6] = "EGAM-MIN  ";
  inputcw[7] = "INT-OPT-NC";
  inputcw[8] = "INT-OPT-CC";
  inputcw[9] = "INT-ONLY  ";
  inputcw[10] = "INT-POINTS";
  inputcw[11] = "SAM-OPT-NC";
  inputcw[12] = "SAM-OPT-CC";
  inputcw[13] = "NUCLEUS   ";
  inputcw[14] = "STRUCTFUNC";
  inputcw[15] = "LHAPATH   ";
  inputcw[16] = "FLONG     ";
  inputcw[17] = "ALFAS     ";
  inputcw[18] = "NFLAVORS  ";
  inputcw[19] = "RNDM-SEEDS";
  inputcw[20] = "START     ";
  inputcw[21] = "SOPHIA    ";
  inputcw[22] = "OUT-LEP   ";
  inputcw[23] = "FRAG      ";
  inputcw[24] = "CASCADES  ";
  inputcw[25] = "MAX-VIRT  ";
  inputcw[26] = "CONTINUE  ";

  for ( pugi::xml_node cCodeWord = doc.child ( "Codeword" ); cCodeWord; cCodeWord = cCodeWord.next_sibling() )
  {
    std::string cCWType = cCodeWord.attribute ( "name" ).value();
    if(!strcmp(cCWType.c_str(), "EL-BEAM" ))
    {
      cout << "\nCodeword : EL-BEAM" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "polari")
          {hsparm_.polari = cData.attribute("value").as_double();cout<<"polari : "<<hsparm_.polari<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "beam")
        {
          cout<<"beam : "<<cData.attribute("value").value()<<endl;
          // Djangoh accept only e and mu
          if      (string(cData.attribute("value").value()) == "e-" ) PID = -1;
          else if (string(cData.attribute("value").value()) == "e+" ) PID = 1;
          else if (string(cData.attribute("value").value()) == "mu-") PID = -3;
          else if (string(cData.attribute("value").value()) == "mu+") PID = 3;
          else
          {
            cout<<"WARNING! In TDjangoh:Initialize():"<<endl;
            cout<<"Specified beam="<<cData.attribute("value").value()<<" is unrecognized."<<endl;
            cout<<"Resetting to \"e+\" ."<<endl;
            PID = 1;
          }
          hsparm_.llept = PID;
        }
      }
    }

    if(!strcmp(cCWType.c_str(), "KINEM-CUTS" ))
    {
      cout << "\nCodeword : KINEM-CUTS" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "icut")
          {hsoptn_.icut = cData.attribute("value").as_int();cout<<"icut : "<<hsoptn_.icut<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "ixmin")
          {ihscut_.ixmin = cData.attribute("value").as_double();cout<<"xmin : "<<ihscut_.ixmin<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "ixmax")
          {ihscut_.ixmax = cData.attribute("value").as_float();cout<<"xmax : "<<ihscut_.ixmax<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "iymin")
          {ihscut_.iymin = cData.attribute("value").as_float();cout<<"ymin : "<<ihscut_.iymin<<endl;}
        else if(std::string(cData.attribute("name").value()) == "iymax")
          {ihscut_.iymax = cData.attribute("value").as_float();cout<<"ymax : "<<ihscut_.iymax<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "iq2min")
          {ihscut_.iq2min = cData.attribute("value").as_float();cout<<"q2min : "<<ihscut_.iq2min<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "iq2max")
          {ihscut_.iq2max = cData.attribute("value").as_float();cout<<"q2max : "<<ihscut_.iq2max<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "iwmin")
          {ihscut_.iwmin = cData.attribute("value").as_float();cout<<"wmin : "<<ihscut_.iwmin<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "GD-OPT" ))
    {
      cout << "\nCodeword : GD-OPT" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "gdmean")
          {hsgrid_.gdmean = cData.attribute("value").as_double();cout<<"gdmean : "<<hsgrid_.gdmean<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "gdsddv")
          {hsgrid_.gdsddv = cData.attribute("value").as_double();cout<<"gdsddv : "<<hsgrid_.gdsddv<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "gdsize")
          {hsgrid_.gdsize = cData.attribute("value").as_int();cout<<"gdsize : "<<hsgrid_.gdsize<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "GSW-PARAM" ))
    {
      cout << "\nCodeword : GSW-PARAM" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "lparin1")
          {hsparl_.lparin[0] = cData.attribute("value").as_int();cout<<"lparin[1] : "<<hsparl_.lparin[0]<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "lparin2")
          {hsparl_.lparin[1] = cData.attribute("value").as_int();cout<<"lparin[2] : "<<hsparl_.lparin[1]<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "lparin3")
          {hsparl_.lparin[2] = cData.attribute("value").as_int();cout<<"lparin[3] : "<<hsparl_.lparin[2]<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "lparin4")
          {hsparl_.lparin[3] = cData.attribute("value").as_int();cout<<"lparin[4] : "<<hsparl_.lparin[3]<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "lparin5")
          {hsparl_.lparin[4] = cData.attribute("value").as_int();cout<<"lparin[5] : "<<hsparl_.lparin[4]<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "lparin6")
          {hsparl_.lparin[5] = cData.attribute("value").as_int();cout<<"lparin[6] : "<<hsparl_.lparin[5]<<endl;}
        else if(std::string(cData.attribute("name").value()) == "lparin7")
          {hsparl_.lparin[6] = cData.attribute("value").as_int();cout<<"lparin[7] : "<<hsparl_.lparin[6]<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "lparin8")
          {hsparl_.lparin[7] = cData.attribute("value").as_int();cout<<"lparin[8] : "<<hsparl_.lparin[7]<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "lparin9")
          {hsparl_.lparin[8] = cData.attribute("value").as_int();cout<<"lparin[9] : "<<hsparl_.lparin[8]<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "lparin10")
          {hsparl_.lparin[9] = cData.attribute("value").as_int();cout<<"lparin[10] : "<<hsparl_.lparin[9]<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "lparin11")
          {hsparl_.lparin[10] = cData.attribute("value").as_int();cout<<"lparin[11] : "<<hsparl_.lparin[10]<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "EGAM-MIN" ))
    {
      cout << "\nCodeword : EGAM-MIN" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "egam")
          {hsirct_.egmin = cData.attribute("value").as_double();cout<<"egmin : "<<hsirct_.egmin<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "INT-OPT-NC" ))
    {
      cout << "\nCodeword : INT-OPT-NC" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "inc2")
          {hsintnc_.inc2 = cData.attribute("value").as_int();cout<<"inc2 : "<<hsintnc_.inc2<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "inc31")
          {hsintnc_.inc31 = cData.attribute("value").as_int();cout<<"inc31 : "<<hsintnc_.inc31<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "inc32")
          {hsintnc_.inc32 = cData.attribute("value").as_int();cout<<"inc32 : "<<hsintnc_.inc32<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "inc33")
          {hsintnc_.inc33 = cData.attribute("value").as_int();cout<<"inc33 : "<<hsintnc_.inc33<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "inc34")
          {hsintnc_.inc34 = cData.attribute("value").as_int();cout<<"inc34 : "<<hsintnc_.inc34<<endl;}
        else if(std::string(cData.attribute("name").value()) == "iel2")
          {hsintnc_.iel2 = cData.attribute("value").as_int();cout<<"iel2 : "<<hsintnc_.iel2<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "iel31")
          {hsintnc_.iel31 = cData.attribute("value").as_int();cout<<"iel31 : "<<hsintnc_.iel31<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "iel32")
          {hsintnc_.iel32 = cData.attribute("value").as_int();cout<<"iel32 : "<<hsintnc_.iel32<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "iel33")
          {hsintnc_.iel33 = cData.attribute("value").as_int();cout<<"iel33 : "<<hsintnc_.iel33<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "INT-OPT-CC" ))
    {
      cout << "\nCodeword : INT-OPT-CC" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "icc2")
          {hsintcc_.icc2 = cData.attribute("value").as_int();cout<<"icc2 : "<<hsintcc_.icc2<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "icc31")
          {hsintcc_.icc31 = cData.attribute("value").as_int();cout<<"icc31 : "<<hsintcc_.icc31<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "icc32")
          {hsintcc_.icc32 = cData.attribute("value").as_int();cout<<"icc32 : "<<hsintcc_.icc32<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "icc33")
          {hsintcc_.icc33 = cData.attribute("value").as_int();cout<<"icc33 : "<<hsintcc_.icc33<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "INT-ONLY" ))
    {
      cout << "\nCodeword : INT-ONLY" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "ioplot")
          {hsoptn_.ioplot = cData.attribute("value").as_int();cout<<"ioplot : "<<hsoptn_.ioplot<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "INT-POINTS" ))
    {
      cout << "\nCodeword : INT-POINTS" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "npoveg")
          {hsvglp_.npoveg = cData.attribute("value").as_int();cout<<"npoveg : "<<hsvglp_.npoveg<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "SAM-OPT-NC" ))
    {
      cout << "\nCodeword : SAM-OPT-NC" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "isnc2")
          {hssamnc_.isnc2 = cData.attribute("value").as_int();cout<<"isnc2 : "<<hssamnc_.isnc2<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "isnc31")
          {hssamnc_.isnc31 = cData.attribute("value").as_int();cout<<"isnc31 : "<<hssamnc_.isnc31<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "isnc32")
          {hssamnc_.isnc32 = cData.attribute("value").as_int();cout<<"isnc32 : "<<hssamnc_.isnc32<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "isnc33")
          {hssamnc_.isnc33 = cData.attribute("value").as_int();cout<<"isnc33 : "<<hssamnc_.isnc33<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "isnc34")
          {hssamnc_.isnc34 = cData.attribute("value").as_int();cout<<"isnc34 : "<<hssamnc_.isnc34<<endl;}
        else if(std::string(cData.attribute("name").value()) == "isel2")
          {hssamnc_.isel2 = cData.attribute("value").as_int();cout<<"isel2 : "<<hssamnc_.isel2<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "isel31")
          {hssamnc_.isel31 = cData.attribute("value").as_int();cout<<"isel31 : "<<hssamnc_.isel31<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "isel32")
          {hssamnc_.isel32 = cData.attribute("value").as_int();cout<<"isel33 : "<<hssamnc_.isel32<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "isel33")
          {hssamnc_.isel33 = cData.attribute("value").as_int();cout<<"isel34 : "<<hssamnc_.isel33<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "SAM-OPT-CC" ))
    {
      cout << "\nCodeword : SAM-OPT-CC" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "iscc2")
          {hssamcc_.iscc2 = cData.attribute("value").as_int();cout<<"iscc2 : "<<hssamcc_.iscc2<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "iscc31")
          {hssamcc_.iscc31 = cData.attribute("value").as_int();cout<<"iscc31 : "<<hssamcc_.iscc31<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "iscc32")
          {hssamcc_.iscc32 = cData.attribute("value").as_int();cout<<"iscc32 : "<<hssamcc_.iscc32<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "iscc33")
          {hssamcc_.iscc33 = cData.attribute("value").as_int();cout<<"iscc33 : "<<hssamcc_.iscc33<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "NUCLEUS" ))
    {
      cout << "\nCodeword : NUCLEUS" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "epro")
          {hselab_.epro = cData.attribute("value").as_double();cout<<"epro : "<<hselab_.epro<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "hpolar")
          {hsparm_.hpolar = cData.attribute("value").as_double();cout<<"hpolar : "<<hsparm_.hpolar<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "hna")
          {hsnucl_.hna = cData.attribute("value").as_int();cout<<"hna : "<<hsnucl_.hna<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "hnz")
          {hsnucl_.hnz = cData.attribute("value").as_int();cout<<"hnz : "<<hsnucl_.hnz<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "STRUCTFUNC" ))
    {
      cout << "\nCodeword : STRUCTFUNC" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "ilqmod")
          {hsstrp_.ilqmod = cData.attribute("value").as_int();cout<<"ilqmod : "<<hsstrp_.ilqmod<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "ilib")
          {hsstrp_.ilib = cData.attribute("value").as_int();cout<<"ilib : "<<hsstrp_.ilib<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "icode")
          {hsstrp_.icode = cData.attribute("value").as_int();cout<<"icode : "<<hsstrp_.icode<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "FLONG" ))
    {
      cout << "\nCodeword : FLONG" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "iflopt")
          {hspdfo_.iflopt = cData.attribute("value").as_int();cout<<"iflopt : "<<hspdfo_.iflopt<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "parl11")
          {hsalfs_.parl11 = cData.attribute("value").as_float();cout<<"parl11 : "<<hsalfs_.parl11<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "parl19")
          {hsalfs_.parl19 = cData.attribute("value").as_float();cout<<"parl19 : "<<hsalfs_.parl19<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "ALFAS" ))
    {
      cout << "\nCodeword : ALFAS" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "mst111")
          {hsalfs_.mst111 = cData.attribute("value").as_int();cout<<"mst111 : "<<hsalfs_.mst111<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "mst115")
          {hsalfs_.mst115 = cData.attribute("value").as_int();cout<<"mst115 : "<<hsalfs_.mst115<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "par111")
          {hsalfs_.par111 = cData.attribute("value").as_float();cout<<"par111 : "<<hsalfs_.par111<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "par112")
          {hsalfs_.par112 = cData.attribute("value").as_float();cout<<"par112 : "<<hsalfs_.par112<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "NFLAVORS" ))
    {
      cout <<"\nCodeword : NFLAVORS" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "npymin")
          {hystfu_.npymin = cData.attribute("value").as_int();cout<<"npymin : "<<hystfu_.npymin<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "npymax")
          {hystfu_.npymax = cData.attribute("value").as_int();cout<<"npymax : "<<hystfu_.npymax<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "RNDM-SEEDS" ))
    {
      cout << "\nCodeword : RNDM-SEEDS" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "isdinp")
          {hsrdio_.isdinp = cData.attribute("value").as_int();cout<<"isdinp : "<<hsrdio_.isdinp<<"\t";}
        else if(std::string(cData.attribute("name").value()) == "isdout")
          {hsrdio_.isdout = cData.attribute("value").as_int();cout<<"isdout : "<<hsrdio_.isdout<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "SOPHIA" ))
    {
      cout << "\nCodeword : SOPHIA" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "wsophia")
          {sophct_.wsophia = cData.attribute("value").as_double();cout<<"wsophia : "<<sophct_.wsophia<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "OUT-LEP" ))
    {
      cout << "\nCodeword : OUT-LEP" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "lst4")
          {hslptu_.hslst[4] = cData.attribute("value").as_int();cout<<"lst4 : "<<hslptu_.hslst[4]<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "FRAME" ))
    {
      cout << "\nCodeword : FRAME" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "lst5")
          {hslptu_.hslst[5] = cData.attribute("value").as_int();cout<<"lst5 : "<<hslptu_.hslst[5]<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "FRAG" ))
    {
      cout << "\nCodeword : FRAG" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "lst7")
          {hslptu_.hslst[7] = cData.attribute("value").as_int();cout<<"lst7 : "<<hslptu_.hslst[7]<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "CASCADES" ))
    {
      cout << "\nCodeword : CASCADES" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "lst8")
          {hslptu_.hslst[8] = cData.attribute("value").as_int();cout<<"lst8 : "<<hslptu_.hslst[8]<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "MAX-VIRT" ))
    {
      cout << "\nCodeword : MAX-VIRT" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "lst9")
          {hslptu_.hslst[9] = cData.attribute("value").as_int();cout<<"lst9 : "<<hslptu_.hslst[9]<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "BARYON" ))
    {
      cout << "\nCodeword : BARYON" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "lst14")
          {hslptu_.hslst[14] = cData.attribute("value").as_int();cout<<"lst14 : "<<hslptu_.hslst[14]<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "KT-PARTON" ))
    {
      cout << "\nCodeword :KT-PARTON" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "parl3")
          {hslptu_.hsparl[3] = cData.attribute("value").as_double();cout<<"parl3 : "<<hslptu_.hsparl[3]<<endl;}
      }
    }

    if(!strcmp(cCWType.c_str(), "VERBOSE" ))
    {
      cout << "\nCodeword : VERBOSE" << endl;
      for(pugi::xml_node cData = cCodeWord.child ( "Data" ); cData; cData = cData.next_sibling())
      {
        if(std::string(cData.attribute("name").value()) == "verboz")
          {hsvrbz_.verboz = cData.attribute("value").as_int();cout<<"vrboz : "<<hsvrbz_.verboz<<endl;}
      }
    }
  }

  for(int i=0; i<27; i++)
    ConvertToFortran(ihscw_.inputcodewd[i], sizeof ihscw_.inputcodewd[i], inputcw[i]);

  // OUTFILENAME
  char* outfilenamei = "TDjangoh";
  ConvertToFortran(hsoutf_.outfilenam, sizeof hsoutf_.outfilenam, outfilenamei);

  // LHAPATH
  char* lhapathi;
  lhapathi = getenv("LHAPATH");
  ConvertToFortran(lhapdfc_.lhapath, sizeof lhapdfc_.lhapath, lhapathi);

  // START
  hsnume_.nevent = 1;

}

void TDjangoh::WriteXMLFile(const string pFilename)
{
  string cbeam;

  if(hsparm_.llept==1) cbeam="e+";
  else if(hsparm_.llept==-1) cbeam="e-";
  else if(hsparm_.llept==3) cbeam="mu+";
  else if(hsparm_.llept==-3) cbeam="mu-";

  ofstream f(pFilename);
  f << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>" << endl;
  f << "<!-- For any information concerning codeword block content, refer to Djangoh manual>" << endl;

  f << "\n<!-- EL-BEAM -->" << endl;
  f << "<Codeword name=\"EL-BEAM\">" << endl;
  f << "\t<Data name=\"polari\" value=\"" << hsparm_.polari << "\"/>" << endl;
  f << "\t<Data name=\"beam\" value=\"" << cbeam << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- KINEM-CUTS -->" << endl;
  f << "<Codeword name=\"KINEM-CUTS\">" << endl;
  f << "\t<Data name=\"icut\" value=\"" << hsoptn_.icut << "\"/>" << endl;
  f << "\t<Data name=\"ixmin\" value=\"" << ihscut_.ixmin << "\"/>" << endl;
  f << "\t<Data name=\"ixmax\" value=\"" << ihscut_.ixmax << "\"/>" << endl;
  f << "\t<Data name=\"iymin\" value=\"" << ihscut_.iymin << "\"/>" << endl;
  f << "\t<Data name=\"iymax\" value=\"" << ihscut_.iymax << "\"/>" << endl;
  f << "\t<Data name=\"iq2min\" value=\"" << ihscut_.iq2min << "\"/>" << endl;
  f << "\t<Data name=\"iq2max\" value=\"" << ihscut_.iq2max << "\"/>" << endl;
  f << "\t<Data name=\"iwmin\" value=\"" << ihscut_.iwmin << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- GSW-PARAM -->" << endl;
  f << "<Codeword name=\"GSW-PARAM\">" << endl;
  f << "\t<Data name=\"lparin1\" value=\"" << hsparl_.lparin[0] << "\"/>" << endl;
  f << "\t<Data name=\"lparin2\" value=\"" << hsparl_.lparin[1] << "\"/>" << endl;
  f << "\t<Data name=\"lparin3\" value=\"" << hsparl_.lparin[2] << "\"/>" << endl;
  f << "\t<Data name=\"lparin4\" value=\"" << hsparl_.lparin[3] << "\"/>" << endl;
  f << "\t<Data name=\"lparin5\" value=\"" << hsparl_.lparin[4] << "\"/>" << endl;
  f << "\t<Data name=\"lparin6\" value=\"" << hsparl_.lparin[5] << "\"/>" << endl;
  f << "\t<Data name=\"lparin7\" value=\"" << hsparl_.lparin[6] << "\"/>" << endl;
  f << "\t<Data name=\"lparin8\" value=\"" << hsparl_.lparin[7] << "\"/>" << endl;
  f << "\t<Data name=\"lparin9\" value=\"" << hsparl_.lparin[8] << "\"/>" << endl;
  f << "\t<Data name=\"lparin10\" value=\"" << hsparl_.lparin[9] << "\"/>" << endl;
  f << "\t<Data name=\"lparin11\" value=\"" << hsparl_.lparin[10] << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- GD-OPT -->" << endl;
  f << "<Codeword name=\"GD-OPT\">" << endl;
  f << "\t<Data name=\"gdmean\" value=\"" << hsgrid_.gdmean << "\"/>" << endl;
  f << "\t<Data name=\"gdsddv\" value=\"" << hsgrid_.gdsddv << "\"/>" << endl;
  f << "\t<Data name=\"gdsize\" value=\"" << hsgrid_.gdsize << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- EGAM-MIN -->" << endl;
  f << "<Codeword name=\"EGAM-MIN\">" << endl;
  f << "\t<Data name=\"egam\" value=\"" << hsirct_.egmin << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- INT-OPT-NC -->" << endl;
  f << "<Codeword name=\"INT-OPT-NC\">" << endl;
  f << "\t<Data name=\"inc2\" value=\"" << hsintnc_.inc2 << "\"/>" << endl;
  f << "\t<Data name=\"inc31\" value=\"" << hsintnc_.inc31 << "\"/>" << endl;
  f << "\t<Data name=\"inc32\" value=\"" << hsintnc_.inc32 << "\"/>" << endl;
  f << "\t<Data name=\"inc33\" value=\"" << hsintnc_.inc33 << "\"/>" << endl;
  f << "\t<Data name=\"inc34\" value=\"" << hsintnc_.inc34 << "\"/>" << endl;
  f << "\t<Data name=\"iel2\" value=\"" << hsintnc_.iel2 << "\"/>" << endl;
  f << "\t<Data name=\"iel31\" value=\"" << hsintnc_.iel31 << "\"/>" << endl;
  f << "\t<Data name=\"iel32\" value=\"" << hsintnc_.iel32 << "\"/>" << endl;
  f << "\t<Data name=\"iel33\" value=\"" << hsintnc_.iel33 << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- INT-OPT-CC -->" << endl;
  f << "<Codeword name=\"INT-OPT-CC\">" << endl;
  f << "\t<Data name=\"icc2\" value=\"" << hsintcc_.icc2 << "\"/>" << endl;
  f << "\t<Data name=\"icc31\" value=\"" << hsintcc_.icc31 << "\"/>" << endl;
  f << "\t<Data name=\"icc32\" value=\"" << hsintcc_.icc32 << "\"/>" << endl;
  f << "\t<Data name=\"icc33\" value=\"" << hsintcc_.icc33 << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- INT-ONLY -->" << endl;
  f << "<Codeword name=\"INT-ONLY\">" << endl;
  f << "\t<Data name=\"ioplot\" value=\"" << hsoptn_.ioplot << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- INT-POINTS -->" << endl;
  f << "<Codeword name=\"INT-POINTS\">" << endl;
  f << "\t<Data name=\"npoveg\" value=\"" << hsvglp_.npoveg << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- SAM-OPT-NC -->" << endl;
  f << "<Codeword name=\"SAM-OPT-NC\">" << endl;
  f << "\t<Data name=\"isnc2\" value=\"" << hssamnc_.isnc2 << "\"/>" << endl;
  f << "\t<Data name=\"isnc31\" value=\"" << hssamnc_.isnc31 << "\"/>" << endl;
  f << "\t<Data name=\"isnc32\" value=\"" << hssamnc_.isnc32 << "\"/>" << endl;
  f << "\t<Data name=\"isnc33\" value=\"" << hssamnc_.isnc33 << "\"/>" << endl;
  f << "\t<Data name=\"isnc34\" value=\"" << hssamnc_.isnc34 << "\"/>" << endl;
  f << "\t<Data name=\"isel2\" value=\"" << hssamnc_.isel2 << "\"/>" << endl;
  f << "\t<Data name=\"isel31\" value=\"" << hssamnc_.isel31 << "\"/>" << endl;
  f << "\t<Data name=\"isel32\" value=\"" << hssamnc_.isel32 << "\"/>" << endl;
  f << "\t<Data name=\"isel33\" value=\"" << hssamnc_.isel33 << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- SAM-OPT-CC -->" << endl;
  f << "<Codeword name=\"SAM-OPT-CC\">" << endl;
  f << "\t<Data name=\"iscc2\" value=\"" << hssamcc_.iscc2 << "\"/>" << endl;
  f << "\t<Data name=\"iscc31\" value=\"" << hssamcc_.iscc31 << "\"/>" << endl;
  f << "\t<Data name=\"iscc32\" value=\"" << hssamcc_.iscc32 << "\"/>" << endl;
  f << "\t<Data name=\"iscc33\" value=\"" << hssamcc_.iscc33 << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- NUCLEUS -->" << endl;
  f << "<Codeword name=\"NUCLEUS\">" << endl;
  f << "\t<Data name=\"epro\" value=\"" << hselab_.epro << "\"/>" << endl;
  f << "\t<Data name=\"hpolar\" value=\"" << hsparm_.hpolar << "\"/>" << endl;
  f << "\t<Data name=\"hna\" value=\"" << hsnucl_.hna << "\"/>" << endl;
  f << "\t<Data name=\"hnz\" value=\"" << hsnucl_.hnz << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- STRUCTFUNC -->" << endl;
  f << "<Codeword name=\"STRUCTFUNC\">" << endl;
  f << "\t<Data name=\"ilqmod\" value=\"" << hsstrp_.ilqmod << "\"/>" << endl;
  f << "\t<Data name=\"ilib\" value=\"" << hsstrp_.ilib << "\"/>" << endl;
  f << "\t<Data name=\"icode\" value=\"" << hsstrp_.icode << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- FLONG -->" << endl;
  f << "<Codeword name=\"FLONG\">" << endl;
  f << "\t<Data name=\"iflopt\" value=\"" << hspdfo_.iflopt << "\"/>" << endl;
  f << "\t<Data name=\"parl11\" value=\"" << hsalfs_.parl11 << "\"/>" << endl;
  f << "\t<Data name=\"parl19\" value=\"" << hsalfs_.parl19 << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- ALFAS -->" << endl;
  f << "<Codeword name=\"ALFAS\">" << endl;
  f << "\t<Data name=\"mst111\" value=\"" << hsalfs_.mst111 << "\"/>" << endl;
  f << "\t<Data name=\"mst115\" value=\"" << hsalfs_.mst115 << "\"/>" << endl;
  f << "\t<Data name=\"par111\" value=\"" << hsalfs_.par111 << "\"/>" << endl;
  f << "\t<Data name=\"par112\" value=\"" << hsalfs_.par112 << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- NFLAVORS -->" << endl;
  f << "<Codeword name=\"NFLAVORS\">" << endl;
  f << "\t<Data name=\"npymin\" value=\"" << hystfu_.npymin << "\"/>" << endl;
  f << "\t<Data name=\"npymax\" value=\"" << hystfu_.npymax << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- RNDM-SEEDS -->" << endl;
  f << "<Codeword name=\"RNDM-SEEDS\">" << endl;
  f << "\t<Data name=\"isdinp\" value=\"" << hsrdio_.isdinp << "\"/>" << endl;
  f << "\t<Data name=\"isdout\" value=\"" << hsrdio_.isdout << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- SOPHIA -->" << endl;
  f << "<Codeword name=\"SOPHIA\">" << endl;
  f << "\t<Data name=\"wsophia\" value=\"" << sophct_.wsophia << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- OUT-LEP -->" << endl;
  f << "<Codeword name=\"OUT-LEP\">" << endl;
  f << "\t<Data name=\"lst4\" value=\"" << hslptu_.hslst[4] << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- FRAME -->" << endl;
  f << "<Codeword name=\"FRAME\">" << endl;
  f << "\t<Data name=\"lst5\" value=\"" << hslptu_.hslst[5] << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- FRAG -->" << endl;
  f << "<Codeword name=\"FRAG\">" << endl;
  f << "\t<Data name=\"lst7\" value=\"" << hslptu_.hslst[7] << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- CASCADES -->" << endl;
  f << "<Codeword name=\"CASCADES\">" << endl;
  f << "\t<Data name=\"lst8\" value=\"" << hslptu_.hslst[8] << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- MAX-VIRT -->" << endl;
  f << "<Codeword name=\"MAX-VIRT\">" << endl;
  f << "\t<Data name=\"lst9\" value=\"" << hslptu_.hslst[9] << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- BARYON -->" << endl;
  f << "<Codeword name=\"BARYON\">" << endl;
  f << "\t<Data name=\"lst14\" value=\"" << hslptu_.hslst[14] << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- KT-PARTON -->" << endl;
  f << "<Codeword name=\"KT-PARTON\">" << endl;
  f << "\t<Data name=\"parl13\" value=\"" << hslptu_.hsparl[3] << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f << "\n<!-- VERBOSE -->" << endl;
  f << "<Codeword name=\"VERBOSE\">" << endl;
  f << "\t<Data name=\"verboz\" value=\"" << hsvrbz_.verboz << "\"/>" << endl;
  f << "</Codeword>" << endl;

  f.close();

  cout << ">>> ****************** <<<" << endl;
  cout << ">>> TDJANGOH message : <<<" << endl;
  cout << ">>>    XML written !   <<<" << endl;
  cout << ">>> ****************** <<<" << endl;
}

void TDjangoh::Initialize()
{
  hsinpt_();

  cout << ">>> ********************* <<<" << endl;
  cout << ">>>   TDJANGOH message :  <<<" << endl;
  cout << ">>> DJANGOH initialized ! <<<" << endl;
  cout << ">>> ********************* <<<" << endl;
}

void TDjangoh::ModKineCuts(int pcut, double pxmin, double pxmax, double pymin, double pymax, double pq2min, double pq2max, double pwmin)
{

    hsoptn_.icut=pcut;
    ihscut_.ixmin=pxmin;
    ihscut_.ixmax=pxmax;
    ihscut_.iymin=pymin;
    ihscut_.iymax=pymax;
    ihscut_.iq2min=pq2min;
    ihscut_.iq2max=pq2max;
    ihscut_.iwmin=pwmin;

}

void TDjangoh::BornWOqelNC()
{

    hsparl_.lparin[1]=0;
    hsintnc_.inc2=1;
    hsintnc_.inc31=0;
    hsintnc_.inc32=0;
    hsintnc_.inc33=0;
    hsintnc_.inc34=0;
    hsintnc_.iel2=0;
    hsintnc_.iel31=0;
    hsintnc_.iel32=0;
    hsintnc_.iel33=0;
    hssamnc_.isnc2=1;
    hssamnc_.isnc31=0;
    hssamnc_.isnc32=0;
    hssamnc_.isnc33=0;
    hssamnc_.isnc34=0;
    hssamnc_.isel2=0;
    hssamnc_.isel31=0;
    hssamnc_.isel32=0;
    hssamnc_.isel33=0;

}

void TDjangoh::RClepWOqelNC()
{

    hsparl_.lparin[1]=1;
    hsintnc_.inc2=1;
    hsintnc_.inc31=18;
    hsintnc_.inc32=18;
    hsintnc_.inc33=18;
    hsintnc_.inc34=0;
    hsintnc_.iel2=0;
    hsintnc_.iel31=0;
    hsintnc_.iel32=0;
    hsintnc_.iel33=0;
    hssamnc_.isnc2=1;
    hssamnc_.isnc31=1;
    hssamnc_.isnc32=1;
    hssamnc_.isnc33=1;
    hssamnc_.isnc34=0;
    hssamnc_.isel2=0;
    hssamnc_.isel31=0;
    hssamnc_.isel32=0;
    hssamnc_.isel33=0;

}

double TDjangoh::GetSigtot()
{
  return hsnume_.sigtot[0];
}

double TDjangoh::GetSigtrr()
{
  return hsnume_.sigtrr[0];
}

int TDjangoh::GetBeamType() {return hsparm_.llept;}

void TDjangoh::SetBeamType(int pvalue) {hsparm_.llept=pvalue;}

void TDjangoh::SetBeamType(const char* pname)
{
  int PID;

  if      (string(pname) == "e-" ) PID = -1;
  else if (string(pname) == "e+" ) PID = 1;
  else if (string(pname) == "mu-") PID = -3;
  else if (string(pname) == "mu+") PID = 3;
  else
  {
    cout<<"WARNING! In TDjangoh:Initialize():"<<endl;
    cout<<"Specified beam="<<pname<<" is unrecognized."<<endl;
    cout<<"Resetting to \"e+\" ."<<endl;
    PID = 11;
  }
  hsparm_.llept = PID;
}

void TDjangoh::SetBeam(double pBeamE, double pPol)
{
  hselab_.eele = pBeamE;
  hsparm_.polari = pPol;
}

double TDjangoh::GetBeamPolar() {return hsparm_.polari;}
double TDjangoh::SetBeamPolar(double pvalue) {hsparm_.polari=pvalue;}

double TDjangoh::GetKinemCut(int i)
{
  if(i==0) return hsoptn_.icut;
  else if(i==1) return ihscut_.ixmin;
  else if(i==2) return ihscut_.ixmax;
  else if(i==3) return ihscut_.iymin;
  else if(i==4) return ihscut_.iymax;
  else if(i==5) return ihscut_.iq2min;
  else if(i==6) return ihscut_.iq2max;
  else if(i==7) return ihscut_.iwmin;
  else return -1;
}

void TDjangoh::SetKinemCut(double pvalue, int i)
{
  if(i==0) hsoptn_.icut = int(pvalue);
  else if(i==1) ihscut_.ixmin = pvalue;
  else if(i==2) ihscut_.ixmax = pvalue;
  else if(i==3) ihscut_.iymin = pvalue;
  else if(i==4) ihscut_.iymax = pvalue;
  else if(i==5) ihscut_.iq2min = pvalue;
  else if(i==6) ihscut_.iq2max = pvalue;
  else if(i==7) ihscut_.iwmin = pvalue;
}

double TDjangoh::GetGdOpt(int i)
{
  if(i==0) return hsgrid_.gdmean;
  else if(i==1) return hsgrid_.gdsddv;
  else if(i==2) return hsgrid_.gdsize;
  else return -1;
}

void TDjangoh::SetGdOpt(double pvalue, int i)
{
  if(i==0) hsgrid_.gdmean = pvalue;
  else if(i==1) hsgrid_.gdsddv = pvalue;
  else if(i==2) hsgrid_.gdsize = pvalue;
}

int TDjangoh::GetGsw(int i)
{
  if(i==0) return hsparl_.lparin[0];
  else if(i==1) return hsparl_.lparin[1];
  else if(i==2) return hsparl_.lparin[2];
  else if(i==3) return hsparl_.lparin[3];
  else if(i==4) return hsparl_.lparin[4];
  else if(i==5) return hsparl_.lparin[5];
  else if(i==6) return hsparl_.lparin[6];
  else if(i==7) return hsparl_.lparin[7];
  else if(i==8) return hsparl_.lparin[8];
  else if(i==9) return hsparl_.lparin[9];
  else if(i==10) return hsparl_.lparin[10];
  else return -1;
}

void TDjangoh::SetGsw(int pvalue, int i)
{
  if(i==0) hsparl_.lparin[0] = pvalue;
  else if(i==1) hsparl_.lparin[1] = pvalue;
  else if(i==2) hsparl_.lparin[2] = pvalue;
  else if(i==3) hsparl_.lparin[3] = pvalue;
  else if(i==4) hsparl_.lparin[4] = pvalue;
  else if(i==5) hsparl_.lparin[5] = pvalue;
  else if(i==6) hsparl_.lparin[6] = pvalue;
  else if(i==7) hsparl_.lparin[7] = pvalue;
  else if(i==8) hsparl_.lparin[8] = pvalue;
  else if(i==9) hsparl_.lparin[9] = pvalue;
  else if(i==10) hsparl_.lparin[10] = pvalue;
}

double TDjangoh::GetEgamMin() {return hsirct_.egmin;}
void TDjangoh::SetEgamMin(double pvalue) {hsirct_.egmin=pvalue;}

int TDjangoh::GetIntOptNC(int i)
{
  if(i==0) return hsintnc_.inc2;
  else if(i==1) return hsintnc_.inc31;
  else if(i==2) return hsintnc_.inc32;
  else if(i==3) return hsintnc_.inc33;
  else if(i==4) return hsintnc_.inc34;
  else if(i==5) return hsintnc_.iel2;
  else if(i==6) return hsintnc_.iel31;
  else if(i==7) return hsintnc_.iel32;
  else if(i==8) return hsintnc_.iel33;
  else return -1;
}

void TDjangoh::SetIntOptNC(int pvalue, int i)
{
  if(i==0) hsintnc_.inc2 = pvalue;
  else if(i==1) hsintnc_.inc31 = pvalue;
  else if(i==2) hsintnc_.inc32 = pvalue;
  else if(i==3) hsintnc_.inc33 = pvalue;
  else if(i==4) hsintnc_.inc34 = pvalue;
  else if(i==5) hsintnc_.iel2 = pvalue;
  else if(i==6) hsintnc_.iel31 = pvalue;
  else if(i==7) hsintnc_.iel32 = pvalue;
  else if(i==8) hsintnc_.iel33 = pvalue;
}

void TDjangoh::SetSamOptNC(int pvalue, int i)
{
  if(i==0) hssamnc_.isnc2 = pvalue;
  else if(i==1) hssamnc_.isnc31 = pvalue;
  else if(i==2) hssamnc_.isnc32 = pvalue;
  else if(i==3) hssamnc_.isnc33 = pvalue;
  else if(i==4) hssamnc_.isnc34 = pvalue;
  else if(i==5) hssamnc_.isel2 = pvalue;
  else if(i==6) hssamnc_.isel31 = pvalue;
  else if(i==7) hssamnc_.isel32 = pvalue;
  else if(i==8) hssamnc_.isel33 = pvalue;
}

int TDjangoh::GetStructFunc(int i)
{
  if(i==0) return hsstrp_.ilqmod;
  else if(i==1) return hsstrp_.ilib;
  else if(i==2) return hsstrp_.icode;
  else return -1;
}

void TDjangoh::SetStructFunc(int pvalue, int i)
{
  if(i==0) hsstrp_.ilqmod = pvalue;
  else if(i==1) hsstrp_.ilib = pvalue;
  else if(i==2) hsstrp_.icode = pvalue;
}

double TDjangoh::GetSophia() {return sophct_.wsophia;}
void TDjangoh::SetSophia(double pvalue) {sophct_.wsophia=pvalue;}

int TDjangoh::GetVerbose() {return hsvrbz_.verboz;}
void TDjangoh::SetVerbose(int pvalue) {hsvrbz_.verboz=pvalue;}

int TDjangoh::GetIntOptCC(int i)
{
  if(i==0) return hsintcc_.icc2;
  else if(i==1) return hsintcc_.icc31;
  else if(i==2) return hsintcc_.icc32;
  else if(i==3) return hsintcc_.icc33;
  else return -1;
}

void TDjangoh::SetIntOptCC(int pvalue, int i)
{
  if(i==0) hsintcc_.icc2 = pvalue;
  else if(i==1) hsintcc_.icc31 = pvalue;
  else if(i==2) hsintcc_.icc32 = pvalue;
  else if(i==3) hsintcc_.icc33 = pvalue;
}


void TDjangoh::SetSamOptCC(int pvalue, int i)
{
  if(i==0) hssamcc_.iscc2 = pvalue;
  else if(i==1) hssamcc_.iscc31 = pvalue;
  else if(i==2) hssamcc_.iscc32 = pvalue;
  else if(i==3) hssamcc_.iscc33 = pvalue;
}

void TDjangoh::SetNucleus(double pHpolar, int pHna, int pHnz, double pEpro)
{
  hselab_.epro = pEpro;
  hsparm_.hpolar = pHpolar;
  hsnucl_.hna = pHna;
  hsnucl_.hnz = pHnz;
}

void TDjangoh::WriteFSInFile()
{
  ofstream finalState("finalState.txt", ofstream::out | ofstream::app);

  Int_t numpart = fLujets->N;

  finalState << numpart << endl;
  finalState << hselab_.eele << "\t" << fDjkin->DJX << "\t" << fDjkin->DJY << "\t" << fDjkin->DJQ2 << endl;

  for (Int_t i = 0; i<numpart; i++)
  {
    if (fLujets->K[0][i-1] == 1 && fLujets->K[2][i-1] == 0)
    {
      finalState << fLujets->K[0][i-1] << "\t"
                 << fLujets->K[1][i-1] << "\t" //ID
                 << fLujets->K[2][i-1] << "\t"
                 << fLujets->K[3][i-1] << "\t"
                 << fLujets->K[4][i-1] << "\t"
                 << fLujets->P[0][i-1] << "\t" //Px
                 << fLujets->P[1][i-1] << "\t" //Py
                 << fLujets->P[2][i-1] << "\t" //Pz
                 << fLujets->P[3][i-1] << "\t" //max(kinE,mass)
                 << fLujets->V[0][i-1] << "\t"
                 << fLujets->V[1][i-1] << "\t"
                 << fLujets->V[2][i-1] << "\t"
                 << fLujets->V[3][i-1] << endl;
    }
  }

  finalState.close();
}

double TDjangoh::GetPHEP(int ip, int i)
{
  return hepsav_.phepsv[i-1][ip-1];
}

double TDjangoh::GetVHKK(int ip, int i)
{
  return hepsav_.vhkksv[i-1][ip-1];
}

int TDjangoh::GetIDPHEP(int i)
{
  return hepsav_.idhsv[i-1];
}

int TDjangoh::GetChannel()
{
  return chnumb_.ichngl;
}

int TDjangoh::GetNucleusID()
{
  int id;
  if(hsnucl_.hna == 1 && hsnucl_.hnz == 1)
    id=2212;
  else if(hsnucl_.hna == 1 && hsnucl_.hnz == 1)
    id=2112;
  else
    id=1000000000+hsnucl_.hnz*10000+hsnucl_.hna*10;
  return id;
}

void TDjangoh::Clean_File()
{
  remove("TDjangoh_evt.dat");
  remove("TDjangoh_out.dat");
  remove("TDjangoh_rnd.dat");
  remove("TDjangoh_smp.dat");
  remove("TDjangoh_sigtot.dat");

  cout << ">>> ************************ <<<" << endl;
  cout << ">>>    TDJANGOH message :    <<<" << endl;
  cout << ">>> TDJANGOH files cleaned ! <<<" << endl;
  cout << ">>> ************************ <<<" << endl;
}
