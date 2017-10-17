#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	vector<Double_t> Tx[5][11], TQ2[5][11], TF1[5][11], TF2[5][11];
	Double_t cTx, cTQ2, cTF1, cTF2;
	string filename[5] = {"terad_SF.dat","nnpdf21dis_SF.dat","mstw08_SF.dat","herapdf15_SF.dat","cteq61.dat"};
	string paramname[5] = {"TERAD","NNPDF21","MSTW08","HERAPDF15","CTEQ61"};
	int MarkerColorAlt[5] = {2,95,209,226,221};
	int MarkerStyleAlt[5] = {24,26,25,27,28};
	double Q2range[12] = {0.2,1,2,3,4,5,6,8,10,15,20,25};
	int xbin, Q2bin;

	for(int i=0; i<5; i++)
	{
		ifstream sff(filename[i].c_str());
		while(sff >> cTx)
	  {
			sff >> cTQ2 >> cTF1 >> cTF2;
	//		cout <<	cTx << " " << cTQ2 << " " << cTF2 << endl;

			xbin=-1;
			Q2bin=-1;

			if(.004<cTx && cTx<.008) xbin=0;
			else if(.01<cTx && cTx<.02) xbin=1;
			else if(.02<cTx && cTx<.03) xbin=2;
			else if(.03<cTx && cTx<.04) xbin=3;
			else if(.04<cTx && cTx<.06) xbin=4;
			else if(.06<cTx && cTx<.08) xbin=5;
			else if(.08<cTx && cTx<.1) xbin=6;
			else if(.1<cTx && cTx<.2) xbin=7;
			else if(.2<cTx && cTx<.3) xbin=8;
			else if(.3<cTx && cTx<.4) xbin=9;

			if(0.01<cTQ2 && cTQ2<1) Q2bin=0;
			else if(1<cTQ2 && cTQ2<2) Q2bin=1;
			else if(2<cTQ2 && cTQ2<3) Q2bin=2;
			else if(3<cTQ2 && cTQ2<4) Q2bin=3;
			else if(4<cTQ2 && cTQ2<5) Q2bin=4;
			else if(5<cTQ2 && cTQ2<6) Q2bin=5;
			else if(6<cTQ2 && cTQ2<8) Q2bin=6;
			else if(8<cTQ2 && cTQ2<10) Q2bin=7;
			else if(10<cTQ2 && cTQ2<15) Q2bin=8;
			else if(15<cTQ2 && cTQ2<20) Q2bin=9;
			else if(20<cTQ2 && cTQ2<25) Q2bin=10;

			if(xbin==-1 || Q2bin==-1) continue;

			Tx[i][Q2bin].push_back(cTx);
			TQ2[i][xbin].push_back(cTQ2);
			TF1[i][Q2bin].push_back(cTF1);
			TF2[i][Q2bin].push_back(cTF2);
		}
		sff.close();
	}

	TCanvas c1("c1","c1",0,0,3200,1600);
	c1.Divide(2,1);

	for(int j=0; j<11; j++)
	{
		for(int i=0; i<5; i++)
		{
			TGraph *sf1  = new TGraph(int(Tx[i][j].size()),&(Tx[i][j][0]),&(TF1[i][j][0]));
			sf1->SetTitle(Form("F1 %s @ %f < Q2 < %f",paramname[i].c_str(),Q2range[j],Q2range[j+1]));
			sf1->GetXaxis()->SetRangeUser(0.004,0.4);
			sf1->GetXaxis()->SetTitle("x");
			sf1->GetYaxis()->SetTitle("F1");

			TGraph *sf2  = new TGraph(int(Tx[i][j].size()),&(Tx[i][j][0]),&(TF2[i][j][0]));
			sf2->SetTitle(Form("F2 %s @ %f < Q2 < %f",paramname[i].c_str(),Q2range[j],Q2range[j+1]));
			sf2->GetXaxis()->SetRangeUser(0.004,0.4);
			sf2->GetXaxis()->SetTitle("x");
			sf2->GetYaxis()->SetTitle("F2");

			sf1->SetMarkerStyle(MarkerStyleAlt[i]);
	    sf1->SetMarkerColor(MarkerColorAlt[i]);
	    sf1->SetMarkerSize(3);
	    sf1->SetFillColor(601);
	    sf1->SetFillStyle(3001);

			sf2->SetMarkerStyle(MarkerStyleAlt[i]);
			sf2->SetMarkerColor(MarkerColorAlt[i]);
			sf2->SetMarkerSize(3);
			sf2->SetFillColor(601);
			sf2->SetFillStyle(3001);

			c1.cd(1);
			sf1->Draw("PA");
			c1.Update();

			c1.cd(2);
			sf2->Draw("PA");
			c1.Update();

			if(!j && !i) c1.Print("SF.pdf(","pdf");
			else if(j==10 && i==4) c1.Print("SF.pdf)","pdf");
			else c1.Print("SF.pdf","pdf");
		}
	}

	return 0;
}
