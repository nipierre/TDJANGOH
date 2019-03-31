#include "ptrc.h"

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
        fNDIS_evt_r[i][j][k]=0;
        fNDIS_evt_b[i][j][k]=0;
      }
    }
  }

  fIsMu = 0;
  fIsE = 0;

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

      if(!(0<E)) continue;

      if(!(Q2>1)) continue;

      if(!(.003<xbj && xbj<.4)) continue;

      if(!(.1<y && y<.9)) continue;

      if(!(5<sqrt(W))) continue;

      if(fXrange[0]<xbj && xbj<fXrange[1]) xbin = 0;
      else if(fXrange[1]<=xbj && xbj<fXrange[2]) xbin = 1;
      else if(fXrange[2]<=xbj && xbj<fXrange[3]) xbin = 2;
      else if(fXrange[3]<=xbj && xbj<fXrange[4]) xbin = 3;
      else if(fXrange[4]<=xbj && xbj<fXrange[5]) xbin = 4;
      else if(fXrange[5]<=xbj && xbj<fXrange[6]) xbin = 5;
      else if(fXrange[6]<=xbj && xbj<fXrange[7]) xbin = 6;
      else if(fXrange[7]<=xbj && xbj<fXrange[8]) xbin = 7;

      if(fQ2range[0]<Q2 && Q2<fQ2range[1]) Q2bin = 0;
      else if(fQ2range[1]<=Q2 && Q2<fQ2range[2]) Q2bin = 1;
      else if(fQ2range[2]<=Q2 && Q2<fQ2range[3]) Q2bin = 2;
      else if(fQ2range[3]<=Q2 && Q2<fQ2range[4]) Q2bin = 3;
      else if(fQ2range[4]<=Q2 && Q2<fQ2range[5]) Q2bin = 4;

      fNDIS_evt_r[0][xbin][Q2bin]++;
      fNDIS_evt_r[1][xbin][Q2bin]++;
      fNDIS_evt_r[2][xbin][Q2bin]++;

      TLorentzVector mu(0,0,160,160);
      TVector3 muv = mu.Vect();
      TLorentzVector mup;
      TVector3 mupv;
      TLorentzVector gammastar;
      TVector3 gammastarv;

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

        if(abs(id)==13 || abs(id)==11)
        {
          E_prime = Eh;
          nu = E - E_prime;
          if(!fIsMu && !fIsE)
          {
            mup.SetPxPyPzE(px,py,pz,Eh);
            mupv = mup.Vect();
            gammastar = mu - mup;
            gammastarv = gammastar.Vect();
            if(abs(id)==11) fIsE = 1;
            else fIsMu = 1;
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
        else if(id != 22 && abs(id) != 13 && abs(id) != 11)//Hadron
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

        if(abs(id) == 13 || abs(id) == 11 || id == 22) continue;

        TLorentzVector had;
        had.SetPxPyPzE(px,py,pz,Eh);
        TVector3 hadv = had.Vect();

        double theta_mu = acos(muv.Dot(mupv)/(muv.Mag()*mupv.Mag()));
        double theta_gs = acos((muv.Mag()-mupv.Mag()*cos(theta_mu))/gammastarv.Mag());

        double phi = (gammastarv.Cross(muv)).Dot(hadv)/(gammastarv.Cross(muv).Mag()*hadv.Mag())
                      *acos((gammastarv.Cross(muv)).Dot(gammastarv.Cross(hadv))/(gammastarv.Cross(muv).Mag()*gammastarv.Cross(hadv).Mag()));

        hadv.RotateY(theta_gs);
        hadv.RotateZ(phi);

        pt = pow(hadv.X(),2)+pow(hadv.Y(),2);
        cout << pt << endl;

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

        if(!(0.2<z && z<0.8)) continue;
        if(!(0.02<pt && pt<3)) continue;

        if(fZrange[0]<z && z<fZrange[1]) zbin = 0;
        else if(fZrange[1]<z && z<fZrange[2]) zbin = 1;
        else if(fZrange[2]<=z && z<fZrange[3]) zbin = 2;
        else if(fZrange[3]<=z && z<fZrange[4]) zbin = 3;

        if(fpTrange[0]<pt && pt<fpTrange[1]) ptbin = 0;
        else if(fpTrange[1]<pt && pt<fpTrange[2]) ptbin = 1;
        else if(fpTrange[2]<pt && pt<fpTrange[3]) ptbin = 2;
        else if(fpTrange[3]<pt && pt<fpTrange[4]) ptbin = 3;
        else if(fpTrange[4]<pt && pt<fpTrange[5]) ptbin = 4;
        else if(fpTrange[5]<pt && pt<fpTrange[6]) ptbin = 5;
        else if(fpTrange[6]<pt && pt<fpTrange[7]) ptbin = 6;
        else if(fpTrange[7]<pt && pt<fpTrange[8]) ptbin = 7;
        else if(fpTrange[8]<pt && pt<fpTrange[9]) ptbin = 8;
        else if(fpTrange[9]<pt && pt<fpTrange[10]) ptbin = 9;
        else if(fpTrange[10]<pt && pt<fpTrange[11]) ptbin = 10;
        else if(fpTrange[11]<pt && pt<fpTrange[12]) ptbin = 11;
        else if(fpTrange[12]<pt && pt<fpTrange[13]) ptbin = 12;
        else if(fpTrange[13]<pt && pt<fpTrange[14]) ptbin = 13;
        else if(fpTrange[14]<pt && pt<fpTrange[15]) ptbin = 14;
        else if(fpTrange[15]<pt && pt<fpTrange[16]) ptbin = 15;
        else if(fpTrange[16]<pt && pt<fpTrange[17]) ptbin = 16;
        else if(fpTrange[17]<pt && pt<fpTrange[18]) ptbin = 17;
        else if(fpTrange[18]<pt && pt<fpTrange[19]) ptbin = 18;
        else if(fpTrange[19]<pt && pt<fpTrange[20]) ptbin = 19;
        else if(fpTrange[20]<pt && pt<fpTrange[21]) ptbin = 20;
        else if(fpTrange[21]<pt && pt<fpTrange[22]) ptbin = 21;
        else if(fpTrange[22]<pt && pt<fpTrange[23]) ptbin = 22;
        else if(fpTrange[23]<pt && pt<fpTrange[24]) ptbin = 23;
        else if(fpTrange[24]<pt && pt<fpTrange[25]) ptbin = 24;
        else if(fpTrange[25]<pt && pt<fpTrange[26]) ptbin = 25;
        else if(fpTrange[26]<pt && pt<fpTrange[27]) ptbin = 26;
        else if(fpTrange[27]<pt && pt<fpTrange[28]) ptbin = 27;
        else if(fpTrange[28]<pt && pt<fpTrange[29]) ptbin = 28;
        else if(fpTrange[29]<pt && pt<fpTrange[30]) ptbin = 29;


        if(fId==0)
        {
          fRe[xbin][Q2bin][zbin][ptbin].tab[1][0][0] += 1;
          fRe[xbin][Q2bin][zbin][ptbin].tab[1][0][3] += 1;
        }
        else if(fId==1)
        {
          fRe[xbin][Q2bin][zbin][ptbin].tab[0][0][0] += 1;
          fRe[xbin][Q2bin][zbin][ptbin].tab[0][0][3] += 1;
        }
        else if(fId==2)
        {
          fRe[xbin][Q2bin][zbin][ptbin].tab[1][0][1] += 1;
          fRe[xbin][Q2bin][zbin][ptbin].tab[1][0][3] += 1;
        }
        else if(fId==3)
        {
          fRe[xbin][Q2bin][zbin][ptbin].tab[0][0][1] += 1;
          fRe[xbin][Q2bin][zbin][ptbin].tab[0][0][3] += 1;
        }
        else if(fId==4)
        {
          fRe[xbin][Q2bin][zbin][ptbin].tab[1][0][2] += 1;
          fRe[xbin][Q2bin][zbin][ptbin].tab[1][0][3] += 1;
        }
        else if(fId==5)
        {
          fRe[xbin][Q2bin][zbin][ptbin].tab[0][0][2] += 1;
          fRe[xbin][Q2bin][zbin][ptbin].tab[0][0][3] += 1;
        }
        else if(fId==6)
        {
          fRe[xbin][Q2bin][zbin][ptbin].tab[1][0][3] += 1;
        }
        else if(fId==7)
        {
          fRe[xbin][Q2bin][zbin][ptbin].tab[0][0][3] += 1;
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

      if(!(0<E)) continue;

      if(!(Q2>1)) continue;

      if(!(.003<xbj && xbj<.4)) continue;

      if(!(.1<y && y<.9)) continue;

      if(!(5<sqrt(W))) continue;

      if(fXrange[0]<xbj && xbj<fXrange[1]) xbin = 0;
      else if(fXrange[1]<=xbj && xbj<fXrange[2]) xbin = 1;
      else if(fXrange[2]<=xbj && xbj<fXrange[3]) xbin = 2;
      else if(fXrange[3]<=xbj && xbj<fXrange[4]) xbin = 3;
      else if(fXrange[4]<=xbj && xbj<fXrange[5]) xbin = 4;
      else if(fXrange[5]<=xbj && xbj<fXrange[6]) xbin = 5;
      else if(fXrange[6]<=xbj && xbj<fXrange[7]) xbin = 6;
      else if(fXrange[7]<=xbj && xbj<fXrange[8]) xbin = 7;

      if(fQ2range[0]<Q2 && Q2<fQ2range[1]) Q2bin = 0;
      else if(fQ2range[1]<=Q2 && Q2<fQ2range[2]) Q2bin = 1;
      else if(fQ2range[2]<=Q2 && Q2<fQ2range[3]) Q2bin = 2;
      else if(fQ2range[3]<=Q2 && Q2<fQ2range[4]) Q2bin = 3;
      else if(fQ2range[4]<=Q2 && Q2<fQ2range[5]) Q2bin = 4;

      fNDIS_evt_r[0][xbin][Q2bin]++;
      fNDIS_evt_r[1][xbin][Q2bin]++;
      fNDIS_evt_r[2][xbin][Q2bin]++;

      TLorentzVector mu(0,0,160,160);
      TVector3 muv = mu.Vect();
      TLorentzVector mup;
      TVector3 mupv;
      TLorentzVector gammastar;
      TVector3 gammastarv;

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

        if(abs(id)==13 || abs(id)==11)
        {
          E_prime = Eh;
          nu = E - E_prime;
          if(!fIsMu && !fIsE)
          {
            mup.SetPxPyPzE(px,py,pz,Eh);
            mupv = mup.Vect();
            gammastar = mu - mup;
            gammastarv = gammastar.Vect();
            if(abs(id)==11) fIsE = 1;
            else fIsMu = 1;
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
        else if(id != 22 && abs(id) != 13 && abs(id) != 11)//Hadron
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

        if(abs(id) == 13 || id == 22 || abs(id)==11) continue;

        TLorentzVector had;
        had.SetPxPyPzE(px,py,pz,Eh);
        TVector3 hadv = had.Vect();

        double theta_mu = acos(muv.Dot(mupv)/(muv.Mag()*mupv.Mag()));
        double theta_gs = acos((muv.Mag()-mupv.Mag()*cos(theta_mu))/gammastarv.Mag());

        double phi = (gammastarv.Cross(muv)).Dot(hadv)/(gammastarv.Cross(muv).Mag()*hadv.Mag())
                      *acos((gammastarv.Cross(muv)).Dot(gammastarv.Cross(hadv))/(gammastarv.Cross(muv).Mag()*gammastarv.Cross(hadv).Mag()));

        hadv.RotateY(theta_gs);
        hadv.RotateZ(phi);

        pt = pow(hadv.X(),2)+pow(hadv.Y(),2);
        //cout << pt << endl;

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

        if(!(0.2<z && z<0.8)) continue;
        if(!(0.02<pt && pt<3)) continue;

        if(fZrange[0]<z && z<fZrange[1]) zbin = 0;
        else if(fZrange[1]<z && z<fZrange[2]) zbin = 1;
        else if(fZrange[2]<=z && z<fZrange[3]) zbin = 2;
        else if(fZrange[3]<=z && z<fZrange[4]) zbin = 3;

        if(fpTrange[0]<pt && pt<fpTrange[1]) ptbin = 0;
        else if(fpTrange[1]<pt && pt<fpTrange[2]) ptbin = 1;
        else if(fpTrange[2]<pt && pt<fpTrange[3]) ptbin = 2;
        else if(fpTrange[3]<pt && pt<fpTrange[4]) ptbin = 3;
        else if(fpTrange[4]<pt && pt<fpTrange[5]) ptbin = 4;
        else if(fpTrange[5]<pt && pt<fpTrange[6]) ptbin = 5;
        else if(fpTrange[6]<pt && pt<fpTrange[7]) ptbin = 6;
        else if(fpTrange[7]<pt && pt<fpTrange[8]) ptbin = 7;
        else if(fpTrange[8]<pt && pt<fpTrange[9]) ptbin = 8;
        else if(fpTrange[9]<pt && pt<fpTrange[10]) ptbin = 9;
        else if(fpTrange[10]<pt && pt<fpTrange[11]) ptbin = 10;
        else if(fpTrange[11]<pt && pt<fpTrange[12]) ptbin = 11;
        else if(fpTrange[12]<pt && pt<fpTrange[13]) ptbin = 12;
        else if(fpTrange[13]<pt && pt<fpTrange[14]) ptbin = 13;
        else if(fpTrange[14]<pt && pt<fpTrange[15]) ptbin = 14;
        else if(fpTrange[15]<pt && pt<fpTrange[16]) ptbin = 15;
        else if(fpTrange[16]<pt && pt<fpTrange[17]) ptbin = 16;
        else if(fpTrange[17]<pt && pt<fpTrange[18]) ptbin = 17;
        else if(fpTrange[18]<pt && pt<fpTrange[19]) ptbin = 18;
        else if(fpTrange[19]<pt && pt<fpTrange[20]) ptbin = 19;
        else if(fpTrange[20]<pt && pt<fpTrange[21]) ptbin = 20;
        else if(fpTrange[21]<pt && pt<fpTrange[22]) ptbin = 21;
        else if(fpTrange[22]<pt && pt<fpTrange[23]) ptbin = 22;
        else if(fpTrange[23]<pt && pt<fpTrange[24]) ptbin = 23;
        else if(fpTrange[24]<pt && pt<fpTrange[25]) ptbin = 24;
        else if(fpTrange[25]<pt && pt<fpTrange[26]) ptbin = 25;
        else if(fpTrange[26]<pt && pt<fpTrange[27]) ptbin = 26;
        else if(fpTrange[27]<pt && pt<fpTrange[28]) ptbin = 27;
        else if(fpTrange[28]<pt && pt<fpTrange[29]) ptbin = 28;
        else if(fpTrange[29]<pt && pt<fpTrange[30]) ptbin = 29;


        if(fId==0)
        {
          fBorn[xbin][Q2bin][zbin][ptbin].tab[1][0][0] += 1;
          fBorn[xbin][Q2bin][zbin][ptbin].tab[1][0][3] += 1;
        }
        else if(fId==1)
        {
          fBorn[xbin][Q2bin][zbin][ptbin].tab[0][0][0] += 1;
          fBorn[xbin][Q2bin][zbin][ptbin].tab[0][0][3] += 1;
        }
        else if(fId==2)
        {
          fBorn[xbin][Q2bin][zbin][ptbin].tab[1][0][1] += 1;
          fBorn[xbin][Q2bin][zbin][ptbin].tab[1][0][3] += 1;
        }
        else if(fId==3)
        {
          fBorn[xbin][Q2bin][zbin][ptbin].tab[0][0][1] += 1;
          fBorn[xbin][Q2bin][zbin][ptbin].tab[0][0][3] += 1;
        }
        else if(fId==4)
        {
          fBorn[xbin][Q2bin][zbin][ptbin].tab[1][0][2] += 1;
          fBorn[xbin][Q2bin][zbin][ptbin].tab[1][0][3] += 1;
        }
        else if(fId==5)
        {
          fBorn[xbin][Q2bin][zbin][ptbin].tab[0][0][2] += 1;
          fBorn[xbin][Q2bin][zbin][ptbin].tab[0][0][3] += 1;
        }
        else if(fId==6)
        {
          fBorn[xbin][Q2bin][zbin][ptbin].tab[1][0][3] += 1;
        }
        else if(fId==7)
        {
          fBorn[xbin][Q2bin][zbin][ptbin].tab[0][0][3] += 1;
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

        if(!(0<E)) continue;

        if(!(Q2>1)) continue;

        if(!(.003<xbj && xbj<.4)) continue;

        if(!(.1<y && y<.9)) continue;

        if(!(5<sqrt(W))) continue;

        if(fXrange[0]<xbj && xbj<fXrange[1]) xbin = 0;
        else if(fXrange[1]<=xbj && xbj<fXrange[2]) xbin = 1;
        else if(fXrange[2]<=xbj && xbj<fXrange[3]) xbin = 2;
        else if(fXrange[3]<=xbj && xbj<fXrange[4]) xbin = 3;
        else if(fXrange[4]<=xbj && xbj<fXrange[5]) xbin = 4;
        else if(fXrange[5]<=xbj && xbj<fXrange[6]) xbin = 5;
        else if(fXrange[6]<=xbj && xbj<fXrange[7]) xbin = 6;
        else if(fXrange[7]<=xbj && xbj<fXrange[8]) xbin = 7;

        if(fQ2range[0]<Q2 && Q2<fQ2range[1]) Q2bin = 0;
        else if(fQ2range[1]<=Q2 && Q2<fQ2range[2]) Q2bin = 1;
        else if(fQ2range[2]<=Q2 && Q2<fQ2range[3]) Q2bin = 2;
        else if(fQ2range[3]<=Q2 && Q2<fQ2range[4]) Q2bin = 3;
        else if(fQ2range[4]<=Q2 && Q2<fQ2range[5]) Q2bin = 4;

        fNDIS_evt_r[0][xbin][Q2bin]++;
        fNDIS_evt_r[1][xbin][Q2bin]++;
        fNDIS_evt_r[2][xbin][Q2bin]++;

        TLorentzVector mu(0,0,160,160);
        TVector3 muv(mu.Px(),mu.Py(),mu.Pz());
        TLorentzVector mup;
        TVector3 mupv;
        TLorentzVector gammastar;
        TVector3 gammastarv;

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

          if(abs(id)==13 || abs(id)==11)
          {
            E_prime = Eh;
            nu = E - E_prime;
            mup.SetPxPyPzE(px,py,pz,Eh);
            mupv.SetXYZ(mup.Px(),mup.Py(),mup.Pz());
            // cout << "mup : " << mupv.X() << " " << mupv.Y() << " " << mupv.Z() << endl;
            gammastar = mu - mup;
            gammastarv.SetXYZ(gammastar.Px(),gammastar.Py(),gammastar.Pz());
            // cout << "gammastar : " << gammastarv.X() << " " << gammastarv.Y() << " " << gammastarv.Z() << endl;
            if(!fIsMu && !fIsE)
            {
              if(abs(id)==11) fIsE = 1;
              else fIsMu = 1;
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
          else if(id != 22 && abs(id) != 13 && abs(id)!=11)//Hadron
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

          if(abs(id) == 13 || id == 22 || abs(id)==11) continue;

          TLorentzVector had;
          had.SetPxPyPzE(px,py,pz,Eh);
          TVector3 hadv(had.Px(),had.Py(),had.Pz());
          // cout << "had : " << hadv.X() << " " << hadv.Y() << " " << hadv.Z() << endl;

          double theta_mu = acos(muv.Dot(mupv)/(muv.Mag()*mupv.Mag()));
          // cout << "theta_mu : " << theta_mu << endl;
          double theta_gs = acos((muv.Mag()-mupv.Mag()*cos(theta_mu))/gammastarv.Mag());
          // cout << "theta_gs : " << theta_gs << endl;

          double phi = (gammastarv.Cross(muv)).Dot(hadv)/(abs((gammastarv.Cross(muv)).Dot(hadv)))
                        *acos((gammastarv.Cross(muv)).Dot(gammastarv.Cross(hadv))/(gammastarv.Cross(muv).Mag()*gammastarv.Cross(hadv).Mag()));
          // cout << "phi : " << phi << endl;

          hadv.RotateY(theta_gs);
          hadv.RotateZ(phi);

          pt = pow(hadv.X(),2)+pow(hadv.Y(),2);
          // cout << pt << endl;

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

          if(!(0.2<z && z<0.8)) continue;
          if(!(0.02<pt && pt<3)) continue;

          if(fZrange[0]<z && z<fZrange[1]) zbin = 0;
          else if(fZrange[1]<z && z<fZrange[2]) zbin = 1;
          else if(fZrange[2]<=z && z<fZrange[3]) zbin = 2;
          else if(fZrange[3]<=z && z<fZrange[4]) zbin = 3;

          if(fpTrange[0]<pt && pt<fpTrange[1]) ptbin = 0;
          else if(fpTrange[1]<pt && pt<fpTrange[2]) ptbin = 1;
          else if(fpTrange[2]<pt && pt<fpTrange[3]) ptbin = 2;
          else if(fpTrange[3]<pt && pt<fpTrange[4]) ptbin = 3;
          else if(fpTrange[4]<pt && pt<fpTrange[5]) ptbin = 4;
          else if(fpTrange[5]<pt && pt<fpTrange[6]) ptbin = 5;
          else if(fpTrange[6]<pt && pt<fpTrange[7]) ptbin = 6;
          else if(fpTrange[7]<pt && pt<fpTrange[8]) ptbin = 7;
          else if(fpTrange[8]<pt && pt<fpTrange[9]) ptbin = 8;
          else if(fpTrange[9]<pt && pt<fpTrange[10]) ptbin = 9;
          else if(fpTrange[10]<pt && pt<fpTrange[11]) ptbin = 10;
          else if(fpTrange[11]<pt && pt<fpTrange[12]) ptbin = 11;
          else if(fpTrange[12]<pt && pt<fpTrange[13]) ptbin = 12;
          else if(fpTrange[13]<pt && pt<fpTrange[14]) ptbin = 13;
          else if(fpTrange[14]<pt && pt<fpTrange[15]) ptbin = 14;
          else if(fpTrange[15]<pt && pt<fpTrange[16]) ptbin = 15;
          else if(fpTrange[16]<pt && pt<fpTrange[17]) ptbin = 16;
          else if(fpTrange[17]<pt && pt<fpTrange[18]) ptbin = 17;
          else if(fpTrange[18]<pt && pt<fpTrange[19]) ptbin = 18;
          else if(fpTrange[19]<pt && pt<fpTrange[20]) ptbin = 19;
          else if(fpTrange[20]<pt && pt<fpTrange[21]) ptbin = 20;
          else if(fpTrange[21]<pt && pt<fpTrange[22]) ptbin = 21;
          else if(fpTrange[22]<pt && pt<fpTrange[23]) ptbin = 22;
          else if(fpTrange[23]<pt && pt<fpTrange[24]) ptbin = 23;
          else if(fpTrange[24]<pt && pt<fpTrange[25]) ptbin = 24;
          else if(fpTrange[25]<pt && pt<fpTrange[26]) ptbin = 25;
          else if(fpTrange[26]<pt && pt<fpTrange[27]) ptbin = 26;
          else if(fpTrange[27]<pt && pt<fpTrange[28]) ptbin = 27;
          else if(fpTrange[28]<pt && pt<fpTrange[29]) ptbin = 28;
          else if(fpTrange[29]<pt && pt<fpTrange[30]) ptbin = 29;


          if(fId==0)
          {
            fRe[xbin][Q2bin][zbin][ptbin].tab[1][0][0] += 1;
            fRe[xbin][Q2bin][zbin][ptbin].tab[1][0][3] += 1;
          }
          else if(fId==1)
          {
            fRe[xbin][Q2bin][zbin][ptbin].tab[0][0][0] += 1;
            fRe[xbin][Q2bin][zbin][ptbin].tab[0][0][3] += 1;
          }
          else if(fId==2)
          {
            fRe[xbin][Q2bin][zbin][ptbin].tab[1][0][1] += 1;
            fRe[xbin][Q2bin][zbin][ptbin].tab[1][0][3] += 1;
          }
          else if(fId==3)
          {
            fRe[xbin][Q2bin][zbin][ptbin].tab[0][0][1] += 1;
            fRe[xbin][Q2bin][zbin][ptbin].tab[0][0][3] += 1;
          }
          else if(fId==4)
          {
            fRe[xbin][Q2bin][zbin][ptbin].tab[1][0][2] += 1;
            fRe[xbin][Q2bin][zbin][ptbin].tab[1][0][3] += 1;
          }
          else if(fId==5)
          {
            fRe[xbin][Q2bin][zbin][ptbin].tab[0][0][2] += 1;
            fRe[xbin][Q2bin][zbin][ptbin].tab[0][0][3] += 1;
          }
          else if(fId==6)
          {
            fRe[xbin][Q2bin][zbin][ptbin].tab[1][0][3] += 1;
          }
          else if(fId==7)
          {
            fRe[xbin][Q2bin][zbin][ptbin].tab[0][0][3] += 1;
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

        if(!(0<E)) continue;

        if(!(Q2>1)) continue;

        if(!(.003<xbj && xbj<.4)) continue;

        if(!(.1<y && y<.9)) continue;

        if(!(5<sqrt(W))) continue;

        if(fXrange[0]<xbj && xbj<fXrange[1]) xbin = 0;
        else if(fXrange[1]<=xbj && xbj<fXrange[2]) xbin = 1;
        else if(fXrange[2]<=xbj && xbj<fXrange[3]) xbin = 2;
        else if(fXrange[3]<=xbj && xbj<fXrange[4]) xbin = 3;
        else if(fXrange[4]<=xbj && xbj<fXrange[5]) xbin = 4;
        else if(fXrange[5]<=xbj && xbj<fXrange[6]) xbin = 5;
        else if(fXrange[6]<=xbj && xbj<fXrange[7]) xbin = 6;
        else if(fXrange[7]<=xbj && xbj<fXrange[8]) xbin = 7;

        if(fQ2range[0]<Q2 && Q2<fQ2range[1]) Q2bin = 0;
        else if(fQ2range[1]<=Q2 && Q2<fQ2range[2]) Q2bin = 1;
        else if(fQ2range[2]<=Q2 && Q2<fQ2range[3]) Q2bin = 2;
        else if(fQ2range[3]<=Q2 && Q2<fQ2range[4]) Q2bin = 3;
        else if(fQ2range[4]<=Q2 && Q2<fQ2range[5]) Q2bin = 4;

        fNDIS_evt_r[0][xbin][Q2bin]++;
        fNDIS_evt_r[1][xbin][Q2bin]++;
        fNDIS_evt_r[2][xbin][Q2bin]++;

        TLorentzVector mu(0,0,160,160);
        TVector3 muv = mu.Vect();
        TLorentzVector mup;
        TVector3 mupv;
        TLorentzVector gammastar;
        TVector3 gammastarv;

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

          if(abs(id)==13 || abs(id)==11)
          {
            E_prime = Eh;
            nu = E - E_prime;
            mup.SetPxPyPzE(px,py,pz,Eh);
            mupv.SetXYZ(mup.Px(),mup.Py(),mup.Pz());
            // cout << "mup : " << mupv.X() << " " << mupv.Y() << " " << mupv.Z() << endl;
            gammastar = mu - mup;
            gammastarv.SetXYZ(gammastar.Px(),gammastar.Py(),gammastar.Pz());
            // cout << "gammastar : " << gammastarv.X() << " " << gammastarv.Y() << " " << gammastarv.Z() << endl;
            if(!fIsMu && !fIsE)
            {
              if(abs(id)==11) fIsE = 1;
              else fIsMu = 1;
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
          else if(id != 22 && abs(id) != 13 && abs(id)!=11)//Hadron
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

          if(abs(id) == 13 || id == 22 || abs(id)==11) continue;

          TLorentzVector had;
          had.SetPxPyPzE(px,py,pz,Eh);
          TVector3 hadv(had.Px(),had.Py(),had.Pz());

          double theta_mu = acos(muv.Dot(mupv)/(muv.Mag()*mupv.Mag()));
          //cout << "theta_mu : " << theta_mu << endl;
          double theta_gs = acos((muv.Mag()-mupv.Mag()*cos(theta_mu))/gammastarv.Mag());
          //cout << "theta_gs : " << theta_gs << endl;

          double phi = (gammastarv.Cross(muv)).Dot(hadv)/(abs((gammastarv.Cross(muv)).Dot(hadv)))
                        *acos((gammastarv.Cross(muv)).Dot(gammastarv.Cross(hadv))/(gammastarv.Cross(muv).Mag()*gammastarv.Cross(hadv).Mag()));
          //cout << "phi : " << phi << endl;

          hadv.RotateY(theta_gs);
          hadv.RotateZ(phi);

          pt = pow(hadv.Px(),2)+pow(hadv.Py(),2);
          //cout << pt << endl;

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

          if(!(0.2<z && z<0.8)) continue;
          if(!(0.02<pt && pt<3)) continue;

          if(fZrange[0]<z && z<fZrange[1]) zbin = 0;
          else if(fZrange[1]<z && z<fZrange[2]) zbin = 1;
          else if(fZrange[2]<=z && z<fZrange[3]) zbin = 2;
          else if(fZrange[3]<=z && z<fZrange[4]) zbin = 3;

          if(fpTrange[0]<pt && pt<fpTrange[1]) ptbin = 0;
          else if(fpTrange[1]<pt && pt<fpTrange[2]) ptbin = 1;
          else if(fpTrange[2]<pt && pt<fpTrange[3]) ptbin = 2;
          else if(fpTrange[3]<pt && pt<fpTrange[4]) ptbin = 3;
          else if(fpTrange[4]<pt && pt<fpTrange[5]) ptbin = 4;
          else if(fpTrange[5]<pt && pt<fpTrange[6]) ptbin = 5;
          else if(fpTrange[6]<pt && pt<fpTrange[7]) ptbin = 6;
          else if(fpTrange[7]<pt && pt<fpTrange[8]) ptbin = 7;
          else if(fpTrange[8]<pt && pt<fpTrange[9]) ptbin = 8;
          else if(fpTrange[9]<pt && pt<fpTrange[10]) ptbin = 9;
          else if(fpTrange[10]<pt && pt<fpTrange[11]) ptbin = 10;
          else if(fpTrange[11]<pt && pt<fpTrange[12]) ptbin = 11;
          else if(fpTrange[12]<pt && pt<fpTrange[13]) ptbin = 12;
          else if(fpTrange[13]<pt && pt<fpTrange[14]) ptbin = 13;
          else if(fpTrange[14]<pt && pt<fpTrange[15]) ptbin = 14;
          else if(fpTrange[15]<pt && pt<fpTrange[16]) ptbin = 15;
          else if(fpTrange[16]<pt && pt<fpTrange[17]) ptbin = 16;
          else if(fpTrange[17]<pt && pt<fpTrange[18]) ptbin = 17;
          else if(fpTrange[18]<pt && pt<fpTrange[19]) ptbin = 18;
          else if(fpTrange[19]<pt && pt<fpTrange[20]) ptbin = 19;
          else if(fpTrange[20]<pt && pt<fpTrange[21]) ptbin = 20;
          else if(fpTrange[21]<pt && pt<fpTrange[22]) ptbin = 21;
          else if(fpTrange[22]<pt && pt<fpTrange[23]) ptbin = 22;
          else if(fpTrange[23]<pt && pt<fpTrange[24]) ptbin = 23;
          else if(fpTrange[24]<pt && pt<fpTrange[25]) ptbin = 24;
          else if(fpTrange[25]<pt && pt<fpTrange[26]) ptbin = 25;
          else if(fpTrange[26]<pt && pt<fpTrange[27]) ptbin = 26;
          else if(fpTrange[27]<pt && pt<fpTrange[28]) ptbin = 27;
          else if(fpTrange[28]<pt && pt<fpTrange[29]) ptbin = 28;
          else if(fpTrange[29]<pt && pt<fpTrange[30]) ptbin = 29;


          if(fId==0)
          {
            fBorn[xbin][Q2bin][zbin][ptbin].tab[1][0][0] += 1;
            fBorn[xbin][Q2bin][zbin][ptbin].tab[1][0][3] += 1;
          }
          else if(fId==1)
          {
            fBorn[xbin][Q2bin][zbin][ptbin].tab[0][0][0] += 1;
            fBorn[xbin][Q2bin][zbin][ptbin].tab[0][0][3] += 1;
          }
          else if(fId==2)
          {
            fBorn[xbin][Q2bin][zbin][ptbin].tab[1][0][1] += 1;
            fBorn[xbin][Q2bin][zbin][ptbin].tab[1][0][3] += 1;
          }
          else if(fId==3)
          {
            fBorn[xbin][Q2bin][zbin][ptbin].tab[0][0][1] += 1;
            fBorn[xbin][Q2bin][zbin][ptbin].tab[0][0][3] += 1;
          }
          else if(fId==4)
          {
            fBorn[xbin][Q2bin][zbin][ptbin].tab[1][0][2] += 1;
            fBorn[xbin][Q2bin][zbin][ptbin].tab[1][0][3] += 1;
          }
          else if(fId==5)
          {
            fBorn[xbin][Q2bin][zbin][ptbin].tab[0][0][2] += 1;
            fBorn[xbin][Q2bin][zbin][ptbin].tab[0][0][3] += 1;
          }
          else if(fId==6)
          {
            fBorn[xbin][Q2bin][zbin][ptbin].tab[1][0][3] += 1;
          }
          else if(fId==7)
          {
            fBorn[xbin][Q2bin][zbin][ptbin].tab[0][0][3] += 1;
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

  ofstream shout("ptRC.txt",std::ofstream::out | std::ofstream::trunc);

  for(int c=0; c<2; c++)
  {
    for(int i=0; i<8; i++)
    {
      for(int j=0; j<5; j++)
      {
        for(int k=0; k<4; k++)
        {
          for(int l=0; l<30; l++)
          {
            // cout << "x,Q2,z,pt : " << i << "," << j << "," << k << "," << l << "\nN DIS evt r : " << fNDIS_evt_r[0][i][j] << " N re : " << fRe[i][j][k][l].tab[c][0][3];
            fReMult[i][j][k][l].tab[c][0][0] = ((fNDIS_evt_r[0][i][j]) ? double(fRe[i][j][k][l].tab[c][0][0])/double(fNDIS_evt_r[0][i][j]*fZ_bin_width[k]*fpT_bin_width[l]) : 0);
            fReMult[i][j][k][l].tab[c][0][1] = ((fNDIS_evt_r[0][i][j]) ? double(fRe[i][j][k][l].tab[c][0][1])/double(fNDIS_evt_r[0][i][j]*fZ_bin_width[k]*fpT_bin_width[l]) : 0);
            fReMult[i][j][k][l].tab[c][0][2] = ((fNDIS_evt_r[0][i][j]) ? double(fRe[i][j][k][l].tab[c][0][2])/double(fNDIS_evt_r[0][i][j]*fZ_bin_width[k]*fpT_bin_width[l]) : 0);
            fReMult[i][j][k][l].tab[c][0][3] = ((fNDIS_evt_r[0][i][j]) ? double(fRe[i][j][k][l].tab[c][0][3])/double(fNDIS_evt_r[0][i][j]*fZ_bin_width[k]*fpT_bin_width[l]) : 0);
            // cout << " Hm : " << fReMult[i][j][k][l].tab[c][0][3] << endl;
            // cout << "N DIS evt B : " << fNDIS_evt_b[0][i][j] <<  " N born : " << fBorn[i][j][k][l].tab[c][0][3];
            fBornMult[i][j][k][l].tab[c][0][0] = ((fNDIS_evt_b[0][i][j]) ? double(fBorn[i][j][k][l].tab[c][0][0])/double(fNDIS_evt_b[0][i][j]*fZ_bin_width[k]*fpT_bin_width[l]) : 0);
            fBornMult[i][j][k][l].tab[c][0][1] = ((fNDIS_evt_b[0][i][j]) ? double(fBorn[i][j][k][l].tab[c][0][1])/double(fNDIS_evt_b[0][i][j]*fZ_bin_width[k]*fpT_bin_width[l]) : 0);
            fBornMult[i][j][k][l].tab[c][0][2] = ((fNDIS_evt_b[0][i][j]) ? double(fBorn[i][j][k][l].tab[c][0][2])/double(fNDIS_evt_b[0][i][j]*fZ_bin_width[k]*fpT_bin_width[l]) : 0);
            fBornMult[i][j][k][l].tab[c][0][3] = ((fNDIS_evt_b[0][i][j]) ? double(fBorn[i][j][k][l].tab[c][0][3])/double(fNDIS_evt_b[0][i][j]*fZ_bin_width[k]*fpT_bin_width[l]) : 0);
            // cout << " Hm : " << fBornMult[i][j][k][l].tab[c][0][3] << endl;

            if(fReMult[i][j][k][l].tab[c][0][0]<0) fReMult[i][j][k][l].tab[c][0][0]=0;
            if(fReMult[i][j][k][l].tab[c][0][1]<0) fReMult[i][j][k][l].tab[c][0][1]=0;
            if(fReMult[i][j][k][l].tab[c][0][2]<0) fReMult[i][j][k][l].tab[c][0][2]=0;
            if(fReMult[i][j][k][l].tab[c][0][3]<0) fReMult[i][j][k][l].tab[c][0][3]=0;

            if(fBornMult[i][j][k][l].tab[c][0][0]<0) fBornMult[i][j][k][l].tab[c][0][0]=0;
            if(fBornMult[i][j][k][l].tab[c][0][1]<0) fBornMult[i][j][k][l].tab[c][0][1]=0;
            if(fBornMult[i][j][k][l].tab[c][0][2]<0) fBornMult[i][j][k][l].tab[c][0][2]=0;
            if(fBornMult[i][j][k][l].tab[c][0][3]<0) fBornMult[i][j][k][l].tab[c][0][3]=0;


            fMult[i][j][k][l].tab[c][0][0] = ((fReMult[i][j][k][l].tab[c][0][0] && fBornMult[i][j][k][l].tab[c][0][0]) ? (fBornMult[i][j][k][l].tab[c][0][0]/fReMult[i][j][k][l].tab[c][0][0]) : 0);
            fMult[i][j][k][l].tab[c][0][1] = ((fReMult[i][j][k][l].tab[c][0][1] && fBornMult[i][j][k][l].tab[c][0][1]) ? (fBornMult[i][j][k][l].tab[c][0][1]/fReMult[i][j][k][l].tab[c][0][1]) : 0);
            fMult[i][j][k][l].tab[c][0][2] = ((fReMult[i][j][k][l].tab[c][0][2] && fBornMult[i][j][k][l].tab[c][0][2]) ? (fBornMult[i][j][k][l].tab[c][0][2]/fReMult[i][j][k][l].tab[c][0][2]) : 0);
            fMult[i][j][k][l].tab[c][0][3] = ((fReMult[i][j][k][l].tab[c][0][3] && fBornMult[i][j][k][l].tab[c][0][3]) ? (fBornMult[i][j][k][l].tab[c][0][3]/fReMult[i][j][k][l].tab[c][0][3]) : 0);


            if(fReMult[i][j][k][l].tab[c][0][0]==0) fReMult[i][j][k][l].tab[c][1][0]=0;
            else fReMult[i][j][k][l].tab[c][1][0] = sqrt(pow(1/sqrt(fRe[i][j][k][l].tab[c][0][0]),2)+pow(1/sqrt(fNDIS_evt_r[0][i][j]),2));
            if(fReMult[i][j][k][l].tab[c][0][1]==0) fReMult[i][j][k][l].tab[c][1][1]=0;
            else fReMult[i][j][k][l].tab[c][1][1] = sqrt(pow(1/sqrt(fRe[i][j][k][l].tab[c][0][1]),2)+pow(1/sqrt(fNDIS_evt_r[0][i][j]),2));
            if(fReMult[i][j][k][l].tab[c][0][2]==0) fReMult[i][j][k][l].tab[c][1][2]=0;
            else fReMult[i][j][k][l].tab[c][1][2] = sqrt(pow(1/sqrt(fRe[i][j][k][l].tab[c][0][2]),2)+pow(1/sqrt(fNDIS_evt_r[0][i][j]),2));
            if(fReMult[i][j][k][l].tab[c][0][3]==0) fReMult[i][j][k][l].tab[c][1][3]=0;
            else fReMult[i][j][k][l].tab[c][1][3] = sqrt(pow(1/sqrt(fRe[i][j][k][l].tab[c][0][3]),2)+pow(1/sqrt(fNDIS_evt_r[0][i][j]),2));

            if(fBornMult[i][j][k][l].tab[c][0][0]==0) fBornMult[i][j][k][l].tab[c][1][0]=0;
            else fBornMult[i][j][k][l].tab[c][1][0] = sqrt(pow(1/sqrt(fBorn[i][j][k][l].tab[c][0][0]),2)+pow(1/sqrt(fNDIS_evt_r[0][i][j]),2));
            if(fBornMult[i][j][k][l].tab[c][0][1]==0) fBornMult[i][j][k][l].tab[c][1][1]=0;
            else fBornMult[i][j][k][l].tab[c][1][1] = sqrt(pow(1/sqrt(fBorn[i][j][k][l].tab[c][0][1]),2)+pow(1/sqrt(fNDIS_evt_r[0][i][j]),2));
            if(fBornMult[i][j][k][l].tab[c][0][2]==0) fBornMult[i][j][k][l].tab[c][1][2]=0;
            else fBornMult[i][j][k][l].tab[c][1][2] = sqrt(pow(1/sqrt(fBorn[i][j][k][l].tab[c][0][2]),2)+pow(1/sqrt(fNDIS_evt_r[0][i][j]),2));
            if(fBornMult[i][j][k][l].tab[c][0][3]==0) fBornMult[i][j][k][l].tab[c][1][3]=0;
            else fBornMult[i][j][k][l].tab[c][1][3] = sqrt(pow(1/sqrt(fBorn[i][j][k][l].tab[c][0][3]),2)+pow(1/sqrt(fNDIS_evt_r[0][i][j]),2));

            if(fMult[i][j][k][l].tab[c][0][0]==0) fMult[i][j][k][l].tab[c][1][0]=0;
            else fMult[i][j][k][l].tab[c][1][0] = sqrt(pow(fBornMult[i][j][k][l].tab[c][1][0],2)+pow(fReMult[i][j][k][l].tab[c][1][0],2));
            if(fMult[i][j][k][l].tab[c][0][1]==0) fMult[i][j][k][l].tab[c][1][1]=0;
            else fMult[i][j][k][l].tab[c][1][1] = sqrt(pow(fBornMult[i][j][k][l].tab[c][1][1],2)+pow(fReMult[i][j][k][l].tab[c][1][1],2));
            if(fMult[i][j][k][l].tab[c][0][2]==0) fMult[i][j][k][l].tab[c][1][2]=0;
            else fMult[i][j][k][l].tab[c][1][2] = sqrt(pow(fBornMult[i][j][k][l].tab[c][1][2],2)+pow(fReMult[i][j][k][l].tab[c][1][2],2));
            if(fMult[i][j][k][l].tab[c][0][3]==0) fMult[i][j][k][l].tab[c][1][3]=0;
            else fMult[i][j][k][l].tab[c][1][3] = sqrt(pow(fBornMult[i][j][k][l].tab[c][1][3],2)+pow(fReMult[i][j][k][l].tab[c][1][3],2));

            if((k==0 && ((j==4 && i<5) || (j==3 && i<4) || (j==2 && i<3) || (j==3 && i>7) || (j==2 && i>6) || (j==1 && i>5) || (j==0 && i>4)))
                || (k==1 && ((j==4 && i<5) || (j==3 && i<4) || (j==2 && i<3) || (j==2 && i>7) || (j==1 && i>6) || (j==0 && i>5)))
                || (k==2 && ((j==4 && i<5) || (j==3 && i<4) || (j==2 && i<3) || (j==2 && i>7) || (j==1 && i>6) || (j==0 && i>5)))
                || (k==3 && ((j==4 && i<5) || (j==3 && i<4) || (j==2 && i<3) || (j==1 && i<2) || (j==2 && i>7) || (j==1 && i>6) || (j==0 && i>5))))
            {
              fReMult[i][j][k][l].tab[c][0][0] = 0;
              fReMult[i][j][k][l].tab[c][0][1] = 0;
              fReMult[i][j][k][l].tab[c][0][2] = 0;
              fReMult[i][j][k][l].tab[c][0][3] = 0;
              fReMult[i][j][k][l].tab[c][1][0] = 0;
              fReMult[i][j][k][l].tab[c][1][1] = 0;
              fReMult[i][j][k][l].tab[c][1][2] = 0;
              fReMult[i][j][k][l].tab[c][1][3] = 0;

              fBornMult[i][j][k][l].tab[c][0][0] = 0;
              fBornMult[i][j][k][l].tab[c][0][1] = 0;
              fBornMult[i][j][k][l].tab[c][0][2] = 0;
              fBornMult[i][j][k][l].tab[c][0][3] = 0;
              fBornMult[i][j][k][l].tab[c][1][0] = 0;
              fBornMult[i][j][k][l].tab[c][1][1] = 0;
              fBornMult[i][j][k][l].tab[c][1][2] = 0;
              fBornMult[i][j][k][l].tab[c][1][3] = 0;

              fMult[i][j][k][l].tab[c][0][0] = 0;
              fMult[i][j][k][l].tab[c][0][1] = 0;
              fMult[i][j][k][l].tab[c][0][2] = 0;
              fMult[i][j][k][l].tab[c][0][3] = 0;
              fMult[i][j][k][l].tab[c][1][0] = 0;
              fMult[i][j][k][l].tab[c][1][1] = 0;
              fMult[i][j][k][l].tab[c][1][2] = 0;
              fMult[i][j][k][l].tab[c][1][3] = 0;
            }

            shout << c << "\t" << fXrange[i] << "\t" << fQ2range[j] << "\t" << fZrange[k] << "\t" << fpTrange[k]
                  << "\t" << fReMult[i][j][k][l].tab[c][0][3]
                  << "\t" << fReMult[i][j][k][l].tab[c][1][3]
                  << "\t" << fBornMult[i][j][k][l].tab[c][0][3]
                  << "\t" << fBornMult[i][j][k][l].tab[c][1][3]
                  << "\t" << fMult[i][j][k][l].tab[c][0][3]
                  << "\t" << fMult[i][j][k][l].tab[c][1][3] << endl;
          }
        }
      }
    }
  }

  shout.close();

  return 0;
}
