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


#define _USE_MATH_DEFINES

#define RCx_pdf "RCx.pdf"
#define RCy_pdf "RCy.pdf"
#define ERx_pdf "ERx.pdf"
#define ERy_pdf "ERy.pdf"
#define ERy_scat_pdf "ERy_scat.pdf"
#define ERREx_pdf "ERREx.pdf"
#define ERREy_pdf "ERREy.pdf"
#define BRx_pdf "BRx.pdf"
#define BRy_pdf "BRy.pdf"
#define BRy_scat_pdf "BRy_scat.pdf"
#define TERy_pdf "TERy.pdf"
#define SIx_pdf "SIx.pdf"
#define SIy_pdf "SIy.pdf"
#define OUTFILE "rc.txt"

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

// LOADING BAR

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

  int XRANGE=0, YRANGE=0;
  double dummy, x, y, Q2, xhad, yhad, Q2had;
  string sdum;
  int id;

  double xtab[21] = {.004,.006,.008,.01,.013,.016,.02,.03,.04,.06,
                     .08,.1,.15,.2,.3,.4,.5,.6,.7,.8,.9};
  double xmid[20] = {.005,.007,.009,.0115,.0145,.018,.025,.035,.05,
                    .07,.09,.125,.175,.25,.35,.45,.55,.65,.75,.85};
  double xtabred[10] = {.004,.01,.02,.03,.04,.06,.1,.14,.18,.4};
  double xmidred[9] = {.007,.015,.025,.035,.05,.08,.12,.16,.29};
  double ytab[17] = {.1,.15,.2,.25,.3,.35,.4,.45,.5,.55,.6,
                    .65,.7,.75,.8,.85,.9};
  double ymid[16] = {.125,.175,.225,.275,.325,.375,.425,.475,
                     .525,.575,.625,.675,.725,.775,.825,.875};
  double ytabred[6] = {.1,.15,.2,.3,.5,.7};
  double ymidred[5] = {.125,.175,.25,.4,.6};
  double xtable[30] = {.000050,.000070,.000100,.000200,.000300,.000500,
                       .000700,.001000,.002000,.004000,.006000,.008000,
                       .010000,.013000,.016000,.020000,.030000,.040000,
                       .060000,.080000,.100000,.150000,.200000,.300000,
                       .400000,.500000,.600000,.700000,.800000,.900000};
  double ytable[19] = {.050000,.100000,.150000,.200000,.250000,.300000,
                       .350000,.400000,.450000,.500000,.550000,.600000,
                       .650000,.700000,.750000,.800000,.850000,.900000,
                       .950000};
  int xsw[10] = {9,12,15,16,17,18,20,21,22,24};
  int ysw[6] = {1,2,3,5,9,13};
  double z_size[14] = {.2,.05,.05,.05,.05,.05,.05,.05,.05,.05,.05,.05,.1,.15};

  double rcx[16][20];
  double rcxb[17][21];
  double erx[16][20];
  double erxb[17][21];
  double errex[16][20];
  double errexb[17][21];
  double rcx_e[16][20];
  double rcx_eb[17][21];
  double rcy[20][16];
  double rcyb[21][17];
  double ery[20][16];
  double eryb[21][17];
  double errey[20][16];
  double erreyb[21][17];
  double rcy_e[20][16];
  double rcy_eb[21][17];
  double rc_table[19][30];
  double rc_table_y[30][19];
  double rc_tablered[18][29];
  double rc_tablered_y[29][18];
  double born_ratio[19][30];
  int re[16][20];
  int born[16][20];
  double erre[17][21];
  double erborn[17][21];
  int evtotre;
  int evtotborn;
  double sigtotre;
  double sigtotborn;
  int evtotre_t[20];
  int evtotborn_t[20];
  double sigtotre_t[20];
  double sigtotborn_t[20];
  double sigre[16][20];
  double sigreb[17][21];
  double sigborn[16][20];
  double sigbornb[17][21];
  double br[17][21];
  double br_y[21][17];
  double sirc[5][9];
  double sircy[9][5];
  double sircz, ztotsize;

  int qel=0;
  string TempFile, FileRC, FileBorn, fileFlag;

  for (int i = 1; i < argc; i++)
  {
    if (string(argv[i]) == "-h")
    {
      cout << FCYN("HELP : available flags :") << endl;
      cout << FCYN("-f [RC_file] [Born_file] : specific files") << endl;
      cout << FCYN("-l [filename_template] : a list of files in xbins (RC//or//Born_[filename_template]_xbin_i_i+1.dat)\n xbins = [.004,.006,.008,.01,.013,.016,.02,.03,.04,.06,.08,.1,.15,.2,.3,.4,.5,.6,.7,.8,.9]") << endl;
      cout << FCYN("-sigf [RC_file] [Born_file] : specific cross-section files") << endl;
      cout << FCYN("-bornratio [Born_file] : compares born contribution") << endl;
      cout << FCYN("-qel : include quasi-elastic contribution for TERAD RC") << endl;
      return 0;
    }
    if (string(argv[i]) == "-qel")
    {
      cout << FBLU("NOTICE : Quasi-elastic included.") << endl;
      qel = 1;
    }
    if (i+1 < argc)
    {
      if (string(argv[i]) == "-l" && fileFlag != "-f" && fileFlag != "-sigf" && string(argv[i]) != "-bornratio" && fileFlag != "-sirc")
      {
        TempFile = argv[i + 1];
        fileFlag = "-l";
        XRANGE=20; YRANGE=16;
      }
      if (string(argv[i]) == "-bornratio" && fileFlag != "-f" && fileFlag != "-sigf" && fileFlag != "-l" && fileFlag != "-sirc")
      {
        FileBorn = argv[i + 1];
        fileFlag = "-bornratio";
      }
      if (string(argv[i]) == "-sirc" && fileFlag != "-f" && fileFlag != "-sigf" && fileFlag != "-l" && fileFlag != "-sirc")
      {
        TempFile = argv[i + 1];
        fileFlag = "-sirc";
      }
    }
    if(i+2 < argc)
    {
      if (string(argv[i]) == "-f" && fileFlag != "-l" && fileFlag != "-sigf" && string(argv[i]) != "-bornratio" && fileFlag != "-sirc")
      {
        FileRC = argv[i+1];
        FileBorn = argv[i+2];
        fileFlag = "-f";
        XRANGE=20; YRANGE=16;
      }
      if (string(argv[i]) == "-sigf" && fileFlag != "-l" && fileFlag != "-f" && string(argv[i]) != "-bornratio" && fileFlag != "-sirc")
      {
        FileRC = argv[i+1];
        FileBorn = argv[i+2];
        fileFlag = "-sigf";
        XRANGE=21; YRANGE=17;
      }
    }
  }

  if(fileFlag != "-l" && fileFlag != "-f" && fileFlag != "-sigf" && fileFlag != "-bornratio" && fileFlag != "-sirc")
  {
    cout << BOLD(FRED("ERROR : expected some flags")) << endl;
    cout << BOLD(FRED("Expected -f or -l for input files !")) << endl;
    cout << BOLD(FRED("USAGE : \n ./rc_calc -f [RC_file] [Born_file] \nOR\n ./rc_calc -l [filename_template] \nOR\n ./rc_calc -sigf [RC_file] [Born_file] \nOR\n ./rc_calc -bornratio [Born_file]")) << endl;
    cout << BOLD(FRED("[-h for further help]")) << endl;
    return 1;
  }

  TCanvas c1("RC_xy_f(x)","RC_xy_f(x)",3200,1600);
  c1.Divide(4,4);
  TCanvas c2("RC_xy_f(y)","RC_xy_f(y)",3200,1600);
  c2.Divide(3,3);
  TCanvas c3("ER_xy_f(x)","ER_xy_f(x)",3200,1600);
  c3.Divide(1,1);
  TCanvas c4("ER_xy_f(y)","ER_xy_f(y)",3200,1600);
  c4.Divide(1,1);
  TCanvas c5("ERRE_xy_f(x)","ERRE_xy_f(x)",3200,1600);
  c5.Divide(1,1);
  TCanvas c6("ERRE_xy_f(y)","ERRE_xy_f(y)",3200,1600);
  c6.Divide(1,1);
  TCanvas c10("ER_xy_f(y) scat","ER_xy_f(y) scat",3200,1600);
  c10.Divide(3,3);

  TGraphErrors* rcx_g[17];
  TGraph* rcxt_g_u[16];
  TGraph* rcxt_g_d[16];
  TGraph* rcxt_g[17];
  TGraph* erx_g[17];
  TMultiGraph *mg_x = new TMultiGraph("ERx","ERx");
  TGraph* errex_g[17];
  TMultiGraph *mge_x = new TMultiGraph("ERREx","ERREx");

  TGraphErrors* rcy_g[21];
  TGraph* rcyt_g_u[20];
  TGraph* rcyt_g_d[20];
  TGraph* rcyt_g[21];
  TGraph* ery_g[21];
  TMultiGraph *mg_y = new TMultiGraph("ERy","ERy");
  TGraph* errey_g[21];
  TMultiGraph *mge_y = new TMultiGraph("ERREy","ERREy");

  for(int i=0;i<16;i++)
  {
  	for(int j=0;j<20;j++)
   	{
  		re[i][j] = 0; born[i][j]=0;
  	}
  }

  if(fileFlag == "-f")
  {
    ifstream revt(FileRC);

    revt >> evtotre;
    revt >> sigtotre;

    while(revt >> dummy)
    {
      revt >> x >> y >> Q2 >> xhad >> yhad >> Q2had;
      // cout << x << " " << y << " " << Q2 << " " << xhad << " " << yhad << " " << Q2had << endl;

      for(int j=0; j<3; j++)
      {
        revt >> id;
        for(int i=0; i<5; i++)
        {
          revt >> dummy;
        }
      }

      int xi = 0;
      int yi = 0;
      int xflag = 0;
      int yflag = 0;

      for(int i=1; i<21; i++)
      {
        if(xtab[i-1] < x && x < xtab[i])
        {
          xi = i-1;
          xflag = 1;
          break;
        }
      }
      for(int i=1; i<17; i++)
      {
        if(ytab[i-1] < y && y < ytab[i])
        {
          yi = i-1;
          yflag = 1;
          break;
        }
      }

      if(xflag && yflag)
      {
        // cout << "x = " << x << ", y = " << y << endl;
        re[yi][xi]++;
      }
      else
      {
        // cout << "x = " << x << ", y = " << y << endl;
      }
    }

    revt.close();

    ifstream bevt(FileBorn);

    bevt >> evtotborn;
    bevt >> sigtotborn;

    while(bevt >> dummy)
    {
      bevt >> x >> y >> Q2 >> xhad >> yhad >> Q2had;

      for(int j=0; j<3; j++)
      {
        bevt >> id;
        for(int i=0; i<5; i++)
        {
          bevt >> dummy;
        }
      }

      int xi = 0;
      int yi = 0;
      int xflag = 0;
      int yflag = 0;

      for(int i=1; i<21; i++)
      {
        if(xtab[i-1] < x && x < xtab[i])
        {
          xi = i-1;
          xflag = 1;
          break;
        }
      }
      for(int i=1; i<17; i++)
      {
        if(ytab[i-1] < y && y < ytab[i])
        {
          yi = i-1;
          yflag = 1;
          break;
        }
      }

      if(xflag && yflag)
      {
        // cout << "x = " << x << ", y = " << y << endl;
        born[yi][xi]++;
      }
      else
      {
        // cout << "x = " << x << ", y = " << y << endl;
      }
    }
    bevt.close();
  }
  else if(fileFlag == "-l")
  {
    for(int i=0; i<20; i++)
    {
      printProgress (2*i+1,40);

      ifstream revt(Form("data/RC_%s_xbin_%d_%d.dat",TempFile.c_str(),i,i+1));

      revt >> evtotre_t[i];
      revt >> sigtotre_t[i];

      while(revt >> dummy)
      {
        revt >> x >> y >> Q2 >> xhad >> yhad >> Q2had;
        // cout << x << " " << y << " " << Q2 << " " << xhad << " " << yhad << " " << Q2had << endl;

        for(int j=0; j<3; j++)
        {
          revt >> id;
          for(int i=0; i<5; i++)
          {
            revt >> dummy;
          }
        }

        int xi = 0;
        int yi = 0;
        int xflag = 0;
        int yflag = 0;

        for(int i=1; i<21; i++)
        {
          if(xtab[i-1] < x && x < xtab[i])
          {
            xi = i-1;
            xflag = 1;
            break;
          }
        }
        for(int i=1; i<17; i++)
        {
          if(ytab[i-1] < y && y < ytab[i])
          {
            yi = i-1;
            yflag = 1;
            break;
          }
        }

        if(xflag && yflag)
        {
          // cout << "x = " << x << ", y = " << y << endl;
          re[yi][xi]++;
        }
        else
        {
          // cout << "x = " << x << ", y = " << y << endl;
        }
      }

      revt.close();

      printProgress (2*i+2,40);

      ifstream bevt(Form("data/Born_%s_xbin_%d_%d.dat",TempFile.c_str(),i,i+1));

      bevt >> evtotborn_t[i];
      bevt >> sigtotborn_t[i];

      while(bevt >> dummy)
      {
        bevt >> x >> y >> Q2 >> xhad >> yhad >> Q2had;

        for(int j=0; j<3; j++)
        {
          bevt >> id;
          for(int i=0; i<5; i++)
          {
            bevt >> dummy;
          }
        }

        int xi = 0;
        int yi = 0;
        int xflag = 0;
        int yflag = 0;

        for(int i=1; i<21; i++)
        {
          if(xtab[i-1] < x && x < xtab[i])
          {
            xi = i-1;
            xflag = 1;
            break;
          }
        }
        for(int i=1; i<17; i++)
        {
          if(ytab[i-1] < y && y < ytab[i])
          {
            yi = i-1;
            yflag = 1;
            break;
          }
        }

        if(xflag && yflag)
        {
          // cout << "x = " << x << ", y = " << y << endl;
          born[yi][xi]++;
        }
        else
        {
          // cout << "x = " << x << ", y = " << y << endl;
        }
      }
      bevt.close();
    }
  }
  else if(fileFlag == "-sigf")
  {
    ifstream revt(FileRC);

    for(int j=0; j<21; j++)
    {
      for(int i=0; i<17; i++)
      {
        revt >> sigreb[i][j];
        revt >> erre[i][j]; //here we stock the error
      	cout << "(" << j << "," << i << ") : " << sigreb[i][j] << "\t" << erre[i][j] << endl;
      }
    }

    revt.close();

    ifstream bevt(FileBorn);

    for(int j=0; j<21; j++)
    {
      for(int i=0; i<17; i++)
      {
        bevt >> sigbornb[i][j];
        bevt >> erborn[i][j];
	      // cout << "(" << j << "," << i << ") : " << sigborn[i][j] << "\t" << erborn[i][j] << endl;
      }
    }

    bevt.close();
  }
  else if(fileFlag == "-bornratio")
  {
    ifstream bevt(FileBorn);

    for(int j=0; j<21; j++)
    {
      for(int i=0; i<17; i++)
      {
        bevt >> sigbornb[i][j];
        bevt >> erborn[i][j];
	      // cout << "(" << j << "," << i << ") : " << sigborn[i][j] << "\t" << erborn[i][j] << endl;
      }
    }

    bevt.close();
  }

  if(fileFlag == "-sirc")
  {
    ifstream table("data/hh160_r1998_f2tulay_compass_grv_had.asy_hcorr.txt");
    for(int i=0; i<19; i++)
    {
      for(int j=0; j<5; j++)
      {
        table >> sdum;
         cout << sdum << "\t";

        for(int k=0; k<6; k++)
        {
          table >> rc_table[i][k+j*6] >> sdum;
          cout << " " << rc_table[i][k+j*6] << sdum;
          rc_table[i][k+j*6] = 1/rc_table[i][k+j*6];
          rc_table_y[k+j*6][i]=rc_table[i][k+j*6];
        }

        cout << endl;
      }
    }
    table.close();

    for(int i=0; i<18; i++)
    {
      for(int j=0; j<29; j++)
      {
        rc_tablered[i][j] = (rc_table[i][j]+rc_table[i+1][j])/2;
        rc_tablered_y[j][i] = (rc_table_y[j][i]+rc_table_y[j+1][i])/2;
      }
    }

    ifstream sircf(TempFile);
    for(int i=0; i<9; i++)
    {
      for(int j=0; j<5; j++)
      {
        ztotsize = 0;
        for(int k=0; k<14; k++)
        {
          for(int l=0; l<7; l++) sircf >> dummy;
          sircf >> sircz >> dummy;
          cout << sircz << " ";
          sircy[i][j] += sircz*z_size[k];
          if(sircz) ztotsize += z_size[k];
        }
        cout << endl;
        if(ztotsize) sircy[i][j] /= ztotsize;
        sirc[j][i] = sircy[i][j];
      }
    }
    sircf.close();

    TCanvas c11("SI_RC_xy_f(x)","SI_RC_xy_f(x)",3200,1600);
    TCanvas c12("SI_RC_xy_f(y)","SI_RC_xy_f(y)",3200,1600);
    c11.Divide(3,2);
    c12.Divide(3,3);

    TGraph* six_g[5];
    TGraph* siy_g[9];
    TGraph* sixt_g[5];
    TGraph* siyt_g[9];

    for(int i=0; i<5; i++)
    {
      six_g[i] = new TGraph(9,xmidred,sirc[i]);
      six_g[i]->SetMarkerStyle(22);
      six_g[i]->SetMarkerColor(601);
      six_g[i]->SetMarkerSize(3);
      six_g[i]->SetFillColor(601);
      six_g[i]->SetFillStyle(3001);
      six_g[i]->SetMinimum(.95);
      six_g[i]->SetMaximum(1.15);
      six_g[i]->SetTitle(Form("RC @ %f < y < %f",ytabred[i],ytabred[i+1]));
      six_g[i]->GetXaxis()->SetTitle("x_{Bj}");
      six_g[i]->GetYaxis()->SetTitle("#eta(x_{Bj})");
      c11.cd(i+1);
      six_g[i]->Draw("SAMEAP");
      c11.Update();
      sixt_g[i] = new TGraph(30,xtable,rc_tablered[ysw[i]]);
      sixt_g[i]->SetLineColor(3);
      sixt_g[i]->SetFillColor(601);
      sixt_g[i]->SetFillStyle(3001);
      c11.cd(i+1);
      sixt_g[i]->Draw("SAMEL");
      c11.Update();
    }

    for(int i=0; i<9; i++)
    {
      siy_g[i] = new TGraph(5,ymidred,sircy[i]);
      siy_g[i]->SetMarkerStyle(22);
      siy_g[i]->SetMarkerColor(601);
      siy_g[i]->SetMarkerSize(3);
      siy_g[i]->SetFillColor(601);
      siy_g[i]->SetFillStyle(3001);
      siy_g[i]->SetMinimum(.95);
      siy_g[i]->SetMaximum(1.15);
      siy_g[i]->GetXaxis()->SetTitle("y");
      siy_g[i]->SetTitle(Form("RC @ %f < x < %f",xtabred[i],xtabred[i+1]));
      c12.cd(i+1);
      siy_g[i]->Draw("SAMEAP");
      c12.Update();
      siyt_g[i] = new TGraph(19,ytable,rc_tablered_y[xsw[i]]);
      siyt_g[i]->SetLineColor(3);
      siyt_g[i]->SetFillColor(601);
      siyt_g[i]->SetFillStyle(3001);
      c12.cd(i+1);
      siyt_g[i]->Draw("SAMEL");
      c12.Update();
    }

    c11.Print(SIx_pdf);
    c12.Print(SIy_pdf);

    return 0;
  }
  if(fileFlag == "-bornratio")
  {
    ifstream table("data/hh160_r1998_f2tulay_compass_grv.new_table.txt");
    for(int i=0; i<30; i++)
    {
      for(int j=0; j<19; j++)
      {
        for(int k=0; k<13; k++)
        {
          table >> sdum;
          // cout << sdum << "\t";
        }
        table >> born_ratio[j][i];
      }
    }
    table.close();

    TCanvas c7("Born_Ratio_xy_f(x)","Born_Ratio_xy_f(x)",3200,1600);
    TCanvas c8("Born_Ratio_xy_f(y)","Born_Ratio_xy_f(y)",3200,1600);
    TCanvas c9("Born_Ratio_xy_f(y) scat","Born_Ratio_xy_f(y) scat",3200,1600);
    c7.Divide(1,1);
    c8.Divide(1,1);
    c9.Divide(3,3);

    TGraph* brx_g[17];
    TMultiGraph *bg_x = new TMultiGraph("BRx","BRx");
    TGraph* bry_g[21];
    TMultiGraph *bg_y = new TMultiGraph("BRy","BRy");
    double norm=sigbornb[0][0]/(born_ratio[1][9]);

    for(int j=0; j<17; j++)
    {
      for(int i=0; i<21; i++)
      {
        br[j][i] = sigbornb[j][i]/(born_ratio[j+1][i+9]);
	      br[j][i] /= norm;
        // cout << "j=" << j << ", i=" << i << "  br[j][i] : " << br[j][i] << "  sigbornb[j][i] : " << sigbornb[j][i] << "  born_ratio[j+1][i+9] : " << born_ratio[j+1][i+9] << endl;
      }

      brx_g[j] = new TGraph(21,xtab,br[j]);
      brx_g[j]->SetMarkerStyle(22);
      brx_g[j]->SetMarkerColor(601);
      brx_g[j]->SetMarkerSize(3);
      brx_g[j]->SetFillColor(601);
      brx_g[j]->SetFillStyle(3001);
      // brx_g[j]->GetXaxis()->SetTitle("x_{Bj}");
      // brx_g[j]->GetYaxis()->SetTitle("BR[BORN_DJANGOH/BORN_TERAD]");
      bg_x->Add(brx_g[j],"AP");
    }

    c7.cd(1);
    bg_x->SetTitle("BR[BORN_DJANGOH/BORN_TERAD]");
    bg_x->Draw("AP");
    c7.Update();

    for(int i=0; i<21; i++)
    {
      for(int j=0; j<17; j++)
      {
        br_y[i][j] = br[j][i];
      }

      bry_g[i] = new TGraph(17,ytab,br_y[i]);
      bry_g[i]->SetMarkerStyle(22);
      bry_g[i]->SetMarkerColor(601);
      bry_g[i]->SetMarkerSize(3);
      bry_g[i]->SetFillColor(601);
      bry_g[i]->SetFillStyle(3001);
      // bry_g[i]->GetXaxis()->SetTitle("y");
      // bry_g[i]->GetYaxis()->SetTitle("BR[BORN_DJANGOH/BORN_TERAD]");
      bg_y->Add(bry_g[i],"AP");

      if(i%2==0 && i<17)
      {
        c9.cd(i/2+1);
        bry_g[i]->SetTitle(Form("BR[BORN_DJANGOH/BORN_TERAD] @ x = %g",xtab[i]));
	bry_g[i]->GetXaxis()->SetTitle("y");
        bry_g[i]->GetYaxis()->SetTitle("Born_{DJANGOH}/Born_{TERAD}]");
        bry_g[i]->GetYaxis()->SetRangeUser(0.9998,1.0002);
        bry_g[i]->GetXaxis()->SetTitleSize(.05);
        bry_g[i]->GetYaxis()->SetTitleSize(.05);
        bry_g[i]->Draw("AP");
        c9.Update();
      }

    }

    c8.cd(1);
    bg_y->SetTitle(Form("BR[BORN_DJANGOH/BORN_TERAD] @ x = %g",xtab[0]));

    bg_y->Draw("AP");
    bg_y->GetXaxis()->SetTitle("y");
    bg_y->GetYaxis()->SetTitle("(Born_{DJANGOH}/Born_{TERAD})");
    bg_y->GetYaxis()->SetTitleOffset(1.4);
    c8.Update();

    c7.Print(BRx_pdf);
    c8.Print(BRy_pdf);
    c9.Print(BRy_scat_pdf);

    return 0;
  }
  else if(qel==1)
  {
    ifstream table("data/hh160_r1998_f2tulay_compass_grv.asy_hcorr.txt");
    for(int i=0; i<19; i++)
    {
      for(int j=0; j<5; j++)
      {
        table >> sdum;
        // cout << sdum << "\t";

        for(int k=0; k<6; k++)
        {
          table >> rc_table[i][k+j*6] >> sdum;
          // cout << " " << rc_table[i][k+j*6] << sdum;
          rc_table[i][k+j*6] = 1/rc_table[i][k+j*6];
          rc_table_y[k+j*6][i]=rc_table[i][k+j*6];
        }

        // cout << endl;
      }
    }
    table.close();
  }
  else if(qel==0)
  {
    ifstream table("data/hh160_r1998_f2tulay_compass_grv_had.asy_hcorr.txt");
    for(int i=0; i<19; i++)
    {
      for(int j=0; j<5; j++)
      {
        table >> sdum;
        // cout << sdum << "\t";

        for(int k=0; k<6; k++)
        {
          table >> rc_table[i][k+j*6] >> sdum;
          rc_table[i][k+j*6] = 1/rc_table[i][k+j*6];
          cout << "(" << xtable[k+j*6] << "," << ytable[i] << ") : " << rc_table[i][k+j*6] << endl;
          rc_table_y[k+j*6][i]=rc_table[i][k+j*6];
        }

        // cout << endl;

      }
    }
    table.close();

    TCanvas c7("TERAD_eta_f(y)","TERAD_eta_f(y)",3200,1600);
    c7.Divide(3,3);

    TGraph* ter_g[21];

     for(int i=0; i<17; i=i+2)
     {
       ter_g[i] = new TGraph(19,ytable,rc_table_y[9+i]);
       ter_g[i]->SetMarkerColor(3);
       ter_g[i]->SetMarkerSize(3);
       ter_g[i]->SetMarkerStyle(20);
       ter_g[i]->GetXaxis()->SetTitle("y");
       ter_g[i]->GetYaxis()->SetTitle("#eta");
       ter_g[i]->GetXaxis()->SetTitleSize(.05);
       ter_g[i]->GetYaxis()->SetTitleSize(.05);
       ter_g[i]->GetYaxis()->SetRangeUser(0.8,1.4);
       ter_g[i]->SetTitle(Form("TERAD #eta(x,y) @ x = %g",xtab[i]));
       c7.cd(i/2+1);
       ter_g[i]->Draw("AP");
       c7.Update();
     }

    c7.Print(TERy_pdf);

  }

  TLine l(0,1,0.94,1);
  TLine l2(0.08,1,0.95,1);

  ofstream outfile(OUTFILE);

  for(int i=0;i<YRANGE;i++)
  {
    for(int j=0;j<XRANGE;j++)
    {
      if((born[i][j] && re[i][j]) || fileFlag == "-sigf")
      {
        if(fileFlag == "-f" || fileFlag == "-l")
        {
          if(fileFlag == "-f")
          {
    	      sigre[i][j] = double(re[i][j])*double(sigtotre)/double(evtotre);
            sigborn[i][j] = double(born[i][j])*double(sigtotborn)/double(evtotborn);
          }
          else if(fileFlag == "-l")
          {
            sigre[i][j] = double(re[i][j])*double(sigtotre_t[j])/double(evtotre_t[j]);
            sigborn[i][j] = double(born[i][j])*double(sigtotborn_t[j])/double(evtotborn_t[j]);
          }
          rcx[i][j] = double(sigborn[i][j])/double(sigre[i][j]);
          rcx_e[i][j] = double(1/sqrt(born[i][j]))+double(1/sqrt(re[i][j]));
          erx[i][j] = (rcx[i][j]-(rc_table[1+i][9+j]
                                  +rc_table[1+i+1][9+j+1]
                                  +rc_table[1+i][9+j+1]
                                  +rc_table[1+i+1][9+j])/4);
          errex[i][j] = abs(rcx[i][j]-(rc_table[1+i][9+j]
                                       +rc_table[1+i+1][9+j+1]
                                       +rc_table[1+i][9+j+1]
                                       +rc_table[1+i+1][9+j])/4)/abs(1-rcx[i][j]);
          // cout << "rcx_e["<<i<<"]["<<j<<"] = " << rcx_e[i][j] << endl;
          // cout << "sigtotre : " << sigtotre
          // << "evtotre : " << evtotre
          // << "sigtotborn : " << sigtotborn
          // << "evtotborn : " << evtotborn << endl;
        }
        else if(fileFlag == "-sigf")
        {
          rcxb[i][j] = double(sigbornb[i][j])/double(sigreb[i][j]);
          //rcxb[i][j] = 1/rcxb[i][j];
          rcx_eb[i][j] = double(erborn[i][j])+double(erre[i][j]);
          erxb[i][j] = rcxb[i][j]-rc_table[1+i][9+j];
          errexb[i][j] = (rc_table[1+i][9+j]-rcxb[i][j])/rcxb[i][j];
          // cout << "rcxb["<<i<<"]["<<j<<"] = " << rcxb[i][j] << endl;
          // cout << "rcx_eb["<<i<<"]["<<j<<"] = " << rcx_eb[i][j] << endl;
          // cout << "sigreb["<<i<<"]["<<j<<"] : " << sigreb[i][j] << " "
          // << "sigbornb["<<i<<"]["<<j<<"] : " << sigbornb[i][j] << endl;
        }
      }
      else
      {
	      rcx[i][j] = 0;
        rcx_e[i][j] = 0;
        erx[i][j] = 0;
        errex[i][j] = 0;
        rcxb[i][j] = 0;
        rcx_eb[i][j] = 0;
        erxb[i][j] = 0;
        errexb[i][j] = 0;
      }
    }


    if(fileFlag == "-f" || fileFlag == "-l")
    {
      rcx_g[i] = new TGraphErrors(20,xmid,rcx[i],0,rcx_e[i]);
      rcxt_g_u[i] = new TGraph(30,xtable,rc_table[1+i+1]);
      rcxt_g_d[i] = new TGraph(30,xtable,rc_table[1+i]);
      erx_g[i] = new TGraph(20,xmid,erx[i]);
      errex_g[i] = new TGraph(20,xmid,errex[i]);
      rcx_g[i]->SetTitle(Form("RC @ %f < y < %f",ytab[i],ytab[i+1]));
    }
    else if(fileFlag == "-sigf")
    {
      rcx_g[i] = new TGraphErrors(21,xtab,rcxb[i],0,rcx_eb[i]);
      rcxt_g[i] = new TGraph(30,xtable,rc_table[1+i]);
      erx_g[i] = new TGraph(21,xtab,erxb[i]);
      errex_g[i] = new TGraph(21,xtab,errexb[i]);
      rcx_g[i]->SetTitle(Form("RC @ y = %f",ytab[i]));
    }


    c1.cd(i+1);
    rcx_g[i]->GetXaxis()->SetTitle("x_{Bj}");
    rcx_g[i]->GetYaxis()->SetTitle("RC");
    rcx_g[i]->GetYaxis()->SetTitleOffset(1.5);
    rcx_g[i]->SetMarkerStyle(22);
    rcx_g[i]->SetMarkerColor(601);
    rcx_g[i]->SetMarkerSize(3);
    rcx_g[i]->GetYaxis()->SetRangeUser(0.6,1.2);
    rcx_g[i]->SetFillColor(601);
    rcx_g[i]->SetFillStyle(3001);
    rcx_g[i]->Draw("A3");
    rcx_g[i]->Draw("P");
    if(fileFlag == "-f" || fileFlag == "-l")
    {
      rcxt_g_u[i]->SetLineColor(3);
      rcxt_g_d[i]->SetLineColor(2);
      rcxt_g_u[i]->Draw("SAME");
      rcxt_g_d[i]->Draw("SAME");
    }
    else if(fileFlag == "-sigf")
    {
      rcxt_g[i]->SetLineColor(3);
      rcxt_g[i]->Draw("SAME");
    }
    l.Draw("SAME");
    c1.Update();

    erx_g[i]->SetMarkerStyle(22);
    erx_g[i]->SetMarkerColor(601);
    erx_g[i]->SetMarkerSize(3);
    erx_g[i]->SetFillColor(601);
    erx_g[i]->SetFillStyle(3001);
    //erx_g[i]->GetYaxis()->SetRangeUser(-.05,.05);
    erx_g[i]->GetXaxis()->SetTitle("x_{Bj}");
    erx_g[i]->GetYaxis()->SetTitle("ER[|DJANGOH-TERAD|/DJANGOH]");
    mg_x->Add(erx_g[i],"AP");
    errex_g[i]->SetMarkerStyle(22);
    errex_g[i]->SetMarkerColor(601);
    errex_g[i]->SetMarkerSize(3);
    errex_g[i]->SetFillColor(601);
    errex_g[i]->SetFillStyle(3001);
    //errex_g[i]->GetYaxis()->SetRangeUser(-.05,.05);
    errex_g[i]->GetXaxis()->SetTitle("x_{Bj}");
    errex_g[i]->GetYaxis()->SetTitle("ER[|DJANGOH-TERAD|/DJANGOH]");
    mge_x->Add(errex_g[i],"AP");
  }

  c3.cd(1);
  // mg_x->SetMinimum(-.05);
  // mg_x->SetMaximum(.05);
  mg_x->SetTitle("ER[DJANGOH-TERAD]");
  mg_x->Draw("AP");
  mg_x->GetXaxis()->SetTitle("x_{Bj}");
  mg_x->GetYaxis()->SetTitle("RC_{DJANGOH}-RC_{TERAD}");
  c3.Update();

  c5.cd(1);
  // mg_x->SetMinimum(-.05);
  // mg_x->SetMaximum(.05);
  mge_x->SetTitle("ER[|DJANGOH-TERAD|/|1-DJANGOH|]");
  mge_x->Draw("AP");
  mge_x->GetXaxis()->SetTitle("x_{Bj}");
  mge_x->GetYaxis()->SetTitle("|RC_{DJANGOH}-RC_{TERAD}|/|1-RC_{DJANGOH}|");
  c5.Update();

  for(int i=0;i<18;i=i+2)
  {
    for(int j=0;j<YRANGE;j++)
    {
      if((born[j][i] && re[j][i]) || fileFlag == "-sigf")
      {
        if(fileFlag == "-f" || fileFlag == "-l")
        {
          rcy[i][j] = double(sigborn[j][i])/double(sigre[j][i]);
          rcy_e[i][j] = double(1/sqrt(born[j][i]))+double(1/sqrt(re[j][i]));
          ery[i][j] = erx[j][i];
          errey[i][j] = errex[j][i];
        }
        else if(fileFlag == "-sigf")
        {
          rcyb[i][j] = double(sigbornb[j][i])/double(sigreb[j][i]);
          //rcyb[i][j] = 1/rcyb[i][j];
          rcy_eb[i][j] = double(erborn[i][j])+double(erre[i][j]);
          eryb[i][j] = erxb[j][i];
          erreyb[i][j] = errexb[j][i];
        }
        // cout << "rcy_e["<<i<<"]["<<j<<"] = " << rcy_e[i][j] << endl;
      }
      else
      {
	      rcy[i][j] = 0;
        rcy_e[i][j] = 0;
        ery[i][j] = 0;
        errey[i][j] = 0;
        rcyb[i][j] = 0;
        rcy_eb[i][j] = 0;
        eryb[i][j] = 0;
        erreyb[i][j] = 0;
      }
    }

    if(fileFlag == "-f" || fileFlag == "-l")
    {
      rcy_g[i] = new TGraphErrors(YRANGE,ymid,rcy[i],0,rcy_e[i]);
      rcyt_g_u[i] = new TGraph(19,ytable,rc_table_y[9+i+1]);
      rcyt_g_d[i] = new TGraph(19,ytable,rc_table_y[9+i]);
      ery_g[i] = new TGraph(YRANGE,ymid,ery[i]);
      errey_g[i] = new TGraph(YRANGE,ymid,errey[i]);
      rcy_g[i]->SetTitle(Form("RC @ %f < x < %f",xtab[i],xtab[i+1]));
    }
    else if(fileFlag == "-sigf")
    {
      rcy_g[i] = new TGraphErrors(YRANGE,ytab,rcyb[i],0,rcy_eb[i]);
      rcyt_g[i] = new TGraph(19,ytable,rc_table_y[9+i]);
      ery_g[i] = new TGraph(YRANGE,ytab,eryb[i]);
      errey_g[i] = new TGraph(YRANGE,ytab,erreyb[i]);
      rcy_g[i]->SetTitle(Form("#eta(x,y) @ x = %g",xtab[i]));
    }



    c2.cd(i/2+1);
    rcy_g[i]->GetXaxis()->SetTitle("y");
    rcy_g[i]->GetYaxis()->SetTitle("#eta");
    rcy_g[i]->SetMarkerStyle(22);
    rcy_g[i]->SetMarkerColor(601);
    rcy_g[i]->SetMarkerSize(3);
    rcy_g[i]->GetYaxis()->SetRangeUser(0.6,1.2);
    rcy_g[i]->GetXaxis()->SetTitleSize(.05);
    rcy_g[i]->GetYaxis()->SetTitleSize(.05);
    rcy_g[i]->SetFillColor(601);
    rcy_g[i]->SetFillStyle(3001);
    rcy_g[i]->Draw("A3");
    rcy_g[i]->Draw("P");
    if(fileFlag == "-f" || fileFlag == "-l")
    {
      rcyt_g_u[i]->SetLineColor(3);
      rcyt_g_d[i]->SetLineColor(2);
      rcyt_g_u[i]->Draw("SAME");
      rcyt_g_d[i]->Draw("SAME");
    }
    else if(fileFlag == "-sigf")
    {
      rcyt_g[i]->SetMarkerStyle(20);
      rcyt_g[i]->SetMarkerColor(3);
      rcyt_g[i]->SetMarkerSize(3);
      rcyt_g[i]->Draw("P");
    }
    //l2.Draw("SAME");
    c2.Update();

    ery_g[i]->SetMarkerStyle(22);
    ery_g[i]->SetMarkerColor(601);
    ery_g[i]->SetMarkerSize(3);
    ery_g[i]->SetFillColor(601);
    ery_g[i]->SetFillStyle(3001);
    //ery_g[i]->GetYaxis()->SetRangeUser(-.05,.05);
    ery_g[i]->GetXaxis()->SetTitle("y");
    ery_g[i]->GetYaxis()->SetTitle("ER[|DJANGOH-TERAD|/DJANGOH]");
    mg_y->Add(ery_g[i],"AP");
    if(i<YRANGE)
    {
      errey_g[i]->SetMarkerStyle(22);
      errey_g[i]->SetMarkerColor(601);
    }
    else
    {
      errey_g[i]->SetMarkerStyle(23);
      errey_g[i]->SetMarkerColor(600+i);
    }
    errey_g[i]->SetMarkerSize(3);
    errey_g[i]->SetFillColor(601);
    errey_g[i]->SetFillStyle(3001);
    //errey_g[i]->GetYaxis()->SetRangeUser(-.05,.05);
    errey_g[i]->GetXaxis()->SetTitle("y");
    errey_g[i]->GetYaxis()->SetTitle("ER[|DJANGOH-TERAD|/DJANGOH]");
    mge_y->Add(errey_g[i],"AP");

    if(i%2==0 && i<17)
    {
       	c10.cd(i/2+1);
        errey_g[i]->SetTitle(Form("ER[(#eta_TERAD-#eta_DJANGOH)/#eta_DJANGOH] @ x = %g",xtab[i]));
        errey_g[i]->GetXaxis()->SetTitle("y");
        errey_g[i]->GetYaxis()->SetTitle("(#eta_{TERAD}-#eta_{DJANGOH})/#eta_{DJANGOH}");
        errey_g[i]->GetYaxis()->SetRangeUser(-0.04,0.04);
        errey_g[i]->GetXaxis()->SetTitleSize(.05);
        errey_g[i]->GetYaxis()->SetTitleSize(.05);
        errey_g[i]->Draw("AP");
        c10.Update();
    }


  }

  c4.cd(1);
  // mg_y->SetMinimum(-.05);
  // mg_y->SetMaximum(.05);
  mg_y->SetTitle("ER[DJANGOH-TERAD]");
  mg_y->Draw("AP");
  mg_y->GetXaxis()->SetTitle("y");
  mg_y->GetYaxis()->SetTitle("RC_{DJANGOH}-RC_{TERAD}");
  c4.Update();

  c6.cd(1);
  // mg_y->SetMinimum(-.05);
  // mg_y->SetMaximum(.05);
  mge_y->SetTitle("ER[|DJANGOH-TERAD|/|1-DJANGOH|]");
  mge_y->Draw("AP");
  mge_y->GetXaxis()->SetTitle("y");
  mge_y->GetYaxis()->SetTitle("|RC_{DJANGOH}-RC_{TERAD}|/|1-RC_{DJANGOH}|");
  c6.Update();


  cout << "x/y";

  for(int i=0;i<YRANGE;i++)
    cout << "\t" << ytab[i];

  cout << endl;

  for(int j=0;j<XRANGE;j++)
  {
    cout << xtab[j];
    for(int i=0;i<YRANGE;i++)
    {
      if(fileFlag == "-f" || fileFlag == "-l") cout << "\t" << born[i][j];
      else if(fileFlag == "-sigf") cout << "\t" << sigbornb[i][j];
    }
    cout << endl;
  }

  cout << "x/y";

  for(int i=0;i<YRANGE;i++)
    cout << "\t" << ytab[i];

  cout << endl;

  for(int j=0;j<XRANGE;j++)
  {
    cout << xtab[j];
    for(int i=0;i<YRANGE;i++)
    {
      if(fileFlag == "-f" || fileFlag == "-l") cout << "\t" << re[i][j];
      else if(fileFlag == "-sigf") cout << "\t" << sigreb[i][j];
    }
    cout << endl;
  }

  outfile << setprecision(5);

  outfile << "x/y";

  for(int i=0;i<YRANGE;i++)
    outfile << "\t" << ytab[i];

  outfile << endl;

  for(int j=0;j<XRANGE;j++)
  {
    outfile << xtab[j];
    for(int i=0;i<YRANGE;i++)
    {
      if(fileFlag == "-f" || fileFlag == "-l") outfile << "\t" << rcx[i][j];
      else if(fileFlag == "-sigf") outfile << "\t" << rcxb[i][j];
    }
    outfile << endl;
  }

  c1.Print(RCx_pdf);
  c2.Print(RCy_pdf);
  c3.Print(ERx_pdf);
  c4.Print(ERy_pdf);
  c5.Print(ERREx_pdf);
  c6.Print(ERREy_pdf);
  c10.Print(ERy_scat_pdf);

  outfile.close();

  return 0;
}
