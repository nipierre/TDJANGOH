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
    cout << BOLD(FRED("USAGE : \n ./xsgen [inputfile] [generatedfile]")) << endl;
    return 1;
  }

  TDjangoh* tDjangoh;

  cout << FBLU("Instance creation..") << endl;
  tDjangoh = new TDjangoh();
  cout << FBLU("Instance created !") << endl;

  tDjangoh->ReadXMLFile(argv[1]);

  tDjangoh->WriteXMLFile(argv[2]);

  cout << FBLU("\n\nXML file generated !") << endl;

  return 0;
}
