#include <iostream>
#include <fstream>
#include <cstring>

#include "TDjangoh.h"

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

# define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
# define PBWIDTH 60

using namespace std;

void printProgress (int event, int total)
{
    string points[6] = {"   ",".  ",".. ","..."," ..","  ."};
    double percentage = double(event)/double(total);
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf (FGRN("\r Progress%s %3d%% [%.*s%*s]"), points[int(event/16)%6].c_str(), val, lpad, PBSTR, rpad, "");
    fflush (stdout);
}

int main(int argc,char *argv[])
{

  if(argc < 3)
  {
    cout << BOLD(FRED("ERROR : Wrong number of arguments")) << endl;
    cout << BOLD(FRED("At least expected 3, received " << argc << " !")) << endl;
    cout << BOLD(FRED("USAGE : \n ./rc_calc input_file nb_evts + flags")) << endl;
    return 1;
  }

  int VERBOSE=1;
  float base_energy=160;
  float rnd=0;
  int finalState = 0;

  for (int i = 1; i < argc; i++)
  {
    if (i+1 != argc)
    {
      if (string(argv[i]) == "-verbose")
      {
        VERBOSE = atoi(argv[i + 1]);
      }
    }
    if (i+1 != argc)
    {
      if (string(argv[i]) == "-rand")
      {
        rnd = atof(argv[i + 1]);
      }
    }
    if (i+1 != argc)
    {
      if (string(argv[i]) == "-energy")
      {
        base_energy = atof(argv[i + 1]);
      }
    }
    if (string(argv[i]) == "-finalstate")
    {
      finalState = 1;
    }
  }

  TDjangoh* tDjangoh;
  int nb;
  float x;
  float y;
  float Q2;
  int NEVENTS = atoi(argv[2]);

  cout << FCYN("Instance creation..") << endl;
  tDjangoh = new TDjangoh();
  cout << FCYN("Instance created !") << endl;

  cout << FCYN("Initialization..") << endl;
  tDjangoh->ReadXMLFile(argv[1]);
  tDjangoh->Initialize();
  cout << FCYN("Initialized !") << endl;

  if(finalState)
  {
    tDjangoh->CleanFSFile();
  }

  cout << FCYN("\n\nEvents Generation (" << NEVENTS << " events)..") << endl;
  for(int i=0; i<NEVENTS; i++)
  {

    if(rnd)
      tDjangoh->SetBeam(base_energy+rand()%40,0);
    else
      tDjangoh->SetBeam(base_energy,0);


    if(VERBOSE>0)
    {
      printProgress (i+1,NEVENTS);
    }

    tDjangoh->GenerateEvent();

    if(finalState)
    {
      tDjangoh->WriteFSInFile();
    }

    if(VERBOSE>2)
    {
       nb = tDjangoh->GetN();
       cout << FCYN("Total number of particles : ") << nb << endl;
       for(int j=0; j<nb; j++)
       {
        	cout
    		<< tDjangoh->GetK(j,1) << " "
    		<< tDjangoh->GetK(j,2) << " "
    		<< tDjangoh->GetK(j,3) << " "
    		<< tDjangoh->GetK(j,4) << " "
    		<< tDjangoh->GetK(j,5) << " \n"
    		<< tDjangoh->GetP(j,1) << " "
      		<< tDjangoh->GetP(j,2) << " "
      		<< tDjangoh->GetP(j,3) << " "
    		<< tDjangoh->GetP(j,4) << " "
      		<< tDjangoh->GetP(j,5) << " \n"
      		<< tDjangoh->GetV(j,1) << " "
      		<< tDjangoh->GetV(j,2) << " "
      		<< tDjangoh->GetV(j,3) << " "
      		<< tDjangoh->GetV(j,4) << " "
      		<< tDjangoh->GetV(j,5) << " \n" << endl;
    	}
    	x = tDjangoh->GetX();
    	y = tDjangoh->GetY();
    	Q2 = tDjangoh->GetQ2();
    	cout << "Xbj : " << x << " y : " << y << " Q2 : " << Q2 << "\n\n" << endl;
     }

  }

  cout << FCYN("\n\nEvents Generated !") << endl;

  tDjangoh->EndRecap();

  return 0;
}
