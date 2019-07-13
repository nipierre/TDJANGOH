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

DISData::DISData():
  p0x(0),p0y(0),p0z(0),E0(0), // beam momentum
  p1x(0),p1y(0),p1z(0),E1(0), // mu1 momentum
  E_beam(0), // beam energie
  E_mu_prim(0), // mu1 energie
  Charge(0),
  theta(0),
  Q2(0),
  nu(0),
  Y(0),
  xBj(0),
  W(0)
{}

HadronData::HadronData():
  px(0),py(0),pz(0),
  P(0),pt(0),th(0),ph(0),ph_pl(0),
  charge(0),
  PID(0),
  E(0),
  z(0)
{}

DISData::~DISData()
{}

HadronData::~HadronData()
{}

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

  // fDISEvt = new DISData();
  // fHadronsPtr = &fHadrons;
  fOutFile = new TFile(disEvtFileName.c_str(),"RECREATE");
  fDISEvtTree = new TTree("DISEvtTree","DIS event and hadron info");
  // fDISEvtTree->Branch("DISEvt","DISData",&fDISEvt);
  // fDISEvtTree->Branch("Hadrons","std::vector<HadronData>",&fHadronsPtr);

  fDISEvtTree->Branch("p0x","Double_t",&p0x);
  fDISEvtTree->Branch("p0y","Double_t",&p0y);
  fDISEvtTree->Branch("p0z","Double_t",&p0z);
  fDISEvtTree->Branch("E0","Double_t",&E0);
  fDISEvtTree->Branch("E1","Double_t",&E1);
  fDISEvtTree->Branch("p1x","Double_t",&p1x);
  fDISEvtTree->Branch("p1y","Double_t",&p1y);
  fDISEvtTree->Branch("p1z","Double_t",&p1z);
  fDISEvtTree->Branch("E_beam","Double_t",&E_beam);
  fDISEvtTree->Branch("E_mu_prim","Double_t",&E_mu_prim);
  fDISEvtTree->Branch("Charge","Int_t",&Charge);
  fDISEvtTree->Branch("theta","Double_t",&theta);
  fDISEvtTree->Branch("Q2","Double_t",&Q2);
  fDISEvtTree->Branch("nu","Double_t",&nu);
  fDISEvtTree->Branch("Y","Double_t",&Y);
  fDISEvtTree->Branch("xBj","Double_t",&xBj);
  fDISEvtTree->Branch("W","Double_t",&W);

  fDISEvtTree->Branch("px","std::vector<Double_t>",&px);
  fDISEvtTree->Branch("py","std::vector<Double_t>",&py);
  fDISEvtTree->Branch("pz","std::vector<Double_t>",&pz);
  fDISEvtTree->Branch("P","std::vector<Double_t>",&P);
  fDISEvtTree->Branch("pt","std::vector<Double_t>",&pt);
  fDISEvtTree->Branch("th","std::vector<Double_t>",&th);
  fDISEvtTree->Branch("ph","std::vector<Double_t>",&ph);
  fDISEvtTree->Branch("ph_pl","std::vector<Double_t>",&ph_pl);
  fDISEvtTree->Branch("charge","std::vector<Int_t>",&charge);
  fDISEvtTree->Branch("PID","std::vector<Int_t>",&PID);
  fDISEvtTree->Branch("E","std::vector<Double_t>",&E);
  fDISEvtTree->Branch("z","std::vector<Double_t>",&z);

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
      PID.push_back(tDjangoh->GetK(j,2)); z.push_back(tDjangoh->GetP(j,4)/fDISEvt->nu);

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
