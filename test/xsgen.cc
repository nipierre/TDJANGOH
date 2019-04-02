#include <iostream>
#include <fstream>
#include <cstring>

#include "TDjangoh.h"

// COLORS

#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST

#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST

using namespace std;

int main(int argc,char *argv[])
{

  if(argc != 3)
  {
    cout << BOLD(FRED("ERROR : Wrong number of arguments")) << endl;
    cout << BOLD(FRED("At least expected 3, received " << argc << " !")) << endl;
    cout << BOLD(FRED("USAGE : \n ./xsgen input_file RC(1)/Born(0)")) << endl;
    return 1;
  }

  TDjangoh* tDjangoh;
  string ext[2] = {"Born","RC"};
  double sigtot;
  double sigtrr;
  double xtab[10] = {.004,.01,.02,.03,.04,.06,.1,.14,.18,.4};
  double ytab[7] = {.1,.15,.2,.3,.5,.7,.9};

  cout << FBLU("Instance creation..") << endl;
  tDjangoh = new TDjangoh();
  cout << FBLU("Instance created !") << endl;

  tDjangoh->ReadXMLFile(argv[1]);

  cout << FBLU("\n\nCross-Section Calculation..") << endl;

    if(atoi(argv[2])==0) tDjangoh->BornWOqelNC();
    else tDjangoh->RClepWOqelNC();
    ofstream outfile(Form("sigtot_%s.dat",ext[atoi(argv[2])].c_str()));

    for(int j=0; j<9; j++)
    {
      for(int k=0; k<7; k++)
      {

        cout << FBLU("\n(x,y) : (") << j << FBLU(",") << k << FBLU(")..") << endl;

        if(atoi(argv[2])==0) tDjangoh->ModKineCuts(3,xtab[j],xtab[j+1],ytab[k],ytab[k+1],0.8,1E5,4);
        else tDjangoh->ModKineCuts(3,xtab[j],xtab[j+1],ytab[k],ytab[k+1],0.8,1E5,0);

        tDjangoh->Initialize();

        sigtot = tDjangoh->GetSigtot();
	      sigtrr = tDjangoh->GetSigtrr();
        cout << sigtot << " " << sigtrr << endl;
        outfile << sigtot << "\t" << sigtrr << "\t";

        tDjangoh->Clean_File();

      }

      outfile << endl;
    }

    outfile.close();

  cout << FBLU("\n\nCross-Section Calculated !") << endl;

  return 0;
}
