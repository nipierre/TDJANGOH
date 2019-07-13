#ifndef EvtGen_h
#define EvtGen_h 1

#include <iostream>
#include <fstream>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>

using namespace std;

class DISData
{

  public:
  // DISData();
  // virtual ~DISData();

  // private:
  Double_t p0x,p0y,p0z,E0; // beam momentum
  Double_t p1x,p1y,p1z,E1; // mu1 momentum
  Double_t E_beam;    // beam energie
  Double_t E_mu_prim; // mu1 energie
  Int_t Charge;

  // void Reset();
  // void CalcKin();

  Double_t  theta; //!
  Double_t     Q2; //!
  Double_t     nu; //!
  Double_t      Y; //!
  Double_t    xBj; //!
  Double_t      W; //!

  //ClassDef(DISData,5);
};

class HadronData
{

  public:
  // HadronData();
  // virtual ~HadronData();

  // private:
  Double_t px, py, pz;
  Double_t P,pt,th,ph,ph_pl;
  Int_t charge;
  Int_t PID;
  Double_t E; //!
  Double_t z; //!

  // void Reset();
  // void CalcVariables(const double& M=-1);

  //ClassDef(HadronData,3);
};

DISData* fDISEvt;
vector<HadronData> fHadrons;
vector<HadronData>* fHadronsPtr;
TFile* fOutFile;
TTree* fDISEvtTree;

#endif
