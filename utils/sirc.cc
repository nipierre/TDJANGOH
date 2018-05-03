#include "sirc.h"

using namespace std;

double average(vector<double> pVec)
{
  double return_value = 0.0;
  double n = pVec.size();

  for ( int i=0; i < n; i++)
  {
      return_value += pVec[i];
  }

  return (return_value / n);
}

int main(int argc,char *argv[])
{
  string FileRC, FileBorn, fileFlag;
  int KMultSave_flag = 0;

  for (int i = 1; i < argc; i++)
  {
    if (string(argv[i]) == "-h")
    {
      cout << FCYN("HELP : available flags :") << endl;
      cout << FCYN("-f [RC_file] [Born_file]") << endl;
      cout << FCYN("-l [RC_filelist] [Born_filelist]") << endl;
      return 0;
    }
    if (string(argv[i]) == "-K")
    {
      KMultSave_flag = 1;
    }
    if(i+2 < argc)
    {
      if (string(argv[i]) == "-f" && fileFlag != "-l")
      {
        FileRC = argv[i+1];
        FileBorn = argv[i+2];
        fileFlag = "-f";
      }
      if (string(argv[i]) == "-l" && fileFlag != "-f")
      {
        FileRC = argv[i+1];
        FileBorn = argv[i+2];
        fileFlag = "-l";
      }
    }
  }

  if(fileFlag != "-f" && fileFlag != "-l")
  {
    cout << BOLD(FRED("ERROR : expected some flags")) << endl;
    cout << BOLD(FRED("Expected -f for input files or -l for lists !")) << endl;
    cout << BOLD(FRED("USAGE : \n ./sirc -f [RC_file] [Born_file]")) << endl;
    cout << BOLD(FRED("USAGE : \n ./sirc -l [RC_filelist] [Born_filelist]")) << endl;
    cout << BOLD(FRED("[-h for further help]")) << endl;
    return 1;
  }

  E_prime = 0;

  for(int i=0; i<3; i++)
  {
    for(int j=0; j<9; j++)
    {
      for(int k=0; k<5; k++)
      {
        fNSIDIS_evt_r[i][j][k]=0;
        fNSIDIS_evt_b[i][j][k]=0;
        for(int l=0; l<14; l++)
        {
          fNDIS_evt_r[i][j][k][l]=0;
          fNDIS_evt_b[i][j][k][l]=0;
        }
      }
    }
  }

  for(int i=0; i<14; i++)
  {
    fNu_max[1][i] = sqrt(pow(40,2)+pow(fM_K,2))/fZrange[i+1];
    fNu_min[1][i] = sqrt(pow(3,2)+pow(fM_K,2))/fZrange[i];

    fNu_max[2][i] = sqrt(pow(40,2)+pow(fM_p,2))/fZrange[i+1];
    fNu_min[2][i] = sqrt(pow(3,2)+pow(fM_p,2))/fZrange[i];

    fNu_max[0][i] = sqrt(pow(40,2)+pow(fM_pi,2))/fZrange[i+1];
    fNu_min[0][i] = sqrt(pow(3,2)+pow(fM_pi,2))/fZrange[i];
  }

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                       SINGLE FILES
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

  if(fileFlag == "-f")
  {
    cout << ".. Opening file " << FileRC << " .." << endl;
    ifstream revt(FileRC);

    while(revt >> npart)
    {
      fDIScuts = 0;
#ifdef DEBUG
      cout << "DEBUG: READ CHECK" << endl;
      cout << npart << endl;
#endif
      revt >> E >> xbj >> y >> Q2;
      W = pow(fM_p,2) + Q2*(1-xbj)/xbj;
#ifdef DEBUG
      cout << E << "\txbj : " << xbj << "\ty : " << y << "\tQ2 : " << Q2 << endl;
      cout << "Particles list" << endl;
#endif
      if(0<E)
      {
        if(Q2>1)
        {
          if(.004<xbj && xbj<.4)
          {
            if(.1<y && y<.9)
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
      else if(0.5<=y && y<0.7) ybin = 4;
      else ybin = 5;

      // cout << ybin << endl;

      if(fDIScuts)
      {
        for(int i=0; i<14; i++)
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
#ifdef DEBUG
       cout << "DEBUG: READ CHECK RE FILE" << endl;
       cout << dummy << "\tid : " << id << "\t" << dummy << "\t" << dummy << "\t" << dummy << "\t";
#endif
        revt >> px >> py >> pz >> Eh >> dummy;
#ifdef DEBUG
       cout << px << "\t" << py << "\t" << pz << "\t" << Eh << "\t" << dummy << "\t";
#endif
        revt >> dummy >> dummy >> dummy >> dummy;
#ifdef DEBUG
       cout << dummy << "\t" << dummy << "\t" << dummy << "\t" << dummy << endl;
#endif

        if(!fDIScuts) continue;

        if(abs(id)==13)
        {
          E_prime = Eh;
          nu = E - E_prime;
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
#ifdef DEBUG
          cout << "Eh not good : " << Eh << endl;
#endif
          continue;
        }

        if(0<z && z<0.2) zbin = 0;
        else if(0.2<z && z<0.25) zbin = 1;
        else if(0.25<=z && z<0.30) zbin = 2;
        else if(0.30<=z && z<0.35) zbin = 3;
        else if(0.35<=z && z<0.40) zbin = 4;
        else if(0.40<=z && z<0.45) zbin = 5;
        else if(0.45<=z && z<0.50) zbin = 6;
        else if(0.50<=z && z<0.55) zbin = 7;
        else if(0.55<=z && z<0.60) zbin = 8;
        else if(0.60<=z && z<0.65) zbin = 9;
        else if(0.65<=z && z<0.70) zbin = 10;
        else if(0.70<=z && z<0.75) zbin = 11;
        else if(0.75<=z && z<0.85) zbin = 12;
        else zbin = 13;

        if(0.0<pt && pt<0.25) ptbin = 0;
        else if(0.25<pt && pt<0.5) ptbin = 1;
        else if(0.5<pt && pt<0.75) ptbin = 2;
        else if(0.75<pt && pt<1.) ptbin = 3;
        else if(1.<pt && pt<1.25) ptbin = 4;
        else if(1.25<pt && pt<1.5) ptbin = 5;
        else if(1.5<pt && pt<1.75) ptbin = 6;
        else if(1.75<pt && pt<2.) ptbin = 7;
        else if(2.<pt && pt<2.25) ptbin = 8;
        else if(2.25<pt && pt<2.5) ptbin = 9;
        else if(2.5<pt && pt<2.75) ptbin = 10;
        else if(2.75<pt && pt<3) ptbin = 11;
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
          fQ2[1][xbin][ybin][zbin].push_back(Q2);
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
          fQ2[0][xbin][ybin][zbin].push_back(Q2);
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
          fQ2[1][xbin][ybin][zbin].push_back(Q2);
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
          fQ2[0][xbin][ybin][zbin].push_back(Q2);
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
          fQ2[1][xbin][ybin][zbin].push_back(Q2);
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
          fQ2[0][xbin][ybin][zbin].push_back(Q2);
        }
        else if(fId==6)
        {
          if(fFlag_r[0][xbin][ybin][zbin]) continue;
          fRe[xbin][ybin][zbin].tab[1][0][3] += 1;
          fRept[xbin][ybin][ptbin].tab[1][0][3] += 1;
          fNSIDIS_evt_r[3][xbin][ybin]++;
          fQ2[1][xbin][ybin][zbin].push_back(Q2);
        }
        else if(fId==7)
        {
          if(fFlag_r[0][xbin][ybin][zbin]) continue;
          fRe[xbin][ybin][zbin].tab[0][0][3] += 1;
          fRept[xbin][ybin][ptbin].tab[0][0][3] += 1;
          fNSIDIS_evt_r[3][xbin][ybin]++;
          fQ2[0][xbin][ybin][zbin].push_back(Q2);
        }
        else
        {
          continue;
        }
      }
    }

    revt.close();

    cout << ".. Opening file " << FileBorn << " .." << endl;
    ifstream bevt(FileBorn);

    while(bevt >> npart)
    {
      fDIScuts = 0;
#ifdef DEBUG
      cout << "DEBUG: READ CHECK" << endl;
      cout << npart << endl;
#endif
      bevt >> E >> xbj >> y >> Q2;
      W = pow(fM_p,2) + Q2*(1-xbj)/xbj;
#ifdef DEBUG
      cout << E << "\txbj : " << xbj << "\ty : " << y << "\tQ2 : " << Q2 << endl;
      cout << "Particles list" << endl;
#endif
      if(0<E)
      {
        if(Q2>1)
        {
          if(.004<xbj && xbj<.4)
          {
            if(.1<y && y<.9)
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
      else if(0.5<=y && y<0.7) ybin = 4;
      else ybin = 5;

      if(fDIScuts)
      {
        for(int i=0; i<14; i++)
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
#ifdef DEBUG
       cout << "DEBUG: READ CHECK BORN FILE" << endl;
       cout << dummy << "\tid : " << id << "\t" << dummy << "\t" << dummy << "\t" << dummy << "\t";
#endif
        bevt >> px >> py >> pz >> Eh >> dummy;
#ifdef DEBUG
       cout << px << "\t" << py << "\t" << pz << "\t" << Eh << "\t" << dummy << "\t";
#endif
        bevt >> dummy >> dummy >> dummy >> dummy;
#ifdef DEBUG
       cout << dummy << "\t" << dummy << "\t" << dummy << "\t" << dummy << endl;
#endif

        if(!fDIScuts) continue;

        if(abs(id)==13)
        {
          E_prime = Eh;
          nu = E - E_prime;
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
#ifdef DEBUG
          cout << "Eh not good : " << Eh << endl;
#endif
          continue;
        }

        if(0<z && z<0.2) zbin = 0;
        else if(0.2<z && z<0.25) zbin = 1;
        else if(0.25<=z && z<0.30) zbin = 2;
        else if(0.30<=z && z<0.35) zbin = 3;
        else if(0.35<=z && z<0.40) zbin = 4;
        else if(0.40<=z && z<0.45) zbin = 5;
        else if(0.45<=z && z<0.50) zbin = 6;
        else if(0.50<=z && z<0.55) zbin = 7;
        else if(0.55<=z && z<0.60) zbin = 8;
        else if(0.60<=z && z<0.65) zbin = 9;
        else if(0.65<=z && z<0.70) zbin = 10;
        else if(0.70<=z && z<0.75) zbin = 11;
        else if(0.75<=z && z<0.85) zbin = 12;
        else zbin = 13;

        if(0.0<pt && pt<0.25) ptbin = 0;
        else if(0.25<pt && pt<0.5) ptbin = 1;
        else if(0.5<pt && pt<0.75) ptbin = 2;
        else if(0.75<pt && pt<1.) ptbin = 3;
        else if(1.<pt && pt<1.25) ptbin = 4;
        else if(1.25<pt && pt<1.5) ptbin = 5;
        else if(1.5<pt && pt<1.75) ptbin = 6;
        else if(1.75<pt && pt<2.) ptbin = 7;
        else if(2.<pt && pt<2.25) ptbin = 8;
        else if(2.25<pt && pt<2.5) ptbin = 9;
        else if(2.5<pt && pt<2.75) ptbin = 10;
        else if(2.75<pt && pt<3) ptbin = 11;
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
    bevt.close();
  }

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                      FILELIST
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

  if(fileFlag == "-l")
  {
    string filename;

    ifstream filelistre(FileRC);

    while(filelistre >> filename)
    {
      cout << ".. Opening file " << filename << " .." << endl;
      ifstream revt(filename.c_str());

      while(revt >> npart)
      {
        fDIScuts = 0;
#ifdef DEBUG
        cout << "DEBUG: READ CHECK" << endl;
        cout << npart << endl;
#endif
        revt >> E >> xbj >> y >> Q2;
        W = pow(fM_p,2) + Q2*(1-xbj)/xbj;
#ifdef DEBUG
        cout << E << "\txbj : " << xbj << "\ty : " << y << "\tQ2 : " << Q2 << endl;
        cout << "Particles list" << endl;
#endif
        if(0<E)
        {
          if(Q2>1)
          {
            if(.004<xbj && xbj<.4)
            {
              if(.1<y && y<.9)
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
        else if(0.5<=y && y<0.7) ybin = 4;
        else ybin = 5;

        if(fDIScuts)
        {
          for(int i=0; i<14; i++)
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
#ifdef DEBUG
          cout << "DEBUG: READ CHECK RE FILE" << endl;
          cout << dummy << "\tid : " << id << "\t" << dummy << "\t" << dummy << "\t" << dummy << "\t";
#endif
          revt >> px >> py >> pz >> Eh >> dummy;
#ifdef DEBUG
          cout << px << "\t" << py << "\t" << pz << "\t" << Eh << "\t" << "\t";
#endif
          revt >> dummy >> dummy >> dummy >> dummy;
#ifdef DEBUG
          cout << dummy << "\t" << dummy << "\t" << dummy << "\t" << dummy << endl;
#endif

          if(!fDIScuts) continue;

          if(abs(id)==13)
          {
            E_prime = Eh;
            nu = E - E_prime;
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
#ifdef DEBUG
            cout << "Eh not good : " << Eh << endl;
#endif
            continue;
          }

          if(0<z && z<0.2) zbin = 0;
          else if(0.2<z && z<0.25) zbin = 1;
          else if(0.25<=z && z<0.30) zbin = 2;
          else if(0.30<=z && z<0.35) zbin = 3;
          else if(0.35<=z && z<0.40) zbin = 4;
          else if(0.40<=z && z<0.45) zbin = 5;
          else if(0.45<=z && z<0.50) zbin = 6;
          else if(0.50<=z && z<0.55) zbin = 7;
          else if(0.55<=z && z<0.60) zbin = 8;
          else if(0.60<=z && z<0.65) zbin = 9;
          else if(0.65<=z && z<0.70) zbin = 10;
          else if(0.70<=z && z<0.75) zbin = 11;
          else if(0.75<=z && z<0.85) zbin = 12;
          else zbin = 13;

          if(0.0<pt && pt<0.25) ptbin = 0;
          else if(0.25<pt && pt<0.5) ptbin = 1;
          else if(0.5<pt && pt<0.75) ptbin = 2;
          else if(0.75<pt && pt<1.) ptbin = 3;
          else if(1.<pt && pt<1.25) ptbin = 4;
          else if(1.25<pt && pt<1.5) ptbin = 5;
          else if(1.5<pt && pt<1.75) ptbin = 6;
          else if(1.75<pt && pt<2.) ptbin = 7;
          else if(2.<pt && pt<2.25) ptbin = 8;
          else if(2.25<pt && pt<2.5) ptbin = 9;
          else if(2.5<pt && pt<2.75) ptbin = 10;
          else if(2.75<pt && pt<3) ptbin = 11;
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
    }

    filelistre.close();

    ifstream filelistborn(FileBorn);

    while(filelistborn >> filename)
    {
      cout << ".. Opening file " << filename << " .." << endl;
      ifstream bevt(filename.c_str());

      while(bevt >> npart)
      {
        fDIScuts = 0;
#ifdef DEBUG
        cout << "DEBUG: READ CHECK" << endl;
        cout << npart << endl;
#endif
        bevt >> E >> xbj >> y >> Q2;
        W = pow(fM_p,2) + Q2*(1-xbj)/xbj;
#ifdef DEBUG
        cout << E << "\txbj : " << xbj << "\ty : " << y << "\tQ2 : " << Q2 << endl;
        cout << "Particles list" << endl;
#endif
        if(0<E)
        {
          if(Q2>1)
          {
            if(.004<xbj && xbj<.4)
            {
              if(.1<y && y<.9)
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
        else if(0.5<=y && y<0.7) ybin = 4;
        else ybin = 5;

        if(fDIScuts)
        {
          for(int i=0; i<14; i++)
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
#ifdef DEBUG
          cout << "DEBUG: READ CHECK BORN FILE" << endl;
          cout << dummy << "\tid : " << id << "\t" << dummy << "\t" << dummy << "\t" << dummy << "\t";
#endif
          bevt >> px >> py >> pz >> Eh >> dummy;
#ifdef DEBUG
          cout << px << "\t" << py << "\t" << pz << "\t" << Eh << "\t" << dummy << "\t";
#endif
          bevt >> dummy >> dummy >> dummy >> dummy;
#ifdef DEBUG
          cout << dummy << "\t" << dummy << "\t" << dummy << "\t" << dummy << endl;
#endif

          if(!fDIScuts) continue;

          if(abs(id)==13)
          {
            E_prime = Eh;
            nu = E - E_prime;
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
#ifdef DEBUG
            cout << "Eh not good : " << Eh << endl;
#endif
            continue;
          }


          if(0<z && z<0.2) zbin = 0;
          else if(0.2<z && z<0.25) zbin = 1;
          else if(0.25<=z && z<0.30) zbin = 2;
          else if(0.30<=z && z<0.35) zbin = 3;
          else if(0.35<=z && z<0.40) zbin = 4;
          else if(0.40<=z && z<0.45) zbin = 5;
          else if(0.45<=z && z<0.50) zbin = 6;
          else if(0.50<=z && z<0.55) zbin = 7;
          else if(0.55<=z && z<0.60) zbin = 8;
          else if(0.60<=z && z<0.65) zbin = 9;
          else if(0.65<=z && z<0.70) zbin = 10;
          else if(0.70<=z && z<0.75) zbin = 11;
          else if(0.75<=z && z<0.85) zbin = 12;
          else zbin = 13;

          if(0.0<pt && pt<0.25) ptbin = 0;
          else if(0.25<pt && pt<0.5) ptbin = 1;
          else if(0.5<pt && pt<0.75) ptbin = 2;
          else if(0.75<pt && pt<1.) ptbin = 3;
          else if(1.<pt && pt<1.25) ptbin = 4;
          else if(1.25<pt && pt<1.5) ptbin = 5;
          else if(1.5<pt && pt<1.75) ptbin = 6;
          else if(1.75<pt && pt<2.) ptbin = 7;
          else if(2.<pt && pt<2.25) ptbin = 8;
          else if(2.25<pt && pt<2.5) ptbin = 9;
          else if(2.5<pt && pt<2.75) ptbin = 10;
          else if(2.75<pt && pt<3) ptbin = 11;
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
      bevt.close();
    }
    filelistborn.close();
  }

  c1.SetFillColor(0);
  c2.SetFillColor(0);
  c3.SetFillColor(0);
  c4.SetFillColor(0);
  c5.SetFillColor(0);
  c6.SetFillColor(0);
  c7.SetFillColor(0);
  c8.SetFillColor(0);
  c9.SetFillColor(0);
  c10.SetFillColor(0);
  c11.SetFillColor(0);
  c12.SetFillColor(0);

  c1.Divide(9,5,0,0);
  c2.Divide(9,5,0,0);
  c3.Divide(9,5,0,0);
  c4.Divide(9,5,0,0);
  c5.Divide(9,5,0,0);
  c6.Divide(9,5,0,0);
  c7.Divide(5,2,0,0);
  c8.Divide(5,2,0,0);
  c9.Divide(5,2,0,0);
  c10.Divide(5,2,0,0);
  c11.Divide(5,2,0,0);
  c12.Divide(5,2,0,0);

  for(int i=0; i<6; i++)
  {
    l1[i] = new TLine(0.,1+i*0.1,1.0,1+i*0.1);
    l1[i]->SetLineStyle(4);
    l1[i]->SetLineColor(fMarkerColorAlt[i]);
    l2[i] = new TLine(0.,1+i*0.1,6.,1+i*0.1);
    l2[i]->SetLineStyle(4);
    l2[i]->SetLineColor(fMarkerColorAlt[i]);
  }

  double z_range[14] = {.1,.225,.275,.325,.375,.425,.475,.525,.575,.625,.675,.725,.8,.925};
  double pt_range[12] = {0.125,0.375,0.625,0.875,1.125,
                      1.375,1.625,1.875,2.125,2.375,
                      2.625,2.875};

  ofstream shout("semi_inclusive.txt",std::ofstream::out | std::ofstream::trunc);
  ofstream ofs_multKp("K+_p_MC.txt",std::ofstream::out | std::ofstream::trunc);
  ofstream ofs_multKm("K-_p_MC.txt",std::ofstream::out | std::ofstream::trunc);

  for(int c=0; c<2; c++)
  {
    for(int i=0; i<9; i++)
    {
      for(int j=0; j<6; j++)
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

        std::vector<double> p_a_d;
        std::vector<double> k_a_d;
        std::vector<double> h_a_d;
        std::vector<double> p_err_d;
        std::vector<double> k_err_d;
        std::vector<double> h_err_d;

        std::vector<double> z_range_p_r;
        std::vector<double> z_range_k_r;
        std::vector<double> z_range_h_r;

        std::vector<double> z_range_p_b;
        std::vector<double> z_range_k_b;
        std::vector<double> z_range_h_b;

        std::vector<double> z_range_p_d;
        std::vector<double> z_range_k_d;
        std::vector<double> z_range_h_d;

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

        std::vector<double> p_pt_d;
        std::vector<double> k_pt_d;
        std::vector<double> h_pt_d;
        std::vector<double> p_pt_err_d;
        std::vector<double> k_pt_err_d;
        std::vector<double> h_pt_err_d;

        std::vector<double> pt_range_p_r;
        std::vector<double> pt_range_k_r;
        std::vector<double> pt_range_h_r;

        std::vector<double> pt_range_p_b;
        std::vector<double> pt_range_k_b;
        std::vector<double> pt_range_h_b;

        std::vector<double> pt_range_p_d;
        std::vector<double> pt_range_k_d;
        std::vector<double> pt_range_h_d;

        for(int l=0; l<14; l++)
        {
          z_range_p_r.push_back(z_range[l]);
          z_range_k_r.push_back(z_range[l]);
          z_range_h_r.push_back(z_range[l]);
          z_range_p_b.push_back(z_range[l]);
          z_range_k_b.push_back(z_range[l]);
          z_range_h_b.push_back(z_range[l]);
          z_range_p_d.push_back(z_range[l]);
          z_range_k_d.push_back(z_range[l]);
          z_range_h_d.push_back(z_range[l]);
        }
        for(int l=0; l<13; l++)
        {
          pt_range_p_r.push_back(pt_range[l]);
          pt_range_k_r.push_back(pt_range[l]);
          pt_range_h_r.push_back(pt_range[l]);
          pt_range_p_b.push_back(pt_range[l]);
          pt_range_k_b.push_back(pt_range[l]);
          pt_range_h_b.push_back(pt_range[l]);
          pt_range_p_d.push_back(pt_range[l]);
          pt_range_k_d.push_back(pt_range[l]);
          pt_range_h_d.push_back(pt_range[l]);
        }

#ifdef DEBUG
        cout << "DEBUG: MULTIPLICITY CONTENT" << endl;
        cout << fRe[i][j][0].tab[0][0][3]+fRe[i][j][1].tab[0][0][3]+fRe[i][j][2].tab[0][0][3]+fRe[i][j][3].tab[0][0][3]
        +fRe[i][j][4].tab[0][0][3]+fRe[i][j][5].tab[0][0][3]+fRe[i][j][6].tab[0][0][3]+fRe[i][j][7].tab[0][0][3]
        +fRe[i][j][8].tab[0][0][3]+fRe[i][j][9].tab[0][0][3]+fRe[i][j][10].tab[0][0][3]+fRe[i][j][11].tab[0][0][3]
        +fRe[i][j][0].tab[1][0][3]+fRe[i][j][1].tab[1][0][3]+fRe[i][j][2].tab[1][0][3]+fRe[i][j][3].tab[1][0][3]
        +fRe[i][j][4].tab[1][0][3]+fRe[i][j][5].tab[1][0][3]+fRe[i][j][6].tab[1][0][3]+fRe[i][j][7].tab[1][0][3]
        +fRe[i][j][8].tab[1][0][3]+fRe[i][j][9].tab[1][0][3]+fRe[i][j][10].tab[1][0][3]+fRe[i][j][11].tab[1][0][3]
        <<
        " " << fNSIDIS_evt_r[3][i][j] << endl;

        cout << fBorn[i][j][0].tab[0][0][3]+fBorn[i][j][1].tab[0][0][3]+fBorn[i][j][2].tab[0][0][3]+fBorn[i][j][3].tab[0][0][3]
        +fBorn[i][j][4].tab[0][0][3]+fBorn[i][j][5].tab[0][0][3]+fBorn[i][j][6].tab[0][0][3]+fBorn[i][j][7].tab[0][0][3]
        +fBorn[i][j][8].tab[0][0][3]+fBorn[i][j][9].tab[0][0][3]+fBorn[i][j][10].tab[0][0][3]+fBorn[i][j][11].tab[0][0][3]
        +fBorn[i][j][0].tab[1][0][3]+fBorn[i][j][1].tab[1][0][3]+fBorn[i][j][2].tab[1][0][3]+fBorn[i][j][3].tab[1][0][3]
        +fBorn[i][j][4].tab[1][0][3]+fBorn[i][j][5].tab[1][0][3]+fBorn[i][j][6].tab[1][0][3]+fBorn[i][j][7].tab[1][0][3]
        +fBorn[i][j][8].tab[1][0][3]+fBorn[i][j][9].tab[1][0][3]+fBorn[i][j][10].tab[1][0][3]+fBorn[i][j][11].tab[1][0][3]
        <<
        " " << fNSIDIS_evt_b[3][i][j] << endl;
#endif

        for(int k=0; k<14; k++)
        {
          cout << "x,y,z : " << i << "," << j << "," << k << "\nN DIS evt r : " << fNDIS_evt_r[0][i][j][k] << " N SIDIS evt r : " << fNSIDIS_evt_r[3][i][j] << " N re : " << fRe[i][j][k].tab[c][0][3];
          fReMult[i][j][k].tab[c][0][0] = ((fNDIS_evt_r[0][i][j][k]) ? double(fRe[i][j][k].tab[c][0][0])/double(fNDIS_evt_r[0][i][j][k]*fZ_bin_width[k]) : 0);
          fReMult[i][j][k].tab[c][0][1] = ((fNDIS_evt_r[0][i][j][k]) ? double(fRe[i][j][k].tab[c][0][1])/double(fNDIS_evt_r[0][i][j][k]*fZ_bin_width[k]) : 0);
          fReMult[i][j][k].tab[c][0][2] = ((fNDIS_evt_r[0][i][j][k]) ? double(fRe[i][j][k].tab[c][0][2])/double(fNDIS_evt_r[0][i][j][k]*fZ_bin_width[k]) : 0);
          fReMult[i][j][k].tab[c][0][3] = ((fNDIS_evt_r[0][i][j][k]) ? double(fRe[i][j][k].tab[c][0][3])/double(fNDIS_evt_r[0][i][j][k]*fZ_bin_width[k]) : 0);
          cout << " Hm : " << fReMult[i][j][k].tab[c][0][3] << endl;
          cout << "N DIS evt B : " << fNDIS_evt_b[0][i][j][k] << " N SIDIS evt B : " << fNSIDIS_evt_b[3][i][j] << " N born : " << fBorn[i][j][k].tab[c][0][3];
          fBornMult[i][j][k].tab[c][0][0] = ((fNDIS_evt_b[0][i][j][k]) ? double(fBorn[i][j][k].tab[c][0][0])/double(fNDIS_evt_b[0][i][j][k]*fZ_bin_width[k]) : 0);
          fBornMult[i][j][k].tab[c][0][1] = ((fNDIS_evt_b[0][i][j][k]) ? double(fBorn[i][j][k].tab[c][0][1])/double(fNDIS_evt_b[0][i][j][k]*fZ_bin_width[k]) : 0);
          fBornMult[i][j][k].tab[c][0][2] = ((fNDIS_evt_b[0][i][j][k]) ? double(fBorn[i][j][k].tab[c][0][2])/double(fNDIS_evt_b[0][i][j][k]*fZ_bin_width[k]) : 0);
          fBornMult[i][j][k].tab[c][0][3] = ((fNDIS_evt_b[0][i][j][k]) ? double(fBorn[i][j][k].tab[c][0][3])/double(fNDIS_evt_b[0][i][j][k]*fZ_bin_width[k]) : 0);
          cout << " Hm : " << fBornMult[i][j][k].tab[c][0][3] << endl;

          if(fReMult[i][j][k].tab[c][0][0]<0) fReMult[i][j][k].tab[c][0][0]=0;
          if(fReMult[i][j][k].tab[c][0][1]<0) fReMult[i][j][k].tab[c][0][1]=0;
          if(fReMult[i][j][k].tab[c][0][2]<0) fReMult[i][j][k].tab[c][0][2]=0;
          if(fReMult[i][j][k].tab[c][0][3]<0) fReMult[i][j][k].tab[c][0][3]=0;

          if(fBornMult[i][j][k].tab[c][0][0]<0) fBornMult[i][j][k].tab[c][0][0]=0;
          if(fBornMult[i][j][k].tab[c][0][1]<0) fBornMult[i][j][k].tab[c][0][1]=0;
          if(fBornMult[i][j][k].tab[c][0][2]<0) fBornMult[i][j][k].tab[c][0][2]=0;
          if(fBornMult[i][j][k].tab[c][0][3]<0) fBornMult[i][j][k].tab[c][0][3]=0;


          fMult[i][j][k].tab[c][0][0] = ((fReMult[i][j][k].tab[c][0][0] && fBornMult[i][j][k].tab[c][0][0]) ? (fBornMult[i][j][k].tab[c][0][0]/fReMult[i][j][k].tab[c][0][0]) : 0);
          fMult[i][j][k].tab[c][0][1] = ((fReMult[i][j][k].tab[c][0][1] && fBornMult[i][j][k].tab[c][0][1]) ? (fBornMult[i][j][k].tab[c][0][1]/fReMult[i][j][k].tab[c][0][1]) : 0);
          fMult[i][j][k].tab[c][0][2] = ((fReMult[i][j][k].tab[c][0][2] && fBornMult[i][j][k].tab[c][0][2]) ? (fBornMult[i][j][k].tab[c][0][2]/fReMult[i][j][k].tab[c][0][2]) : 0);
          fMult[i][j][k].tab[c][0][3] = ((fReMult[i][j][k].tab[c][0][3] && fBornMult[i][j][k].tab[c][0][3]) ? (fBornMult[i][j][k].tab[c][0][3]/fReMult[i][j][k].tab[c][0][3]) : 0);


          if(fReMult[i][j][k].tab[c][0][0]==0) fReMult[i][j][k].tab[c][1][0]=0;
          else fReMult[i][j][k].tab[c][1][0] = sqrt(pow(1/sqrt(fRe[i][j][k].tab[c][0][0]),2)+pow(1/sqrt(fNDIS_evt_r[0][i][j][k]),2));
          if(fReMult[i][j][k].tab[c][0][1]==0) fReMult[i][j][k].tab[c][1][1]=0;
          else fReMult[i][j][k].tab[c][1][1] = sqrt(pow(1/sqrt(fRe[i][j][k].tab[c][0][1]),2)+pow(1/sqrt(fNDIS_evt_r[0][i][j][k]),2));
          if(fReMult[i][j][k].tab[c][0][2]==0) fReMult[i][j][k].tab[c][1][2]=0;
          else fReMult[i][j][k].tab[c][1][2] = sqrt(pow(1/sqrt(fRe[i][j][k].tab[c][0][2]),2)+pow(1/sqrt(fNDIS_evt_r[0][i][j][k]),2));
          if(fReMult[i][j][k].tab[c][0][3]==0) fReMult[i][j][k].tab[c][1][3]=0;
          else fReMult[i][j][k].tab[c][1][3] = sqrt(pow(1/sqrt(fRe[i][j][k].tab[c][0][3]),2)+pow(1/sqrt(fNDIS_evt_r[0][i][j][k]),2));

          if(fBornMult[i][j][k].tab[c][0][0]==0) fBornMult[i][j][k].tab[c][1][0]=0;
          else fBornMult[i][j][k].tab[c][1][0] = sqrt(pow(1/sqrt(fBorn[i][j][k].tab[c][0][0]),2)+pow(1/sqrt(fNDIS_evt_r[0][i][j][k]),2));
          if(fBornMult[i][j][k].tab[c][0][1]==0) fBornMult[i][j][k].tab[c][1][1]=0;
          else fBornMult[i][j][k].tab[c][1][1] = sqrt(pow(1/sqrt(fBorn[i][j][k].tab[c][0][1]),2)+pow(1/sqrt(fNDIS_evt_r[0][i][j][k]),2));
          if(fBornMult[i][j][k].tab[c][0][2]==0) fBornMult[i][j][k].tab[c][1][2]=0;
          else fBornMult[i][j][k].tab[c][1][2] = sqrt(pow(1/sqrt(fBorn[i][j][k].tab[c][0][2]),2)+pow(1/sqrt(fNDIS_evt_r[0][i][j][k]),2));
          if(fBornMult[i][j][k].tab[c][0][3]==0) fBornMult[i][j][k].tab[c][1][3]=0;
          else fBornMult[i][j][k].tab[c][1][3] = sqrt(pow(1/sqrt(fBorn[i][j][k].tab[c][0][3]),2)+pow(1/sqrt(fNDIS_evt_r[0][i][j][k]),2));

          if(fMult[i][j][k].tab[c][0][0]==0) fMult[i][j][k].tab[c][1][0]=0;
          else fMult[i][j][k].tab[c][1][0] = sqrt(pow(fBornMult[i][j][k].tab[c][1][0],2)+pow(fReMult[i][j][k].tab[c][1][0],2));
          if(fMult[i][j][k].tab[c][0][1]==0) fMult[i][j][k].tab[c][1][1]=0;
          else fMult[i][j][k].tab[c][1][1] = sqrt(pow(fBornMult[i][j][k].tab[c][1][1],2)+pow(fReMult[i][j][k].tab[c][1][1],2));
          if(fMult[i][j][k].tab[c][0][2]==0) fMult[i][j][k].tab[c][1][2]=0;
          else fMult[i][j][k].tab[c][1][2] = sqrt(pow(fBornMult[i][j][k].tab[c][1][2],2)+pow(fReMult[i][j][k].tab[c][1][2],2));
          if(fMult[i][j][k].tab[c][0][3]==0) fMult[i][j][k].tab[c][1][3]=0;
          else fMult[i][j][k].tab[c][1][3] = sqrt(pow(fBornMult[i][j][k].tab[c][1][3],2)+pow(fReMult[i][j][k].tab[c][1][3],2));


          // if((i==8 && j==0) || (i==0 && j==0) || (i==0 && j==1)
          //                   || (i==0 && j==2) || (i==1 && j==0))
          // {
          //   fReMult[i][j][k].tab[c][0][0] = 0;
          //   fReMult[i][j][k].tab[c][0][1] = 0;
          //   fReMult[i][j][k].tab[c][0][2] = 0;
          //   fReMult[i][j][k].tab[c][0][3] = 0;
          //   fReMult[i][j][k].tab[c][1][0] = 0;
          //   fReMult[i][j][k].tab[c][1][1] = 0;
          //   fReMult[i][j][k].tab[c][1][2] = 0;
          //   fReMult[i][j][k].tab[c][1][3] = 0;
          //
          //   fBornMult[i][j][k].tab[c][0][0] = 0;
          //   fBornMult[i][j][k].tab[c][0][1] = 0;
          //   fBornMult[i][j][k].tab[c][0][2] = 0;
          //   fBornMult[i][j][k].tab[c][0][3] = 0;
          //   fBornMult[i][j][k].tab[c][1][0] = 0;
          //   fBornMult[i][j][k].tab[c][1][1] = 0;
          //   fBornMult[i][j][k].tab[c][1][2] = 0;
          //   fBornMult[i][j][k].tab[c][1][3] = 0;
          //
          //   fMult[i][j][k].tab[c][0][0] = 0;
          //   fMult[i][j][k].tab[c][0][1] = 0;
          //   fMult[i][j][k].tab[c][0][2] = 0;
          //   fMult[i][j][k].tab[c][0][3] = 0;
          //   fMult[i][j][k].tab[c][1][0] = 0;
          //   fMult[i][j][k].tab[c][1][1] = 0;
          //   fMult[i][j][k].tab[c][1][2] = 0;
          //   fMult[i][j][k].tab[c][1][3] = 0;
          // }

          if((k==4 && j==5))
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

            fMult[i][j][k].tab[c][0][0] = 0;
            fMult[i][j][k].tab[c][0][1] = 0;
            fMult[i][j][k].tab[c][0][2] = 0;
            fMult[i][j][k].tab[c][0][3] = 0;
            fMult[i][j][k].tab[c][1][0] = 0;
            fMult[i][j][k].tab[c][1][1] = 0;
            fMult[i][j][k].tab[c][1][2] = 0;
            fMult[i][j][k].tab[c][1][3] = 0;
          }

          //Here save your Mult for K test activated by a hidden flag.
          if(KMultSave_flag && fReMult[i][j][k].tab[c][0][1])
          {
            double Q2_av = average(fQ2[c][i][j][k]);
            if(!c)
            {
              ofs_multKp << (fXrange[i]+fXrange[i+1])/2 << " " << fXrange[i] << " " << fXrange[i+1] << " "
              << (fYrange[j]+fYrange[j+1])/2 << " " << fYrange[j] << " " << fYrange[j+1] << " "
              << Q2_av << " "
              << (fZrange[k]+fZrange[k+1])/2 << " " << fZrange[k] << " " << fZrange[k+1] << " "
              << fReMult[i][j][k].tab[c][0][1] << " 0 0 0 0 " << endl;
            }
            else
            {
              ofs_multKm << (fXrange[i]+fXrange[i+1])/2 << " " << fXrange[i] << " " << fXrange[i+1] << " "
              << (fYrange[j]+fYrange[j+1])/2 << " " << fYrange[j] << " " << fYrange[j+1] << " "
              << Q2_av << " "
              << (fZrange[k]+fZrange[k+1])/2 << " " << fZrange[k] << " " << fZrange[k+1] << " "
              << fReMult[i][j][k].tab[c][0][1] << " 0 0 0 0 " << endl;
            }
          }

          shout << fXrange[i] << "\t" << fYrange[j] << "\t" << fZrange[k]
                << "\t" << fReMult[i][j][k].tab[c][0][3]
                << "\t" << fReMult[i][j][k].tab[c][1][3]
                << "\t" << fBornMult[i][j][k].tab[c][0][3]
                << "\t" << fBornMult[i][j][k].tab[c][1][3]
                << "\t" << fMult[i][j][k].tab[c][0][3]
                << "\t" << fMult[i][j][k].tab[c][1][3] << endl;

          fMult[i][j][k].tab[c][0][0] = ((fMult[i][j][k].tab[c][0][0]) ? (fMult[i][j][k].tab[c][0][0]+j*0.1) : 0);
          fMult[i][j][k].tab[c][0][1] = ((fMult[i][j][k].tab[c][0][1]) ? (fMult[i][j][k].tab[c][0][1]+j*0.1) : 0);
          fMult[i][j][k].tab[c][0][2] = ((fMult[i][j][k].tab[c][0][2]) ? (fMult[i][j][k].tab[c][0][2]+j*0.1) : 0);
          fMult[i][j][k].tab[c][0][3] = ((fMult[i][j][k].tab[c][0][3]) ? (fMult[i][j][k].tab[c][0][3]+j*0.1) : 0);

          p_a_r.push_back(fReMult[i][j][k].tab[c][0][0]);
          k_a_r.push_back(fReMult[i][j][k].tab[c][0][1]);
          h_a_r.push_back(fReMult[i][j][k].tab[c][0][3]);
          p_a_b.push_back(fBornMult[i][j][k].tab[c][0][0]);
          k_a_b.push_back(fBornMult[i][j][k].tab[c][0][1]);
          h_a_b.push_back(fBornMult[i][j][k].tab[c][0][3]);
          p_a_d.push_back(fMult[i][j][k].tab[c][0][0]);
          k_a_d.push_back(fMult[i][j][k].tab[c][0][1]);
          h_a_d.push_back(fMult[i][j][k].tab[c][0][3]);

          p_err_r.push_back(fReMult[i][j][k].tab[c][1][0]);
          k_err_r.push_back(fReMult[i][j][k].tab[c][1][1]);
          h_err_r.push_back(fReMult[i][j][k].tab[c][1][3]);
          p_err_b.push_back(fBornMult[i][j][k].tab[c][1][0]);
          k_err_b.push_back(fBornMult[i][j][k].tab[c][1][1]);
          h_err_b.push_back(fBornMult[i][j][k].tab[c][1][3]);
          p_err_d.push_back(fMult[i][j][k].tab[c][1][0]);
          k_err_d.push_back(fMult[i][j][k].tab[c][1][1]);
          h_err_d.push_back(fMult[i][j][k].tab[c][1][3]);

        }

        for(int k=0; k<12; k++)
        {

          int max_el = *max_element(fNDIS_evt_r[0][i][j],fNDIS_evt_r[0][i][j]+14);

          fReptMult[i][j][k].tab[c][0][0] = ((max_el) ? double(fRept[i][j][k].tab[c][0][0])/double(max_el*fPt_bin_width) : 0);
          fReptMult[i][j][k].tab[c][0][1] = ((max_el) ? double(fRept[i][j][k].tab[c][0][1])/double(max_el*fPt_bin_width) : 0);
          fReptMult[i][j][k].tab[c][0][2] = ((max_el) ? double(fRept[i][j][k].tab[c][0][2])/double(max_el*fPt_bin_width) : 0);
          fReptMult[i][j][k].tab[c][0][3] = ((max_el) ? double(fRept[i][j][k].tab[c][0][3])/double(max_el*fPt_bin_width) : 0);

          fBornptMult[i][j][k].tab[c][0][0] = ((max_el) ? double(fBornpt[i][j][k].tab[c][0][0])/double(max_el*fPt_bin_width) : 0);
          fBornptMult[i][j][k].tab[c][0][1] = ((max_el) ? double(fBornpt[i][j][k].tab[c][0][1])/double(max_el*fPt_bin_width) : 0);
          fBornptMult[i][j][k].tab[c][0][2] = ((max_el) ? double(fBornpt[i][j][k].tab[c][0][2])/double(max_el*fPt_bin_width) : 0);
          fBornptMult[i][j][k].tab[c][0][3] = ((max_el) ? double(fBornpt[i][j][k].tab[c][0][3])/double(max_el*fPt_bin_width) : 0);

          if(fReptMult[i][j][k].tab[c][0][0]<0) fReptMult[i][j][k].tab[c][0][0]=0;
          if(fReptMult[i][j][k].tab[c][0][1]<0) fReptMult[i][j][k].tab[c][0][1]=0;
          if(fReptMult[i][j][k].tab[c][0][2]<0) fReptMult[i][j][k].tab[c][0][2]=0;
          if(fReptMult[i][j][k].tab[c][0][3]<0) fReptMult[i][j][k].tab[c][0][3]=0;

          if(fBornptMult[i][j][k].tab[c][0][0]<0) fBornptMult[i][j][k].tab[c][0][0]=0;
          if(fBornptMult[i][j][k].tab[c][0][1]<0) fBornptMult[i][j][k].tab[c][0][1]=0;
          if(fBornptMult[i][j][k].tab[c][0][2]<0) fBornptMult[i][j][k].tab[c][0][2]=0;
          if(fBornptMult[i][j][k].tab[c][0][3]<0) fBornptMult[i][j][k].tab[c][0][3]=0;

          fPtMult[i][j][k].tab[c][0][0] = ((fReptMult[i][j][k].tab[c][0][0] && fBornptMult[i][j][k].tab[c][0][0]) ? (fBornptMult[i][j][k].tab[c][0][0]/fReptMult[i][j][k].tab[c][0][0])+j*0.1 : 0);
          fPtMult[i][j][k].tab[c][0][1] = ((fReptMult[i][j][k].tab[c][0][1] && fBornptMult[i][j][k].tab[c][0][1]) ? (fBornptMult[i][j][k].tab[c][0][1]/fReptMult[i][j][k].tab[c][0][1])+j*0.1 : 0);
          fPtMult[i][j][k].tab[c][0][2] = ((fReptMult[i][j][k].tab[c][0][2] && fBornptMult[i][j][k].tab[c][0][2]) ? (fBornptMult[i][j][k].tab[c][0][2]/fReptMult[i][j][k].tab[c][0][2])+j*0.1 : 0);
          fPtMult[i][j][k].tab[c][0][3] = ((fReptMult[i][j][k].tab[c][0][3] && fBornptMult[i][j][k].tab[c][0][3]) ? (fBornptMult[i][j][k].tab[c][0][3]/fReptMult[i][j][k].tab[c][0][3])+j*0.1 : 0);

          if(fReptMult[i][j][k].tab[c][0][0]==0) fReptMult[i][j][k].tab[c][1][0]=0;
          else fReptMult[i][j][k].tab[c][1][0] = sqrt(pow(1/sqrt(fRept[i][j][k].tab[c][0][0]),2)+pow(1/sqrt(max_el),2));
          if(fReptMult[i][j][k].tab[c][0][1]==0) fReptMult[i][j][k].tab[c][1][1]=0;
          else fReptMult[i][j][k].tab[c][1][1] = sqrt(pow(1/sqrt(fRept[i][j][k].tab[c][0][1]),2)+pow(1/sqrt(max_el),2));
          if(fReptMult[i][j][k].tab[c][0][2]==0) fReptMult[i][j][k].tab[c][1][2]=0;
          else fReptMult[i][j][k].tab[c][1][2] = sqrt(pow(1/sqrt(fRept[i][j][k].tab[c][0][2]),2)+pow(1/sqrt(max_el),2));
          if(fReptMult[i][j][k].tab[c][0][3]==0) fReptMult[i][j][k].tab[c][1][3]=0;
          else fReptMult[i][j][k].tab[c][1][3] = sqrt(pow(1/sqrt(fRept[i][j][k].tab[c][0][3]),2)+pow(1/sqrt(max_el),2));

          if(fBornptMult[i][j][k].tab[c][0][0]==0) fBornptMult[i][j][k].tab[c][1][0]=0;
          else fBornptMult[i][j][k].tab[c][1][0] = sqrt(pow(1/sqrt(fBornpt[i][j][k].tab[c][0][0]),2)+pow(1/sqrt(max_el),2));
          if(fBornptMult[i][j][k].tab[c][0][1]==0) fBornptMult[i][j][k].tab[c][1][1]=0;
          else fBornptMult[i][j][k].tab[c][1][1] = sqrt(pow(1/sqrt(fBornpt[i][j][k].tab[c][0][1]),2)+pow(1/sqrt(max_el),2));
          if(fBornptMult[i][j][k].tab[c][0][2]==0) fBornptMult[i][j][k].tab[c][1][2]=0;
          else fBornptMult[i][j][k].tab[c][1][2] = sqrt(pow(1/sqrt(fBornpt[i][j][k].tab[c][0][2]),2)+pow(1/sqrt(max_el),2));
          if(fBornptMult[i][j][k].tab[c][0][3]==0) fBornptMult[i][j][k].tab[c][1][3]=0;
          else fBornptMult[i][j][k].tab[c][1][3] = sqrt(pow(1/sqrt(fBornpt[i][j][k].tab[c][0][3]),2)+pow(1/sqrt(max_el),2));

          if(fPtMult[i][j][k].tab[c][0][0]==0) fPtMult[i][j][k].tab[c][1][0]=0;
          else fPtMult[i][j][k].tab[c][1][0] = sqrt(pow(fBornptMult[i][j][k].tab[c][1][0],2)+pow(fReptMult[i][j][k].tab[c][1][0],2));
          if(fPtMult[i][j][k].tab[c][0][1]==0) fPtMult[i][j][k].tab[c][1][1]=0;
          else fPtMult[i][j][k].tab[c][1][1] = sqrt(pow(fBornptMult[i][j][k].tab[c][1][1],2)+pow(fReptMult[i][j][k].tab[c][1][1],2));
          if(fPtMult[i][j][k].tab[c][0][2]==0) fPtMult[i][j][k].tab[c][1][2]=0;
          else fPtMult[i][j][k].tab[c][1][2] = sqrt(pow(fBornptMult[i][j][k].tab[c][1][2],2)+pow(fReptMult[i][j][k].tab[c][1][2],2));
          if(fPtMult[i][j][k].tab[c][0][3]==0) fPtMult[i][j][k].tab[c][1][3]=0;
          else fPtMult[i][j][k].tab[c][1][3] = sqrt(pow(fBornptMult[i][j][k].tab[c][1][3],2)+pow(fReptMult[i][j][k].tab[c][1][3],2));


          if((i==7 && j==4) || (i==8 && j==0) || (i==8 && j==4)
                            || (i==0 && j==0) || (i==0 && j==1)
                            || (i==0 && j==2) || (i==1 && j==0))
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

            fPtMult[i][j][k].tab[c][0][0] = 0;
            fPtMult[i][j][k].tab[c][0][1] = 0;
            fPtMult[i][j][k].tab[c][0][2] = 0;
            fPtMult[i][j][k].tab[c][0][3] = 0;
            fPtMult[i][j][k].tab[c][1][0] = 0;
            fPtMult[i][j][k].tab[c][1][1] = 0;
            fPtMult[i][j][k].tab[c][1][2] = 0;
            fPtMult[i][j][k].tab[c][1][3] = 0;
          }

          p_pt_r.push_back(fReptMult[i][j][k].tab[c][0][0]);
          k_pt_r.push_back(fReptMult[i][j][k].tab[c][0][1]);
          h_pt_r.push_back(fReptMult[i][j][k].tab[c][0][3]);
          p_pt_b.push_back(fBornptMult[i][j][k].tab[c][0][0]);
          k_pt_b.push_back(fBornptMult[i][j][k].tab[c][0][1]);
          h_pt_b.push_back(fBornptMult[i][j][k].tab[c][0][3]);
          p_pt_d.push_back(fPtMult[i][j][k].tab[c][0][0]);
          k_pt_d.push_back(fPtMult[i][j][k].tab[c][0][1]);
          h_pt_d.push_back(fPtMult[i][j][k].tab[c][0][3]);

          p_pt_err_r.push_back(fReptMult[i][j][k].tab[c][1][0]);
          k_pt_err_r.push_back(fReptMult[i][j][k].tab[c][1][1]);
          h_pt_err_r.push_back(fReptMult[i][j][k].tab[c][1][3]);
          p_pt_err_b.push_back(fBornptMult[i][j][k].tab[c][1][0]);
          k_pt_err_b.push_back(fBornptMult[i][j][k].tab[c][1][1]);
          h_pt_err_b.push_back(fBornptMult[i][j][k].tab[c][1][3]);
          p_pt_err_d.push_back(fPtMult[i][j][k].tab[c][1][0]);
          k_pt_err_d.push_back(fPtMult[i][j][k].tab[c][1][1]);
          h_pt_err_d.push_back(fPtMult[i][j][k].tab[c][1][3]);

        }

        for(int k=14; k>0; k--)
        {
          if(!p_a_r[k-1]) {p_a_r.erase(p_a_r.begin()+k-1); p_err_r.erase(p_err_r.begin()+k-1); z_range_p_r.erase(z_range_p_r.begin()+k-1);}
          if(!p_a_b[k-1]) {p_a_b.erase(p_a_b.begin()+k-1); p_err_b.erase(p_err_b.begin()+k-1); z_range_p_b.erase(z_range_p_b.begin()+k-1);}
          if(!p_a_d[k-1]) {p_a_d.erase(p_a_d.begin()+k-1); p_err_d.erase(p_err_d.begin()+k-1); z_range_p_d.erase(z_range_p_d.begin()+k-1);}
          if(!k_a_r[k-1]) {k_a_r.erase(k_a_r.begin()+k-1); k_err_r.erase(k_err_r.begin()+k-1); z_range_k_r.erase(z_range_k_r.begin()+k-1);}
          if(!k_a_b[k-1]) {k_a_b.erase(k_a_b.begin()+k-1); k_err_b.erase(k_err_b.begin()+k-1); z_range_k_b.erase(z_range_k_b.begin()+k-1);}
          if(!k_a_d[k-1]) {k_a_d.erase(k_a_d.begin()+k-1); k_err_d.erase(k_err_d.begin()+k-1); z_range_k_d.erase(z_range_k_d.begin()+k-1);}
          if(!h_a_r[k-1]) {h_a_r.erase(h_a_r.begin()+k-1); h_err_r.erase(h_err_r.begin()+k-1); z_range_h_r.erase(z_range_h_r.begin()+k-1);}
          if(!h_a_b[k-1]) {h_a_b.erase(h_a_b.begin()+k-1); h_err_b.erase(h_err_b.begin()+k-1); z_range_h_b.erase(z_range_h_b.begin()+k-1);}
          if(!h_a_d[k-1]) {h_a_d.erase(h_a_d.begin()+k-1); h_err_d.erase(h_err_d.begin()+k-1); z_range_h_d.erase(z_range_h_d.begin()+k-1);}
        }

        for(int k=12; k>0; k--)
        {
          if(!p_pt_r[k-1]) {p_pt_r.erase(p_pt_r.begin()+k-1); p_pt_err_r.erase(p_pt_err_r.begin()+k-1); pt_range_p_r.erase(pt_range_p_r.begin()+k-1);}
          if(!p_pt_b[k-1]) {p_pt_b.erase(p_pt_b.begin()+k-1); p_pt_err_b.erase(p_pt_err_b.begin()+k-1); pt_range_p_b.erase(pt_range_p_b.begin()+k-1);}
          if(!p_pt_d[k-1]) {p_pt_d.erase(p_pt_d.begin()+k-1); p_pt_err_d.erase(p_pt_err_d.begin()+k-1); pt_range_p_d.erase(pt_range_p_d.begin()+k-1);}
          if(!k_pt_r[k-1]) {k_pt_r.erase(k_pt_r.begin()+k-1); k_pt_err_r.erase(k_pt_err_r.begin()+k-1); pt_range_k_r.erase(pt_range_k_r.begin()+k-1);}
          if(!k_pt_b[k-1]) {k_pt_b.erase(k_pt_b.begin()+k-1); k_pt_err_b.erase(k_pt_err_b.begin()+k-1); pt_range_k_b.erase(pt_range_k_b.begin()+k-1);}
          if(!k_pt_d[k-1]) {k_pt_d.erase(k_pt_d.begin()+k-1); k_pt_err_d.erase(k_pt_err_d.begin()+k-1); pt_range_k_d.erase(pt_range_k_d.begin()+k-1);}
          if(!h_pt_r[k-1]) {h_pt_r.erase(h_pt_r.begin()+k-1); h_pt_err_r.erase(h_pt_err_r.begin()+k-1); pt_range_h_r.erase(pt_range_h_r.begin()+k-1);}
          if(!h_pt_b[k-1]) {h_pt_b.erase(h_pt_b.begin()+k-1); h_pt_err_b.erase(h_pt_err_b.begin()+k-1); pt_range_h_b.erase(pt_range_h_b.begin()+k-1);}
          if(!h_pt_d[k-1]) {h_pt_d.erase(h_pt_d.begin()+k-1); h_pt_err_d.erase(h_pt_err_d.begin()+k-1); pt_range_h_d.erase(pt_range_h_d.begin()+k-1);}
        }

        bool p_a_r_empty = 0;
        bool k_a_r_empty = 0;
        bool h_a_r_empty = 0;
        bool p_a_b_empty = 0;
        bool k_a_b_empty = 0;
        bool h_a_b_empty = 0;
        bool p_a_d_empty = 0;
        bool k_a_d_empty = 0;
        bool h_a_d_empty = 0;

        bool p_pt_r_empty = 0;
        bool k_pt_r_empty = 0;
        bool h_pt_r_empty = 0;
        bool p_pt_b_empty = 0;
        bool k_pt_b_empty = 0;
        bool h_pt_b_empty = 0;
        bool p_pt_d_empty = 0;
        bool k_pt_d_empty = 0;
        bool h_pt_d_empty = 0;

        if(!(int(p_a_r.size()))) p_pt_r_empty = 1;
        if(!(int(k_a_r.size()))) k_pt_r_empty = 1;
        if(!(int(h_a_r.size()))) h_pt_r_empty = 1;
        if(!(int(p_a_b.size()))) p_pt_b_empty = 1;
        if(!(int(k_a_b.size()))) k_pt_b_empty = 1;
        if(!(int(h_a_b.size()))) h_pt_b_empty = 1;
        if(!(int(p_a_d.size()))) p_pt_d_empty = 1;
        if(!(int(k_a_d.size()))) k_pt_d_empty = 1;
        if(!(int(h_a_d.size()))) h_pt_d_empty = 1;

        if(!(int(p_pt_r.size()))) p_pt_r_empty = 1;
        if(!(int(k_pt_r.size()))) k_pt_r_empty = 1;
        if(!(int(h_pt_r.size()))) h_pt_r_empty = 1;
        if(!(int(p_pt_b.size()))) p_pt_b_empty = 1;
        if(!(int(k_pt_b.size()))) k_pt_b_empty = 1;
        if(!(int(h_pt_b.size()))) h_pt_b_empty = 1;
        if(!(int(p_pt_d.size()))) p_pt_d_empty = 1;
        if(!(int(k_pt_d.size()))) k_pt_d_empty = 1;
        if(!(int(h_pt_d.size()))) h_pt_d_empty = 1;

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
              H_r[c][i][j]->SetMaximum(4.);
            }
            else
            {
              H_r[c][i][j]->Draw("SAMEP");
              H_r[c][i][j]->GetXaxis()->SetLimits(0.1,0.9);
              H_r[c][i][j]->SetMinimum(0.);
              H_r[c][i][j]->SetMaximum(4.);
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
            H_b[c][i][j]->SetMaximum(4.);
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
              P_r[c][i][j]->SetMaximum(4.);
            }
            else
            {
              P_r[c][i][j]->Draw("SAMEP");
              P_r[c][i][j]->GetXaxis()->SetLimits(0.1,0.9);
              P_r[c][i][j]->SetMinimum(0.0);
              P_r[c][i][j]->SetMaximum(4.);
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
            P_b[c][i][j]->SetMaximum(4.);
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
              K_r[c][i][j]->SetMaximum(4.);
            }
            else
            {
              K_r[c][i][j]->Draw("SAMEP");
              K_r[c][i][j]->GetXaxis()->SetLimits(0.1,0.9);
              K_r[c][i][j]->SetMinimum(0.0);
              K_r[c][i][j]->SetMaximum(4.);
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
            K_b[c][i][j]->SetMaximum(4.);
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
              H_pt_r[c][i][j]->GetXaxis()->SetLimits(0.,2.0);
              H_pt_r[c][i][j]->SetMinimum(0.);
              H_pt_r[c][i][j]->SetMaximum(3.);
            }
            else
            {
              H_pt_r[c][i][j]->Draw("SAMEP");
              H_pt_r[c][i][j]->GetXaxis()->SetLimits(0.,2.0);
              H_pt_r[c][i][j]->SetMinimum(0.);
              H_pt_r[c][i][j]->SetMaximum(3.);
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
            H_pt_b[c][i][j]->GetXaxis()->SetLimits(0.,2.0);
            H_pt_b[c][i][j]->SetMinimum(0.);
            H_pt_b[c][i][j]->SetMaximum(3.);
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
              P_pt_r[c][i][j]->GetXaxis()->SetLimits(0.,2.0);
              P_pt_r[c][i][j]->SetMinimum(0.0);
              P_pt_r[c][i][j]->SetMaximum(3.);
            }
            else
            {
              P_pt_r[c][i][j]->Draw("SAMEP");
              P_pt_r[c][i][j]->GetXaxis()->SetLimits(0.,2.0);
              P_pt_r[c][i][j]->SetMinimum(0.0);
              P_pt_r[c][i][j]->SetMaximum(3.);
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
            P_pt_b[c][i][j]->GetXaxis()->SetLimits(0.,2.0);
            P_pt_b[c][i][j]->SetMinimum(0.0);
            P_pt_b[c][i][j]->SetMaximum(3.);
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
              K_pt_r[c][i][j]->GetXaxis()->SetLimits(0.,2.0);
              K_pt_r[c][i][j]->SetMinimum(0.0);
              K_pt_r[c][i][j]->SetMaximum(3.);
            }
            else
            {
              K_pt_r[c][i][j]->Draw("SAMEP");
              K_pt_r[c][i][j]->GetXaxis()->SetLimits(0.,2.0);
              K_pt_r[c][i][j]->SetMinimum(0.0);
              K_pt_r[c][i][j]->SetMaximum(3.);
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
            K_pt_b[c][i][j]->GetXaxis()->SetLimits(0.,2.0);
            K_pt_b[c][i][j]->SetMinimum(0.0);
            K_pt_b[c][i][j]->SetMaximum(3.);
          }
          c6.Update();
        }

        H_d[c][i][j] = new TGraphErrors(int(h_a_d.size()),&(z_range_h_d[0]),&(h_a_d[0]),0,&(h_err_d[0]));
        P_d[c][i][j] = new TGraphErrors(int(p_a_d.size()),&(z_range_p_d[0]),&(p_a_d[0]),0,&(p_err_d[0]));
        K_d[c][i][j] = new TGraphErrors(int(k_a_d.size()),&(z_range_k_d[0]),&(k_a_d[0]),0,&(k_err_d[0]));

        H_d[c][i][j]->SetMarkerColor(fMarkerColorAlt[j]);
        P_d[c][i][j]->SetMarkerColor(fMarkerColorAlt[j]);
        K_d[c][i][j]->SetMarkerColor(fMarkerColorAlt[j]);

        H_d[c][i][j]->SetMarkerSize(3);
        P_d[c][i][j]->SetMarkerSize(3);
        K_d[c][i][j]->SetMarkerSize(3);

        H_d[c][i][j]->SetMarkerStyle(fMarkerStyleAlt[j][c]);
        P_d[c][i][j]->SetMarkerStyle(fMarkerStyleAlt[j][c]);
        K_d[c][i][j]->SetMarkerStyle(fMarkerStyleAlt[j][c]);

        H_d[c][i][j]->GetYaxis()->SetTitle("");
        P_d[c][i][j]->GetYaxis()->SetTitle("");
        K_d[c][i][j]->GetYaxis()->SetTitle("");

        H_d[c][i][j]->GetXaxis()->SetTitle("");
        P_d[c][i][j]->GetXaxis()->SetTitle("");
        K_d[c][i][j]->GetXaxis()->SetTitle("");

        H_d[c][i][j]->SetTitle("");
        P_d[c][i][j]->SetTitle("");
        K_d[c][i][j]->SetTitle("");

        if(!h_a_d_empty)
        {
          c7.cd(i+1);
          gPad->SetFillStyle(4000);
          if(H_d[c][i][j])
          {
            if(!c && j==3)
            {
              H_d[c][i][j]->Draw("SAMEPA");
              H_d[c][i][j]->GetXaxis()->SetLimits(-0.05,1.05);
              H_d[c][i][j]->SetMinimum(0.85);
              H_d[c][i][j]->SetMaximum(1.65);
              H_d[c][i][j]->GetXaxis()->SetLabelSize(0.06);
              H_d[c][i][j]->GetYaxis()->SetLabelSize(0.06);
              H_d[c][i][j]->SetTitle("");
              if(i>4) gPad->SetBottomMargin(.15);
              if(i==0 || i==5) gPad->SetLeftMargin(.22);
              if(i==8)
              {
                H_d[c][i][j]->GetXaxis()->SetTitle("#font[ 12]{z}");
                H_d[c][i][j]->GetXaxis()->SetTitleSize(0.08);
                H_d[c][i][j]->GetXaxis()->SetTitleOffset(.8);
              }
              H_d[c][i][j]->GetXaxis()->SetNdivisions(304,kTRUE);
              H_d[c][i][j]->GetYaxis()->SetNdivisions(304,kTRUE);
              if(i==0)
              {
                H_d[c][i][j]->GetYaxis()->SetTitle("#font[12]{#eta}^{#font[ 12]{h}}+ #font[ 12]{#delta}");
                H_d[c][i][j]->GetYaxis()->SetTitleSize(0.08);
              }
              H_d[c][i][0]->Draw("SAMEP");
              H_d[c][i][0]->GetXaxis()->SetLimits(-0.05,1.05);
              H_d[c][i][0]->SetMinimum(0.85);
              H_d[c][i][0]->SetMaximum(1.65);
              H_d[c][i][1]->Draw("SAMEP");
              H_d[c][i][1]->GetXaxis()->SetLimits(-0.05,1.05);
              H_d[c][i][1]->SetMinimum(0.85);
              H_d[c][i][1]->SetMaximum(1.65);
              H_d[c][i][2]->Draw("SAMEP");
              H_d[c][i][2]->GetXaxis()->SetLimits(-0.05,1.05);
              H_d[c][i][2]->SetMinimum(0.85);
              H_d[c][i][2]->SetMaximum(1.65);
              c7.Range(.0,.85,1.,1.65);
              l1[0]->Draw();
              l1[1]->Draw();
              l1[2]->Draw();
              l1[3]->Draw();
              l1[4]->Draw();
              l1[5]->Draw();
            }
            else
            {
              H_d[c][i][j]->Draw("SAMEP");
              H_d[c][i][j]->GetXaxis()->SetLimits(-0.05,1.05);
              H_d[c][i][j]->SetMinimum(0.85);
              H_d[c][i][j]->SetMaximum(1.65);
            }
          }
          c7.Update();
        }

        if(!p_a_d_empty)
        {
          c8.cd(i+1);
          gPad->SetFillStyle(4000);
          if(P_d[c][i][j])
          {
            if(!c && j==3)
            {
              P_d[c][i][j]->Draw("SAMEPA");
              P_d[c][i][j]->GetXaxis()->SetLimits(-0.05,1.05);
              P_d[c][i][j]->SetMinimum(0.85);
              P_d[c][i][j]->SetMaximum(1.65);
              P_d[c][i][j]->GetXaxis()->SetLabelSize(0.06);
              P_d[c][i][j]->GetYaxis()->SetLabelSize(0.06);
              P_d[c][i][j]->SetTitle("");
              if(i>4) gPad->SetBottomMargin(.15);
              if(i==0 || i==5) gPad->SetLeftMargin(.22);
              if(i==8)
              {
                P_d[c][i][j]->GetXaxis()->SetTitle("#font[ 12]{z}");
                P_d[c][i][j]->GetXaxis()->SetTitleSize(0.08);
                P_d[c][i][j]->GetXaxis()->SetTitleOffset(.8);
              }
              P_d[c][i][j]->GetXaxis()->SetNdivisions(304,kTRUE);
              P_d[c][i][j]->GetYaxis()->SetNdivisions(304,kTRUE);
              if(i==0)
              {
                P_d[c][i][j]->GetYaxis()->SetTitle("#font[12]{#eta}^{#font[ 12]{#pi}}+ #font[ 12]{#delta}");
                P_d[c][i][j]->GetYaxis()->SetTitleSize(0.08);
              }
              P_d[c][i][0]->Draw("SAMEP");
              P_d[c][i][0]->GetXaxis()->SetLimits(-0.05,1.05);
              P_d[c][i][0]->SetMinimum(0.85);
              P_d[c][i][0]->SetMaximum(1.65);
              P_d[c][i][1]->Draw("SAMEP");
              P_d[c][i][1]->GetXaxis()->SetLimits(-0.05,1.05);
              P_d[c][i][1]->SetMinimum(0.85);
              P_d[c][i][1]->SetMaximum(1.65);
              P_d[c][i][2]->Draw("SAMEP");
              P_d[c][i][2]->GetXaxis()->SetLimits(-0.05,1.05);
              P_d[c][i][2]->SetMinimum(0.85);
              P_d[c][i][2]->SetMaximum(1.65);
              c8.Range(0.,.85,1.,1.65);
              l1[0]->Draw();
              l1[1]->Draw();
              l1[2]->Draw();
              l1[3]->Draw();
              l1[4]->Draw();
              l1[5]->Draw();
            }
            else
            {
              P_d[c][i][j]->Draw("SAMEP");
              P_d[c][i][j]->GetXaxis()->SetLimits(-0.05,1.05);
              P_d[c][i][j]->SetMinimum(0.85);
              P_d[c][i][j]->SetMaximum(1.55);
            }
          }
          c8.Update();
        }

        if(!k_a_d_empty)
        {
          c9.cd(i+1);
          gPad->SetFillStyle(4000);
          if(K_d[c][i][j])
          {
            if(!c && j==3)
            {
              K_d[c][i][j]->Draw("SAMEPA");
              K_d[c][i][j]->GetXaxis()->SetLimits(-0.05,1.05);
              K_d[c][i][j]->SetMinimum(0.85);
              K_d[c][i][j]->SetMaximum(1.65);
              K_d[c][i][j]->GetXaxis()->SetLabelSize(0.06);
              K_d[c][i][j]->GetYaxis()->SetLabelSize(0.06);
              K_d[c][i][j]->SetTitle("");
              if(i>4) gPad->SetBottomMargin(.15);
              if(i==0 || i==5) gPad->SetLeftMargin(.22);
              if(i==8)
              {
                K_d[c][i][j]->GetXaxis()->SetTitle("#font[ 12]{z}");
                K_d[c][i][j]->GetXaxis()->SetTitleSize(0.08);
                K_d[c][i][j]->GetXaxis()->SetTitleOffset(.8);
              }
              K_d[c][i][j]->GetXaxis()->SetNdivisions(304,kTRUE);
              K_d[c][i][j]->GetYaxis()->SetNdivisions(304,kTRUE);
              if(i==0)
              {
                K_d[c][i][j]->GetYaxis()->SetTitle("#font[12]{#eta}^{#font[ 12]{K}}+ #font[ 12]{#delta}");
                K_d[c][i][j]->GetYaxis()->SetTitleSize(0.08);
              }
              K_d[c][i][0]->Draw("SAMEP");
              K_d[c][i][0]->GetXaxis()->SetLimits(-0.05,1.05);
              K_d[c][i][0]->SetMinimum(0.85);
              K_d[c][i][0]->SetMaximum(1.65);
              K_d[c][i][1]->Draw("SAMEP");
              K_d[c][i][1]->GetXaxis()->SetLimits(-0.05,1.05);
              K_d[c][i][1]->SetMinimum(0.85);
              K_d[c][i][1]->SetMaximum(1.65);
              K_d[c][i][2]->Draw("SAMEP");
              K_d[c][i][2]->GetXaxis()->SetLimits(-0.05,1.05);
              K_d[c][i][2]->SetMinimum(0.85);
              K_d[c][i][2]->SetMaximum(1.65);
              c9.Range(0.,.85,1.,1.65);
              l1[0]->Draw();
              l1[1]->Draw();
              l1[2]->Draw();
              l1[3]->Draw();
              l1[4]->Draw();
              l1[5]->Draw();
            }
            else
            {
              K_d[c][i][j]->Draw("SAMEP");
              K_d[c][i][j]->GetXaxis()->SetLimits(-0.05,1.05);
              K_d[c][i][j]->SetMinimum(0.85);
              K_d[c][i][j]->SetMaximum(1.65);
            }
          }
          c9.Update();
        }

        H_pt_d[c][i][j] = new TGraphErrors(int(h_pt_d.size()),&(pt_range_h_d[0]),&(h_pt_d[0]),0,&(h_pt_err_d[0]));
        P_pt_d[c][i][j] = new TGraphErrors(int(p_pt_d.size()),&(pt_range_p_d[0]),&(p_pt_d[0]),0,&(p_pt_err_d[0]));
        K_pt_d[c][i][j] = new TGraphErrors(int(k_pt_d.size()),&(pt_range_k_d[0]),&(k_pt_d[0]),0,&(k_pt_err_d[0]));

        H_pt_d[c][i][j]->SetMarkerColor(fMarkerColorAlt[j]);
        P_pt_d[c][i][j]->SetMarkerColor(fMarkerColorAlt[j]);
        K_pt_d[c][i][j]->SetMarkerColor(fMarkerColorAlt[j]);

        H_pt_d[c][i][j]->SetMarkerSize(3);
        P_pt_d[c][i][j]->SetMarkerSize(3);
        K_pt_d[c][i][j]->SetMarkerSize(3);

        H_pt_d[c][i][j]->SetMarkerStyle(fMarkerStyleAlt[j][c]);
        P_pt_d[c][i][j]->SetMarkerStyle(fMarkerStyleAlt[j][c]);
        K_pt_d[c][i][j]->SetMarkerStyle(fMarkerStyleAlt[j][c]);

        H_pt_d[c][i][j]->GetYaxis()->SetTitle("");
        P_pt_d[c][i][j]->GetYaxis()->SetTitle("");
        K_pt_d[c][i][j]->GetYaxis()->SetTitle("");

        H_pt_d[c][i][j]->GetXaxis()->SetTitle("");
        P_pt_d[c][i][j]->GetXaxis()->SetTitle("");
        K_pt_d[c][i][j]->GetXaxis()->SetTitle("");

        H_pt_d[c][i][j]->SetTitle("");
        P_pt_d[c][i][j]->SetTitle("");
        K_pt_d[c][i][j]->SetTitle("");

        if(!h_pt_d_empty)
        {
          c10.cd(i+1);
          gPad->SetFillStyle(4000);
          if(H_pt_d[c][i][j])
          {
            if(!c && j==3)
            {
              H_pt_d[c][i][j]->Draw("SAMEPA");
              H_pt_d[c][i][j]->GetXaxis()->SetLimits(0,3);
              H_pt_d[c][i][j]->SetMinimum(0.85);
              H_pt_d[c][i][j]->SetMaximum(1.65);
              H_pt_d[c][i][j]->GetXaxis()->SetLabelSize(0.06);
              H_pt_d[c][i][j]->GetYaxis()->SetLabelSize(0.06);
              H_pt_d[c][i][j]->SetTitle("");
              if(i>4) gPad->SetBottomMargin(.15);
              if(i==0 || i==5) gPad->SetLeftMargin(.22);
              if(i==8)
              {
                H_pt_d[c][i][j]->GetXaxis()->SetTitle("#font[ 12]{p_{T}}");
                H_pt_d[c][i][j]->GetXaxis()->SetTitleSize(0.08);
                H_pt_d[c][i][j]->GetXaxis()->SetTitleOffset(.8);
              }
              H_pt_d[c][i][j]->GetXaxis()->SetNdivisions(304,kTRUE);
              H_pt_d[c][i][j]->GetYaxis()->SetNdivisions(304,kTRUE);
              if(i==0)
              {
                H_pt_d[c][i][j]->GetYaxis()->SetTitle("#font[12]{#eta}^{#font[ 12]{h}}+ #font[ 12]{#delta}");
                H_pt_d[c][i][j]->GetYaxis()->SetTitleSize(0.08);
              }
              H_pt_d[c][i][0]->Draw("SAMEP");
              H_pt_d[c][i][0]->GetXaxis()->SetLimits(0,3);
              H_pt_d[c][i][0]->SetMinimum(0.85);
              H_pt_d[c][i][0]->SetMaximum(1.65);
              H_pt_d[c][i][1]->Draw("SAMEP");
              H_pt_d[c][i][1]->GetXaxis()->SetLimits(0,3);
              H_pt_d[c][i][1]->SetMinimum(0.85);
              H_pt_d[c][i][1]->SetMaximum(1.65);
              H_pt_d[c][i][2]->Draw("SAMEP");
              H_pt_d[c][i][2]->GetXaxis()->SetLimits(0,3);
              H_pt_d[c][i][2]->SetMinimum(0.85);
              H_pt_d[c][i][2]->SetMaximum(1.65);
              c10.Range(0,.85,3,1.65);
              l2[0]->Draw();
              l2[1]->Draw();
              l2[2]->Draw();
              l2[3]->Draw();
              l2[4]->Draw();
              l2[5]->Draw();
            }
            else
            {
              H_pt_d[c][i][j]->Draw("SAMEP");
              H_pt_d[c][i][j]->GetXaxis()->SetLimits(0,3);
              H_pt_d[c][i][j]->SetMinimum(0.85);
              H_pt_d[c][i][j]->SetMaximum(1.65);
            }
          }
          c10.Update();
        }

        if(!p_pt_d_empty)
        {
          c11.cd(i+1);
          gPad->SetFillStyle(4000);
          if(P_pt_d[c][i][j])
          {
            if(!c && j==3)
            {
              P_pt_d[c][i][j]->Draw("SAMEPA");
              P_pt_d[c][i][j]->GetXaxis()->SetLimits(.05,5.95);
              P_pt_d[c][i][j]->SetMinimum(0.85);
              P_pt_d[c][i][j]->SetMaximum(1.65);
              P_pt_d[c][i][j]->GetXaxis()->SetLabelSize(0.06);
              P_pt_d[c][i][j]->GetYaxis()->SetLabelSize(0.06);
              P_pt_d[c][i][j]->SetTitle("");
              if(i>4) gPad->SetBottomMargin(.15);
              if(i==0 || i==5) gPad->SetLeftMargin(.22);
              if(i==8)
              {
                P_pt_d[c][i][j]->GetXaxis()->SetTitle("#font[ 12]{p_{T}}");
                P_pt_d[c][i][j]->GetXaxis()->SetTitleSize(0.08);
                P_pt_d[c][i][j]->GetXaxis()->SetTitleOffset(.8);
              }
              P_pt_d[c][i][j]->GetXaxis()->SetNdivisions(304,kTRUE);
              P_pt_d[c][i][j]->GetYaxis()->SetNdivisions(304,kTRUE);
              if(i==0)
              {
                P_pt_d[c][i][j]->GetYaxis()->SetTitle("#font[12]{#eta}^{#font[ 12]{#pi}}+ #font[ 12]{#delta}");
                P_pt_d[c][i][j]->GetYaxis()->SetTitleSize(0.08);
              }
              P_pt_d[c][i][0]->Draw("SAMEP");
              P_pt_d[c][i][0]->GetXaxis()->SetLimits(.05,5.95);
              P_pt_d[c][i][0]->SetMinimum(0.85);
              P_pt_d[c][i][0]->SetMaximum(1.65);
              P_pt_d[c][i][1]->Draw("SAMEP");
              P_pt_d[c][i][1]->GetXaxis()->SetLimits(.05,5.95);
              P_pt_d[c][i][1]->SetMinimum(0.85);
              P_pt_d[c][i][1]->SetMaximum(1.65);
              P_pt_d[c][i][2]->Draw("SAMEP");
              P_pt_d[c][i][2]->GetXaxis()->SetLimits(.05,5.95);
              P_pt_d[c][i][2]->SetMinimum(0.85);
              P_pt_d[c][i][2]->SetMaximum(1.65);
              c11.Range(.05,.85,5.95,1.65);
              l2[0]->Draw();
              l2[1]->Draw();
              l2[2]->Draw();
              l2[3]->Draw();
              l2[4]->Draw();
              l2[5]->Draw();
            }
            else
            {
              P_pt_d[c][i][j]->Draw("SAMEP");
              P_pt_d[c][i][j]->GetXaxis()->SetLimits(.05,5.95);
              P_pt_d[c][i][j]->SetMinimum(0.85);
              P_pt_d[c][i][j]->SetMaximum(1.65);
            }
          }
          c11.Update();
        }

        if(!k_pt_d_empty)
        {
          c12.cd(i+1);
          gPad->SetFillStyle(4000);
          if(H_pt_d[c][i][j])
          {
            if(!c && j==3)
            {
              K_pt_d[c][i][j]->Draw("SAMEPA");
              K_pt_d[c][i][j]->GetXaxis()->SetLimits(.05,5.95);
              K_pt_d[c][i][j]->SetMinimum(0.85);
              K_pt_d[c][i][j]->SetMaximum(1.65);
              K_pt_d[c][i][j]->GetXaxis()->SetLabelSize(0.06);
              K_pt_d[c][i][j]->GetYaxis()->SetLabelSize(0.06);
              K_pt_d[c][i][j]->SetTitle("");
              if(i>4) gPad->SetBottomMargin(.15);
              if(i==0 || i==5) gPad->SetLeftMargin(.22);
              if(i==8)
              {
                K_pt_d[c][i][j]->GetXaxis()->SetTitle("#font[ 12]{p_{T}}");
                K_pt_d[c][i][j]->GetXaxis()->SetTitleSize(0.08);
                K_pt_d[c][i][j]->GetXaxis()->SetTitleOffset(.8);
              }
              K_pt_d[c][i][j]->GetXaxis()->SetNdivisions(304,kTRUE);
              K_pt_d[c][i][j]->GetYaxis()->SetNdivisions(304,kTRUE);
              if(i==0)
              {
                K_pt_d[c][i][j]->GetYaxis()->SetTitle("#font[12]{#eta}^{#font[ 12]{K}}+ #font[ 12]{#delta}");
                K_pt_d[c][i][j]->GetYaxis()->SetTitleSize(0.08);
              }
              K_pt_d[c][i][0]->Draw("SAMEP");
              K_pt_d[c][i][0]->GetXaxis()->SetLimits(.05,5.95);
              K_pt_d[c][i][0]->SetMinimum(0.85);
              K_pt_d[c][i][0]->SetMaximum(1.65);
              K_pt_d[c][i][1]->Draw("SAMEP");
              K_pt_d[c][i][1]->GetXaxis()->SetLimits(.05,5.95);
              K_pt_d[c][i][1]->SetMinimum(0.85);
              K_pt_d[c][i][1]->SetMaximum(1.65);
              K_pt_d[c][i][2]->Draw("SAMEP");
              K_pt_d[c][i][2]->GetXaxis()->SetLimits(.05,5.95);
              K_pt_d[c][i][2]->SetMinimum(0.85);
              K_pt_d[c][i][2]->SetMaximum(1.65);
              c12.Range(.05,.85,5.95,1.65);
              l2[0]->Draw();
              l2[1]->Draw();
              l2[2]->Draw();
              l2[3]->Draw();
              l2[4]->Draw();
              l2[5]->Draw();
            }
            else
            {
              K_pt_d[c][i][j]->Draw("SAMEP");
              K_pt_d[c][i][j]->GetXaxis()->SetLimits(.05,5.95);
              K_pt_d[c][i][j]->SetMinimum(0.85);
              K_pt_d[c][i][j]->SetMaximum(1.65);
            }
          }
          c12.Update();
        }
      }
    }
  }

  shout.close();
  ofs_multKm.close();
  ofs_multKp.close();

  TLatex fTitle;

  c7.cd(1);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.004#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.01");

  c7.cd(2);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.01#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.02");

  c7.cd(3);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.02#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.03");

  c7.cd(4);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.03#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.04");

  c7.cd(5);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.04#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.06");

  c7.cd(6);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.06#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.1");

  c7.cd(7);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.1#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.14");

  c7.cd(8);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.14#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.18");

  c7.cd(9);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.18#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.4");

  c7.cd(10);
  fTitle.SetTextSize(0.095);
  fTitle.SetTextAlign(11);
  fTitle.DrawLatex(0.05, 0.72,"#color[221]{0.70#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.90, #delta = 0.5}");
  fTitle.DrawLatex(0.05, 0.64,"#color[4]{0.50#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.70, #delta = 0.4}");
  fTitle.DrawLatex(0.05, 0.56,"#color[226]{0.30#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.50, #delta = 0.3}");
  fTitle.DrawLatex(0.05, 0.48,"#color[209]{0.20#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.30, #delta = 0.2}");
  fTitle.DrawLatex(0.05, 0.40,"#color[95]{0.15#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.20, #delta = 0.1}");
  fTitle.DrawLatex(0.05, 0.32,"#color[2]{0.10#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.15, #delta = 0}");


  c8.cd(1);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.004#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.01");

  c8.cd(2);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.01#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.02");

  c8.cd(3);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.02#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.03");

  c8.cd(4);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.03#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.04");

  c8.cd(5);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.04#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.06");

  c8.cd(6);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.06#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.1");

  c8.cd(7);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.1#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.14");

  c8.cd(8);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.14#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.18");

  c8.cd(9);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.18#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.4");

  c8.cd(10);
  fTitle.SetTextSize(0.095);
  fTitle.SetTextAlign(11);
  fTitle.DrawLatex(0.05, 0.72,"#color[221]{0.70#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.90, #delta = 0.5}");
  fTitle.DrawLatex(0.05, 0.64,"#color[4]{0.50#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.70, #delta = 0.4}");
  fTitle.DrawLatex(0.05, 0.56,"#color[226]{0.30#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.50, #delta = 0.3}");
  fTitle.DrawLatex(0.05, 0.48,"#color[209]{0.20#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.30, #delta = 0.2}");
  fTitle.DrawLatex(0.05, 0.40,"#color[95]{0.15#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.20, #delta = 0.1}");
  fTitle.DrawLatex(0.05, 0.32,"#color[2]{0.10#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.15, #delta = 0}");

  c9.cd(1);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.004#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.01");

  c9.cd(2);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.01#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.02");

  c9.cd(3);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.02#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.03");

  c9.cd(4);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.03#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.04");

  c9.cd(5);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.04#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.06");

  c9.cd(6);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.06#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.1");

  c9.cd(7);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.1#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.14");

  c9.cd(8);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.14#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.18");

  c9.cd(9);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(0.5, 1.6,"0.18#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.4");

  c9.cd(10);
  fTitle.SetTextSize(0.095);
  fTitle.SetTextAlign(11);
  fTitle.DrawLatex(0.05, 0.72,"#color[221]{0.70#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.90, #delta = 0.5}");
  fTitle.DrawLatex(0.05, 0.64,"#color[4]{0.50#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.70, #delta = 0.4}");
  fTitle.DrawLatex(0.05, 0.56,"#color[226]{0.30#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.50, #delta = 0.3}");
  fTitle.DrawLatex(0.05, 0.48,"#color[209]{0.20#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.30, #delta = 0.2}");
  fTitle.DrawLatex(0.05, 0.40,"#color[95]{0.15#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.20, #delta = 0.1}");
  fTitle.DrawLatex(0.05, 0.32,"#color[2]{0.10#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.15, #delta = 0}");


  c10.cd(1);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.004#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.01");

  c10.cd(2);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(3, 1.6,"0.01#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.02");

  c10.cd(3);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.02#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.03");

  c10.cd(4);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.03#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.04");

  c10.cd(5);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.04#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.06");

  c10.cd(6);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.06#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.1");

  c10.cd(7);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.1#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.14");

  c10.cd(8);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.14#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.18");

  c10.cd(9);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.18#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.4");

  c10.cd(10);
  fTitle.SetTextSize(0.095);
  fTitle.SetTextAlign(11);
  fTitle.DrawLatex(0.05, 0.72,"#color[221]{0.70#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.90, #delta = 0.5}");
  fTitle.DrawLatex(0.05, 0.64,"#color[4]{0.50#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.70, #delta = 0.4}");
  fTitle.DrawLatex(0.05, 0.56,"#color[226]{0.30#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.50, #delta = 0.3}");
  fTitle.DrawLatex(0.05, 0.48,"#color[209]{0.20#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.30, #delta = 0.2}");
  fTitle.DrawLatex(0.05, 0.40,"#color[95]{0.15#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.20, #delta = 0.1}");
  fTitle.DrawLatex(0.05, 0.32,"#color[2]{0.10#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.15, #delta = 0}");


  c11.cd(1);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.004#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.01");

  c11.cd(2);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.01#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.02");

  c11.cd(3);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.02#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.03");

  c11.cd(4);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.03#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.04");

  c11.cd(5);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.04#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.06");

  c11.cd(6);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.06#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.1");

  c11.cd(7);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.1#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.14");

  c11.cd(8);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.14#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.18");

  c11.cd(9);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.18#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.4");

  c11.cd(10);
  fTitle.SetTextSize(0.095);
  fTitle.SetTextAlign(11);
  fTitle.DrawLatex(0.05, 0.72,"#color[221]{0.70#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.90, #delta = 0.5}");
  fTitle.DrawLatex(0.05, 0.64,"#color[4]{0.50#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.70, #delta = 0.4}");
  fTitle.DrawLatex(0.05, 0.56,"#color[226]{0.30#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.50, #delta = 0.3}");
  fTitle.DrawLatex(0.05, 0.48,"#color[209]{0.20#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.30, #delta = 0.2}");
  fTitle.DrawLatex(0.05, 0.40,"#color[95]{0.15#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.20, #delta = 0.1}");
  fTitle.DrawLatex(0.05, 0.32,"#color[2]{0.10#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.15, #delta = 0}");


  c12.cd(1);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.004#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.01");

  c12.cd(2);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.01#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.02");

  c12.cd(3);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.02#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.03");

  c12.cd(4);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.03#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.04");

  c12.cd(5);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.04#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.06");

  c12.cd(6);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.06#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.1");

  c12.cd(7);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.1#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.14");

  c12.cd(8);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.14#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.18");

  c12.cd(9);
  fTitle.SetTextSize(0.078);
  fTitle.SetTextAlign(21);
  fTitle.DrawLatex(1.5, 1.6,"0.18#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{x}#scale[0.5]{ }<#scale[0.5]{ }0.4");

  c12.cd(10);
  fTitle.SetTextSize(0.095);
  fTitle.SetTextAlign(11);
  fTitle.DrawLatex(0.05, 0.72,"#color[221]{0.70#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.90, #delta = 0.5}");
  fTitle.DrawLatex(0.05, 0.64,"#color[4]{0.50#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.70, #delta = 0.4}");
  fTitle.DrawLatex(0.05, 0.56,"#color[226]{0.30#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.50, #delta = 0.3}");
  fTitle.DrawLatex(0.05, 0.48,"#color[209]{0.20#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.30, #delta = 0.2}");
  fTitle.DrawLatex(0.05, 0.40,"#color[95]{0.15#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.20, #delta = 0.1}");
  fTitle.DrawLatex(0.05, 0.32,"#color[2]{0.10#scale[0.5]{ }<#scale[0.5]{ }#font[ 12]{y}#scale[0.5]{ }<#scale[0.5]{ }0.15, #delta = 0}");

  c1.Update();
  c2.Update();
  c3.Update();
  c4.Update();
  c5.Update();
  c6.Update();
  c7.Update();
  c8.Update();
  c9.Update();
  c10.Update();
  c11.Update();
  c12.Update();

  c1.Print(hadron_z_pdf);
  c2.Print(pion_z_pdf);
  c3.Print(kaon_z_pdf);
  c4.Print(hadron_pt_pdf);
  c5.Print(pion_pt_pdf);
  c6.Print(kaon_pt_pdf);
  c7.Print(hadron_ratio_pdf);
  c8.Print(pion_ratio_pdf);
  c9.Print(kaon_ratio_pdf);
  c10.Print(hadron_ratio_pt_pdf);
  c11.Print(pion_ratio_pt_pdf);
  c12.Print(kaon_ratio_pt_pdf);


  return 0;
}
