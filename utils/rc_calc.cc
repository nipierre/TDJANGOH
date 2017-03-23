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

  if(argc != 3)
  {
    cout << "ERROR : Wrong number of arguments" << endl;
    cout << "Expected 3, received " << argc << " !" << endl;
    cout << "USAGE : \n ./rc all_process_file born_file" << endl;
    return 1;
  }

  double dummy, x, y, Q2, xhad, yhad, Q2had;
  int id;

  double xtab[10] = {.004,.01,.02,.03,.04,.06,.1,.14,.18,.4};
  double ytab[6] = {.1,.15,.20,.30,.50,.70};

  double rc[5][9];
  int re[5][9];
  int born[5][9];

  TCanvas c1("RC_xy","RC_xy",3200,1600);
  c1.Divide(3,2);

  TGraph* rc_g[5];

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
      if(xhad < xtab[i])
      {
        xi = i-1;
        xflag = 1;
        break;
      }
    }
    for(int i=1; i<6; i++)
    {
      if(yhad < ytab[i])
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
      if(x < xtab[i])
      {
        xi = i-1;
        xflag = 1;
        break;
      }
    }
    for(int i=1; i<6; i++)
    {
      if(y < ytab[i])
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

  TLine l(0,1,0.2,1);

  ofstream outfile(OUTFILE);

  for(int i=0;i<5;i++)
  {
    for(int j=0;j<9;j++)
    {
      if(re[i][j])
	rc[i][j] = double(born[i][j])/double(re[i][j]);
      else
	rc[i][j] = 0;
    }
    rc_g[i] = new TGraph(9,xtab,rc[i]);

    c1.cd(i+1);
    rc_g[i]->GetXaxis()->SetTitle("x_{Bj}");
    rc_g[i]->GetYaxis()->SetTitle("RC");
    rc_g[i]->GetYaxis()->SetTitleOffset(1.5);
    rc_g[i]->SetTitle(Form("RC @ %f < y < %f",ytab[i],ytab[i+1]));
    rc_g[i]->SetMarkerStyle(22);
    rc_g[i]->SetMarkerColor(601);
    rc_g[i]->SetMarkerSize(3);
    rc_g[i]->GetYaxis()->SetRangeUser(0.,1.3);
    rc_g[i]->Draw("AP");
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
