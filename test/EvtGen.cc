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
  Double_t base_energy=160;
  Double_t rnd=0;
  Double_t energy_beam = 0;
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

  fOutFile = new TFile(disEvtFileName.c_str(),"RECREATE");
  fDISEvtTree = new TTree("DISEvtTree","DIS event and hadron info");

  fDISEvtTree->Branch("p0x",&p0x,"p0x/D");
  fDISEvtTree->Branch("p0y",&p0y,"p0y/D");
  fDISEvtTree->Branch("p0z",&p0z,"p0z/D");
  fDISEvtTree->Branch("E0",&E0,"E0/D");
  fDISEvtTree->Branch("p1x",&p1x,"p1x/D");
  fDISEvtTree->Branch("p1y",&p1y,"p1y/D");
  fDISEvtTree->Branch("p1z",&p1z,"p1z/D");
  fDISEvtTree->Branch("E1",&E1,"E1/D");
  fDISEvtTree->Branch("Charge",&Charge,"Charge/I");
  fDISEvtTree->Branch("theta",&theta,"theta/D");
  fDISEvtTree->Branch("Q2",&Q2,"Q2/D");
  fDISEvtTree->Branch("nu",&nu,"nu/D");
  fDISEvtTree->Branch("Y",&Y,"Y/D");
  fDISEvtTree->Branch("xBj",&xBj,"xBj/D");

  fDISEvtTree->Branch("h_px","std::vector<Double_t>",&px);
  fDISEvtTree->Branch("h_py","std::vector<Double_t>",&py);
  fDISEvtTree->Branch("h_pz","std::vector<Double_t>",&pz);
  fDISEvtTree->Branch("h_P","std::vector<Double_t>",&P);
  fDISEvtTree->Branch("h_pt","std::vector<Double_t>",&pt);
  fDISEvtTree->Branch("h_th","std::vector<Double_t>",&th);
  fDISEvtTree->Branch("h_ph","std::vector<Double_t>",&ph);
  fDISEvtTree->Branch("h_ph_pl","std::vector<Double_t>",&ph_pl);
  fDISEvtTree->Branch("h_charge","std::vector<Int_t>",&charge);
  fDISEvtTree->Branch("h_PID","std::vector<Int_t>",&PID);
  fDISEvtTree->Branch("h_E","std::vector<Double_t>",&E);
  fDISEvtTree->Branch("h_z","std::vector<Double_t>",&z);

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

    p1x = tDjangoh->GetP(0,1); p1y = tDjangoh->GetP(0,2); p1z = tDjangoh->GetP(0,3); E1 = tDjangoh->GetP(0,4);
    p0x = 0; p0y = 0; p0z = tDjangoh->GetEBeam(); E0 = tDjangoh->GetEBeam();
  	xBj = tDjangoh->GetX(); Y = tDjangoh->GetY(); Q2 = tDjangoh->GetQ2();
    nu = tDjangoh->GetEBeam() - tDjangoh->GetP(0,4);

    mup.SetPxPyPzE(p1x,p1x,p1x,E1);
    mupv = mup.Vect();
    gammastar = mu - mup;
    gammastarv = gammastar.Vect();

    for(int j=1; j<nb; j++)
    {

      px.push_back(tDjangoh->GetP(j,1)); py.push_back(tDjangoh->GetP(j,2)); pz.push_back(tDjangoh->GetP(j,3)); E.push_back(tDjangoh->GetP(j,4));
      PID.push_back(tDjangoh->GetK(j,2)); z.push_back(tDjangoh->GetP(j,4)/nu);

      TLorentzVector had;
      had.SetPxPyPzE(px[j-1],py[j-1],pz[j-1],E[j-1]);
      TVector3 hadv = had.Vect();

      double theta_mu = acos(muv.Dot(mupv)/(muv.Mag()*mupv.Mag()));
      double theta_gs = acos((muv.Mag()-mupv.Mag()*cos(theta_mu))/gammastarv.Mag());

      double phi = (gammastarv.Cross(muv)).Dot(hadv)/(gammastarv.Cross(muv).Mag()*hadv.Mag())
                    *acos((gammastarv.Cross(muv)).Dot(gammastarv.Cross(hadv))/(gammastarv.Cross(muv).Mag()*gammastarv.Cross(hadv).Mag()));

      hadv.RotateY(theta_gs);
      hadv.RotateZ(phi);

      P.push_back(sqrt(pow(px[j-1],2)+pow(py[j-1],2)+pow(pz[j-1],2)));
      pt.push_back(sqrt(pow(hadv.X(),2)+pow(hadv.Y(),2)));
  	}

    fDISEvtTree->Fill();

    px.clear(); py.clear(); pz.clear();
    P.clear(); pt.clear(); th.clear(); ph.clear(); ph_pl.clear();
    charge.clear(); PID.clear(); E.clear(); z.clear();
  }

  cout << FCYN("\n\nEvents Generated !") << endl;

  tDjangoh->EndRecap();

  fDISEvtTree->AutoSave();

  fOutFile->Close();

  return 0;
}
