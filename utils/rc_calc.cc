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

#define _USE_MATH_DEFINES

#define RC_pdf "RC.pdf"
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

  double rc[5][9];
  double rc_e[5][9];
  double rc_table[19][30];
  int re[5][9];
  int born[5][9];

  TCanvas c1("RC_xy","RC_xy",3200,1600);
  c1.Divide(3,2);

  TGraphErrors* rc_g[5];
  TGraph* rct_g_u[5];
  TGraph* rct_g_d[5];

  for(int i=0;i<5;i++)
  {
	for(int j=0;j<9;j++)
 	{
		re[i][j] = 0; born[i][j]=0;
	}
   }

  ifstream revt(argv[1]);

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
      cout << sdum << "\t";

      for(int k=0; k<6; k++)
      {
        table >> rc_table[i][k+j*6] >> sdum;
        cout << " " << rc_table[i][k+j*6] << sdum;
      }

      cout << endl;
    }
  }
  table.close();

  TLine l(0,1,0.32,1);

  ofstream outfile(OUTFILE);

  for(int i=0;i<5;i++)
  {
    for(int j=0;j<9;j++)
    {
      if(born[i][j] && re[i][j])
      {
	      rc[i][j] = double(born[i][j])/double(re[i][j]);
        rc_e[i][j] = double(1/sqrt(born[i][j]))+double(1/sqrt(re[i][j]));
        cout << "rc_e[i][j] = " << rc_e[i][j] << endl;
      }
      else
      {
	      rc[i][j] = 0;
        rc_e[i][j] = 0;
      }
    }
    rc_g[i] = new TGraphErrors(9,xmid,rc[i],0,rc_e[i]);
    rct_g_u[i] = new TGraph(30,xtable,rc_table[y_xch[i+1]]);
    rct_g_d[i] = new TGraph(30,xtable,rc_table[y_xch[i]]);

    c1.cd(i+1);
    rc_g[i]->GetXaxis()->SetTitle("x_{Bj}");
    rc_g[i]->GetYaxis()->SetTitle("RC");
    rc_g[i]->GetYaxis()->SetTitleOffset(1.5);
    rc_g[i]->SetTitle(Form("RC @ %f < y < %f",ytab[i],ytab[i+1]));
    rc_g[i]->SetMarkerStyle(22);
    rc_g[i]->SetMarkerColor(601);
    rc_g[i]->SetMarkerSize(3);
    rc_g[i]->GetYaxis()->SetRangeUser(0.,1.3);
    rc_g[i]->SetFillColor(601);
    rc_g[i]->SetFillStyle(3001);
    rc_g[i]->Draw("A3");
    rc_g[i]->Draw("P");
    rct_g_u[i]->SetLineColor(3);
    rct_g_d[i]->SetLineColor(2);
    rct_g_u[i]->Draw("SAME");
    rct_g_d[i]->Draw("SAME");
    l.Draw("SAME");
    c1.Update();
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
      outfile << "\t" << rc[i][j];
    }
    outfile << endl;
  }

  c1.Print(RC_pdf);

  outfile.close();

  return 0;
}
