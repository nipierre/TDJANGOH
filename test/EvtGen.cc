#include <iostream>
#include <fstream>
#include <cstring>

#include "TDjangoh.h"
#include "EvtGen.h"

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
    cout << BOLD(FRED("USAGE : \n ./evtgen input_file nb_evts + flags")) << endl;
    return 1;
  }

  int VERBOSE=1;
  int kCluster = 0;
  float base_energy=160;
  float rnd=0;
  float energy_beam = 0;
  int finalState = 0;
  string disEvtFileName = "DISEvt.root";

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
    if (i+1 != argc)
    {
      if (string(argv[i]) == "-o")
      {
        disEvtFileName = Form("%s.root",argv[i+1]);
      }
    }
    if (string(argv[i]) == "-finalstate")
    {
      finalState = 1;
    }
    if (string(argv[i]) == "-batch")
    {
      kCluster = 1;
    }
  }

  // fDISEvt = new DISData();
  fHadronsPtr = &fHadrons;
  fOutFile = new TFile(disEvtFileName.c_str(),"RECREATE");
  fDISEvtTree = new TTree("DISEvtTree","DIS event and hadron info");
  fDISEvtTree->Branch("DISEvt","DISData",&fDISEvt);
  fDISEvtTree->Branch("Hadrons","std::vector<HadronData>",&fHadronsPtr);
  fDISEvtTree->SetMaxTreeSize(1000000000);

  TDjangoh* tDjangoh;
  int nb;
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
    HadronData hadron;
    TLorentzVector mup;
    TVector3 mupv;
    TLorentzVector gammastar;
    TVector3 gammastarv;

    if(rnd)
      energy_beam = base_energy+rand()%40;
    else
      energy_beam = base_energy;

    tDjangoh->SetBeam(energy_beam,0);

    TLorentzVector mu(0,0,energy_beam,energy_beam);
    TVector3 muv = mu.Vect();


    if(VERBOSE>0 || kCluster)
    {
      printProgress (i+1,NEVENTS);
    }

    tDjangoh->GenerateEvent();

    nb = tDjangoh->GetN();

    fDISEvt.p1x = tDjangoh->GetP(0,1); fDISEvt.p1y = tDjangoh->GetP(0,2); fDISEvt.p1z = tDjangoh->GetP(0,3); fDISEvt.E1 = tDjangoh->GetP(0,4);
    fDISEvt.p0x = 0; fDISEvt.p0y = 0; fDISEvt.p0z = tDjangoh->GetEBeam(); fDISEvt.E0 = tDjangoh->GetEBeam();
  	fDISEvt.xBj = tDjangoh->GetX(); fDISEvt.Y = tDjangoh->GetY(); fDISEvt.Q2 = tDjangoh->GetQ2();
    fDISEvt.nu = tDjangoh.GetEBeam() - tDjangoh->GetP(0,4);

    mup.SetPxPyPzE(fDISEvt.p1x,fDISEvt.p1x,fDISEvt.p1x,fDISEvt.E1);
    mupv = mup.Vect();
    gammastar = mu - mup;
    gammastarv = gammastar.Vect();

    for(int j=1; j<nb; j++)
    {
      hadron.px = tDjangoh->GetP(0,1); hadron.py = tDjangoh->GetP(0,2); hadron.pz = tDjangoh->GetP(0,3); hadron.E = tDjangoh->GetP(0,4);
      hadron.PID = tDjangoh->GetK(j,2); hadron.z = tDjangoh->GetP(0,4)/fDISEvt->nu;

      TLorentzVector had;
      had.SetPxPyPzE(hadron.px,hadron.py,hadron.pz,hadron.E);
      TVector3 hadv = had.Vect();

      double theta_mu = acos(muv.Dot(mupv)/(muv.Mag()*mupv.Mag()));
      double theta_gs = acos((muv.Mag()-mupv.Mag()*cos(theta_mu))/gammastarv.Mag());

      double phi = (gammastarv.Cross(muv)).Dot(hadv)/(gammastarv.Cross(muv).Mag()*hadv.Mag())
                    *acos((gammastarv.Cross(muv)).Dot(gammastarv.Cross(hadv))/(gammastarv.Cross(muv).Mag()*gammastarv.Cross(hadv).Mag()));

      hadv.RotateY(theta_gs);
      hadv.RotateZ(phi);

      hadron.P = sqrt(pow(hadron.px,2)+pow(hadron.py,2)+pow(hadron.pz,2));
      hadron.pt = sqrt(pow(hadv.X(),2)+pow(hadv.Y(),2));

      fHadrons.push_back(hadron);
  	}

    fDISEvtTree->Fill();
  }

  cout << FCYN("\n\nEvents Generated !") << endl;

  tDjangoh->EndRecap();

  return 0;
}
