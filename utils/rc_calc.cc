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
#include <TGraphErrors.h>
#include <TArrow.h>
#include <TAxis.h>
#include <TLine.h>
#include <vector>
#include <math.h>
#include <iomanip>

#define _USE_MATH_DEFINES

#define RCx_pdf "RCx.pdf"
#define RCy_pdf "RCy.pdf"
#define OUTFILE "rc.txt"

using namespace std;

int main(int argc,char *argv[])
{

  if(argc != 4)
  {
    cout << "ERROR : Wrong number of arguments" << endl;
    cout << "Expected 4, received " << argc << " !" << endl;
    cout << "USAGE : \n ./rc_calc all_process_file born_file rc_table_file" << endl;
    return 1;
  }

  double dummy, x, y, Q2, xhad, yhad, Q2had;
  string sdum;
  int id;

  double xtab[10] = {.004,.01,.02,.03,.04,.06,.1,.14,.18,.4};
  double xmid[9] = {.007,.015,.025,.035,.05,.08,.12,.16,.29};
  double ytab[6] = {.1,.15,.20,.30,.50,.70};
  double ymid[5] = {.125,.175,.25,.40,.60};
  double xtable[30] = {.000050,.000070,.000100,.000200,.000300,.000500,
                       .000700,.001000,.002000,.004000,.006000,.008000,
                       .010000,.013000,.016000,.020000,.030000,.040000,
                       .060000,.080000,.100000,.150000,.200000,.300000,
                       .400000,.500000,.600000,.700000,.800000,.900000};
  double ytable[19] = {.050000,.100000,.150000,.200000,.250000,.300000,
                       .350000,.400000,.450000,.500000,.550000,.600000,
                       .650000,.700000,.750000,.800000,.850000,.900000,
                       .950000};
  int y_xch[6] = {1,2,3,5,9,13};
  int x_ych[10] = {9,12,15,16,17,18,20,21,22,24};

  double rcx[5][9];
  double rcx_e[5][9];
  double rcy[9][5];
  double rcy_e[9][5];
  double rc_table[19][30];
  double rc_table_y[30][19];
  int re[5][9];
  int born[5][9];
  int evtotre;
  int evtotborn;
  double sigtotre;
  double sigtotborn;
  double sigre[5][9];
  double sigborn[5][9];

  TCanvas c1("RC_xy_f(x)","RC_xy_f(x)",3200,1600);
  c1.Divide(3,2);
  TCanvas c2("RC_xy_f(y)","RC_xy_f(y)",3200,1600);
  c2.Divide(3,3);

  TGraphErrors* rcx_g[5];
  TGraph* rcxt_g_u[5];
  TGraph* rcxt_g_d[5];

  TGraphErrors* rcy_g[5];
  TGraph* rcyt_g_u[5];
  TGraph* rcyt_g_d[5];

  for(int i=0;i<5;i++)
  {
	for(int j=0;j<9;j++)
 	{
		re[i][j] = 0; born[i][j]=0;
	}
   }

  ifstream revt(argv[1]);

  revt >> evtotre;
  revt >> sigtotre;

  while(revt >> dummy)
  {
    revt >> x >> y >> Q2 >> xhad >> yhad >> Q2had;

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

    for(int i=1; i<10; i++)
    {
      if(xtab[i-1] < xhad && xhad < xtab[i])
      {
        xi = i-1;
        xflag = 1;
        break;
      }
    }
    for(int i=1; i<6; i++)
    {
      if(ytab[i-1] < yhad && yhad < ytab[i])
      {
        yi = i-1;
        yflag = 1;
        break;
      }
    }

    if(xflag && yflag)
      re[yi][xi]++;
  }

  revt.close();

  ifstream bevt(argv[2]);

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

    for(int i=1; i<10; i++)
    {
      if(xtab[i-1] < x && x < xtab[i])
      {
        xi = i-1;
        xflag = 1;
        break;
      }
    }
    for(int i=1; i<6; i++)
    {
      if(ytab[i-1] < y && y < ytab[i])
      {
        yi = i-1;
        yflag = 1;
        break;
      }
    }

    if(xflag && yflag)
      born[yi][xi]++;
  }

  bevt.close();

  ifstream table(argv[3]);
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
        rc_table_y[k+j*6][i]=rc_table[i][k+j*6];
      }

      // cout << endl;
    }
  }
  table.close();

  TLine l(0,1,0.32,1);
  TLine l2(0.08,1,0.645,1);

  ofstream outfile(OUTFILE);

  for(int i=0;i<5;i++)
  {
    for(int j=0;j<9;j++)
    {
      if(born[i][j] && re[i][j])
      {
	      sigre[i][j] = double(re[i][j]*double(sigtotre))/double(evtotre);
        sigborn[i][j] = double(born[i][j]*double(sigtotborn))/double(evtotborn);
	      rcx[i][j] = double(sigre[i][j])/double(sigborn[i][j]);
        rcx_e[i][j] = double(1/sqrt(born[i][j]))+double(1/sqrt(re[i][j]));
        // cout << "rcx_e["<<i<<"]["<<j<<"] = " << rcx_e[i][j] << endl;
      }
      else
      {
	      rcx[i][j] = 0;
        rcx_e[i][j] = 0;
      }
    }
    rcx_g[i] = new TGraphErrors(9,xmid,rcx[i],0,rcx_e[i]);
    rcxt_g_u[i] = new TGraph(30,xtable,rc_table[y_xch[i+1]]);
    rcxt_g_d[i] = new TGraph(30,xtable,rc_table[y_xch[i]]);

    c1.cd(i+1);
    rcx_g[i]->GetXaxis()->SetTitle("x_{Bj}");
    rcx_g[i]->GetYaxis()->SetTitle("RC");
    rcx_g[i]->GetYaxis()->SetTitleOffset(1.5);
    rcx_g[i]->SetTitle(Form("RC @ %f < y < %f",ytab[i],ytab[i+1]));
    rcx_g[i]->SetMarkerStyle(22);
    rcx_g[i]->SetMarkerColor(601);
    rcx_g[i]->SetMarkerSize(3);
    rcx_g[i]->GetYaxis()->SetRangeUser(0.,1.3);
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
  }

  for(int i=0;i<9;i++)
  {
    for(int j=0;j<5;j++)
    {
      if(born[j][i] && re[j][i])
      {
	      sigre[j][i] = double(re[j][i]*double(sigtotre))/double(evtotre);
        sigborn[j][i] = double(born[j][i]*double(sigtotborn))/double(evtotborn);
	      rcy[i][j] = double(sigre[j][i])/double(sigborn[j][i]);
        rcy_e[i][j] = double(1/sqrt(born[j][i]))+double(1/sqrt(re[j][i]));
        // cout << "rcy_e["<<i<<"]["<<j<<"] = " << rcy_e[i][j] << endl;
      }
      else
      {
	      rcy[i][j] = 0;
        rcy_e[i][j] = 0;
      }
    }
    rcy_g[i] = new TGraphErrors(5,ymid,rcy[i],0,rcy_e[i]);
    rcyt_g_u[i] = new TGraph(19,ytable,rc_table_y[x_ych[i+1]]);
    rcyt_g_d[i] = new TGraph(19,ytable,rc_table_y[x_ych[i]]);

    c2.cd(i+1);
    rcy_g[i]->GetXaxis()->SetTitle("y");
    rcy_g[i]->GetYaxis()->SetTitle("RC");
    rcy_g[i]->GetYaxis()->SetTitleOffset(1.5);
    rcy_g[i]->SetTitle(Form("RC @ %f < x < %f",xtab[i],xtab[i+1]));
    rcy_g[i]->SetMarkerStyle(22);
    rcy_g[i]->SetMarkerColor(601);
    rcy_g[i]->SetMarkerSize(3);
    rcy_g[i]->GetYaxis()->SetRangeUser(0.,1.3);
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
  }


  cout << "x/y";

  for(int i=0;i<5;i++)
    cout << "\t" << ytab[i];

  cout << endl;

  for(int j=0;j<9;j++)
  {
    cout << xtab[j];
    for(int i=0;i<5;i++)
    {
      cout << "\t" << born[i][j];
    }
    cout << endl;
  }

  cout << "x/y";

  for(int i=0;i<5;i++)
    cout << "\t" << ytab[i];

  cout << endl;

  for(int j=0;j<9;j++)
  {
    cout << xtab[j];
    for(int i=0;i<5;i++)
    {
      cout << "\t" << re[i][j];
    }
    cout << endl;
  }

  outfile << setprecision(5);

  outfile << "x/y";

  for(int i=0;i<5;i++)
    outfile << "\t" << ytab[i];

  outfile << endl;

  for(int j=0;j<9;j++)
  {
    outfile << xtab[j];
    for(int i=0;i<5;i++)
    {
      outfile << "\t" << rcx[i][j];
    }
    outfile << endl;
  }

  c1.Print(RCx_pdf);
  c2.Print(RCy_pdf);

  outfile.close();

  return 0;
}
