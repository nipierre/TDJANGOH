#include <iostream>
#include <fstream>
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
#include <vector>
#include <math.h>

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

#define RCz_pdf "RCz.pdf"
#define RCpt_pdf "RCpt.pdf"
#define RCx_pdf "RCxh.pdf"
#define RCy_pdf "RCyh.pdf"

#define _USE_MATH_DEFINES

using namespace std;

int main(int argc,char *argv[])
{
  string FileRC, FileBorn, fileFlag;

  for (int i = 1; i < argc; i++)
  {
    if (string(argv[i]) == "-h")
    {
      cout << FCYN("HELP : available flags :") << endl;
      cout << FCYN("-f [RC_file] [Born_file]") << endl;
      return 0;
    }
    if(i+2 < argc)
    {
      if (string(argv[i]) == "-f" && fileFlag != "-l" && fileFlag != "-sigf")
      {
        FileRC = argv[i+1];
        FileBorn = argv[i+2];
        fileFlag = "-f";
      }
    }
  }

  if(fileFlag != "-f")
  {
    cout << BOLD(FRED("ERROR : expected some flags")) << endl;
    cout << BOLD(FRED("Expected -f for input files !")) << endl;
    cout << BOLD(FRED("USAGE : \n ./rc_calc -f [RC_file] [Born_file]")) << endl;
    cout << BOLD(FRED("[-h for further help]")) << endl;
    return 1;
  }

  double sigtotre = 142.5;
  double sigtotborn = 129.5;
  double sigre_z[2][20];
  double sigborn_z[2][20];
  double sigre_pt[2][20];
  double sigborn_pt[2][20];
  double sigre_x[2][20];
  double sigborn_x[2][20];
  double sigre_y[2][20];
  double sigborn_y[2][20];
  double rcz[2][20];
  double rcze[2][20];
  double rcpt[2][20];
  double rcpte[2][20];
  double rcx[2][20];
  double rcxe[2][20];
  double rcy[2][20];
  double rcye[2][20];
  double z_t_re[2][20];
  double z_t_be[2][20];
  double pt_t_re[2][20];
  double pt_t_be[2][20];
  double x_t_re[2][20];
  double x_t_be[2][20];
  double y_t_re[2][20];
  double y_t_be[2][20];
  double zmid[20] = {0.025,0.075,0.125,0.175,0.225,
                     0.275,0.325,0.375,0.425,0.475,
                     0.525,0.575,0.625,0.675,0.725,
                     0.775,0.825,0.875,0.925,0.975};
  double ptmid[20] = {0.075,0.225,0.375,0.525,0.675,
                      0.825,0.975,1.125,1.275,1.425,
                      1.575,1.725,1.875,2.025,2.175,
                      2.325,2.475,2.625,2.775,2.925};
  double xmid[20] = {0.025,0.075,0.125,0.175,0.225,
                     0.275,0.325,0.375,0.425,0.475,
                     0.525,0.575,0.625,0.675,0.725,
                     0.775,0.825,0.875,0.925,0.975};
  double ymid[20] = {0.025,0.075,0.125,0.175,0.225,
                     0.275,0.325,0.375,0.425,0.475,
                     0.525,0.575,0.625,0.675,0.725,
                     0.775,0.825,0.875,0.925,0.975};
  string hadron_t[2] = {"h^{+}","h^{-}"};
  TCanvas c1("RC_f(z)","RC_f(z)",3200,1600);
  c1.Divide(4,2);
  TCanvas c2("RC_f(pt)","RC_f(pt)",3200,1600);
  c2.Divide(4,2);
  TCanvas c3("RC_f(x)","RC_f(x)",3200,1600);
  c3.Divide(4,2);
  TCanvas c4("RC_f(y)","RC_f(y)",3200,1600);
  c4.Divide(4,2);

  TGraphErrors* rcz_g[8];
  TGraphErrors* rcpt_g[8];
  TGraphErrors* rcx_g[8];
  TGraphErrors* rcy_g[8];

  int evtotre, evtotborn;
  int id, npart;
  double E, E_prime, xbj, y, Q2, px, py, pz, Eh, dummy, pt, z;
  double M_pi = 0.13957018;

  evtotre = 0;
  evtotborn = 0;
  E_prime = 0;

  if(fileFlag == "-f")
  {
    ifstream revt(FileRC);

    while(revt >> npart)
    {
      revt >> E >> xbj >> y >> Q2;
      for(int i=0; i<npart; i++)
      {
        revt >> dummy >> id >> dummy >> dummy >> dummy;
        revt >> px >> py >> pz >> Eh;
        revt >> dummy >> dummy >> dummy >> dummy;

        if(abs(id)==13) E_prime = Eh;
        if(id!=22 && abs(id)!=13 && id>0)
        {
          pt = sqrt(pow(px,2)+pow(py,2));
          z = sqrt(pow(Eh,2)+pow(M_pi,2))/(E-E_prime);

          if(0<z && z<0.05) z_t_re[0][0]++;
          else if(0.05<z && z<0.10) z_t_re[0][1]++;
          else if(0.10<z && z<0.15) z_t_re[0][2]++;
          else if(0.15<z && z<0.20) z_t_re[0][3]++;
          else if(0.20<z && z<0.25) z_t_re[0][4]++;
          else if(0.25<z && z<0.30) z_t_re[0][5]++;
          else if(0.30<z && z<0.35) z_t_re[0][6]++;
          else if(0.35<z && z<0.40) z_t_re[0][7]++;
          else if(0.40<z && z<0.45) z_t_re[0][8]++;
          else if(0.45<z && z<0.50) z_t_re[0][9]++;
          else if(0.50<z && z<0.55) z_t_re[0][10]++;
          else if(0.55<z && z<0.60) z_t_re[0][11]++;
          else if(0.60<z && z<0.65) z_t_re[0][12]++;
          else if(0.65<z && z<0.70) z_t_re[0][13]++;
          else if(0.70<z && z<0.75) z_t_re[0][14]++;
          else if(0.75<z && z<0.80) z_t_re[0][15]++;
          else if(0.80<z && z<0.85) z_t_re[0][16]++;
          else if(0.85<z && z<0.90) z_t_re[0][17]++;
          else if(0.90<z && z<0.95) z_t_re[0][18]++;
          else z_t_re[0][19]++;

          if(0.00<pt && pt<0.15) pt_t_re[0][0]++;
          else if(0.15<pt && pt<0.30) pt_t_re[0][1]++;
          else if(0.30<pt && pt<0.45) pt_t_re[0][2]++;
          else if(0.45<pt && pt<0.60) pt_t_re[0][3]++;
          else if(0.60<pt && pt<0.75) pt_t_re[0][4]++;
          else if(0.75<pt && pt<0.90) pt_t_re[0][5]++;
          else if(0.90<pt && pt<1.05) pt_t_re[0][6]++;
          else if(1.05<pt && pt<1.20) pt_t_re[0][7]++;
          else if(1.20<pt && pt<1.35) pt_t_re[0][8]++;
          else if(1.35<pt && pt<1.50) pt_t_re[0][9]++;
          else if(1.50<pt && pt<1.65) pt_t_re[0][10]++;
          else if(1.65<pt && pt<1.80) pt_t_re[0][11]++;
          else if(1.80<pt && pt<1.95) pt_t_re[0][12]++;
          else if(1.95<pt && pt<2.10) pt_t_re[0][13]++;
          else if(2.10<pt && pt<2.25) pt_t_re[0][14]++;
          else if(2.25<pt && pt<2.40) pt_t_re[0][15]++;
          else if(2.40<pt && pt<2.55) pt_t_re[0][16]++;
          else if(2.55<pt && pt<2.70) pt_t_re[0][17]++;
          else if(2.70<pt && pt<2.85) pt_t_re[0][18]++;
          else if(2.85<pt && pt<3.00) pt_t_re[0][19]++;
          if(pt>6) cout << "large pt : " << pt << " > 6.0." << endl;

          if(0<xbj && xbj<0.05) x_t_re[0][0]++;
          else if(0.05<xbj && xbj<0.10) x_t_re[0][1]++;
          else if(0.10<xbj && xbj<0.15) x_t_re[0][2]++;
          else if(0.15<xbj && xbj<0.20) x_t_re[0][3]++;
          else if(0.20<xbj && xbj<0.25) x_t_re[0][4]++;
          else if(0.25<xbj && xbj<0.30) x_t_re[0][5]++;
          else if(0.30<xbj && xbj<0.35) x_t_re[0][6]++;
          else if(0.35<xbj && xbj<0.40) x_t_re[0][7]++;
          else if(0.40<xbj && xbj<0.45) x_t_re[0][8]++;
          else if(0.45<xbj && xbj<0.50) x_t_re[0][9]++;
          else if(0.50<xbj && xbj<0.55) x_t_re[0][10]++;
          else if(0.55<xbj && xbj<0.60) x_t_re[0][11]++;
          else if(0.60<xbj && xbj<0.65) x_t_re[0][12]++;
          else if(0.65<xbj && xbj<0.70) x_t_re[0][13]++;
          else if(0.70<xbj && xbj<0.75) x_t_re[0][14]++;
          else if(0.75<xbj && xbj<0.80) x_t_re[0][15]++;
          else if(0.80<xbj && xbj<0.85) x_t_re[0][16]++;
          else if(0.85<xbj && xbj<0.90) x_t_re[0][17]++;
          else if(0.90<xbj && xbj<0.95) x_t_re[0][18]++;
          else x_t_re[0][19]++;

          if(0<y && y<0.05) y_t_re[0][0]++;
          else if(0.05<y && y<0.10) y_t_re[0][1]++;
          else if(0.10<y && y<0.15) y_t_re[0][2]++;
          else if(0.15<y && y<0.20) y_t_re[0][3]++;
          else if(0.20<y && y<0.25) y_t_re[0][4]++;
          else if(0.25<y && y<0.30) y_t_re[0][5]++;
          else if(0.30<y && y<0.35) y_t_re[0][6]++;
          else if(0.35<y && y<0.40) y_t_re[0][7]++;
          else if(0.40<y && y<0.45) y_t_re[0][8]++;
          else if(0.45<y && y<0.50) y_t_re[0][9]++;
          else if(0.50<y && y<0.55) y_t_re[0][10]++;
          else if(0.55<y && y<0.60) y_t_re[0][11]++;
          else if(0.60<y && y<0.65) y_t_re[0][12]++;
          else if(0.65<y && y<0.70) y_t_re[0][13]++;
          else if(0.70<y && y<0.75) y_t_re[0][14]++;
          else if(0.75<y && y<0.80) y_t_re[0][15]++;
          else if(0.80<y && y<0.85) y_t_re[0][16]++;
          else if(0.85<y && y<0.90) y_t_re[0][17]++;
          else if(0.90<y && y<0.95) y_t_re[0][18]++;
          else y_t_re[0][19]++;
        }
        if(id!=22 && abs(id)!=13 && id<0)
        {
          pt = sqrt(pow(px,2)+pow(py,2));
          z = sqrt(pow(Eh,2)+pow(M_pi,2))/(E-E_prime);

          if(0<z && z<0.05) z_t_re[1][0]++;
          else if(0.05<z && z<0.10) z_t_re[1][1]++;
          else if(0.10<z && z<0.15) z_t_re[1][2]++;
          else if(0.15<z && z<0.20) z_t_re[1][3]++;
          else if(0.20<z && z<0.25) z_t_re[1][4]++;
          else if(0.25<z && z<0.30) z_t_re[1][5]++;
          else if(0.30<z && z<0.35) z_t_re[1][6]++;
          else if(0.35<z && z<0.40) z_t_re[1][7]++;
          else if(0.40<z && z<0.45) z_t_re[1][8]++;
          else if(0.45<z && z<0.50) z_t_re[1][9]++;
          else if(0.50<z && z<0.55) z_t_re[1][10]++;
          else if(0.55<z && z<0.60) z_t_re[1][11]++;
          else if(0.60<z && z<0.65) z_t_re[1][12]++;
          else if(0.65<z && z<0.70) z_t_re[1][13]++;
          else if(0.70<z && z<0.75) z_t_re[1][14]++;
          else if(0.75<z && z<0.80) z_t_re[1][15]++;
          else if(0.80<z && z<0.85) z_t_re[1][16]++;
          else if(0.85<z && z<0.90) z_t_re[1][17]++;
          else if(0.90<z && z<0.95) z_t_re[1][18]++;
          else z_t_re[1][19]++;

          if(0.00<pt && pt<0.15) pt_t_re[1][0]++;
          else if(0.15<pt && pt<0.30) pt_t_re[1][1]++;
          else if(0.30<pt && pt<0.45) pt_t_re[1][2]++;
          else if(0.45<pt && pt<0.60) pt_t_re[1][3]++;
          else if(0.60<pt && pt<0.75) pt_t_re[1][4]++;
          else if(0.75<pt && pt<0.90) pt_t_re[1][5]++;
          else if(0.90<pt && pt<1.05) pt_t_re[1][6]++;
          else if(1.05<pt && pt<1.20) pt_t_re[1][7]++;
          else if(1.20<pt && pt<1.35) pt_t_re[1][8]++;
          else if(1.35<pt && pt<1.50) pt_t_re[1][9]++;
          else if(1.50<pt && pt<1.65) pt_t_re[1][10]++;
          else if(1.65<pt && pt<1.80) pt_t_re[1][11]++;
          else if(1.80<pt && pt<1.95) pt_t_re[1][12]++;
          else if(1.95<pt && pt<2.10) pt_t_re[1][13]++;
          else if(2.10<pt && pt<2.25) pt_t_re[1][14]++;
          else if(2.25<pt && pt<2.40) pt_t_re[1][15]++;
          else if(2.40<pt && pt<2.55) pt_t_re[1][16]++;
          else if(2.55<pt && pt<2.70) pt_t_re[1][17]++;
          else if(2.70<pt && pt<2.85) pt_t_re[1][18]++;
          else if(2.85<pt && pt<3.00) pt_t_re[1][19]++;
          if(pt>6) cout << "large pt : " << pt << " > 6.0." << endl;

          if(0<xbj && xbj<0.05) x_t_re[1][0]++;
          else if(0.05<xbj && xbj<0.10) x_t_re[1][1]++;
          else if(0.10<xbj && xbj<0.15) x_t_re[1][2]++;
          else if(0.15<xbj && xbj<0.20) x_t_re[1][3]++;
          else if(0.20<xbj && xbj<0.25) x_t_re[1][4]++;
          else if(0.25<xbj && xbj<0.30) x_t_re[1][5]++;
          else if(0.30<xbj && xbj<0.35) x_t_re[1][6]++;
          else if(0.35<xbj && xbj<0.40) x_t_re[1][7]++;
          else if(0.40<xbj && xbj<0.45) x_t_re[1][8]++;
          else if(0.45<xbj && xbj<0.50) x_t_re[1][9]++;
          else if(0.50<xbj && xbj<0.55) x_t_re[1][10]++;
          else if(0.55<xbj && xbj<0.60) x_t_re[1][11]++;
          else if(0.60<xbj && xbj<0.65) x_t_re[1][12]++;
          else if(0.65<xbj && xbj<0.70) x_t_re[1][13]++;
          else if(0.70<xbj && xbj<0.75) x_t_re[1][14]++;
          else if(0.75<xbj && xbj<0.80) x_t_re[1][15]++;
          else if(0.80<xbj && xbj<0.85) x_t_re[1][16]++;
          else if(0.85<xbj && xbj<0.90) x_t_re[1][17]++;
          else if(0.90<xbj && xbj<0.95) x_t_re[1][18]++;
          else x_t_re[1][19]++;

          if(0<y && y<0.05) y_t_re[1][0]++;
          else if(0.05<y && y<0.10) y_t_re[1][1]++;
          else if(0.10<y && y<0.15) y_t_re[1][2]++;
          else if(0.15<y && y<0.20) y_t_re[1][3]++;
          else if(0.20<y && y<0.25) y_t_re[1][4]++;
          else if(0.25<y && y<0.30) y_t_re[1][5]++;
          else if(0.30<y && y<0.35) y_t_re[1][6]++;
          else if(0.35<y && y<0.40) y_t_re[1][7]++;
          else if(0.40<y && y<0.45) y_t_re[1][8]++;
          else if(0.45<y && y<0.50) y_t_re[1][9]++;
          else if(0.50<y && y<0.55) y_t_re[1][10]++;
          else if(0.55<y && y<0.60) y_t_re[1][11]++;
          else if(0.60<y && y<0.65) y_t_re[1][12]++;
          else if(0.65<y && y<0.70) y_t_re[1][13]++;
          else if(0.70<y && y<0.75) y_t_re[1][14]++;
          else if(0.75<y && y<0.80) y_t_re[1][15]++;
          else if(0.80<y && y<0.85) y_t_re[1][16]++;
          else if(0.85<y && y<0.90) y_t_re[1][17]++;
          else if(0.90<y && y<0.95) y_t_re[1][18]++;
          else y_t_re[1][19]++;
        }
      }
      evtotre++;
    }

    revt.close();

    ifstream bevt(FileBorn);

    while(bevt >> npart)
    {
      bevt >> E >> xbj >> y >> Q2;
      for(int i=0; i<npart; i++)
      {
        bevt >> dummy >> id >> dummy >> dummy >> dummy;
        bevt >> px >> py >> pz >> Eh;
        bevt >> dummy >> dummy >> dummy >> dummy;

        if(abs(id)==13) E_prime = Eh;
        if(id!=22 && abs(id)!=13 && id>0)
        {
          pt = sqrt(pow(px,2)+pow(py,2));
          z = sqrt(pow(Eh,2)+pow(M_pi,2))/(E-E_prime);

          if(0<z && z<0.05) z_t_re[0][0]++;
          else if(0.05<z && z<0.10) z_t_re[0][1]++;
          else if(0.10<z && z<0.15) z_t_re[0][2]++;
          else if(0.15<z && z<0.20) z_t_re[0][3]++;
          else if(0.20<z && z<0.25) z_t_re[0][4]++;
          else if(0.25<z && z<0.30) z_t_re[0][5]++;
          else if(0.30<z && z<0.35) z_t_re[0][6]++;
          else if(0.35<z && z<0.40) z_t_re[0][7]++;
          else if(0.40<z && z<0.45) z_t_re[0][8]++;
          else if(0.45<z && z<0.50) z_t_re[0][9]++;
          else if(0.50<z && z<0.55) z_t_re[0][10]++;
          else if(0.55<z && z<0.60) z_t_re[0][11]++;
          else if(0.60<z && z<0.65) z_t_re[0][12]++;
          else if(0.65<z && z<0.70) z_t_re[0][13]++;
          else if(0.70<z && z<0.75) z_t_re[0][14]++;
          else if(0.75<z && z<0.80) z_t_re[0][15]++;
          else if(0.80<z && z<0.85) z_t_re[0][16]++;
          else if(0.85<z && z<0.90) z_t_re[0][17]++;
          else if(0.90<z && z<0.95) z_t_re[0][18]++;
          else z_t_re[0][19]++;

          if(0.00<pt && pt<0.15) pt_t_re[0][0]++;
          else if(0.15<pt && pt<0.30) pt_t_re[0][1]++;
          else if(0.30<pt && pt<0.45) pt_t_re[0][2]++;
          else if(0.45<pt && pt<0.60) pt_t_re[0][3]++;
          else if(0.60<pt && pt<0.75) pt_t_re[0][4]++;
          else if(0.75<pt && pt<0.90) pt_t_re[0][5]++;
          else if(0.90<pt && pt<1.05) pt_t_re[0][6]++;
          else if(1.05<pt && pt<1.20) pt_t_re[0][7]++;
          else if(1.20<pt && pt<1.35) pt_t_re[0][8]++;
          else if(1.35<pt && pt<1.50) pt_t_re[0][9]++;
          else if(1.50<pt && pt<1.65) pt_t_re[0][10]++;
          else if(1.65<pt && pt<1.80) pt_t_re[0][11]++;
          else if(1.80<pt && pt<1.95) pt_t_re[0][12]++;
          else if(1.95<pt && pt<2.10) pt_t_re[0][13]++;
          else if(2.10<pt && pt<2.25) pt_t_re[0][14]++;
          else if(2.25<pt && pt<2.40) pt_t_re[0][15]++;
          else if(2.40<pt && pt<2.55) pt_t_re[0][16]++;
          else if(2.55<pt && pt<2.70) pt_t_re[0][17]++;
          else if(2.70<pt && pt<2.85) pt_t_re[0][18]++;
          else if(2.85<pt && pt<3.00) pt_t_re[0][19]++;
          if(pt>6) cout << "large pt : " << pt << " > 6.0." << endl;

          if(0<xbj && xbj<0.05) x_t_re[0][0]++;
          else if(0.05<xbj && xbj<0.10) x_t_re[0][1]++;
          else if(0.10<xbj && xbj<0.15) x_t_re[0][2]++;
          else if(0.15<xbj && xbj<0.20) x_t_re[0][3]++;
          else if(0.20<xbj && xbj<0.25) x_t_re[0][4]++;
          else if(0.25<xbj && xbj<0.30) x_t_re[0][5]++;
          else if(0.30<xbj && xbj<0.35) x_t_re[0][6]++;
          else if(0.35<xbj && xbj<0.40) x_t_re[0][7]++;
          else if(0.40<xbj && xbj<0.45) x_t_re[0][8]++;
          else if(0.45<xbj && xbj<0.50) x_t_re[0][9]++;
          else if(0.50<xbj && xbj<0.55) x_t_re[0][10]++;
          else if(0.55<xbj && xbj<0.60) x_t_re[0][11]++;
          else if(0.60<xbj && xbj<0.65) x_t_re[0][12]++;
          else if(0.65<xbj && xbj<0.70) x_t_re[0][13]++;
          else if(0.70<xbj && xbj<0.75) x_t_re[0][14]++;
          else if(0.75<xbj && xbj<0.80) x_t_re[0][15]++;
          else if(0.80<xbj && xbj<0.85) x_t_re[0][16]++;
          else if(0.85<xbj && xbj<0.90) x_t_re[0][17]++;
          else if(0.90<xbj && xbj<0.95) x_t_re[0][18]++;
          else x_t_re[0][19]++;

          if(0<y && y<0.05) y_t_re[0][0]++;
          else if(0.05<y && y<0.10) y_t_re[0][1]++;
          else if(0.10<y && y<0.15) y_t_re[0][2]++;
          else if(0.15<y && y<0.20) y_t_re[0][3]++;
          else if(0.20<y && y<0.25) y_t_re[0][4]++;
          else if(0.25<y && y<0.30) y_t_re[0][5]++;
          else if(0.30<y && y<0.35) y_t_re[0][6]++;
          else if(0.35<y && y<0.40) y_t_re[0][7]++;
          else if(0.40<y && y<0.45) y_t_re[0][8]++;
          else if(0.45<y && y<0.50) y_t_re[0][9]++;
          else if(0.50<y && y<0.55) y_t_re[0][10]++;
          else if(0.55<y && y<0.60) y_t_re[0][11]++;
          else if(0.60<y && y<0.65) y_t_re[0][12]++;
          else if(0.65<y && y<0.70) y_t_re[0][13]++;
          else if(0.70<y && y<0.75) y_t_re[0][14]++;
          else if(0.75<y && y<0.80) y_t_re[0][15]++;
          else if(0.80<y && y<0.85) y_t_re[0][16]++;
          else if(0.85<y && y<0.90) y_t_re[0][17]++;
          else if(0.90<y && y<0.95) y_t_re[0][18]++;
          else y_t_re[0][19]++;
        }
        if(id!=22 && abs(id)!=13 && id<0)
        {
          pt = sqrt(pow(px,2)+pow(py,2));
          z = sqrt(pow(Eh,2)+pow(M_pi,2))/(E-E_prime);

          if(0<z && z<0.05) z_t_re[1][0]++;
          else if(0.05<z && z<0.10) z_t_re[1][1]++;
          else if(0.10<z && z<0.15) z_t_re[1][2]++;
          else if(0.15<z && z<0.20) z_t_re[1][3]++;
          else if(0.20<z && z<0.25) z_t_re[1][4]++;
          else if(0.25<z && z<0.30) z_t_re[1][5]++;
          else if(0.30<z && z<0.35) z_t_re[1][6]++;
          else if(0.35<z && z<0.40) z_t_re[1][7]++;
          else if(0.40<z && z<0.45) z_t_re[1][8]++;
          else if(0.45<z && z<0.50) z_t_re[1][9]++;
          else if(0.50<z && z<0.55) z_t_re[1][10]++;
          else if(0.55<z && z<0.60) z_t_re[1][11]++;
          else if(0.60<z && z<0.65) z_t_re[1][12]++;
          else if(0.65<z && z<0.70) z_t_re[1][13]++;
          else if(0.70<z && z<0.75) z_t_re[1][14]++;
          else if(0.75<z && z<0.80) z_t_re[1][15]++;
          else if(0.80<z && z<0.85) z_t_re[1][16]++;
          else if(0.85<z && z<0.90) z_t_re[1][17]++;
          else if(0.90<z && z<0.95) z_t_re[1][18]++;
          else z_t_re[1][19]++;

          if(0.00<pt && pt<0.15) pt_t_re[1][0]++;
          else if(0.15<pt && pt<0.30) pt_t_re[1][1]++;
          else if(0.30<pt && pt<0.45) pt_t_re[1][2]++;
          else if(0.45<pt && pt<0.60) pt_t_re[1][3]++;
          else if(0.60<pt && pt<0.75) pt_t_re[1][4]++;
          else if(0.75<pt && pt<0.90) pt_t_re[1][5]++;
          else if(0.90<pt && pt<1.05) pt_t_re[1][6]++;
          else if(1.05<pt && pt<1.20) pt_t_re[1][7]++;
          else if(1.20<pt && pt<1.35) pt_t_re[1][8]++;
          else if(1.35<pt && pt<1.50) pt_t_re[1][9]++;
          else if(1.50<pt && pt<1.65) pt_t_re[1][10]++;
          else if(1.65<pt && pt<1.80) pt_t_re[1][11]++;
          else if(1.80<pt && pt<1.95) pt_t_re[1][12]++;
          else if(1.95<pt && pt<2.10) pt_t_re[1][13]++;
          else if(2.10<pt && pt<2.25) pt_t_re[1][14]++;
          else if(2.25<pt && pt<2.40) pt_t_re[1][15]++;
          else if(2.40<pt && pt<2.55) pt_t_re[1][16]++;
          else if(2.55<pt && pt<2.70) pt_t_re[1][17]++;
          else if(2.70<pt && pt<2.85) pt_t_re[1][18]++;
          else if(2.85<pt && pt<3.00) pt_t_re[1][19]++;
          if(pt>6) cout << "large pt : " << pt << " > 6.0." << endl;

          if(0<xbj && xbj<0.05) x_t_re[1][0]++;
          else if(0.05<xbj && xbj<0.10) x_t_re[1][1]++;
          else if(0.10<xbj && xbj<0.15) x_t_re[1][2]++;
          else if(0.15<xbj && xbj<0.20) x_t_re[1][3]++;
          else if(0.20<xbj && xbj<0.25) x_t_re[1][4]++;
          else if(0.25<xbj && xbj<0.30) x_t_re[1][5]++;
          else if(0.30<xbj && xbj<0.35) x_t_re[1][6]++;
          else if(0.35<xbj && xbj<0.40) x_t_re[1][7]++;
          else if(0.40<xbj && xbj<0.45) x_t_re[1][8]++;
          else if(0.45<xbj && xbj<0.50) x_t_re[1][9]++;
          else if(0.50<xbj && xbj<0.55) x_t_re[1][10]++;
          else if(0.55<xbj && xbj<0.60) x_t_re[1][11]++;
          else if(0.60<xbj && xbj<0.65) x_t_re[1][12]++;
          else if(0.65<xbj && xbj<0.70) x_t_re[1][13]++;
          else if(0.70<xbj && xbj<0.75) x_t_re[1][14]++;
          else if(0.75<xbj && xbj<0.80) x_t_re[1][15]++;
          else if(0.80<xbj && xbj<0.85) x_t_re[1][16]++;
          else if(0.85<xbj && xbj<0.90) x_t_re[1][17]++;
          else if(0.90<xbj && xbj<0.95) x_t_re[1][18]++;
          else x_t_re[1][19]++;

          if(0<y && y<0.05) y_t_re[1][0]++;
          else if(0.05<y && y<0.10) y_t_re[1][1]++;
          else if(0.10<y && y<0.15) y_t_re[1][2]++;
          else if(0.15<y && y<0.20) y_t_re[1][3]++;
          else if(0.20<y && y<0.25) y_t_re[1][4]++;
          else if(0.25<y && y<0.30) y_t_re[1][5]++;
          else if(0.30<y && y<0.35) y_t_re[1][6]++;
          else if(0.35<y && y<0.40) y_t_re[1][7]++;
          else if(0.40<y && y<0.45) y_t_re[1][8]++;
          else if(0.45<y && y<0.50) y_t_re[1][9]++;
          else if(0.50<y && y<0.55) y_t_re[1][10]++;
          else if(0.55<y && y<0.60) y_t_re[1][11]++;
          else if(0.60<y && y<0.65) y_t_re[1][12]++;
          else if(0.65<y && y<0.70) y_t_re[1][13]++;
          else if(0.70<y && y<0.75) y_t_re[1][14]++;
          else if(0.75<y && y<0.80) y_t_re[1][15]++;
          else if(0.80<y && y<0.85) y_t_re[1][16]++;
          else if(0.85<y && y<0.90) y_t_re[1][17]++;
          else if(0.90<y && y<0.95) y_t_re[1][18]++;
          else y_t_re[1][19]++;
        }
      }
      evtotborn++;
    }
  }

  for(int i=0; i<2; i++)
  {
    for(int j=0; j<20; j++)
    {
      sigre_z[i][j] = double(z_t_re[i][j])*double(sigtotre)/double(evtotre);
      sigborn_z[i][j] = double(z_t_be[i][j])*double(sigtotborn)/double(evtotborn);
      sigre_pt[i][j] = double(pt_t_re[i][j])*double(sigtotre)/double(evtotre);
      sigborn_pt[i][j] = double(pt_t_be[i][j])*double(sigtotborn)/double(evtotborn);
      sigre_x[i][j] = double(x_t_re[i][j])*double(sigtotre)/double(evtotre);
      sigborn_x[i][j] = double(x_t_be[i][j])*double(sigtotborn)/double(evtotborn);
      sigre_y[i][j] = double(y_t_re[i][j])*double(sigtotre)/double(evtotre);
      sigborn_y[i][j] = double(y_t_be[i][j])*double(sigtotborn)/double(evtotborn);
      rcz[i][j] = double(sigre_z[i][j])/double(sigborn_z[i][j]);
      rcze[i][j] = double((z_t_re[i][j] ? 1/sqrt(z_t_re[i][j]) : 0)+(z_t_be[i][j] ? 1/sqrt(z_t_be[i][j]) : 0));
      rcpt[i][j] = double(sigre_pt[i][j])/double(sigborn_pt[i][j]);
      rcpte[i][j] = double((pt_t_re[i][j] ? 1/sqrt(pt_t_re[i][j]) : 0)+(pt_t_be[i][j] ? 1/sqrt(pt_t_be[i][j]) : 0));
      rcx[i][j] = (sigborn_x[i][j] ? double(sigre_x[i][j])/double(sigborn_x[i][j]) : 0);
      rcxe[i][j] = double((x_t_re[i][j] ? 1/sqrt(x_t_re[i][j]) : 0)+(x_t_be[i][j] ? 1/sqrt(x_t_be[i][j]) : 0));
      cout << "rcx : " << rcx[i][j] << endl;
      rcy[i][j] = (sigborn_y[i][j] ? double(sigre_y[i][j])/double(sigborn_y[i][j]) : 0);
      rcye[i][j] = double((y_t_re[i][j] ? 1/sqrt(y_t_re[i][j]) : 0)+(y_t_be[i][j] ? 1/sqrt(y_t_be[i][j]) : 0));
      cout << "rcy : " << rcy[i][j] << endl;
    }

    rcz_g[i] = new TGraphErrors(20,zmid,rcz[i],0,rcze[i]);
    rcpt_g[i] = new TGraphErrors(20,ptmid,rcpt[i],0,rcpte[i]);
    rcx_g[i] = new TGraphErrors(20,xmid,rcx[i],0,rcxe[i]);
    rcy_g[i] = new TGraphErrors(20,ymid,rcy[i],0,rcye[i]);

    c1.cd(i+1);
    rcz_g[i]->GetXaxis()->SetTitle("z");
    rcz_g[i]->GetYaxis()->SetTitle("RC");
    rcz_g[i]->GetYaxis()->SetTitleOffset(1.5);
    rcz_g[i]->SetTitle(Form("RC(z) of %s",hadron_t[i].c_str()));
    rcz_g[i]->SetMarkerStyle(22);
    rcz_g[i]->SetMarkerColor(601);
    rcz_g[i]->SetMarkerSize(3);
    rcz_g[i]->GetYaxis()->SetRangeUser(0.8,1.5);
    rcz_g[i]->SetFillColor(601);
    rcz_g[i]->SetFillStyle(3001);
    rcz_g[i]->Draw("AP");
    c1.Update();

    c2.cd(i+1);
    rcpt_g[i]->GetXaxis()->SetTitle("P_{t}");
    rcpt_g[i]->GetYaxis()->SetTitle("RC");
    rcpt_g[i]->GetYaxis()->SetTitleOffset(1.5);
    rcpt_g[i]->SetTitle(Form("RC(P_{t}) of %s",hadron_t[i].c_str()));
    rcpt_g[i]->SetMarkerStyle(22);
    rcpt_g[i]->SetMarkerColor(601);
    rcpt_g[i]->SetMarkerSize(3);
    rcpt_g[i]->GetYaxis()->SetRangeUser(0.8,1.5);
    rcpt_g[i]->SetFillColor(601);
    rcpt_g[i]->SetFillStyle(3001);
    rcpt_g[i]->Draw("AP");
    c2.Update();

    c3.cd(i+1);
    rcx_g[i]->GetXaxis()->SetTitle("x_{Bj}");
    rcx_g[i]->GetYaxis()->SetTitle("RC");
    rcx_g[i]->GetYaxis()->SetTitleOffset(1.5);
    rcx_g[i]->SetTitle(Form("RC(x_{Bj}) of %s",hadron_t[i].c_str()));
    rcx_g[i]->SetMarkerStyle(22);
    rcx_g[i]->SetMarkerColor(601);
    rcx_g[i]->SetMarkerSize(3);
    rcx_g[i]->GetYaxis()->SetRangeUser(0.8,1.5);
    rcx_g[i]->SetFillColor(601);
    rcx_g[i]->SetFillStyle(3001);
    rcx_g[i]->Draw("AP");
    c3.Update();

    c4.cd(i+1);
    rcy_g[i]->GetXaxis()->SetTitle("y");
    rcy_g[i]->GetYaxis()->SetTitle("RC");
    rcy_g[i]->GetYaxis()->SetTitleOffset(1.5);
    rcy_g[i]->SetTitle(Form("RC(y) of %s",hadron_t[i].c_str()));
    rcy_g[i]->SetMarkerStyle(22);
    rcy_g[i]->SetMarkerColor(601);
    rcy_g[i]->SetMarkerSize(3);
    rcy_g[i]->GetYaxis()->SetRangeUser(0.8,1.5);
    rcy_g[i]->SetFillColor(601);
    rcy_g[i]->SetFillStyle(3001);
    rcy_g[i]->Draw("AP");
    c4.Update();

  }

  c1.Print(RCz_pdf);
  c2.Print(RCpt_pdf);
  c3.Print(RCx_pdf);
  c4.Print(RCy_pdf);

  return 0;
}
