#include "TGraph2D.h"
#include "TCanvas.h"
#include "TAxis.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	vector<Double_t> Tx, Cx, TQ2, CQ2, TF2, CF2;
	Double_t cTx, cCx, cTQ2, cCQ2, cTF2, cCF2;
	Int_t i =0;

	ifstream f2t("F2_TERAD.dat");

	while(f2t >> cTx && i<5000)
        {
		f2t >> cTQ2 >> cTF2;
//		cout <<	cTx << " " << cTQ2 << " " << cTF2 << endl;
		if(cTQ2<0.2)
		{
			Tx.push_back(cTx);
			TQ2.push_back(cTQ2);
			TF2.push_back(cTF2);
			i++;
		}
	}

	f2t.close();

	i=0;

	ifstream f2c("F2_cteq.dat");

        while(f2c >> cCx && i<5000)
        {
                f2c >> cCQ2 >> cCF2;
//		cout << cCx << " " << cCQ2 << " " << cCF2 << endl;
                if(cCQ2<0.2)
		{
			Cx.push_back(cCx);
                	CQ2.push_back(cCQ2);
                	CF2.push_back(cCF2);
        		i++;
		}
	}

        f2c.close();


	cout << Tx.size() << " " << TQ2.size() << " " << TF2.size() << endl;
	cout <<	Cx.size() << " " << CQ2.size() << " " << CF2.size() << endl;

	TCanvas c1("c1","c1",0,0,3200,1600);
	c1.Divide(2,1);
	TCanvas c2("c2","c2",0,0,3200,1600);
	c2.Divide(1,1);
	TCanvas c3("c3","c3",0,0,3200,1600);
	c3.Divide(1,1);

	TGraph2D *terad = new TGraph2D("TERAD","TERAD",int(Tx.size()),&(Tx[0]),&(TQ2[0]),&(TF2[0]));
	terad->GetXaxis()->SetRangeUser(0,0.16);
	terad->GetXaxis()->SetTitle("x");
	terad->GetYaxis()->SetRangeUser(0,0.2);
	terad->GetYaxis()->SetTitle("Q2");

	c2.cd(1);
        terad->Draw("TRI1");
        c2.Update();

	c1.cd(1);
	terad->Draw("TRI1");
	c1.Update();

	TGraph2D *cteq  = new TGraph2D("CTEQ","CTEQ",int(Cx.size()),&(Cx[0]),&(CQ2[0]),&(CF2[0]));
	cteq->GetXaxis()->SetRangeUser(0,0.16);
	cteq->GetXaxis()->SetTitle("x");
  cteq->GetYaxis()->SetRangeUser(0,0.2);
	cteq->GetYaxis()->SetTitle("Q2");

	c3.cd(1);
	cteq->Draw("TRI1");
	c3.Update();

	c2.Print("3D_F2_T.pdf");
        c3.Print("3D_F2_C.pdf");

	c1.cd(2);
	cteq->Draw("TRI1");
	c1.Update();

	terad->SetMaximum(.5);
	cteq->SetMaximum(.5);

	c1.Print("3D_F2.pdf");

	return 0;
}
