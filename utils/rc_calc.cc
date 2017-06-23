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

  double dummy, x, y, Q2, xhad, yhad, Q2had;
  string sdum;
  double quasiel;
  int id;

  double xtab[21] = {.004,.006,.008,.01,.013,.016,.02,.03,.04,.06,
                     .08,.1,.15,.2,.3,.4,.5,.6,.7,.8,.9};
  double xmid[20] = {.005,.007,.009,.0115,.0145,.018,.025,.035,.05,
                    .07,.09,.125,.175,.25,.35,.45,.55,.65,.75,.85};
  double ytab[17] = {.1,.15,.2,.25,.3,.35,.4,.45,.5,.55,.6,
                    .65,.7,.75,.8,.85,.9};
  double ymid[16] = {.125,.175,.225,.275,.325,.375,.425,.475,
                     .525,.575,.625,.675,.725,.775,.825,.875};
  double xtable[30] = {.000050,.000070,.000100,.000200,.000300,.000500,
                       .000700,.001000,.002000,.004000,.006000,.008000,
                       .010000,.013000,.016000,.020000,.030000,.040000,
                       .060000,.080000,.100000,.150000,.200000,.300000,
                       .400000,.500000,.600000,.700000,.800000,.900000};
  double ytable[19] = {.050000,.100000,.150000,.200000,.250000,.300000,
                       .350000,.400000,.450000,.500000,.550000,.600000,
                       .650000,.700000,.750000,.800000,.850000,.900000,
                       .950000};

  double rcx[16][20];
  double erx[16][20];
  double rcx_e[16][20];
  double rcy[20][16];
  double ery[20][16];
  double rcy_e[20][16];
  double rc_table[19][30];
  double rc_table_y[30][19];
  int re[16][20];
  int born[16][20];
  double erre[16][20];
  double erborn[16][20];
  int evtotre;
  int evtotborn;
  double sigtotre;
  double sigtotborn;
  int evtotre_t[20];
  int evtotborn_t[20];
  double sigtotre_t[20];
  double sigtotborn_t[20];
  double sigre[16][20];
  double sigborn[16][20];

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
      if (string(argv[i]) == "-l" && fileFlag != "-f" && fileFlag != "-sigf")
      {
        TempFile = argv[i + 1];
        fileFlag = "-l";
      }
    }
    if(i+2 < argc)
    {
      if (string(argv[i]) == "-f" && fileFlag != "-l" && fileFlag != "-sigf")
      {
        FileRC = argv[i+1];
        FileBorn = argv[i+2];
        fileFlag = "-f";
      }
      if (string(argv[i]) == "-sigf" && fileFlag != "-l" && fileFlag != "-f")
      {
        FileRC = argv[i+1];
        FileBorn = argv[i+2];
        fileFlag = "-sigf";
      }
    }
  }

  if(fileFlag != "-l" && fileFlag != "-f" && fileFlag != "-sigf")
  {
    cout << BOLD(FRED("ERROR : expected some flags")) << endl;
    cout << BOLD(FRED("Expected -f or -l for input files !")) << endl;
    cout << BOLD(FRED("USAGE : \n ./rc_calc -f [RC_file] [Born_file] \nOR\n ./rc_calc -l [filename_template] \nOR\n ./rc_calc -sigf [RC_file] [Born_file]")) << endl;
    cout << BOLD(FRED("[-h for further help]")) << endl;
    return 1;
  }

  TCanvas c1("RC_xy_f(x)","RC_xy_f(x)",3200,1600);
  c1.Divide(4,4);
  TCanvas c2("RC_xy_f(y)","RC_xy_f(y)",3200,1600);
  c2.Divide(5,4);
  TCanvas c3("ER_xy_f(x)","ER_xy_f(x)",3200,1600);
  c3.Divide(1,1);
  TCanvas c4("ER_xy_f(y)","ER_xy_f(y)",3200,1600);
  c4.Divide(1,1);

  TGraphErrors* rcx_g[16];
  TGraph* rcxt_g_u[16];
  TGraph* rcxt_g_d[16];
  TGraph* erx_g[16];
  TMultiGraph *mg_x = new TMultiGraph();

  TGraphErrors* rcy_g[20];
  TGraph* rcyt_g_u[20];
  TGraph* rcyt_g_d[20];
  TGraph* ery_g[20];
  TMultiGraph *mg_y = new TMultiGraph();

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

    for(int j=0; j<20; j++)
    {
      for(int i=0; i<16; i++)
      {
        revt >> sigre[i][j];
        revt >> erre[i][j]; //here we stock the error
      	cout << "(" << j << "," << i << ") : " << sigre[i][j] << "\t" << erre[i][j] << endl;
      }
    }

    revt.close();

    ifstream bevt(FileBorn);

    for(int j=0; j<20; j++)
    {
      for(int i=0; i<16; i++)
      {
        bevt >> sigborn[i][j];
        bevt >> erborn[i][j];
	cout << "(" << j << "," << i << ") : " << sigborn[i][j] << "\t" << erborn[i][j] << endl;
      }
    }

    bevt.close();
  }

  if(qel==1)
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
          // rc_table[i][k+j*6] = 1/rc_table[i][k+j*6];
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
        cout << sdum << "\t";

        for(int k=0; k<6; k++)
        {
          table >> rc_table[i][k+j*6] >> sdum;
          cout << " " << rc_table[i][k+j*6] << sdum;
          // rc_table[i][k+j*6] = 1/rc_table[i][k+j*6];
          rc_table_y[k+j*6][i]=rc_table[i][k+j*6];
        }

        cout << endl;

      }
    }
    table.close();
  }
  // else if(qel==0)
  // {
  //   ifstream table("data/hh160_r1998_f2tulay_compass_grv.new_table.txt");
  //   for(int i=0; i<30; i++)
  //   {
  //     for(int j=0; j<19; j++)
  //     {
  //       for(int k=0; k<8; k++)
  //       {
  //         table >> sdum;
  //         // cout << sdum << "\t";
  //       }
  //       table >> quasiel >> sdum >> sdum >> sdum >> rc_table[j][i] >> sdum;
  //       /*cout << "\t" << quasiel
  //       << "\t" << sdum
  //       << "\t" << sdum
  //       << "\t" << sdum
  //       << "\t" << rc_table[j][i]
  //       << "\t" << sdum;*/
  //       rc_table[j][i] -= quasiel;
  //       rc_table[j][i] = 1+rc_table[j][i]/100;
  //       // rc_table[j][i] = 1/rc_table[j][i];
  //       rc_table_y[i][j]=rc_table[j][i];
  //
  //       //cout << endl;
  //     }
  //   }
  //   table.close();
  // }

  TLine l(0,1,0.94,1);
  TLine l2(0.08,1,0.95,1);

  ofstream outfile(OUTFILE);

  for(int i=0;i<16;i++)
  {
    for(int j=0;j<20;j++)
    {
      if((born[i][j] && re[i][j]) || fileFlag == "-sigf")
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
	      rcx[i][j] = double(sigre[i][j])/double(sigborn[i][j]);
        if(fileFlag == "-f" || fileFlag == "-l") rcx_e[i][j] = double(1/sqrt(born[i][j]))+double(1/sqrt(re[i][j]));
        else if(fileFlag == "-sigf") rcx_e[i][j] = double(erborn[i][j])+double(erre[i][j]);
        erx[i][j] = (rcx[i][j]-(rc_table[1+i][9+j]
                                  +rc_table[1+i+1][9+j+1]
                                  +rc_table[1+i][9+j+1]
                                  +rc_table[1+i+1][9+j])/4);
        // cout << "rcx_e["<<i<<"]["<<j<<"] = " << rcx_e[i][j] << endl;
        // cout << "sigtotre : " << sigtotre
        // << "evtotre : " << evtotre
        // << "sigtotborn : " << sigtotborn
        // << "evtotborn : " << evtotborn << endl;
      }
      else
      {
	      rcx[i][j] = 0;
        rcx_e[i][j] = 0;
        erx[i][j] = 0;
      }
    }

    rcx_g[i] = new TGraphErrors(20,xmid,rcx[i],0,rcx_e[i]);
    rcxt_g_u[i] = new TGraph(30,xtable,rc_table[1+i+1]);
    rcxt_g_d[i] = new TGraph(30,xtable,rc_table[1+i]);
    erx_g[i] = new TGraph(20,xmid,erx[i]);

    c1.cd(i+1);
    rcx_g[i]->GetXaxis()->SetTitle("x_{Bj}");
    rcx_g[i]->GetYaxis()->SetTitle("RC");
    rcx_g[i]->GetYaxis()->SetTitleOffset(1.5);
    rcx_g[i]->SetTitle(Form("RC @ %f < y < %f",ytab[i],ytab[i+1]));
    rcx_g[i]->SetMarkerStyle(22);
    rcx_g[i]->SetMarkerColor(601);
    rcx_g[i]->SetMarkerSize(3);
    rcx_g[i]->GetYaxis()->SetRangeUser(0.8,1.5);
    rcx_g[i]->SetFillColor(601);
    rcx_g[i]->SetFillStyle(3001);
    rcx_g[i]->Draw("A3");
    rcx_g[i]->Draw("P");
    rcxt_g_u[i]->SetLineColor(3);
    rcxt_g_d[i]->SetLineColor(2);
    rcxt_g_u[i]->Draw("SAME");
    rcxt_g_d[i]->Draw("SAME");
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
  }

  c3.cd(1);
  // mg_x->SetMinimum(-.05);
  // mg_x->SetMaximum(.05);
  mg_x->SetTitle("ER");
  mg_x->Draw("AP");
  c3.Update();

  for(int i=0;i<20;i++)
  {
    for(int j=0;j<16;j++)
    {
      if((born[j][i] && re[j][i]) || fileFlag == "-sigf")
      {
	      rcy[i][j] = double(sigre[j][i])/double(sigborn[j][i]);
        if(fileFlag == "-f" || fileFlag == "-l") rcy_e[i][j] = double(1/sqrt(born[j][i]))+double(1/sqrt(re[j][i]));
        else if(fileFlag == "-sigf") rcx_e[i][j] = double(erborn[i][j])+double(erre[i][j]);
        ery[i][j] = (rcy[i][j]-(rc_table_y[9+i][1+j]
                                  +rc_table_y[9+i+1][1+j+1]
                                  +rc_table_y[9+i][1+j+1]
                                  +rc_table_y[9+i+1][1+j])/4);
        // cout << "rcy_e["<<i<<"]["<<j<<"] = " << rcy_e[i][j] << endl;
      }
      else
      {
	      rcy[i][j] = 0;
        rcy_e[i][j] = 0;
        ery[i][j] = 0;
      }
    }
    rcy_g[i] = new TGraphErrors(16,ymid,rcy[i],0,rcy_e[i]);
    rcyt_g_u[i] = new TGraph(19,ytable,rc_table_y[9+i+1]);
    rcyt_g_d[i] = new TGraph(19,ytable,rc_table_y[9+i]);
    ery_g[i] = new TGraph(16,ymid,ery[i]);


    c2.cd(i+1);
    rcy_g[i]->GetXaxis()->SetTitle("y");
    rcy_g[i]->GetYaxis()->SetTitle("RC");
    rcy_g[i]->GetYaxis()->SetTitleOffset(1.5);
    rcy_g[i]->SetTitle(Form("RC @ %f < x < %f",xtab[i],xtab[i+1]));
    rcy_g[i]->SetMarkerStyle(22);
    rcy_g[i]->SetMarkerColor(601);
    rcy_g[i]->SetMarkerSize(3);
    rcy_g[i]->GetYaxis()->SetRangeUser(0.8,1.5);
    rcy_g[i]->SetFillColor(601);
    rcy_g[i]->SetFillStyle(3001);
    rcy_g[i]->Draw("A3");
    rcy_g[i]->Draw("P");
    rcyt_g_u[i]->SetLineColor(3);
    rcyt_g_d[i]->SetLineColor(2);
    rcyt_g_u[i]->Draw("SAME");
    rcyt_g_d[i]->Draw("SAME");
    l2.Draw("SAME");
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
  }

  c4.cd(1);
  // mg_y->SetMinimum(-.05);
  // mg_y->SetMaximum(.05);
  mg_y->SetTitle("ER");
  mg_y->Draw("AP");
  c4.Update();


  cout << "x/y";

  for(int i=0;i<16;i++)
    cout << "\t" << ytab[i];

  cout << endl;

  for(int j=0;j<20;j++)
  {
    cout << xtab[j];
    for(int i=0;i<16;i++)
    {
      cout << "\t" << born[i][j];
    }
    cout << endl;
  }

  cout << "x/y";

  for(int i=0;i<16;i++)
    cout << "\t" << ytab[i];

  cout << endl;

  for(int j=0;j<20;j++)
  {
    cout << xtab[j];
    for(int i=0;i<16;i++)
    {
      cout << "\t" << re[i][j];
    }
    cout << endl;
  }

  outfile << setprecision(5);

  outfile << "x/y";

  for(int i=0;i<16;i++)
    outfile << "\t" << ytab[i];

  outfile << endl;

  for(int j=0;j<20;j++)
  {
    outfile << xtab[j];
    for(int i=0;i<16;i++)
    {
      outfile << "\t" << rcx[i][j];
    }
    outfile << endl;
  }

  c1.Print(RCx_pdf);
  c2.Print(RCy_pdf);
  c3.Print(ERx_pdf);
  c4.Print(ERy_pdf);

  outfile.close();

  return 0;
}
