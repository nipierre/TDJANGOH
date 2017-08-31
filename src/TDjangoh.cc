//==============================================================================
/*!

    \file                         TDjangoh.cc
    \brief                        Interface class to Djangoh generator
    \author                       Nicolas PIERRE
    \version                      1.1
    \date                         14/06/16
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
# define SUBVERSION 1

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
  int inc31 ;
  int inc32 ;
  int inc33 ;
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
  double sigg[100][20];
  double siggrr[100][20];
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
  << BOLD(FCYN("*******"))<<BOLD(FMAG("\\ /  /  /"))<<BOLD(FCYN("**"))<<BOLD(FBLU("/ //*\n"))
  << BOLD(FBLU("          *//      "))<<BOLD(FMAG("/  /  /  /  \\  \\   /  /  /  /   |  | /|  |   /  /"))<<BOLD(FCYN("*"))<<BOLD(FMAG("/        /"))
  << BOLD(FCYN("**"))<<BOLD(FMAG("/   \\"))<<BOLD(FCYN("**"))<<BOLD(FMAG("\\  /__/"))<<BOLD(FCYN("**"))<<BOLD(FBLU("/ //*\n"))
  << BOLD(FBLU("         *//      "))<<BOLD(FMAG("/  /  /  /   /  /  /  /  /  /____|  |/ |  |  /  /"))<<BOLD(FCYN("*"))<<BOLD(FMAG("/    ____/"))
  << BOLD(FCYN("**"))<<BOLD(FMAG("/     \\"))<<BOLD(FCYN("**"))<<BOLD(FMAG("\\___ "))<<BOLD(FCYN("**"))<<BOLD(FBLU("/ //*\n"))
  << BOLD(FBLU("        *//      "))<<BOLD(FMAG("/  /  /  /   /  /  /  /  /  ______   |  |  | /  /\\"))<<BOLD(FCYN("**"))<<BOLD(FMAG("\\  |"))
  << BOLD(FCYN("****"))<<BOLD(FMAG("\\"))<<BOLD(FCYN("**"))<<BOLD(FMAG("\\     /"))<<BOLD(FCYN("**"))<<BOLD(FMAG("/  /"))<<BOLD(FCYN("**"))<<BOLD(FBLU("/ //*\n"))
  << BOLD(FBLU("       *//      "))<<BOLD(FMAG("/  /  /  /___/  /__/  /  /  /      |  |  |  |/  /  \\"))<<BOLD(FCYN("**"))<<BOLD(FMAG("\\___/"))
  << BOLD(FCYN("**"))<<BOLD(FMAG("/\\"))<<BOLD(FCYN("**"))<<BOLD(FMAG("\\___/"))<<BOLD(FCYN("**"))<<BOLD(FBLU("/  ___  //*\n"))
  << BOLD(FBLU("      *//      "))<<BOLD(FMAG("/__/  /_________/_____/  /__/       |__|  |_____/    \\"))<<BOLD(FCYN("*******"))
  << BOLD(FMAG("/  \\"))<<BOLD(FCYN("*******"))<<BOLD(FMAG("/  /"))<<BOLD(FCYN("**"))<<BOLD(FBLU("/ //*\n"))
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

          //  printf("%d %d %d! ",i,fLujets->K[1][i],numpart);
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
            PID = 11;
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
          {hslptu_.hslst[9] = cData.attribute("value").as_int();cout<<"lst7 : "<<hslptu_.hslst[9]<<endl;}
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

    if(!strcmp(cCWType.c_str(), "VERBOZE" ))
    {
      cout << "\nCodeword : VERBOZE" << endl;
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

void TDjangoh::Initialize()
{
  hsinpt_();

  cout << ">>> TDJANGOH message : <<<" << endl;
  cout << "*** DJANGOH initialized ! ***" << endl;
  cout << ">>>         ***        <<<" << endl;
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

void TDjangoh::SetParticle(const char* pname)
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

double TDjangoh::GetSigtot()
{
  return hsnume_.sigtot[0];
}

double TDjangoh::GetSigtrr()
{
  return hsnume_.sigtrr[0];
}

void TDjangoh::SetBeam(double pBeamE, double pPol)
{
  hselab_.eele = pBeamE;
  hsparm_.polari = pPol;
}

void TDjangoh::SetGridOpt(double pGdMean, double pGdStdDev, int pGdSize)
{
  hsgrid_.gdmean = pGdMean;
  hsgrid_.gdsddv = pGdStdDev;
  hsgrid_.gdsize = pGdSize;
}

void TDjangoh::SetGSWParam(int pLparin1, int pLparin2, int pLparin3, int pLparin4,
                           int pLparin5, int pLparin6, int pLparin7, int pLparin8,
                           int pLparin9, int pLparin10, int pLparin11)
{
  hsparl_.lparin[0] = pLparin1;
  hsparl_.lparin[1] = pLparin2;
  hsparl_.lparin[2] = pLparin3;
  hsparl_.lparin[3] = pLparin4;
  hsparl_.lparin[4] = pLparin5;
  hsparl_.lparin[5] = pLparin6;
  hsparl_.lparin[6] = pLparin7;
  hsparl_.lparin[7] = pLparin8;
  hsparl_.lparin[8] = pLparin9;
  hsparl_.lparin[9] = pLparin10;
  hsparl_.lparin[10] = pLparin11;
}

void TDjangoh::SetEgamMin(double pEgamMin)
{
  hsirct_.egmin = pEgamMin;
}

void TDjangoh::SetIntOptNC(int pInc2, int pInc31, int pInc32, int pInc33, int pInc34,
                           int pIel2, int pIel31, int pIel32, int pIel33)
{
  hsintnc_.inc2 = pInc2;
  hsintnc_.inc31 = pInc31;
  hsintnc_.inc32 = pInc32;
  hsintnc_.inc33 = pInc33;
  hsintnc_.inc34 = pInc34;
  hsintnc_.iel2 = pIel2;
  hsintnc_.iel31 = pIel31;
  hsintnc_.iel32 = pIel32;
  hsintnc_.iel33 = pIel33;
}

void TDjangoh::SetIntOptCC(int pIcc2, int pIcc31, int pIcc32, int pIcc33)
{
  hsintcc_.icc2 = pIcc2;
  hsintcc_.icc31 = pIcc31;
  hsintcc_.icc32 = pIcc32;
  hsintcc_.icc33 = pIcc33;
}

void TDjangoh::SetSamOptNC(int pIsnc2, int pIsnc31, int pIsnc32, int pIsnc33, int pIsnc34,
                           int pIsel2, int pIsel31, int pIsel32, int pIsel33)
{
  hssamnc_.isnc2 = pIsnc2;
  hssamnc_.isnc31 = pIsnc31;
  hssamnc_.isnc32 = pIsnc32;
  hssamnc_.isnc33 = pIsnc33;
  hssamnc_.isnc34 = pIsnc34;
  hssamnc_.isel2 = pIsel2;
  hssamnc_.isel31 = pIsel31;
  hssamnc_.isel32 = pIsel32;
  hssamnc_.isel33 = pIsel33;
}

void TDjangoh::SetSamOptCC(int pIscc2, int pIscc31, int pIscc32, int pIscc33)
{
  hssamcc_.iscc2 = pIscc2;
  hssamcc_.iscc31 = pIscc31;
  hssamcc_.iscc32 = pIscc32;
  hssamcc_.iscc33 = pIscc33;
}

void TDjangoh::SetNucleus(double pEpro, double pHpolar, int pHna, int pHnz)
{
  hselab_.epro = pEpro;
  hsparm_.hpolar = pHpolar;
  hsnucl_.hna = pHna;
  hsnucl_.hnz = pHnz;
}

void TDjangoh::SetStructFunc(int pIlqmod, int pIlib, int pIcode)
{
  hsstrp_.ilqmod = pIlqmod;
  hsstrp_.ilib = pIlib;
  hsstrp_.icode = pIcode;
}

void TDjangoh::SetVerboze(int pVerboz)
{
  hsvrbz_.verboz = pVerboz;
}

void TDjangoh::WriteFSInFile()
{

  ofstream finalState("finalState.txt", ofstream::out | ofstream::app);

  Int_t numpart = fLujets->N;

  finalState << numpart << endl;
  finalState << fDjkin->DJX << "\t" << fDjkin->DJY << "\t" << fDjkin->DJQ2 << endl;

  for (Int_t i = 0; i<numpart; i++)
  {
    if (fLujets->K[0][i] == 1)
    {
      finalState << fLujets->K[0][i] << "\t"
                 << fLujets->K[1][i] << "\t" //ID
                 << fLujets->K[2][i] << "\t"
                 << fLujets->K[3][i] << "\t"
                 << fLujets->K[4][i] << "\t"
                 << fLujets->P[0][i] << "\t" //Px
                 << fLujets->P[1][i] << "\t" //Py
                 << fLujets->P[2][i] << "\t" //Pz
                 << fLujets->P[3][i] << "\t" //max(kinE,mass)
                 << fLujets->V[0][i] << "\t"
                 << fLujets->V[1][i] << "\t"
                 << fLujets->V[2][i] << "\t"
                 << fLujets->V[3][i] << endl;
    }
  }

  finalState.close();
}

void TDjangoh::Clean_File()
{
  remove("TDjangoh_evt.dat");
  remove("TDjangoh_out.dat");
  remove("TDjangoh_rnd.dat");
  remove("TDjangoh_smp.dat");
  remove("TDjangoh_sigtot.dat");
}
