#include <iostream>
#include <fstream>
#include <cstring>

#include "TDjangoh.h"

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
    printf ("\r Progress%s %3d%% [%.*s%*s]", points[int(event/16)%6].c_str(), val, lpad, PBSTR, rpad, "");
    fflush (stdout);
}

int main(int argc,char *argv[])
{

  if(argc < 3)
  {
    cout << "ERROR : Wrong number of arguments" << endl;
    cout << "At least expected 3, received " << argc << " !" << endl;
    cout << "USAGE : \n ./rc_calc input_file nb_evts + flags" << endl;
    return 1;
  }

  int VERBOSE=1;
  float base_energy=160;
  float rnd=0;

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
  }

  TDjangoh* tDjangoh;
  int nb;
  float x;
  float y;
  float Q2;
  int NEVENTS = atoi(argv[2]);

  cout << "Instance creation.." << endl;
  tDjangoh = new TDjangoh();
  cout << "Instance created !" << endl;

  cout << "Initialization.." << endl;
  tDjangoh->Initialize(argv[1]);
  cout << "Initialized !" << endl;

  cout << "\n\nEvents Generation (" << NEVENTS << " events).." << endl;
  for(int i=0; i<NEVENTS; i++)
  {

    if(rnd)
      tDjangoh->Configure(base_energy+rand()%40,0);
    else
      tDjangoh->Configure(base_energy,0);


    if(VERBOSE>0)
    {
      printProgress (i+1,NEVENTS);
    }

    tDjangoh->GenerateEvent();

    if(VERBOSE>2)
    {
       nb = tDjangoh->GetN();
       cout << "Total number of particles : " << nb << endl;
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

  cout << "\n\nEvents Generated !" << endl;

  tDjangoh->EndRecap();

  return 0;
}
