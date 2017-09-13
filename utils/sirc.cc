#include "sirc.h"

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
    cout << BOLD(FRED("USAGE : \n ./sirc -f [RC_file] [Born_file]")) << endl;
    cout << BOLD(FRED("[-h for further help]")) << endl;
    return 1;
  }

  string hadron_t[2] = {"h^{+}","h^{-}"};
  TCanvas c1("RC_f(z)","RC_f(z)",3200,1600);
  c1.Divide(2,1);
  TCanvas c2("RC_f(pt)","RC_f(pt)",3200,1600);
  c2.Divide(2,1);
  TCanvas c3("RC_f(x)","RC_f(x)",3200,1600);
  c3.Divide(2,1);
  TCanvas c4("RC_f(y)","RC_f(y)",3200,1600);
  c4.Divide(2,1);

  E_prime = 0;

  for(int i=0; i<3; i++)
  {
    for(int j=0; j<9; j++)
    {
      for(int k=0; k<5; k++)
      {
        fNSIDIS_evt_r[i][j][k]=0;
        fNSIDIS_evt_b[i][j][k]=0;
        for(int l=0; l<12; l++)
        {
          fNDIS_evt_r[i][j][k][l]=0;
          fNDIS_evt_b[i][j][k][l]=0;
        }
      }
    }
  }

  for(int i=0; i<12; i++)
  {
    fNu_max[1][i] = sqrt(pow(40,2)+pow(fM_K,2))/fZrange[i+1];
    fNu_min[1][i] = sqrt(pow(3,2)+pow(fM_K,2))/fZrange[i];

    fNu_max[2][i] = sqrt(pow(40,2)+pow(fM_p,2))/fZrange[i+1];
    fNu_min[2][i] = sqrt(pow(3,2)+pow(fM_p,2))/fZrange[i];

    fNu_max[0][i] = sqrt(pow(40,2)+pow(fM_pi,2))/fZrange[i+1];
    fNu_min[0][i] = sqrt(pow(3,2)+pow(fM_pi,2))/fZrange[i];
  }

  if(fileFlag == "-f")
  {
    ifstream revt(FileRC);

    while(revt >> npart)
    {
      fDIScuts = 0;
//      cout << npart << endl;
      revt >> E >> xbj >> y >> Q2;
      W = pow(fM_p,2) + Q2*(1-xbj)/xbj;
//      cout << E << "\txbj : " << xbj << "\ty : " << y << "\tQ2 : " << Q2 << endl;
//      cout << "Particles list" << endl;
      if(0<E)
      {
        if(Q2>1)
        {
          if(.004<xbj && xbj<.4)
          {
            if(.1<y && y<.7)
            {
              if(5<sqrt(W) && sqrt(W)<17)
              {
                fDIScuts = 1;
              }
            }
          }
        }
      }

      if(0.004<xbj && xbj<0.01) xbin = 0;
      else if(0.01<=xbj && xbj<0.02) xbin = 1;
      else if(0.02<=xbj && xbj<0.03) xbin = 2;
      else if(0.03<=xbj && xbj<0.04) xbin = 3;
      else if(0.04<=xbj && xbj<0.06) xbin = 4;
      else if(0.06<=xbj && xbj<0.1) xbin = 5;
      else if(0.1<=xbj && xbj<0.14) xbin = 6;
      else if(0.14<=xbj && xbj<0.18) xbin = 7;
      else xbin = 8;

      if(0.1<y && y<0.15) ybin = 0;
      else if(0.15<=y && y<0.2) ybin = 1;
      else if(0.2<=y && y<0.3) ybin = 2;
      else if(0.3<=y && y<0.5) ybin = 3;
      else ybin = 4;

      if(fDIScuts)
      {
        for(int i=0; i<12; i++)
        {
          fNDIS_evt_r[0][xbin][ybin][i]++;
          fNDIS_evt_r[1][xbin][ybin][i]++;
          fNDIS_evt_r[2][xbin][ybin][i]++;

          fFlag_r[0][xbin][ybin][i]=0;
          fFlag_r[1][xbin][ybin][i]=0;
          fFlag_r[2][xbin][ybin][i]=0;
        }
      }

      for(int i=0; i<npart; i++)
      {
        revt >> dummy >> id >> dummy >> dummy >> dummy;
      //  cout << dummy << "\tid : " << id << "\t" << dummy << "\t" << dummy << "\t" << dummy << "\t";
        revt >> px >> py >> pz >> Eh;
      //  cout << px << "\t" << py << "\t" << pz << "\t" << Eh << "\t";
        revt >> dummy >> dummy >> dummy >> dummy;
      //  cout << dummy << "\t" << dummy << "\t" << dummy << "\t" << dummy << endl;

        if(!fDIScuts) continue;

        if(abs(id)==13)
        {
          E_prime = Eh;
          nu = E - E_prime;

          // nu cut
          for(int j=0; j<12; j++)
          {
            if(!(fNu_min[0][j]<nu && nu<fNu_max[0][j]))
            {
              fFlag_r[0][xbin][ybin][j]=1;
            }
            if(!(fNu_min[1][j]<nu && nu<fNu_max[1][j]))
            {
              fFlag_r[1][xbin][ybin][j]=1;
            }
            if(!(fNu_min[2][j]<nu && nu<fNu_max[2][j]))
            {
              fFlag_r[2][xbin][ybin][j]=1;
            }
            if(fFlag_r[0][xbin][ybin][j] || fFlag_r[1][xbin][ybin][j] || fFlag_r[2][xbin][ybin][j])
            {
              fNDIS_evt_r[0][xbin][ybin][j]--; fNDIS_evt_r[1][xbin][ybin][j]--; fNDIS_evt_r[2][xbin][ybin][j]--;
            }
          }
        }
        else if(id == 211)//pi+
        {
          fId = 0;
        }
        else if(id == -211)//pi-
        {
          fId = 1;
        }
        else if(id == 321)//K+
        {
          fId = 2;
        }
        else if(id == -321)//K-
        {
          fId = 3;
        }
        else if(id == 2212)//p
        {
          fId = 4;
        }
        else if(id == -2212)//pb
        {
          fId = 5;
        }
        else if(id != 22 && abs(id) != 13)//Hadron
        {
          if(id>0)
          {
            fId = 6;
          }
          else
          {
            fId = 7;
          }
        }

        if(abs(id) == 13 || id == 22) continue;

        pt = sqrt(pow(px,2)+pow(py,2));

        if(nu)
        {
          if(fId == 2 || fId == 3)
            z = sqrt(pow(Eh,2)+pow(fM_K,2))/nu;
          else if(fId == 4 || fId == 5)
            z = sqrt(pow(Eh,2)+pow(fM_p,2))/nu;
          else
            z = sqrt(pow(Eh,2)+pow(fM_pi,2))/nu;
        }
        else
        {
          z = 0;
        }

        if(!(3<Eh && Eh<40))
        {
          // cout << "Eh not good : " << Eh << endl;
          continue;
        }

        if(!(0.2<z && z<0.85))
        {
          // cout << "z not good : " << z << endl;
          continue;
        }

        if(0.2<z && z<0.25) zbin = 0;
        else if(0.25<=z && z<0.30) zbin = 1;
        else if(0.30<=z && z<0.35) zbin = 2;
        else if(0.35<=z && z<0.40) zbin = 3;
        else if(0.40<=z && z<0.45) zbin = 4;
        else if(0.45<=z && z<0.50) zbin = 5;
        else if(0.50<=z && z<0.55) zbin = 6;
        else if(0.55<=z && z<0.60) zbin = 7;
        else if(0.60<=z && z<0.65) zbin = 8;
        else if(0.65<=z && z<0.70) zbin = 9;
        else if(0.70<=z && z<0.75) zbin = 10;
        else zbin = 11;

        if(0.00<pt && pt<0.15) ptbin = 0;
        else if(0.15<pt && pt<0.30) ptbin = 1;
        else if(0.30<pt && pt<0.45) ptbin = 2;
        else if(0.45<pt && pt<0.60) ptbin = 3;
        else if(0.60<pt && pt<0.75) ptbin = 4;
        else if(0.75<pt && pt<0.90) ptbin = 5;
        else if(0.90<pt && pt<1.05) ptbin = 6;
        else if(1.05<pt && pt<1.20) ptbin = 7;
        else if(1.20<pt && pt<1.35) ptbin = 8;
        else if(1.35<pt && pt<1.50) ptbin = 9;
        else if(1.50<pt && pt<1.65) ptbin = 10;
        else if(1.65<pt && pt<1.80) ptbin = 11;
        else if(1.80<pt && pt<1.95) ptbin = 12;
        else if(1.95<pt && pt<2.10) ptbin = 13;
        else if(2.10<pt && pt<2.25) ptbin = 14;
        else if(2.25<pt && pt<2.40) ptbin = 15;
        else if(2.40<pt && pt<2.55) ptbin = 16;
        else if(2.55<pt && pt<2.70) ptbin = 17;
        else if(2.70<pt && pt<2.85) ptbin = 18;
        else if(2.85<pt && pt<3.00) ptbin = 19;
        if(pt>6) cout << "large pt : " << pt << " > 6.0." << endl;

        if(fId==0)
        {
          if(fFlag_r[0][xbin][ybin][zbin]) continue;
          fRe[xbin][ybin][zbin].tab[1][0][0] += 1;
          fRe[xbin][ybin][zbin].tab[1][0][3] += 1;
          fRept[xbin][ybin][ptbin].tab[1][0][0] += 1;
          fRept[xbin][ybin][ptbin].tab[1][0][3] += 1;
          fNSIDIS_evt_r[0][xbin][ybin]++;
          fNSIDIS_evt_r[3][xbin][ybin]++;
        }
        else if(fId==1)
        {
          if(fFlag_r[0][xbin][ybin][zbin]) continue;
          fRe[xbin][ybin][zbin].tab[0][0][0] += 1;
          fRe[xbin][ybin][zbin].tab[0][0][3] += 1;
          fRept[xbin][ybin][ptbin].tab[0][0][0] += 1;
          fRept[xbin][ybin][ptbin].tab[0][0][3] += 1;
          fNSIDIS_evt_r[0][xbin][ybin]++;
          fNSIDIS_evt_r[3][xbin][ybin]++;
        }
        else if(fId==2)
        {
          if(fFlag_r[1][xbin][ybin][zbin]) continue;
          fRe[xbin][ybin][zbin].tab[1][0][1] += 1;
          fRe[xbin][ybin][zbin].tab[1][0][3] += 1;
          fRept[xbin][ybin][ptbin].tab[1][0][1] += 1;
          fRept[xbin][ybin][ptbin].tab[1][0][3] += 1;
          fNSIDIS_evt_r[1][xbin][ybin]++;
          fNSIDIS_evt_r[3][xbin][ybin]++;
        }
        else if(fId==3)
        {
          if(fFlag_r[1][xbin][ybin][zbin]) continue;
          fRe[xbin][ybin][zbin].tab[0][0][1] += 1;
          fRe[xbin][ybin][zbin].tab[0][0][3] += 1;
          fRept[xbin][ybin][ptbin].tab[0][0][1] += 1;
          fRept[xbin][ybin][ptbin].tab[0][0][3] += 1;
          fNSIDIS_evt_r[1][xbin][ybin]++;
          fNSIDIS_evt_r[3][xbin][ybin]++;
        }
        else if(fId==4)
        {
          if(fFlag_r[2][xbin][ybin][zbin]) continue;
          fRe[xbin][ybin][zbin].tab[1][0][2] += 1;
          fRe[xbin][ybin][zbin].tab[1][0][3] += 1;
          fRept[xbin][ybin][ptbin].tab[1][0][2] += 1;
          fRept[xbin][ybin][ptbin].tab[1][0][3] += 1;
          fNSIDIS_evt_r[2][xbin][ybin]++;
          fNSIDIS_evt_r[3][xbin][ybin]++;
        }
        else if(fId==5)
        {
          if(fFlag_r[2][xbin][ybin][zbin]) continue;
          fRe[xbin][ybin][zbin].tab[0][0][2] += 1;
          fRe[xbin][ybin][zbin].tab[0][0][3] += 1;
          fRept[xbin][ybin][ptbin].tab[0][0][2] += 1;
          fRept[xbin][ybin][ptbin].tab[0][0][3] += 1;
          fNSIDIS_evt_r[2][xbin][ybin]++;
          fNSIDIS_evt_r[3][xbin][ybin]++;
        }
        else if(fId==6)
        {
          if(fFlag_r[0][xbin][ybin][zbin]) continue;
          fRe[xbin][ybin][zbin].tab[1][0][3] += 1;
          fRept[xbin][ybin][ptbin].tab[1][0][3] += 1;
          fNSIDIS_evt_r[3][xbin][ybin]++;
        }
        else if(fId==7)
        {
          if(fFlag_r[0][xbin][ybin][zbin]) continue;
          fRe[xbin][ybin][zbin].tab[0][0][3] += 1;
          fRept[xbin][ybin][ptbin].tab[0][0][3] += 1;
          fNSIDIS_evt_r[3][xbin][ybin]++;
        }
        else
        {
          continue;
        }
      }
    }

    revt.close();

    ifstream bevt(FileBorn);

    while(bevt >> npart)
    {
      fDIScuts = 0;
//      cout << npart << endl;
      bevt >> E >> xbj >> y >> Q2;
      W = pow(fM_p,2) + Q2*(1-xbj)/xbj;
//      cout << E << "\txbj : " << xbj << "\ty : " << y << "\tQ2 : " << Q2 << endl;
//      cout << "Particles list" << endl;
      if(0<E)
      {
        if(Q2>1)
        {
          if(.004<xbj && xbj<.4)
          {
            if(.1<y && y<.7)
            {
              if(5<sqrt(W) && sqrt(W)<17)
              {
                fDIScuts = 1;
              }
            }
          }
        }
      }

      if(0.004<xbj && xbj<0.01) xbin = 0;
      else if(0.01<=xbj && xbj<0.02) xbin = 1;
      else if(0.02<=xbj && xbj<0.03) xbin = 2;
      else if(0.03<=xbj && xbj<0.04) xbin = 3;
      else if(0.04<=xbj && xbj<0.06) xbin = 4;
      else if(0.06<=xbj && xbj<0.1) xbin = 5;
      else if(0.1<=xbj && xbj<0.14) xbin = 6;
      else if(0.14<=xbj && xbj<0.18) xbin = 7;
      else xbin = 8;

      if(0.1<y && y<0.15) ybin = 0;
      else if(0.15<=y && y<0.2) ybin = 1;
      else if(0.2<=y && y<0.3) ybin = 2;
      else if(0.3<=y && y<0.5) ybin = 3;
      else ybin = 4;

      if(fDIScuts)
      {
        for(int i=0; i<12; i++)
        {
          fNDIS_evt_b[0][xbin][ybin][i]++;
          fNDIS_evt_b[1][xbin][ybin][i]++;
          fNDIS_evt_b[2][xbin][ybin][i]++;

          fFlag_b[0][xbin][ybin][i]=0;
          fFlag_b[1][xbin][ybin][i]=0;
          fFlag_b[2][xbin][ybin][i]=0;
        }
      }

      for(int i=0; i<npart; i++)
      {
        bevt >> dummy >> id >> dummy >> dummy >> dummy;
//        cout << dummy << "\tid : " << id << "\t" << dummy << "\t" << dummy << "\t" << dummy << "\t";
        bevt >> px >> py >> pz >> Eh;
//        cout << px << "\t" << py << "\t" << pz << "\t" << Eh << "\t";
        bevt >> dummy >> dummy >> dummy >> dummy;
//        cout << dummy << "\t" << dummy << "\t" << dummy << "\t" << dummy << endl;

        if(!fDIScuts) continue;

        if(abs(id)==13)
        {
          E_prime = Eh;
          nu = E - E_prime;

          // nu cut
          for(int j=0; j<12; j++)
          {
            if(!(fNu_min[0][j]<nu && nu<fNu_max[0][j]))
            {
              fFlag_b[0][xbin][ybin][j]=1;
            }
            if(!(fNu_min[1][j]<nu && nu<fNu_max[1][j]))
            {
              fFlag_b[1][xbin][ybin][j]=1;
            }
            if(!(fNu_min[2][j]<nu && nu<fNu_max[2][j]))
            {
              fFlag_b[2][xbin][ybin][j]=1;
            }
            if(fFlag_b[0][xbin][ybin][j] || fFlag_b[1][xbin][ybin][j] || fFlag_b[2][xbin][ybin][j])
            {
              fNDIS_evt_b[0][xbin][ybin][j]--; fNDIS_evt_b[1][xbin][ybin][j]--; fNDIS_evt_b[2][xbin][ybin][j]--;
            }
          }
        }
        else if(id == 211)//pi+
        {
          fId = 0;
        }
        else if(id == -211)//pi-
        {
          fId = 1;
        }
        else if(id == 321)//K+
        {
          fId = 2;
        }
        else if(id == -321)//K-
        {
          fId = 3;
        }
        else if(id == 2212)//p
        {
          fId = 4;
        }
        else if(id == -2212)//pb
        {
          fId = 5;
        }
        else if(id != 22 && abs(id) != 13)//Hadron
        {
          if(id>0)
          {
            fId = 6;
          }
          else
          {
            fId = 7;
          }
        }

        if(abs(id) == 13 || id == 22) continue;

        pt = sqrt(pow(px,2)+pow(py,2));

        if(nu)
        {
          if(fId == 2 || fId == 3)
            z = sqrt(pow(Eh,2)+pow(fM_K,2))/nu;
          else if(fId == 4 || fId == 5)
            z = sqrt(pow(Eh,2)+pow(fM_p,2))/nu;
          else
            z = sqrt(pow(Eh,2)+pow(fM_pi,2))/nu;
        }
        else
        {
          z = 0;
        }

        if(!(3<Eh && Eh<40))
        {
          // cout << "Eh not good : " << Eh << endl;
          continue;
        }

        if(!(0.2<z && z<0.85))
        {
          // cout << "z not good : " << z << endl;
          continue;
        }

        if(0.2<z && z<0.25) zbin = 0;
        else if(0.25<=z && z<0.30) zbin = 1;
        else if(0.30<=z && z<0.35) zbin = 2;
        else if(0.35<=z && z<0.40) zbin = 3;
        else if(0.40<=z && z<0.45) zbin = 4;
        else if(0.45<=z && z<0.50) zbin = 5;
        else if(0.50<=z && z<0.55) zbin = 6;
        else if(0.55<=z && z<0.60) zbin = 7;
        else if(0.60<=z && z<0.65) zbin = 8;
        else if(0.65<=z && z<0.70) zbin = 9;
        else if(0.70<=z && z<0.75) zbin = 10;
        else zbin = 11;

        if(0.00<pt && pt<0.15) ptbin = 0;
        else if(0.15<pt && pt<0.30) ptbin = 1;
        else if(0.30<pt && pt<0.45) ptbin = 2;
        else if(0.45<pt && pt<0.60) ptbin = 3;
        else if(0.60<pt && pt<0.75) ptbin = 4;
        else if(0.75<pt && pt<0.90) ptbin = 5;
        else if(0.90<pt && pt<1.05) ptbin = 6;
        else if(1.05<pt && pt<1.20) ptbin = 7;
        else if(1.20<pt && pt<1.35) ptbin = 8;
        else if(1.35<pt && pt<1.50) ptbin = 9;
        else if(1.50<pt && pt<1.65) ptbin = 10;
        else if(1.65<pt && pt<1.80) ptbin = 11;
        else if(1.80<pt && pt<1.95) ptbin = 12;
        else if(1.95<pt && pt<2.10) ptbin = 13;
        else if(2.10<pt && pt<2.25) ptbin = 14;
        else if(2.25<pt && pt<2.40) ptbin = 15;
        else if(2.40<pt && pt<2.55) ptbin = 16;
        else if(2.55<pt && pt<2.70) ptbin = 17;
        else if(2.70<pt && pt<2.85) ptbin = 18;
        else if(2.85<pt && pt<3.00) ptbin = 19;
        if(pt>6) cout << "large pt : " << pt << " > 6.0." << endl;

        if(fId==0)
        {
          if(fFlag_b[0][xbin][ybin][zbin]) continue;
          fBorn[xbin][ybin][zbin].tab[1][0][0] += 1;
          fBorn[xbin][ybin][zbin].tab[1][0][3] += 1;
          fBornpt[xbin][ybin][ptbin].tab[1][0][0] += 1;
          fBornpt[xbin][ybin][ptbin].tab[1][0][3] += 1;
          fNSIDIS_evt_b[0][xbin][ybin]++;
          fNSIDIS_evt_b[3][xbin][ybin]++;
        }
        else if(fId==1)
        {
          if(fFlag_b[0][xbin][ybin][zbin]) continue;
          fBorn[xbin][ybin][zbin].tab[0][0][0] += 1;
          fBorn[xbin][ybin][zbin].tab[0][0][3] += 1;
          fBornpt[xbin][ybin][ptbin].tab[0][0][0] += 1;
          fBornpt[xbin][ybin][ptbin].tab[0][0][3] += 1;
          fNSIDIS_evt_b[0][xbin][ybin]++;
          fNSIDIS_evt_b[3][xbin][ybin]++;
        }
        else if(fId==2)
        {
          if(fFlag_b[1][xbin][ybin][zbin]) continue;
          fBorn[xbin][ybin][zbin].tab[1][0][1] += 1;
          fBorn[xbin][ybin][zbin].tab[1][0][3] += 1;
          fBornpt[xbin][ybin][ptbin].tab[1][0][1] += 1;
          fBornpt[xbin][ybin][ptbin].tab[1][0][3] += 1;
          fNSIDIS_evt_b[1][xbin][ybin]++;
          fNSIDIS_evt_b[3][xbin][ybin]++;
        }
        else if(fId==3)
        {
          if(fFlag_b[1][xbin][ybin][zbin]) continue;
          fBorn[xbin][ybin][zbin].tab[0][0][1] += 1;
          fBorn[xbin][ybin][zbin].tab[0][0][3] += 1;
          fBornpt[xbin][ybin][ptbin].tab[0][0][1] += 1;
          fBornpt[xbin][ybin][ptbin].tab[0][0][3] += 1;
          fNSIDIS_evt_b[1][xbin][ybin]++;
          fNSIDIS_evt_b[3][xbin][ybin]++;
        }
        else if(fId==4)
        {
          if(fFlag_b[2][xbin][ybin][zbin]) continue;
          fBorn[xbin][ybin][zbin].tab[1][0][2] += 1;
          fBorn[xbin][ybin][zbin].tab[1][0][3] += 1;
          fBornpt[xbin][ybin][ptbin].tab[1][0][2] += 1;
          fBornpt[xbin][ybin][ptbin].tab[1][0][3] += 1;
          fNSIDIS_evt_b[2][xbin][ybin]++;
          fNSIDIS_evt_b[3][xbin][ybin]++;
        }
        else if(fId==5)
        {
          if(fFlag_b[2][xbin][ybin][zbin]) continue;
          fBorn[xbin][ybin][zbin].tab[0][0][2] += 1;
          fBorn[xbin][ybin][zbin].tab[0][0][3] += 1;
          fBornpt[xbin][ybin][ptbin].tab[0][0][2] += 1;
          fBornpt[xbin][ybin][ptbin].tab[0][0][3] += 1;
          fNSIDIS_evt_b[2][xbin][ybin]++;
          fNSIDIS_evt_b[3][xbin][ybin]++;
        }
        else if(fId==6)
        {
          if(fFlag_b[0][xbin][ybin][zbin]) continue;
          fBorn[xbin][ybin][zbin].tab[1][0][3] += 1;
          fBornpt[xbin][ybin][ptbin].tab[1][0][3] += 1;
          fNSIDIS_evt_b[3][xbin][ybin]++;
        }
        else if(fId==7)
        {
          if(fFlag_b[0][xbin][ybin][zbin]) continue;
          fBorn[xbin][ybin][zbin].tab[0][0][3] += 1;
          fBornpt[xbin][ybin][ptbin].tab[0][0][3] += 1;
          fNSIDIS_evt_b[3][xbin][ybin]++;
        }
        else
        {
          continue;
        }
      }
    }
  }

  c1.SetFillColor(0);
  c2.SetFillColor(0);
  c3.SetFillColor(0);
  c4.SetFillColor(0);
  c5.SetFillColor(0);
  c6.SetFillColor(0);

  c1.Divide(9,5,0,0);
  c2.Divide(9,5,0,0);
  c3.Divide(9,5,0,0);
  c4.Divide(9,5,0,0);
  c5.Divide(9,5,0,0);
  c6.Divide(9,5,0,0);

  double z_range[12] = {.225,.275,.325,.375,.425,.475,.525,.575,.625,.675,.725,.8};
  double pt_range[20] = {0.075,0.225,0.375,0.525,0.675,
                      0.825,0.975,1.125,1.275,1.425,
                      1.575,1.725,1.875,2.025,2.175,
                      2.325,2.475,2.625,2.775,2.925};

  for(int c=0; c<2; c++)
  {
    for(int i=0; i<9; i++)
    {
      for(int j=0; j<5; j++)
      {

        std::vector<double> p_a_r;
        std::vector<double> k_a_r;
        std::vector<double> h_a_r;
        std::vector<double> p_err_r;
        std::vector<double> k_err_r;
        std::vector<double> h_err_r;

        std::vector<double> p_a_b;
        std::vector<double> k_a_b;
        std::vector<double> h_a_b;
        std::vector<double> p_err_b;
        std::vector<double> k_err_b;
        std::vector<double> h_err_b;

        std::vector<double> z_range_p_r;
        std::vector<double> z_range_k_r;
        std::vector<double> z_range_h_r;

        std::vector<double> z_range_p_b;
        std::vector<double> z_range_k_b;
        std::vector<double> z_range_h_b;

        std::vector<double> p_pt_r;
        std::vector<double> k_pt_r;
        std::vector<double> h_pt_r;
        std::vector<double> p_pt_err_r;
        std::vector<double> k_pt_err_r;
        std::vector<double> h_pt_err_r;

        std::vector<double> p_pt_b;
        std::vector<double> k_pt_b;
        std::vector<double> h_pt_b;
        std::vector<double> p_pt_err_b;
        std::vector<double> k_pt_err_b;
        std::vector<double> h_pt_err_b;

        std::vector<double> pt_range_p_r;
        std::vector<double> pt_range_k_r;
        std::vector<double> pt_range_h_r;

        std::vector<double> pt_range_p_b;
        std::vector<double> pt_range_k_b;
        std::vector<double> pt_range_h_b;

        for(int l=0; l<12; l++)
        {
          z_range_p_r.push_back(z_range[l]);
          z_range_k_r.push_back(z_range[l]);
          z_range_h_r.push_back(z_range[l]);
          z_range_p_b.push_back(z_range[l]);
          z_range_k_b.push_back(z_range[l]);
          z_range_h_b.push_back(z_range[l]);
        }
        for(int l=0; l<20; l++)
        {
          pt_range_p_r.push_back(pt_range[l]);
          pt_range_k_r.push_back(pt_range[l]);
          pt_range_h_r.push_back(pt_range[l]);
          pt_range_p_b.push_back(pt_range[l]);
          pt_range_k_b.push_back(pt_range[l]);
          pt_range_h_b.push_back(pt_range[l]);
        }

        // cout << fRe[i][j][0].tab[0][0][3]+fRe[i][j][1].tab[0][0][3]+fRe[i][j][2].tab[0][0][3]+fRe[i][j][3].tab[0][0][3]
        // +fRe[i][j][4].tab[0][0][3]+fRe[i][j][5].tab[0][0][3]+fRe[i][j][6].tab[0][0][3]+fRe[i][j][7].tab[0][0][3]
        // +fRe[i][j][8].tab[0][0][3]+fRe[i][j][9].tab[0][0][3]+fRe[i][j][10].tab[0][0][3]+fRe[i][j][11].tab[0][0][3]
        // +fRe[i][j][0].tab[1][0][3]+fRe[i][j][1].tab[1][0][3]+fRe[i][j][2].tab[1][0][3]+fRe[i][j][3].tab[1][0][3]
        // +fRe[i][j][4].tab[1][0][3]+fRe[i][j][5].tab[1][0][3]+fRe[i][j][6].tab[1][0][3]+fRe[i][j][7].tab[1][0][3]
        // +fRe[i][j][8].tab[1][0][3]+fRe[i][j][9].tab[1][0][3]+fRe[i][j][10].tab[1][0][3]+fRe[i][j][11].tab[1][0][3]
        // <<
        // " " << fNSIDIS_evt_r[3][i][j] << endl;
        //
        // cout << fBorn[i][j][0].tab[0][0][3]+fBorn[i][j][1].tab[0][0][3]+fBorn[i][j][2].tab[0][0][3]+fBorn[i][j][3].tab[0][0][3]
        // +fBorn[i][j][4].tab[0][0][3]+fBorn[i][j][5].tab[0][0][3]+fBorn[i][j][6].tab[0][0][3]+fBorn[i][j][7].tab[0][0][3]
        // +fBorn[i][j][8].tab[0][0][3]+fBorn[i][j][9].tab[0][0][3]+fBorn[i][j][10].tab[0][0][3]+fBorn[i][j][11].tab[0][0][3]
        // +fBorn[i][j][0].tab[1][0][3]+fBorn[i][j][1].tab[1][0][3]+fBorn[i][j][2].tab[1][0][3]+fBorn[i][j][3].tab[1][0][3]
        // +fBorn[i][j][4].tab[1][0][3]+fBorn[i][j][5].tab[1][0][3]+fBorn[i][j][6].tab[1][0][3]+fBorn[i][j][7].tab[1][0][3]
        // +fBorn[i][j][8].tab[1][0][3]+fBorn[i][j][9].tab[1][0][3]+fBorn[i][j][10].tab[1][0][3]+fBorn[i][j][11].tab[1][0][3]
        // <<
        // " " << fNSIDIS_evt_b[3][i][j] << endl;

        for(int k=0; k<12; k++)
        {
          cout << "x,y,z : " << i << "," << j << "," << k << "\nN DIS evt r : " << fNDIS_evt_r[0][i][j][k] << " N SIDIS evt r : " << fNSIDIS_evt_r[3][i][j] << " N re : " << fRe[i][j][k].tab[c][0][3];
          fReMult[i][j][k].tab[c][0][0] = ((fNDIS_evt_r[0][i][j][k]) ? double(fRe[i][j][k].tab[c][0][0])/double(fNDIS_evt_r[0][i][j][k]) : 0);
          fReMult[i][j][k].tab[c][0][1] = ((fNDIS_evt_r[0][i][j][k]) ? double(fRe[i][j][k].tab[c][0][1])/double(fNDIS_evt_r[0][i][j][k]) : 0);
          fReMult[i][j][k].tab[c][0][2] = ((fNDIS_evt_r[0][i][j][k]) ? double(fRe[i][j][k].tab[c][0][2])/double(fNDIS_evt_r[0][i][j][k]) : 0);
          fReMult[i][j][k].tab[c][0][3] = ((fNDIS_evt_r[0][i][j][k]) ? double(fRe[i][j][k].tab[c][0][3])/double(fNDIS_evt_r[0][i][j][k]) : 0);
          cout << " Hm : " << fReMult[i][j][k].tab[c][0][3] << endl;
          cout << "N DIS evt B : " << fNDIS_evt_b[0][i][j][k] << " N SIDIS evt r : " << fNSIDIS_evt_b[3][i][j] << " N born : " << fBorn[i][j][k].tab[c][0][3];
          fBornMult[i][j][k].tab[c][0][0] = ((fNDIS_evt_b[0][i][j][k]) ? double(fBorn[i][j][k].tab[c][0][0])/double(fNDIS_evt_b[0][i][j][k]) : 0);
          fBornMult[i][j][k].tab[c][0][1] = ((fNDIS_evt_b[0][i][j][k]) ? double(fBorn[i][j][k].tab[c][0][1])/double(fNDIS_evt_b[0][i][j][k]) : 0);
          fBornMult[i][j][k].tab[c][0][2] = ((fNDIS_evt_b[0][i][j][k]) ? double(fBorn[i][j][k].tab[c][0][2])/double(fNDIS_evt_b[0][i][j][k]) : 0);
          fBornMult[i][j][k].tab[c][0][3] = ((fNDIS_evt_b[0][i][j][k]) ? double(fBorn[i][j][k].tab[c][0][3])/double(fNDIS_evt_b[0][i][j][k]) : 0);
          cout << " Hm : " << fBornMult[i][j][k].tab[c][0][3] << endl;

          // fBornMult[i][j][k].tab[c][0][0] /= fReMult[i][j][k].tab[c][0][0];
          // fBornMult[i][j][k].tab[c][0][1] /= fReMult[i][j][k].tab[c][0][1];
          // fBornMult[i][j][k].tab[c][0][2] /= fReMult[i][j][k].tab[c][0][2];
          // fBornMult[i][j][k].tab[c][0][3] /= fReMult[i][j][k].tab[c][0][3];
          //
          // fReMult[i][j][k].tab[c][0][0] /= fReMult[i][j][k].tab[c][0][0];
          // fReMult[i][j][k].tab[c][0][1] /= fReMult[i][j][k].tab[c][0][1];
          // fReMult[i][j][k].tab[c][0][2] /= fReMult[i][j][k].tab[c][0][2];
          // fReMult[i][j][k].tab[c][0][3] /= fReMult[i][j][k].tab[c][0][3];

          if(fReMult[i][j][k].tab[c][0][0]<0) fReMult[i][j][k].tab[c][0][0]=0;
          if(fReMult[i][j][k].tab[c][0][1]<0) fReMult[i][j][k].tab[c][0][1]=0;
          if(fReMult[i][j][k].tab[c][0][2]<0) fReMult[i][j][k].tab[c][0][2]=0;
          if(fReMult[i][j][k].tab[c][0][3]<0) fReMult[i][j][k].tab[c][0][3]=0;

          if(fBornMult[i][j][k].tab[c][0][0]<0) fBornMult[i][j][k].tab[c][0][0]=0;
          if(fBornMult[i][j][k].tab[c][0][1]<0) fBornMult[i][j][k].tab[c][0][1]=0;
          if(fBornMult[i][j][k].tab[c][0][2]<0) fBornMult[i][j][k].tab[c][0][2]=0;
          if(fBornMult[i][j][k].tab[c][0][3]<0) fBornMult[i][j][k].tab[c][0][3]=0;

          if(fReMult[i][j][k].tab[c][0][0]==0) fReMult[i][j][k].tab[c][1][0]=0;
          else fReMult[i][j][k].tab[c][1][0] = sqrt(pow(1/fRe[i][j][k].tab[c][0][0],2)+pow(1/fNDIS_evt_r[0][i][j][k],2));
          if(fReMult[i][j][k].tab[c][0][1]==0) fReMult[i][j][k].tab[c][1][1]=0;
          else fReMult[i][j][k].tab[c][1][1] = sqrt(pow(1/fRe[i][j][k].tab[c][0][1],2)+pow(1/fNDIS_evt_r[0][i][j][k],2));
          if(fReMult[i][j][k].tab[c][0][2]==0) fReMult[i][j][k].tab[c][1][2]=0;
          else fReMult[i][j][k].tab[c][1][2] = sqrt(pow(1/fRe[i][j][k].tab[c][0][2],2)+pow(1/fNDIS_evt_r[0][i][j][k],2));
          if(fReMult[i][j][k].tab[c][0][3]==0) fReMult[i][j][k].tab[c][1][3]=0;
          else fReMult[i][j][k].tab[c][1][3] = sqrt(pow(1/fRe[i][j][k].tab[c][0][3],2)+pow(1/fNDIS_evt_r[0][i][j][k],2));

          if(fBornMult[i][j][k].tab[c][0][0]==0) fBornMult[i][j][k].tab[c][1][0]=0;
          else fBornMult[i][j][k].tab[c][1][0] = sqrt(pow(1/fBorn[i][j][k].tab[c][0][0],2)+pow(1/fNDIS_evt_r[0][i][j][k],2));
          if(fBornMult[i][j][k].tab[c][0][1]==0) fBornMult[i][j][k].tab[c][1][1]=0;
          else fBornMult[i][j][k].tab[c][1][1] = sqrt(pow(1/fBorn[i][j][k].tab[c][0][1],2)+pow(1/fNDIS_evt_r[0][i][j][k],2));
          if(fBornMult[i][j][k].tab[c][0][2]==0) fBornMult[i][j][k].tab[c][1][2]=0;
          else fBornMult[i][j][k].tab[c][1][2] = sqrt(pow(1/fBorn[i][j][k].tab[c][0][2],2)+pow(1/fNDIS_evt_r[0][i][j][k],2));
          if(fBornMult[i][j][k].tab[c][0][3]==0) fBornMult[i][j][k].tab[c][1][3]=0;
          else fBornMult[i][j][k].tab[c][1][3] = sqrt(pow(1/fBorn[i][j][k].tab[c][0][3],2)+pow(1/fNDIS_evt_r[0][i][j][k],2));

          if((i==7 && j==4) || (i==8 && j==0) || (i==8 && j==4))
          {
            fReMult[i][j][k].tab[c][0][0] = 0;
            fReMult[i][j][k].tab[c][0][1] = 0;
            fReMult[i][j][k].tab[c][0][2] = 0;
            fReMult[i][j][k].tab[c][0][3] = 0;
            fReMult[i][j][k].tab[c][1][0] = 0;
            fReMult[i][j][k].tab[c][1][1] = 0;
            fReMult[i][j][k].tab[c][1][2] = 0;
            fReMult[i][j][k].tab[c][1][3] = 0;

            fBornMult[i][j][k].tab[c][0][0] = 0;
            fBornMult[i][j][k].tab[c][0][1] = 0;
            fBornMult[i][j][k].tab[c][0][2] = 0;
            fBornMult[i][j][k].tab[c][0][3] = 0;
            fBornMult[i][j][k].tab[c][1][0] = 0;
            fBornMult[i][j][k].tab[c][1][1] = 0;
            fBornMult[i][j][k].tab[c][1][2] = 0;
            fBornMult[i][j][k].tab[c][1][3] = 0;
          }

          p_a_r.push_back(fReMult[i][j][k].tab[c][0][0]);
          k_a_r.push_back(fReMult[i][j][k].tab[c][0][1]);
          h_a_r.push_back(fReMult[i][j][k].tab[c][0][3]);
          p_a_b.push_back(fBornMult[i][j][k].tab[c][0][0]);
          k_a_b.push_back(fBornMult[i][j][k].tab[c][0][1]);
          h_a_b.push_back(fBornMult[i][j][k].tab[c][0][3]);

          p_err_r.push_back(fReMult[i][j][k].tab[c][1][0]);
          k_err_r.push_back(fReMult[i][j][k].tab[c][1][1]);
          h_err_r.push_back(fReMult[i][j][k].tab[c][1][3]);
          p_err_b.push_back(fBornMult[i][j][k].tab[c][1][0]);
          k_err_b.push_back(fBornMult[i][j][k].tab[c][1][1]);
          h_err_b.push_back(fBornMult[i][j][k].tab[c][1][3]);

        }

        for(int k=0; k<20; k++)
        {

          int max_el = *max_element(fNDIS_evt_r[0][i][j],fNDIS_evt_r[0][i][j]+12);

          fReptMult[i][j][k].tab[c][0][0] = ((max_el) ? double(fRept[i][j][k].tab[c][0][0])/double(max_el) : 0);
          fReptMult[i][j][k].tab[c][0][1] = ((max_el) ? double(fRept[i][j][k].tab[c][0][1])/double(max_el) : 0);
          fReptMult[i][j][k].tab[c][0][2] = ((max_el) ? double(fRept[i][j][k].tab[c][0][2])/double(max_el) : 0);
          fReptMult[i][j][k].tab[c][0][3] = ((max_el) ? double(fRept[i][j][k].tab[c][0][3])/double(max_el) : 0);

          fBornptMult[i][j][k].tab[c][0][0] = ((max_el) ? double(fBornpt[i][j][k].tab[c][0][0])/double(max_el) : 0);
          fBornptMult[i][j][k].tab[c][0][1] = ((max_el) ? double(fBornpt[i][j][k].tab[c][0][1])/double(max_el) : 0);
          fBornptMult[i][j][k].tab[c][0][2] = ((max_el) ? double(fBornpt[i][j][k].tab[c][0][2])/double(max_el) : 0);
          fBornptMult[i][j][k].tab[c][0][3] = ((max_el) ? double(fBornpt[i][j][k].tab[c][0][3])/double(max_el) : 0);

          if(fReptMult[i][j][k].tab[c][0][0]<0) fReptMult[i][j][k].tab[c][0][0]=0;
          if(fReptMult[i][j][k].tab[c][0][1]<0) fReptMult[i][j][k].tab[c][0][1]=0;
          if(fReptMult[i][j][k].tab[c][0][2]<0) fReptMult[i][j][k].tab[c][0][2]=0;
          if(fReptMult[i][j][k].tab[c][0][3]<0) fReptMult[i][j][k].tab[c][0][3]=0;

          if(fBornptMult[i][j][k].tab[c][0][0]<0) fBornptMult[i][j][k].tab[c][0][0]=0;
          if(fBornptMult[i][j][k].tab[c][0][1]<0) fBornptMult[i][j][k].tab[c][0][1]=0;
          if(fBornptMult[i][j][k].tab[c][0][2]<0) fBornptMult[i][j][k].tab[c][0][2]=0;
          if(fBornptMult[i][j][k].tab[c][0][3]<0) fBornptMult[i][j][k].tab[c][0][3]=0;

          if(fReptMult[i][j][k].tab[c][0][0]==0) fReptMult[i][j][k].tab[c][1][0]=0;
          else fReptMult[i][j][k].tab[c][1][0] = sqrt(pow(1/fRept[i][j][k].tab[c][0][0],2)+pow(1/max_el,2));
          if(fReptMult[i][j][k].tab[c][0][1]==0) fReptMult[i][j][k].tab[c][1][1]=0;
          else fReptMult[i][j][k].tab[c][1][1] = sqrt(pow(1/fRept[i][j][k].tab[c][0][1],2)+pow(1/max_el,2));
          if(fReptMult[i][j][k].tab[c][0][2]==0) fReptMult[i][j][k].tab[c][1][2]=0;
          else fReptMult[i][j][k].tab[c][1][2] = sqrt(pow(1/fRept[i][j][k].tab[c][0][2],2)+pow(1/max_el,2));
          if(fReptMult[i][j][k].tab[c][0][3]==0) fReptMult[i][j][k].tab[c][1][3]=0;
          else fReptMult[i][j][k].tab[c][1][3] = sqrt(pow(1/fRept[i][j][k].tab[c][0][3],2)+pow(1/max_el,2));

          if(fBornptMult[i][j][k].tab[c][0][0]==0) fBornptMult[i][j][k].tab[c][1][0]=0;
          else fBornptMult[i][j][k].tab[c][1][0] = sqrt(pow(1/fBornpt[i][j][k].tab[c][0][0],2)+pow(1/max_el,2));
          if(fBornptMult[i][j][k].tab[c][0][1]==0) fBornptMult[i][j][k].tab[c][1][1]=0;
          else fBornptMult[i][j][k].tab[c][1][1] = sqrt(pow(1/fBornpt[i][j][k].tab[c][0][1],2)+pow(1/max_el,2));
          if(fBornptMult[i][j][k].tab[c][0][2]==0) fBornptMult[i][j][k].tab[c][1][2]=0;
          else fBornptMult[i][j][k].tab[c][1][2] = sqrt(pow(1/fBornpt[i][j][k].tab[c][0][2],2)+pow(1/max_el,2));
          if(fBornptMult[i][j][k].tab[c][0][3]==0) fBornptMult[i][j][k].tab[c][1][3]=0;
          else fBornptMult[i][j][k].tab[c][1][3] = sqrt(pow(1/fBornpt[i][j][k].tab[c][0][3],2)+pow(1/max_el,2));


          if((i==7 && j==4) || (i==8 && j==0) || (i==8 && j==4))
          {
            fReptMult[i][j][k].tab[c][0][0] = 0;
            fReptMult[i][j][k].tab[c][0][1] = 0;
            fReptMult[i][j][k].tab[c][0][2] = 0;
            fReptMult[i][j][k].tab[c][0][3] = 0;
            fReptMult[i][j][k].tab[c][1][0] = 0;
            fReptMult[i][j][k].tab[c][1][1] = 0;
            fReptMult[i][j][k].tab[c][1][2] = 0;
            fReptMult[i][j][k].tab[c][1][3] = 0;

            fBornptMult[i][j][k].tab[c][0][0] = 0;
            fBornptMult[i][j][k].tab[c][0][1] = 0;
            fBornptMult[i][j][k].tab[c][0][2] = 0;
            fBornptMult[i][j][k].tab[c][0][3] = 0;
            fBornptMult[i][j][k].tab[c][1][0] = 0;
            fBornptMult[i][j][k].tab[c][1][1] = 0;
            fBornptMult[i][j][k].tab[c][1][2] = 0;
            fBornptMult[i][j][k].tab[c][1][3] = 0;
          }

          p_pt_r.push_back(fReptMult[i][j][k].tab[c][0][0]);
          k_pt_r.push_back(fReptMult[i][j][k].tab[c][0][1]);
          h_pt_r.push_back(fReptMult[i][j][k].tab[c][0][3]);
          p_pt_b.push_back(fBornptMult[i][j][k].tab[c][0][0]);
          k_pt_b.push_back(fBornptMult[i][j][k].tab[c][0][1]);
          h_pt_b.push_back(fBornptMult[i][j][k].tab[c][0][3]);

          p_pt_err_r.push_back(fReptMult[i][j][k].tab[c][1][0]);
          k_pt_err_r.push_back(fReptMult[i][j][k].tab[c][1][1]);
          h_pt_err_r.push_back(fReptMult[i][j][k].tab[c][1][3]);
          p_pt_err_b.push_back(fBornptMult[i][j][k].tab[c][1][0]);
          k_pt_err_b.push_back(fBornptMult[i][j][k].tab[c][1][1]);
          h_pt_err_b.push_back(fBornptMult[i][j][k].tab[c][1][3]);

        }

        for(int k=12; k>0; k--)
        {
          if(!p_a_r[k-1]) {p_a_r.erase(p_a_r.begin()+k-1); p_err_r.erase(p_err_r.begin()+k-1); z_range_p_r.erase(z_range_p_r.begin()+k-1);}
          if(!p_a_b[k-1]) {p_a_b.erase(p_a_b.begin()+k-1); p_err_b.erase(p_err_b.begin()+k-1); z_range_p_b.erase(z_range_p_b.begin()+k-1);}
          if(!k_a_r[k-1]) {k_a_r.erase(k_a_r.begin()+k-1); k_err_r.erase(k_err_r.begin()+k-1); z_range_k_r.erase(z_range_k_r.begin()+k-1);}
          if(!k_a_b[k-1]) {k_a_b.erase(k_a_b.begin()+k-1); k_err_b.erase(k_err_b.begin()+k-1); z_range_k_b.erase(z_range_k_b.begin()+k-1);}
          if(!h_a_r[k-1]) {h_a_r.erase(h_a_r.begin()+k-1); h_err_r.erase(h_err_r.begin()+k-1); z_range_h_r.erase(z_range_h_r.begin()+k-1);}
          if(!h_a_b[k-1]) {h_a_b.erase(h_a_b.begin()+k-1); h_err_b.erase(h_err_b.begin()+k-1); z_range_h_b.erase(z_range_h_b.begin()+k-1);}
        }

        for(int k=20; k>0; k--)
        {
          if(!p_pt_r[k-1]) {p_pt_r.erase(p_pt_r.begin()+k-1); p_pt_err_r.erase(p_pt_err_r.begin()+k-1); pt_range_p_r.erase(pt_range_p_r.begin()+k-1);}
          if(!p_pt_b[k-1]) {p_pt_b.erase(p_pt_b.begin()+k-1); p_pt_err_b.erase(p_pt_err_b.begin()+k-1); pt_range_p_b.erase(pt_range_p_b.begin()+k-1);}
          if(!k_pt_r[k-1]) {k_pt_r.erase(k_pt_r.begin()+k-1); k_pt_err_r.erase(k_pt_err_r.begin()+k-1); pt_range_k_r.erase(pt_range_k_r.begin()+k-1);}
          if(!k_pt_b[k-1]) {k_pt_b.erase(k_pt_b.begin()+k-1); k_pt_err_b.erase(k_pt_err_b.begin()+k-1); pt_range_k_b.erase(pt_range_k_b.begin()+k-1);}
          if(!h_pt_r[k-1]) {h_pt_r.erase(h_pt_r.begin()+k-1); h_pt_err_r.erase(h_pt_err_r.begin()+k-1); pt_range_h_r.erase(pt_range_h_r.begin()+k-1);}
          if(!h_pt_b[k-1]) {h_pt_b.erase(h_pt_b.begin()+k-1); h_pt_err_b.erase(h_pt_err_b.begin()+k-1); pt_range_h_b.erase(pt_range_h_b.begin()+k-1);}
        }

        bool p_a_r_empty = 0;
        bool k_a_r_empty = 0;
        bool h_a_r_empty = 0;
        bool p_a_b_empty = 0;
        bool k_a_b_empty = 0;
        bool h_a_b_empty = 0;

        bool p_pt_r_empty = 0;
        bool k_pt_r_empty = 0;
        bool h_pt_r_empty = 0;
        bool p_pt_b_empty = 0;
        bool k_pt_b_empty = 0;
        bool h_pt_b_empty = 0;

        if(!(int(p_a_r.size()))) p_pt_r_empty = 1;
        if(!(int(k_a_r.size()))) k_pt_r_empty = 1;
        if(!(int(h_a_r.size()))) h_pt_r_empty = 1;
        if(!(int(p_a_b.size()))) p_pt_b_empty = 1;
        if(!(int(k_a_b.size()))) k_pt_b_empty = 1;
        if(!(int(h_a_b.size()))) h_pt_b_empty = 1;

        if(!(int(p_pt_r.size()))) p_pt_r_empty = 1;
        if(!(int(k_pt_r.size()))) k_pt_r_empty = 1;
        if(!(int(h_pt_r.size()))) h_pt_r_empty = 1;
        if(!(int(p_pt_b.size()))) p_pt_b_empty = 1;
        if(!(int(k_pt_b.size()))) k_pt_b_empty = 1;
        if(!(int(h_pt_b.size()))) h_pt_b_empty = 1;

        H_r[c][i][j] = new TGraphErrors(int(h_a_r.size()),&(z_range_h_r[0]),&(h_a_r[0]),0,&(h_err_r[0]));
        P_r[c][i][j] = new TGraphErrors(int(p_a_r.size()),&(z_range_p_r[0]),&(p_a_r[0]),0,&(p_err_r[0]));
        K_r[c][i][j] = new TGraphErrors(int(k_a_r.size()),&(z_range_k_r[0]),&(k_a_r[0]),0,&(k_err_r[0]));
        H_b[c][i][j] = new TGraphErrors(int(h_a_b.size()),&(z_range_h_b[0]),&(h_a_b[0]),0,&(h_err_b[0]));
        P_b[c][i][j] = new TGraphErrors(int(p_a_b.size()),&(z_range_p_b[0]),&(p_a_b[0]),0,&(p_err_b[0]));
        K_b[c][i][j] = new TGraphErrors(int(k_a_b.size()),&(z_range_k_b[0]),&(k_a_b[0]),0,&(k_err_b[0]));

        H_r[c][i][j]->SetMarkerColor(fMarkerColor[c]);
        P_r[c][i][j]->SetMarkerColor(fMarkerColor[c]);
        K_r[c][i][j]->SetMarkerColor(fMarkerColor[c]);
        H_b[c][i][j]->SetMarkerColor(fMarkerColor[c]);
        P_b[c][i][j]->SetMarkerColor(fMarkerColor[c]);
        K_b[c][i][j]->SetMarkerColor(fMarkerColor[c]);

        H_r[c][i][j]->SetMarkerSize(2);
        P_r[c][i][j]->SetMarkerSize(2);
        K_r[c][i][j]->SetMarkerSize(2);
        H_b[c][i][j]->SetMarkerSize(2);
        P_b[c][i][j]->SetMarkerSize(2);
        K_b[c][i][j]->SetMarkerSize(2);

        H_r[c][i][j]->SetMarkerStyle(fMarkerStyle[0][c]);
        P_r[c][i][j]->SetMarkerStyle(fMarkerStyle[0][c]);
        K_r[c][i][j]->SetMarkerStyle(fMarkerStyle[0][c]);
        H_b[c][i][j]->SetMarkerStyle(fMarkerStyle[1][c]);
        P_b[c][i][j]->SetMarkerStyle(fMarkerStyle[1][c]);
        K_b[c][i][j]->SetMarkerStyle(fMarkerStyle[1][c]);

        H_r[c][i][j]->GetYaxis()->SetTitle("");
        P_r[c][i][j]->GetYaxis()->SetTitle("");
        K_r[c][i][j]->GetYaxis()->SetTitle("");
        H_b[c][i][j]->GetYaxis()->SetTitle("");
        P_b[c][i][j]->GetYaxis()->SetTitle("");
        K_b[c][i][j]->GetYaxis()->SetTitle("");

        H_r[c][i][j]->GetXaxis()->SetTitle("");
        P_r[c][i][j]->GetXaxis()->SetTitle("");
        K_r[c][i][j]->GetXaxis()->SetTitle("");
        H_b[c][i][j]->GetXaxis()->SetTitle("");
        P_b[c][i][j]->GetXaxis()->SetTitle("");
        K_b[c][i][j]->GetXaxis()->SetTitle("");

        H_r[c][i][j]->SetTitle("");
        P_r[c][i][j]->SetTitle("");
        K_r[c][i][j]->SetTitle("");
        H_b[c][i][j]->SetTitle("");
        P_b[c][i][j]->SetTitle("");
        K_b[c][i][j]->SetTitle("");

        if(!h_a_r_empty)
        {
          c1.cd(i+1+j*9);
          if(H_r[c][i][j])
          {
            if(!c)
            {
              H_r[c][i][j]->Draw("SAMEPA");
              H_r[c][i][j]->GetXaxis()->SetLimits(0.1,0.9);
              H_r[c][i][j]->SetMinimum(0.);
              H_r[c][i][j]->SetMaximum(0.3);
            }
            else
            {
              H_r[c][i][j]->Draw("SAMEP");
              H_r[c][i][j]->GetXaxis()->SetLimits(0.1,0.9);
              H_r[c][i][j]->SetMinimum(0.);
              H_r[c][i][j]->SetMaximum(0.3);
            }
          }
          c1.Update();
        }

        if(!h_a_b_empty)
        {
          c1.cd(i+1+j*9);
          if(H_b[c][i][j])
          {
            H_b[c][i][j]->Draw("SAMEP");
            H_b[c][i][j]->GetXaxis()->SetLimits(0.1,0.9);
            H_b[c][i][j]->SetMinimum(0.);
            H_b[c][i][j]->SetMaximum(0.3);
          }
          c1.Update();
        }

        if(!p_a_r_empty)
        {
          c2.cd(i+1+j*9);
          if(P_r[c][i][j])
          {
            if(!c)
            {
              P_r[c][i][j]->Draw("SAMEPA");
              P_r[c][i][j]->GetXaxis()->SetLimits(0.1,0.9);
              P_r[c][i][j]->SetMinimum(0.0);
              P_r[c][i][j]->SetMaximum(0.3);
            }
            else
            {
              P_r[c][i][j]->Draw("SAMEP");
              P_r[c][i][j]->GetXaxis()->SetLimits(0.1,0.9);
              P_r[c][i][j]->SetMinimum(0.0);
              P_r[c][i][j]->SetMaximum(0.3);
            }
          }
          c2.Update();
        }

        if(!p_a_b_empty)
        {
          c2.cd(i+1+j*9);
          if(P_b[c][i][j])
          {
            P_b[c][i][j]->Draw("SAMEP");
            P_b[c][i][j]->GetXaxis()->SetLimits(0.1,0.9);
            P_b[c][i][j]->SetMinimum(0.0);
            P_b[c][i][j]->SetMaximum(0.3);
          }
          c2.Update();
        }

        if(!k_a_r_empty)
        {
          c3.cd(i+1+j*9);
          if(K_r[c][i][j])
          {
            if(!c)
            {
              K_r[c][i][j]->Draw("SAMEPA");
              K_r[c][i][j]->GetXaxis()->SetLimits(0.1,0.9);
              K_r[c][i][j]->SetMinimum(0.0);
              K_r[c][i][j]->SetMaximum(0.3);
            }
            else
            {
              K_r[c][i][j]->Draw("SAMEP");
              K_r[c][i][j]->GetXaxis()->SetLimits(0.1,0.9);
              K_r[c][i][j]->SetMinimum(0.0);
              K_r[c][i][j]->SetMaximum(0.3);
            }
          }
          c3.Update();
        }

        if(!k_a_b_empty)
        {
          c3.cd(i+1+j*9);
          if(K_b[c][i][j])
          {
            K_b[c][i][j]->Draw("SAMEP");
            K_b[c][i][j]->GetXaxis()->SetLimits(0.1,0.9);
            K_b[c][i][j]->SetMinimum(0.0);
            K_b[c][i][j]->SetMaximum(0.3);
          }
          c3.Update();
        }

        H_pt_r[c][i][j] = new TGraphErrors(int(h_pt_r.size()),&(pt_range_h_r[0]),&(h_pt_r[0]),0,&(h_pt_err_r[0]));
        P_pt_r[c][i][j] = new TGraphErrors(int(p_pt_r.size()),&(pt_range_p_r[0]),&(p_pt_r[0]),0,&(p_pt_err_r[0]));
        K_pt_r[c][i][j] = new TGraphErrors(int(k_pt_r.size()),&(pt_range_k_r[0]),&(k_pt_r[0]),0,&(k_pt_err_r[0]));
        H_pt_b[c][i][j] = new TGraphErrors(int(h_pt_b.size()),&(pt_range_h_b[0]),&(h_pt_b[0]),0,&(h_pt_err_b[0]));
        P_pt_b[c][i][j] = new TGraphErrors(int(p_pt_b.size()),&(pt_range_p_b[0]),&(p_pt_b[0]),0,&(p_pt_err_b[0]));
        K_pt_b[c][i][j] = new TGraphErrors(int(k_pt_b.size()),&(pt_range_k_b[0]),&(k_pt_b[0]),0,&(k_pt_err_b[0]));

        H_pt_r[c][i][j]->SetMarkerColor(fMarkerColor[c]);
        P_pt_r[c][i][j]->SetMarkerColor(fMarkerColor[c]);
        K_pt_r[c][i][j]->SetMarkerColor(fMarkerColor[c]);
        H_pt_b[c][i][j]->SetMarkerColor(fMarkerColor[c]);
        P_pt_b[c][i][j]->SetMarkerColor(fMarkerColor[c]);
        K_pt_b[c][i][j]->SetMarkerColor(fMarkerColor[c]);

        H_pt_r[c][i][j]->SetMarkerSize(2);
        P_pt_r[c][i][j]->SetMarkerSize(2);
        K_pt_r[c][i][j]->SetMarkerSize(2);
        H_pt_b[c][i][j]->SetMarkerSize(2);
        P_pt_b[c][i][j]->SetMarkerSize(2);
        K_pt_b[c][i][j]->SetMarkerSize(2);

        H_pt_r[c][i][j]->SetMarkerStyle(fMarkerStyle[0][c]);
        P_pt_r[c][i][j]->SetMarkerStyle(fMarkerStyle[0][c]);
        K_pt_r[c][i][j]->SetMarkerStyle(fMarkerStyle[0][c]);
        H_pt_b[c][i][j]->SetMarkerStyle(fMarkerStyle[1][c]);
        P_pt_b[c][i][j]->SetMarkerStyle(fMarkerStyle[1][c]);
        K_pt_b[c][i][j]->SetMarkerStyle(fMarkerStyle[1][c]);

        H_pt_r[c][i][j]->GetYaxis()->SetTitle("");
        P_pt_r[c][i][j]->GetYaxis()->SetTitle("");
        K_pt_r[c][i][j]->GetYaxis()->SetTitle("");
        H_pt_b[c][i][j]->GetYaxis()->SetTitle("");
        P_pt_b[c][i][j]->GetYaxis()->SetTitle("");
        K_pt_b[c][i][j]->GetYaxis()->SetTitle("");

        H_pt_r[c][i][j]->GetXaxis()->SetTitle("");
        P_pt_r[c][i][j]->GetXaxis()->SetTitle("");
        K_pt_r[c][i][j]->GetXaxis()->SetTitle("");
        H_pt_b[c][i][j]->GetXaxis()->SetTitle("");
        P_pt_b[c][i][j]->GetXaxis()->SetTitle("");
        K_pt_b[c][i][j]->GetXaxis()->SetTitle("");

        H_pt_r[c][i][j]->SetTitle("");
        P_pt_r[c][i][j]->SetTitle("");
        K_pt_r[c][i][j]->SetTitle("");
        H_pt_b[c][i][j]->SetTitle("");
        P_pt_b[c][i][j]->SetTitle("");
        K_pt_b[c][i][j]->SetTitle("");

        if(!h_pt_r_empty)
        {
          c4.cd(i+1+j*9);
          if(H_pt_r[c][i][j])
          {
            if(!c)
            {
              H_pt_r[c][i][j]->Draw("SAMEPA");
              H_pt_r[c][i][j]->GetXaxis()->SetLimits(0.,3.0);
              H_pt_r[c][i][j]->SetMinimum(0.);
              H_pt_r[c][i][j]->SetMaximum(0.3);
            }
            else
            {
              H_pt_r[c][i][j]->Draw("SAMEP");
              H_pt_r[c][i][j]->GetXaxis()->SetLimits(0.,3.0);
              H_pt_r[c][i][j]->SetMinimum(0.);
              H_pt_r[c][i][j]->SetMaximum(0.3);
            }
          }
          c4.Update();
        }

        if(!h_pt_b_empty)
        {
          c4.cd(i+1+j*9);
          if(H_pt_b[c][i][j])
          {
            H_pt_b[c][i][j]->Draw("SAMEP");
            H_pt_b[c][i][j]->GetXaxis()->SetLimits(0.,3.0);
            H_pt_b[c][i][j]->SetMinimum(0.);
            H_pt_b[c][i][j]->SetMaximum(0.3);
          }
          c4.Update();
        }

        if(!p_pt_r_empty)
        {
          c5.cd(i+1+j*9);
          if(P_pt_r[c][i][j])
          {
            if(!c)
            {
              P_pt_r[c][i][j]->Draw("SAMEPA");
              P_pt_r[c][i][j]->GetXaxis()->SetLimits(0.,3.0);
              P_pt_r[c][i][j]->SetMinimum(0.0);
              P_pt_r[c][i][j]->SetMaximum(0.3);
            }
            else
            {
              P_pt_r[c][i][j]->Draw("SAMEP");
              P_pt_r[c][i][j]->GetXaxis()->SetLimits(0.,3.0);
              P_pt_r[c][i][j]->SetMinimum(0.0);
              P_pt_r[c][i][j]->SetMaximum(0.3);
            }
          }
          c5.Update();
        }

        if(!p_pt_b_empty)
        {
          c5.cd(i+1+j*9);
          if(P_pt_b[c][i][j])
          {
            P_pt_b[c][i][j]->Draw("SAMEP");
            P_pt_b[c][i][j]->GetXaxis()->SetLimits(0.,3.0);
            P_pt_b[c][i][j]->SetMinimum(0.0);
            P_pt_b[c][i][j]->SetMaximum(0.3);
          }
          c5.Update();
        }

        if(!k_pt_r_empty)
        {
          c6.cd(i+1+j*9);
          if(K_pt_r[c][i][j])
          {
            if(!c)
            {
              K_pt_r[c][i][j]->Draw("SAMEPA");
              K_pt_r[c][i][j]->GetXaxis()->SetLimits(0.,3.0);
              K_pt_r[c][i][j]->SetMinimum(0.0);
              K_pt_r[c][i][j]->SetMaximum(0.3);
            }
            else
            {
              K_pt_r[c][i][j]->Draw("SAMEP");
              K_pt_r[c][i][j]->GetXaxis()->SetLimits(0.,3.0);
              K_pt_r[c][i][j]->SetMinimum(0.0);
              K_pt_r[c][i][j]->SetMaximum(0.3);
            }
          }
          c6.Update();
        }

        if(!k_pt_b_empty)
        {
          c6.cd(i+1+j*9);
          if(K_pt_b[c][i][j])
          {
            K_pt_b[c][i][j]->Draw("SAMEP");
            K_pt_b[c][i][j]->GetXaxis()->SetLimits(0.,3.0);
            K_pt_b[c][i][j]->SetMinimum(0.0);
            K_pt_b[c][i][j]->SetMaximum(0.3);
          }
          c6.Update();
        }
      }
    }
  }

  c1.Update();
  c2.Update();
  c3.Update();
  c4.Update();
  c5.Update();
  c6.Update();

  c1.Print(hadron_z_pdf);
  c2.Print(pion_z_pdf);
  c3.Print(kaon_z_pdf);
  c4.Print(hadron_pt_pdf);
  c5.Print(pion_pt_pdf);
  c6.Print(kaon_pt_pdf);


  return 0;
}
