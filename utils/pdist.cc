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
#include <vector>
#include <math.h>

#define _USE_MATH_DEFINES

#define E_distribution_pdf "pmdist_E.pdf"
#define t_distribution_pdf "pmdist_t.pdf"
#define Photon_p_distribution_pdf "pdist_p.pdf"
#define Photon_t_spread_pdf "pspread_t.pdf"
#define Photon_E_spread_y_pdf "pspready_E.pdf"
#define Photon_E_spread_x_pdf "pspreadx_E.pdf"
#define Photon_E_norm_pdf "pnorm_E.pdf"
#define t_quark_pdf "pdist_q.pdf"
#define p_quark_pdf "pdist_pq.pdf"
#define pt_quark_pdf "pt_q.pdf"
#define Q2_pdf "pQ2_lh.pdf"
#define Ef_pdf "pE_f.pdf"

#define EMIN 0
#define EMAX 160
#define XMIN 0.004
#define XMAX 0.4
#define YMIN 0.1
#define YMAX 0.7
#define Q2MIN 1
#define Q2MAX 70

using namespace std;

void BinLogX(TH1*h)
{
   TAxis *axis = h->GetXaxis();
   int bins = axis->GetNbins();

   Axis_t from = axis->GetXmin();
   Axis_t to = axis->GetXmax();
   Axis_t width = (to - from) / bins;
   Axis_t *new_bins = new Axis_t[bins + 1];

   for (int i = 0; i <= bins; i++) {
     new_bins[i] = pow(10, from + i * width);
   }
   axis->Set(bins, new_bins);
   delete new_bins;
}

int main(int argc,char *argv[])
{

  if(argc != 2)
  {
    cout << "ERROR : Wrong number of arguments" << endl;
    cout << "Expected 2, received " << argc << " !" << endl;
    cout << "USAGE : \n ./pdist file" << endl;
    return 1;
  }

  double dummy, px, py, pz, E, m, x, y, Q2, xhad, yhad, Q2had, phi_m, phi_g,
  theta_g, theta_m, phi_q, theta_q, q_flag, px_q, py_q;
  int id;
  int g_flag;

  double xtab[10] = {.004,.01,.02,.03,.04,.06,.1,.14,.18,.4};
  double ytab[6] = {.1,.15,.2,.3,.5,.7};

  TCanvas c1("E_distribution","E_distribution",3200,1600);
  TCanvas c2("#theta_distribution","#theta_distribution",3200,1600);
  TCanvas c3("Photon_#phi_distribution","Photon_#phi_distribution",3200,1600);
  TCanvas c4("Photon_#theta_distribution","Photon_#theta_distribution",3200,1600);
  TCanvas c5("Quarks_#theta_distribution","Quarks_#theta_distribution",3200,1600);
  TCanvas c6("Quarks_#Phi_distribution","Quarks_#Phi_distribution",3200,1600);
  TCanvas c7("Quarks_p_{t}_distribution","Quarks_p_{t}_distribution",3200,1600);
  TCanvas c8("E_distribution_spread_y","E_distribution_spread_y",3200,1600);
  TCanvas c9("E_dist_norm","E_dist_norm",3200,1600);
  TCanvas c10("Q^{2}","Q^{2}",3200,1600);
  TCanvas c11("E dep","E dep",3200,1600);
  TCanvas c12("Photon_E_distribution","Photon_E_distribution",2600,1600);
  TCanvas c13("E_distribution_spread_x","E_distribution_spread_x",3200,1600);
  c1.Divide(3,1);
  c2.Divide(3,2);
  c3.Divide(3,1);
  c4.Divide(3,2);
  c5.Divide(3,1);
  c6.Divide(3,1);
  c7.Divide(2,2);
  c8.Divide(3,2);
  c9.Divide(1,1);
  c10.Divide(3,2);
  c11.Divide(4,2);
  c12.Divide(1,1);
  c13.Divide(3,3);

  TH1F edist("E_{#gamma}","E_{#gamma}",100,-20,200);
  TH1F* edist_spready[5];
  TH1F* edist_spreadx[9];
  TH1F* tdist = new TH1F("#theta_{#gamma} log","#theta_{#gamma}",100,-4,-1);
  TH1F tdist2("#theta_{#gamma}","#theta_{#gamma}",100,0,0.1);
  TH1F pdist("#Phi_{#gamma}","#Phi_{#gamma}",100,-4,4);
  TH1F edist_m("E_{#mu}","E_{#mu}",100,0,200);
  TH1F edist_n("E_{#gamma,norm}","E_{#gamma,norm}",8,0,200);
  TH1F tdist_m("#theta_{#mu}","#theta_{#mu}",100,0,0.1);
  TH1F pdist_mg("#Phi_{#gamma-#mu}","#Phi_{#gamma-#mu}",100,-4,4);
  TH1F* tdist2_m = new TH1F("#theta_{#mu} 2","#theta_{#mu}",100,-4,-1);
  TH1F* tdist_spread[5];
  TH1F edist_q("E_{q}","E_{q}",100,0,200);
  TH1F tdist_q("#theta_{q}","#theta_{q}",100,0,0.2);
  TH1F tdist_qg("#theta_{q,bremsstrahlung}","#theta_{q,bremsstrahlung}",100,0,0.2);
  TH1F tdist_r("#theta_{ratio}","#theta_{ratio}",100,0,0.2);
  TH1F pdist_q("#Phi_{q}","#Phi_{q}",100,-4,4);
  TH1F pdist_mq("#Phi_{q-#mu}","#Phi_{q-#mu}",100,-4,4);
  TH1F pdist_bq("#Phi_{q-#mu,bremsstrahlung}","#Phi_{q-#mu,bremsstrahlung}",100,-4,4);
  TH1F pt_q("p_{t,q}^{2}","p_{t,q}^{2}",100,0,20);
  TH1F pt_qb("p_{t,q,brehmsstrahlung}^{2}","p_{t,q,brehmsstrahlung}^{2}",100,0,20);
  TH1F pt_qn("p_{t,q,no brehmsstrahlung}^{2}","p_{t,q,no brehmsstrahlung}^{2}",100,0,20);
  TH1F pt_qr("p_{t,q,ratio}^{2}","p_{t,q,ratio}^{2}",100,0,20);
  TH1F Q2dist_h("Q^{2}_{had}","Q^{2}_{had}",100,Q2MIN,Q2MAX);
  TH1F Q2dist_l("Q^{2}_{lep}","Q^{2}_{lep}",100,Q2MIN,Q2MAX);
  TH1F Q2dist_r("Q^{2}_{ratio}","Q^{2}_{ratio}",100,Q2MIN,Q2MAX);
  TH2D Q2dist_corr("Q^{2}_{corr}","Q^{2}_{corr}",100,0,Q2MAX,100,0,Q2MAX);
  TH2D Q2dist_corr_zoom("Q^{2}_{corr_zoom}","Q^{2}_{corr_zoom}",100,1,2,100,0,2);
  TH2D xdist_0("x_E_0","x_E_0",100,EMIN,EMAX,100,XMIN,XMAX);
  TH2D ydist_0("y_E_0","y_E_0",100,EMIN,EMAX,100,YMIN,YMAX);
  TH2D Q2dist_0("Q2_E_0","Q2_E_0",100,EMIN,EMAX,100,Q2MIN,Q2MAX);
  TH2D Q2dist_corr_0("Q2_corr_0","Q2_corr_0",100,Q2MIN,Q2MAX,100,Q2MIN,Q2MAX);
  TH2D xdist_t("x_E_t","x_E_t",100,EMIN,EMAX,100,XMIN,XMAX);
  TH2D ydist_t("y_E_t","y_E_t",100,EMIN,EMAX,100,YMIN,YMAX);
  TH2D Q2dist_t("Q2_E_t","Q2_E_t",100,EMIN,EMAX,100,Q2MIN,Q2MAX);
  TH2D Q2dist_corr_t("Q2_corr_t","Q2_corr_t",100,Q2MIN,Q2MAX,100,Q2MIN,Q2MAX);
  TH2D pdist_t("#theta_{#gamma} vs #Phi_{#gamma}","#theta_{#gamma} vs #Phi_{#gamma}",100,-M_PI,M_PI,100,0,0.025);

  for(int i=0;i<5;i++)
  {
    tdist_spread[i] = new TH1F(Form("#theta_{#gamma} %d",i),"#theta_{#gamma}",100,0,0.6);
    edist_spready[i] = new TH1F(Form("#E_{#gamma} %f < y < %f",ytab[i],ytab[i+1]),"#E_{#gamma}",100,0,200);
  }

  for(int i=0;i<9;i++)
  {
    edist_spreadx[i] = new TH1F(Form("#E_{#gamma} %f < x < %f",xtab[i],xtab[i+1]),"#E_{#gamma}",100,0,200);
  }

  ifstream evt(argv[1]);

  while(evt >> dummy)
  {
    g_flag = 0;
    phi_m = 0;
    phi_g = 0;
    phi_q = 0;
    theta_g = 0;
    theta_m = 0;
    theta_q = 0;

    //evt >> dummy;
    //cout << dummy << " ";
    evt >> x >> y >> Q2 >> xhad >> yhad >> Q2had;

    Q2dist_h.Fill(Q2had);
    Q2dist_l.Fill(Q2);

    Q2dist_corr.Fill(Q2,Q2had);
    Q2dist_corr_zoom.Fill(Q2,Q2had);

    for(int j=0; j<3; j++)
    {
      evt >> id;
      if(id == 22)
      {
        evt >> px;
        evt >> py;
        evt >> pz;
        evt >> E;
        evt >> m;

        if(px || py)
        {
          g_flag = 1;
          //cout << px << " " << py << " " << pz << " " << E << " " << m << endl;

          theta_g = asin(sqrt(pow(px/E,2)+pow(py/E,2)));
          tdist->Fill(log10(theta_g));
          tdist2.Fill(theta_g);

          // py~cos px~sin
          if(px>0.0 && py>0.0)
          {
            pdist.Fill(asin(px/sqrt(pow(px,2)+pow(py,2))));
            phi_g = asin(px/sqrt(pow(px,2)+pow(py,2)));
          }
          else if(px<0.0 && py>0.0)
          {
            pdist.Fill(asin(px/sqrt(pow(px,2)+pow(py,2))));
            phi_g = asin(px/sqrt(pow(px,2)+pow(py,2)));
          }
          else if(px>0.0 && py<0.0)
          {
            pdist.Fill(M_PI-asin(px/sqrt(pow(px,2)+pow(py,2))));
            phi_g = M_PI-asin(px/sqrt(pow(px,2)+pow(py,2)));
          }
          else
          {
            pdist.Fill(-M_PI-asin(px/sqrt(pow(px,2)+pow(py,2))));
            phi_g = -M_PI-asin(px/sqrt(pow(px,2)+pow(py,2)));
          }

          if(y<0.1) tdist_spread[0]->Fill(asin(sqrt(pow(px/E,2)+pow(py/E,2))));
          else if(0.1<y && y<0.2) tdist_spread[1]->Fill(asin(sqrt(pow(px/E,2)+pow(py/E,2))));
          else if(0.2<y && y<0.4) tdist_spread[2]->Fill(asin(sqrt(pow(px/E,2)+pow(py/E,2))));
          else if(0.4<y && y<0.6) tdist_spread[3]->Fill(asin(sqrt(pow(px/E,2)+pow(py/E,2))));
          else if(0.6<y && y<0.7) tdist_spread[4]->Fill(asin(sqrt(pow(px/E,2)+pow(py/E,2))));
        }
        if(E)
        {
          edist.Fill(E);
          if(E>=20) edist_n.Fill(E);
          if(ytab[0]<=y && y<ytab[1]) edist_spready[0]->Fill(E);
          else if(ytab[1]<=y && y<ytab[2]) edist_spready[1]->Fill(E);
          else if(ytab[2]<=y && y<ytab[3]) edist_spready[2]->Fill(E);
          else if(ytab[3]<=y && y<=ytab[4]) edist_spready[3]->Fill(E);
          else if(ytab[4]<=y && y<=ytab[5]) edist_spready[4]->Fill(E);
          if(xtab[0]<=x && x<xtab[1]) edist_spreadx[0]->Fill(E);
          else if(xtab[1]<=x && x<xtab[2]) edist_spreadx[1]->Fill(E);
          else if(xtab[2]<=x && x<xtab[3]) edist_spreadx[2]->Fill(E);
          else if(xtab[3]<=x && x<=xtab[4]) edist_spreadx[3]->Fill(E);
          else if(xtab[4]<=x && x<=xtab[5]) edist_spreadx[4]->Fill(E);
          else if(xtab[5]<=x && x<=xtab[6]) edist_spreadx[5]->Fill(E);
          else if(xtab[6]<=x && x<=xtab[7]) edist_spreadx[6]->Fill(E);
          else if(xtab[7]<=x && x<=xtab[8]) edist_spreadx[7]->Fill(E);
          else if(xtab[8]<=x && x<=xtab[9]) edist_spreadx[8]->Fill(E);
        }

      }
      else if(0 < abs(id) && abs(id) < 9)
      {
        evt >> px;
        evt >> py;
        evt >> pz;
        evt >> E;
        evt >> m;

        px_q = px;
        py_q = py;

        if(px || py)
        {
          q_flag = 1;
          pt_q.Fill(pow(px,2)+pow(py,2));

          theta_q = asin(sqrt(pow(px/sqrt(pow(E,2)-pow(m,2)),2)+pow(py/sqrt(pow(E,2)-pow(m,2)),2)));
          if(px>0.0 && py>0.0)
          {
            pdist_q.Fill(asin(px/sqrt(pow(px,2)+pow(py,2))));
            phi_q = asin(px/sqrt(pow(px,2)+pow(py,2)));
          }
          else if(px<0.0 && py>0.0)
          {
            pdist_q.Fill(asin(px/sqrt(pow(px,2)+pow(py,2))));
            phi_q = asin(px/sqrt(pow(px,2)+pow(py,2)));
          }
          else if(px>0.0 && py<0.0)
          {
            pdist_q.Fill(M_PI-asin(px/sqrt(pow(px,2)+pow(py,2))));
            phi_q = M_PI-asin(px/sqrt(pow(px,2)+pow(py,2)));
          }
          else
          {
            pdist_q.Fill(-M_PI-asin(px/sqrt(pow(px,2)+pow(py,2))));
            phi_q = -M_PI-asin(px/sqrt(pow(px,2)+pow(py,2)));
          }
        }
        if(E)
        {
          edist_q.Fill(E);
        }
      }
      else if(id == 13)
      {
        evt >> px;
        evt >> py;
        evt >> pz;
        evt >> E;
        evt >> m;
        //cout << px << " " << py << " " << pz << " " << E << " " << m << endl;
        edist_m.Fill(E);
        theta_m = asin(sqrt(pow(px/sqrt(pow(E,2)-pow(m,2)),2)+pow(py/sqrt(pow(E,2)-pow(m,2)),2)));
        tdist_m.Fill(theta_m);
        tdist2_m->Fill(log10(theta_m));
        if(px>0.0 && py>0.0)
        {
          phi_m = asin(px/sqrt(pow(px,2)+pow(py,2)));
        }
        else if(px<0.0 && py>0.0)
        {
          phi_m = asin(px/sqrt(pow(px,2)+pow(py,2)));
        }
        else if(px>0.0 && py<0.0)
        {
          phi_m = M_PI-asin(px/sqrt(pow(px,2)+pow(py,2)));
        }
        else
        {
          phi_m = -M_PI-asin(px/sqrt(pow(px,2)+pow(py,2)));
        }
      }
      else
      {
        for(int i=0; i<5; i++)
        {
          evt >> dummy;
        }
      }
    }
    if(g_flag)
    {
      if(phi_g-phi_m>M_PI)
      {
        pdist_mg.Fill(-2*M_PI+phi_g-phi_m);
        pdist_t.Fill(-2*M_PI+phi_g-phi_m,theta_g);
      }
      else if(phi_g-phi_m<-M_PI)
      {
        pdist_mg.Fill(2*M_PI+phi_g-phi_m);
        pdist_t.Fill(2*M_PI+phi_g-phi_m,theta_g);
      }
      else
      {
        pdist_mg.Fill(phi_g-phi_m);
        pdist_t.Fill(phi_g-phi_m,theta_g);
      }

      if(0<theta_g && theta_g<0.001)
      {
        xdist_0.Fill(E,x);
        ydist_0.Fill(E,y);
        Q2dist_0.Fill(E,Q2);
        Q2dist_corr_0.Fill(Q2,Q2had);
      }
      if(theta_m*0.99<theta_g && theta_g<theta_m*1.01)
      {
        xdist_t.Fill(E,x);
        ydist_t.Fill(E,y);
        Q2dist_t.Fill(E,Q2);
        Q2dist_corr_t.Fill(Q2,Q2had);
      }

    }
    if(q_flag)
    {
      if(g_flag)
      {
        if(theta_q) tdist_qg.Fill(theta_q);
        if(phi_q-phi_m>M_PI) {pdist_bq.Fill(-2*M_PI+phi_q-phi_m);}
        else if(phi_q-phi_m<-M_PI) {pdist_bq.Fill(2*M_PI+phi_q-phi_m);}
        else {pdist_bq.Fill(phi_q-phi_m);}
        pt_qb.Fill(pow(px_q,2)+pow(py_q,2));
      }
      else
      {
        if(theta_q) tdist_q.Fill(theta_q);
        pt_qn.Fill(pow(px_q,2)+pow(py_q,2));
      }

      if(phi_q-phi_m>M_PI) {pdist_mq.Fill(-2*M_PI+phi_q-phi_m);}
      else if(phi_q-phi_m<-M_PI) {pdist_mq.Fill(2*M_PI+phi_q-phi_m);}
      else {pdist_mq.Fill(phi_q-phi_m);}
    }
  }

  c1.cd(1);
  edist.GetXaxis()->SetTitle("E_{#gamma} (GeV)");
  edist.GetYaxis()->SetTitle("# Events");
  edist.GetYaxis()->SetTitleOffset(1.5);
  edist.Draw();
  //gPad->SetLogy();
  c1.Update();

  c1.cd(2);
  edist_m.GetXaxis()->SetTitle("E_{#mu} (GeV)");
  edist_m.GetYaxis()->SetTitle("# Events");
  edist_m.GetYaxis()->SetTitleOffset(1.5);
  edist_m.Draw();
  c1.Update();

  c1.cd(3);
  edist_q.GetXaxis()->SetTitle("E_{q} (GeV)");
  edist_q.GetYaxis()->SetTitle("# Events");
  edist_q.GetYaxis()->SetTitleOffset(1.3);
  edist_q.GetXaxis()->SetTitleOffset(1.2);
  edist_q.Draw();
  c5.Update();

  c2.cd(1);
  tdist2.GetXaxis()->SetTitle("E_{#gamma} (GeV)");
  tdist2.GetXaxis()->SetTitle("# Events");
  tdist2.GetXaxis()->SetTitleOffset(1.3);
  tdist2.GetXaxis()->SetTitleOffset(1.2);
  tdist2.Draw();
  c2.Update();

  c2.cd(3);
  tdist->GetXaxis()->SetTitle("#theta_{#gamma}");
  tdist->GetYaxis()->SetTitle("# Events");
  tdist->GetYaxis()->SetTitleOffset(1.3);
  tdist->GetXaxis()->SetTitleOffset(1.2);
  BinLogX(tdist);
  tdist->Draw();
  gPad->SetLogx();
  c2.Update();

  c2.cd(2);
  tdist2.GetXaxis()->SetTitle("#theta_{#gamma}");
  tdist2.GetYaxis()->SetTitle("# Events");
  tdist2.GetYaxis()->SetTitleOffset(1.3);
  tdist2.GetXaxis()->SetTitleOffset(1.2);
  tdist2.Draw();
  gPad->SetLogx();
  c2.Update();

  c3.cd(1);
  pdist.GetXaxis()->SetTitle("#Phi_{#gamma}");
  pdist.GetYaxis()->SetTitle("# Events");
  pdist.GetYaxis()->SetTitleOffset(1.3);
  pdist.GetXaxis()->SetTitleOffset(1.2);
  pdist.Draw();
  c3.Update();

  c3.cd(2);
  pdist_mg.GetXaxis()->SetTitle("#Phi_{#gamma-#mu}");
  pdist_mg.GetYaxis()->SetTitle("# Events");
  pdist_mg.GetYaxis()->SetTitleOffset(1.3);
  pdist_mg.GetXaxis()->SetTitleOffset(1.2);
  pdist_mg.Draw();
  c3.Update();

  c3.cd(3);
  pdist_t.GetXaxis()->SetTitle("#Phi_{#gamma}");
  pdist_t.GetYaxis()->SetTitle("#theta_{#gamma}");
  pdist_t.GetYaxis()->SetTitleOffset(1.3);
  pdist_t.GetXaxis()->SetTitleOffset(1.2);
  pdist_t.SetStats(false);
  pdist_t.Draw("colz");
  c3.Update();

  c2.cd(4);
  tdist_m.GetXaxis()->SetTitle("#theta_{#mu}");
  tdist_m.GetYaxis()->SetTitle("# Events");
  tdist_m.GetYaxis()->SetTitleOffset(1.3);
  tdist_m.GetXaxis()->SetTitleOffset(1.2);
  tdist_m.Draw();
  c2.Update();

  c2.cd(5);
  tdist_m.GetXaxis()->SetTitle("#theta_{#mu}");
  tdist_m.GetYaxis()->SetTitle("# Events");
  tdist_m.GetYaxis()->SetTitleOffset(1.3);
  tdist_m.GetXaxis()->SetTitleOffset(1.2);
  tdist_m.Draw();
  gPad->SetLogx();
  c2.Update();

  c2.cd(6);
  tdist2_m->GetXaxis()->SetTitle("#theta_{#mu}");
  tdist2_m->GetYaxis()->SetTitle("# Events");
  tdist2_m->GetYaxis()->SetTitleOffset(1.3);
  tdist2_m->GetXaxis()->SetTitleOffset(1.2);
  BinLogX(tdist2_m);
  tdist2_m->Draw();
  gPad->SetLogx();
  c2.Update();

  for(int i=0;i<5;i++)
  {
    c4.cd(i+1);
    tdist_spread[i]->GetXaxis()->SetTitle("#theta_{#gamma}");
    tdist_spread[i]->GetYaxis()->SetTitle("# Events");
    tdist_spread[i]->GetYaxis()->SetTitleOffset(1.3);
    tdist_spread[i]->GetXaxis()->SetTitleOffset(1.2);
    tdist_spread[i]->Draw();
    c4.Update();
  }

  c5.cd(1);
  tdist_q.GetXaxis()->SetTitle("#theta_{q}");
  tdist_q.GetYaxis()->SetTitle("# Events_{norm}");
  tdist_q.GetYaxis()->SetTitleOffset(1.3);
  tdist_q.GetXaxis()->SetTitleOffset(1.2);
  tdist_q.Draw();
  c5.Update();

  c5.cd(2);
  tdist_qg.GetXaxis()->SetTitle("#theta_{q,bremsstrahlung}");
  tdist_qg.GetYaxis()->SetTitle("# Events_{norm}");
  tdist_qg.GetYaxis()->SetTitleOffset(1.3);
  tdist_qg.GetXaxis()->SetTitleOffset(1.2);
  tdist_qg.Draw();
  c5.Update();

  c5.cd(3);
  tdist_q.Scale(1.0/tdist_q.GetEntries());
  tdist_qg.Scale(1.0/tdist_qg.GetEntries());
  tdist_r = tdist_q;
  tdist_r.Divide(&tdist_qg);
  tdist_r.SetTitle("#theta_{ratio}");
  tdist_r.GetXaxis()->SetTitle("#theta_{ratio}");
  tdist_r.GetYaxis()->SetTitle("Ratio of Events");
  tdist_r.GetYaxis()->SetTitleOffset(1.3);
  tdist_r.GetXaxis()->SetTitleOffset(1.2);
  tdist_r.SetStats(false);
  tdist_r.Draw();
  TLine l(0,1,0.2,1);
  l.Draw();
  c5.Update();

  c6.cd(1);
  pdist_q.GetXaxis()->SetTitle("#Phi_{q}");
  pdist_q.GetYaxis()->SetTitle("# Events");
  pdist_q.GetYaxis()->SetTitleOffset(1.3);
  pdist_q.GetXaxis()->SetTitleOffset(1.2);
  pdist_q.Draw();
  c6.Update();

  c6.cd(2);
  pdist_mq.GetXaxis()->SetTitle("#Phi_{q-#mu}");
  pdist_mq.GetYaxis()->SetTitle("# Events");
  pdist_mq.GetYaxis()->SetTitleOffset(1.3);
  pdist_mq.GetXaxis()->SetTitleOffset(1.2);
  pdist_mq.Draw();
  c6.Update();

  c6.cd(3);
  pdist_bq.GetXaxis()->SetTitle("#Phi_{q-#mu,bremsstrahlung}");
  pdist_bq.GetYaxis()->SetTitle("# Events");
  pdist_bq.GetYaxis()->SetTitleOffset(1.3);
  pdist_bq.GetXaxis()->SetTitleOffset(1.2);
  pdist_bq.Draw();
  c6.Update();

  c7.cd(1);
  pt_q.GetXaxis()->SetTitle("p_{t,all}^{2} (GeV^{2})");
  pt_q.GetYaxis()->SetTitle("# Events");
  pt_q.GetYaxis()->SetTitleOffset(1.3);
  pt_q.GetXaxis()->SetTitleOffset(1.2);
  pt_q.Draw();
  c7.Update();

  c7.cd(4);
  pt_qb.GetXaxis()->SetTitle("p_{t,brehmsstrahlung}^{2} (GeV^{2})");
  pt_qb.GetYaxis()->SetTitle("# Events norm");
  pt_qb.GetYaxis()->SetTitleOffset(1.3);
  pt_qb.GetXaxis()->SetTitleOffset(1.2);
  pt_qb.Draw();
  c7.Update();

  c7.cd(3);
  pt_qn.GetXaxis()->SetTitle("p_{t,no brehmsstrahlung}^{2} (GeV^{2})");
  pt_qn.GetYaxis()->SetTitle("# Events norm");
  pt_qn.GetYaxis()->SetTitleOffset(1.3);
  pt_qn.GetXaxis()->SetTitleOffset(1.2);
  pt_qn.Draw();
  c7.Update();

  c7.cd(2);
  pt_qn.Scale(1.0/pt_qn.GetEntries());
  pt_qb.Scale(1.0/pt_qb.GetEntries());
  pt_qr = pt_qn;
  pt_qr.Divide(&pt_qb);
  pt_qr.SetTitle("p_{t,ratio}^{2}");
  pt_qr.GetXaxis()->SetTitle("p_{t,ratio}^{2} (GeV^{2})");
  pt_qr.GetYaxis()->SetTitle("Ratio of Events");
  pt_qr.GetYaxis()->SetTitleOffset(1.3);
  pt_qr.GetXaxis()->SetTitleOffset(1.2);
  pt_qr.SetStats(false);
  pt_qr.Draw();
  TLine lq(0,1,20,1);
  lq.Draw();
  c7.Update();

  for(int i=0; i<5; i++)
  {
    c8.cd(i+1);
    edist_spready[i]->GetXaxis()->SetTitle("E_{#gamma} (GeV)");
    edist_spready[i]->GetYaxis()->SetTitle("# Events");
    edist_spready[i]->GetYaxis()->SetTitleOffset(1.3);
    edist_spready[i]->GetXaxis()->SetTitleOffset(1.2);
    edist_spready[i]->SetTitle("E_{#gamma}");
    edist_spready[i]->Draw();
    gPad->SetLogy();
    c8.Update();
  }

  c9.cd(1);
  for(int i=2; i<8; i++)
  {
    //cout << edist_n.GetBinContent(i) << " " << edist_n.GetBinContent(i)*100/100000 << endl;
  }
  edist_n.Scale(1.0/100000);
  edist_n.GetXaxis()->SetTitle("E_{#gamma} (GeV)");
  edist_n.GetYaxis()->SetTitle("# Events_{norm}");
  edist_n.GetYaxis()->SetTitleOffset(1.3);
  edist_n.GetXaxis()->SetTitleOffset(1.2);
  edist_n.Draw();
  c9.Update();

  c10.cd(1);
  Q2dist_l.GetXaxis()->SetTitle("Q^{2}_{lep} (GeV)^{2}");
  Q2dist_l.GetYaxis()->SetTitle("# Events");
  Q2dist_l.GetYaxis()->SetTitleOffset(1.3);
  Q2dist_l.GetXaxis()->SetTitleOffset(1.2);
  Q2dist_l.Draw();
  gPad->SetLogy();
  c10.Update();

  c10.cd(2);
  Q2dist_h.GetXaxis()->SetTitle("Q^{2}_{had} (GeV)^{2}");
  Q2dist_h.GetYaxis()->SetTitle("# Events");
  Q2dist_h.GetYaxis()->SetTitleOffset(1.3);
  Q2dist_h.GetXaxis()->SetTitleOffset(1.2);
  Q2dist_h.Draw();
  gPad->SetLogy();
  c10.Update();

  c10.cd(3);
  Q2dist_r = Q2dist_l;
  Q2dist_r.Divide(&Q2dist_h);
  Q2dist_r.SetTitle("Q^{2}_{ratio}");
  Q2dist_r.GetXaxis()->SetTitle("Q^{2}_{lep}/Q^{2}_{had}");
  Q2dist_r.GetYaxis()->SetTitle("Ratio of Events");
  Q2dist_r.GetYaxis()->SetTitleOffset(1.3);
  Q2dist_r.GetXaxis()->SetTitleOffset(1.2);
  Q2dist_r.Draw();
  TLine l2(Q2MIN,1,Q2MAX,1);
  l2.Draw();
  c10.Update();

  c10.cd(4);
  Q2dist_corr.GetXaxis()->SetTitle("Q^{2}_{lep}");
  Q2dist_corr.GetYaxis()->SetTitle("Q^{2}_{had}");
  Q2dist_corr.GetYaxis()->SetTitleOffset(1.3);
  Q2dist_corr.GetXaxis()->SetTitleOffset(1.2);
  Q2dist_corr.SetStats(false);
  Q2dist_corr.Draw("colz");
  c10.Update();

  c10.cd(5);
  Q2dist_corr_zoom.GetXaxis()->SetTitle("Q^{2}_{lep}");
  Q2dist_corr_zoom.GetYaxis()->SetTitle("Q^{2}_{had}");
  Q2dist_corr_zoom.GetYaxis()->SetTitleOffset(1.3);
  Q2dist_corr_zoom.GetXaxis()->SetTitleOffset(1.2);
  Q2dist_corr_zoom.SetStats(false);
  Q2dist_corr_zoom.Draw("colz");
  c10.Update();

  c11.cd(1);
  xdist_0.GetXaxis()->SetTitle("E_{#gamma}");
  xdist_0.GetYaxis()->SetTitle("x_{Bj}");
  xdist_0.GetYaxis()->SetTitleOffset(1.3);
  xdist_0.GetXaxis()->SetTitleOffset(1.2);
  xdist_0.Draw();
  c11.Update();

  c11.cd(2);
  ydist_0.GetXaxis()->SetTitle("E_{#gamma}");
  ydist_0.GetYaxis()->SetTitle("y");
  ydist_0.GetYaxis()->SetTitleOffset(1.3);
  ydist_0.GetXaxis()->SetTitleOffset(1.2);
  ydist_0.Draw();
  c11.Update();

  c11.cd(3);
  Q2dist_0.GetXaxis()->SetTitle("E_{#gamma}");
  Q2dist_0.GetYaxis()->SetTitle("Q^{2}");
  Q2dist_0.GetYaxis()->SetTitleOffset(1.3);
  Q2dist_0.GetXaxis()->SetTitleOffset(1.2);
  Q2dist_0.Draw();
  c11.Update();

  c11.cd(4);
  Q2dist_corr_0.GetXaxis()->SetTitle("Q^{2}_{lep}");
  Q2dist_corr_0.GetYaxis()->SetTitle("Q^{2}_{had}");
  Q2dist_corr_0.GetYaxis()->SetTitleOffset(1.3);
  Q2dist_corr_0.GetXaxis()->SetTitleOffset(1.2);
  Q2dist_corr_0.Draw();
  c11.Update();

  c11.cd(5);
  xdist_t.GetXaxis()->SetTitle("E_{#gamma}");
  xdist_t.GetYaxis()->SetTitle("x_{Bj}");
  xdist_t.GetYaxis()->SetTitleOffset(1.3);
  xdist_t.GetXaxis()->SetTitleOffset(1.2);
  xdist_t.Draw();
  c11.Update();

  c11.cd(6);
  ydist_t.GetXaxis()->SetTitle("E_{#gamma}");
  ydist_t.GetYaxis()->SetTitle("y");
  ydist_t.GetYaxis()->SetTitleOffset(1.3);
  ydist_t.GetXaxis()->SetTitleOffset(1.2);
  ydist_t.Draw();
  c11.Update();

  c11.cd(7);
  Q2dist_t.GetXaxis()->SetTitle("E_{#gamma}");
  Q2dist_t.GetYaxis()->SetTitle("Q^{2}");
  Q2dist_t.GetYaxis()->SetTitleOffset(1.3);
  Q2dist_t.GetXaxis()->SetTitleOffset(1.2);
  Q2dist_t.Draw();
  c11.Update();

  c11.cd(8);
  Q2dist_corr_t.GetXaxis()->SetTitle("Q^{2}_{lep}");
  Q2dist_corr_t.GetYaxis()->SetTitle("Q^{2}_{had}");
  Q2dist_corr_t.GetYaxis()->SetTitleOffset(1.3);
  Q2dist_corr_t.GetXaxis()->SetTitleOffset(1.2);
  Q2dist_corr_t.Draw();
  c11.Update();

  c12.cd(1);
  edist.GetXaxis()->SetTitle("E_{#gamma} (GeV)");
  edist.GetYaxis()->SetTitle("# Events");
  edist.GetYaxis()->SetTitleOffset(1.5);
  edist.Draw();
  //gPad->SetLogy();
  c12.Update();

  for(int i=0; i<9; i++)
  {
    c13.cd(i+1);
    edist_spreadx[i]->GetXaxis()->SetTitle("E_{#gamma} (GeV)");
    edist_spreadx[i]->GetYaxis()->SetTitle("# Events");
    edist_spreadx[i]->GetYaxis()->SetTitleOffset(1.3);
    edist_spreadx[i]->GetXaxis()->SetTitleOffset(1.2);
    edist_spreadx[i]->SetTitle("E_{#gamma}");
    edist_spreadx[i]->Draw();
    gPad->SetLogy();
    c13.Update();
  }



  c1.Print(E_distribution_pdf);
  c2.Print(t_distribution_pdf);
  c3.Print(Photon_p_distribution_pdf);
  c4.Print(Photon_t_spread_pdf);
  c5.Print(t_quark_pdf);
  c6.Print(p_quark_pdf);
  c7.Print(pt_quark_pdf);
  c8.Print(Photon_E_spread_y_pdf);
  c9.Print(Photon_E_norm_pdf);
  c10.Print(Q2_pdf);
  c11.Print(Ef_pdf);
  c12.Print("pED.pdf");
  c13.Print(Photon_E_spread_x_pdf);

  return 0;
}
