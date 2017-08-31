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

#define TERADx_pdf "TERADx.pdf"
#define TERADy_pdf "TERADy.pdf"

using namespace std;

int main(int argc,char *argv[])
{

  if(argc != 2)
  {
    cout << "ERROR : Wrong number of arguments" << endl;
    cout << "Expected 2, received " << argc << " !" << endl;
    cout << "USAGE : \n ./terad_plot rc_table_file" << endl;
    return 1;
  }

  string sdum;

  double xtable[30] = {.000050,.000070,.000100,.000200,.000300,.000500,
                       .000700,.001000,.002000,.004000,.006000,.008000,
                       .010000,.013000,.016000,.020000,.030000,.040000,
                       .060000,.080000,.100000,.150000,.200000,.300000,
                       .400000,.500000,.600000,.700000,.800000,.900000};
  double ytable[19] = {.050000,.100000,.150000,.200000,.250000,.300000,
                       .350000,.400000,.450000,.500000,.550000,.600000,
                       .650000,.700000,.750000,.800000,.850000,.900000,
                       .950000};

  double rc_table[19][30];
  double rc_table_y[30][19];

  TCanvas c1("TERAD_xy_f(x)","TERAD_xy_f(x)",3200,1600);
  c1.Divide(1,1);
  TCanvas c2("TERAD_xy_f(y)","TERAD_xy_f(y)",3200,1600);
  c2.Divide(1,1);

  TGraph* rcxt[19];
  TGraph* rcyt[30];

  ifstream table(argv[1]);
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

  c1.cd(1);

  for(int i=0;i<19;i++)
  {
    rcxt[i] = new TGraph(30,xtable,rc_table[i]);

    rcxt[i]->GetXaxis()->SetTitle("x_{Bj}");
    rcxt[i]->GetYaxis()->SetTitle("RC");
    rcxt[i]->GetYaxis()->SetTitleOffset(1.5);
    rcxt[i]->SetTitle("RC(x))");
    // rcxt[i]->GetYaxis()->SetRangeUser(0.,1.3);
    rcxt[i]->SetFillColor(601);
    rcxt[i]->SetFillStyle(3001);
    rcxt[i]->SetLineColor(i+1);
    if(i==0) rcxt[i]->Draw();
    else rcxt[i]->Draw("SAME");
    c1.Update();
  }

  c2.cd(1);

  for(int i=0;i<30;i++)
  {
    rcyt[i] = new TGraph(19,ytable,rc_table_y[i]);

    rcyt[i]->GetXaxis()->SetTitle("y");
    rcyt[i]->GetYaxis()->SetTitle("RC");
    rcyt[i]->GetYaxis()->SetTitleOffset(1.5);
    rcyt[i]->SetTitle("RC(y)");
    // rcyt[i]->GetYaxis()->SetRangeUser(0.,1.3);
    rcyt[i]->SetFillColor(601);
    rcyt[i]->SetFillStyle(3001);
    rcyt[i]->SetLineColor(i+1);
    if(i==0) rcyt[i]->Draw();
    else rcyt[i]->Draw("SAME");
    c2.Update();
  }

  c1.Print(TERADx_pdf);
  c2.Print(TERADy_pdf);

  return 0;
}
