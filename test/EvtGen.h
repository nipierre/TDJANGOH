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

Double_t p0x,p0y,p0z,E0; // beam momentum
Double_t p1x,p1y,p1z,E1; // mu1 momentum
Int_t Charge;

Double_t  theta; //!
Double_t     Q2; //!
Double_t     nu; //!
Double_t      Y; //!
Double_t    xBj; //!


vector<Double_t> px, py, pz;
vector<Double_t> P,pt,th,ph,ph_pl;
vector<Int_t> charge;
vector<Int_t> PID;
vector<Double_t> E; //!
vector<Double_t> z; //!

TFile* fOutFile;
TTree* fDISEvtTree;

#endif
