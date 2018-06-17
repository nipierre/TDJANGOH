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

//Outputs

#define hadron_z_pdf "hadron_z.pdf"
#define pion_z_pdf "pion_z.pdf"
#define kaon_z_pdf "kaon_z.pdf"
#define hadron_pt_pdf "hadron_pt.pdf"
#define pion_pt_pdf "pion_pt.pdf"
#define kaon_pt_pdf "kaon_pt.pdf"
#define hadron_ratio_pdf "hadron_ratio.pdf"
#define pion_ratio_pdf "pion_ratio.pdf"
#define kaon_ratio_pdf "kaon_ratio.pdf"
#define hadron_ratio_pt_pdf "hadron_pt_ratio.pdf"
#define pion_ratio_pt_pdf "pion_pt_ratio.pdf"
#define kaon_ratio_pt_pdf "kaon_pt_ratio.pdf"

// Structures

struct Wrapper { double tab[2][2][4]; };
struct Pvsz { vector<double> vec[2][5]; };
struct hadiden { vector<double> vec; };
struct studyxy { vector<double> vec[2]; };

//Binning

Wrapper fRe[9][6][14];
Wrapper fBorn[9][6][14];
Wrapper fReMult[9][6][14];
Wrapper fBornMult[9][6][14];
Wrapper fMult[9][6][14];
Wrapper fRept[9][6][12];
Wrapper fBornpt[9][6][12];
Wrapper fReptMult[9][6][12];
Wrapper fBornptMult[9][6][12];
Wrapper fPtMult[9][6][12];
double fNDIS_evt_r[3][9][6][14];
double fNDIS_evt_b[3][9][6][14];
double fNSIDIS_evt_r[4][9][6];
double fNSIDIS_evt_b[4][9][6];
int xbin, ybin, zbin, ptbin;
double fZrange[15] = {0,.20,.25,.30,.35,.40,.45,.50,.55,.60,.65,.70,.75,.85,1};
double fXrange[10] = {.004,.01,.02,.03,.04,.06,.1,.14,.18,.4};
double fYrange[7] = {.1,.15,.2,.3,.5,.7,.9};
int fFlag_r[3][9][6][14];
int fFlag_b[3][9][6][14];
double fZ_bin_width[14] = {.1,.05,.05,.05,.05,.05,.05,.05,.05,.05,.05,.05,.1,.15};
double fPt_bin_width = 0.5;
vector<double> fQ2[2][9][6][14];

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

TCanvas c1("Hadron_z","Hadron_z",3200,1600);
TCanvas c2("Pion_z","Pion_z",3200,1600);
TCanvas c3("Kaon_z","Kaon_z",3200,1600);
TCanvas c4("Hadron_pt","Hadron_pt",3200,1600);
TCanvas c5("Pion_pt","Pion_pt",3200,1600);
TCanvas c6("Kaon_pt","Kaon_pt",3200,1600);
TCanvas c7("Hadron_ratio_z","Hadron_ratio_z",3200,1600);
TCanvas c8("Pion_ratio_z","Pion_ratio_z",3200,1600);
TCanvas c9("Kaon_ratio_z","Kaon_ratio_z",3200,1600);
TCanvas c10("Hadron_ratio_pt","Hadron_ratio_pt",3200,1600);
TCanvas c11("Pion_ratio_pt","Pion_ratio_pt",3200,1600);
TCanvas c12("Kaon_ratio_pt","Kaon_ratio_pt",3200,1600);

TGraphErrors* H_r[2][9][6];
TGraphErrors* P_r[2][9][6];
TGraphErrors* K_r[2][9][6];

TGraphErrors* H_b[2][9][6];
TGraphErrors* P_b[2][9][6];
TGraphErrors* K_b[2][9][6];

TGraphErrors* H_d[2][9][6];
TGraphErrors* P_d[2][9][6];
TGraphErrors* K_d[2][9][6];

TGraphErrors* H_pt_r[2][9][6];
TGraphErrors* P_pt_r[2][9][6];
TGraphErrors* K_pt_r[2][9][6];

TGraphErrors* H_pt_b[2][9][6];
TGraphErrors* P_pt_b[2][9][6];
TGraphErrors* K_pt_b[2][9][6];

TGraphErrors* H_pt_d[2][9][6];
TGraphErrors* P_pt_d[2][9][6];
TGraphErrors* K_pt_d[2][9][6];

TLine* l1[6];
TLine* l2[6];

// Others

int id, npart;
double E, E_prime, xbj, y, Q2, px, py, pz, Eh, pt, z, nu, W;
string dummy;

int fId;
double fNu_max[3][14];
double fNu_min[3][14];
int fDIScuts;
