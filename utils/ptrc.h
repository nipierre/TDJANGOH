#include <iostream>
#include <fstream>
#include <algorithm>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TMatrixD.h>
#include <TMatrixTUtils.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include <TArrow.h>
#include <TAxis.h>
#include <TLorentzVector.h>
#include <TVector.h>
#include <TLine.h>
#include <TLatex.h>
#include <vector>
#include <math.h>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <utility>

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

#define _USE_MATH_DEFINES

using namespace std;

// Structures

struct Wrapper { double tab[2][2][4]; };
struct Pvsz { vector<double> vec[2][5]; };
struct hadiden { vector<double> vec; };
struct studyxy { vector<double> vec[2]; };

//Binning

Wrapper fRe[8][5][4][30];
Wrapper fBorn[8][5][4][30];
Wrapper fReMult[8][5][4][30];
Wrapper fBornMult[8][5][4][30];
Wrapper fMult[8][5][4][30];
double fNDIS_evt_r[3][8][5];
double fNDIS_evt_b[3][8][5];
int xbin, Q2bin, zbin, ptbin;
double fXrange[9] = {.003,.008,.013,.02,.032,.055,.1,.21,.4};
double fQ2range[6] = {1.0,1.7,3.0,7.0,16,81};
double fZrange[7] = {.2,.3,.4,.6,.8};
double fpTrange[31] = {.02,.04,.06,.08,.1,.12,.14,.17,.196,
                       .23,.27,.30,.35,.40,.46,.52,.6,.68,
                       .76,.87,1.,1.12,1.24,1.38,1.52,1.68,1.85,
                       2.05,2.35,2.65,3.};
double fZ_bin_width[14] = {.1,.05,.05,.05,.05,.05,.05,.05,.05,.05,.05,.05,.1,.15};
double fPt_bin_width = 0.5;

//Graphic Style

int fMarkerColor[2] = {4,2};
int fMarkerStyle[2][2] = {{24,20},{26,22}};
int fMarkerColorAlt[6] = {2,95,209,226,4,221};
int fMarkerStyleAlt[6][2] = {{24,20},{26,22},{25,21},{27,33},{28,34},{30,29}};

//Constants

static const double fM_p = 938.272046/(1e3);
static const double fM_mu = 105.6583715/(1e3);
static const double fM_K = 493.677/(1e3);
static const double fM_pi = 139.57018/(1e3);

// Canvas & Graphs

TLine* l1[6];
TLine* l2[6];

// Others

int id, npart;
double E, E_prime, xbj, y, Q2, px, py, pz, Eh, pt, z, nu, W;
string dummy;

int fId;
int fDIScuts;

int fIsMu, fIsE;
