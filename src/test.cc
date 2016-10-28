#include <iostream>
#include <fstream>

#include "TDjangoh.h"

using namespace std;

int main()
{
  TDjangoh* tDjangoh;

  if (tDjangoh != NULL)
    delete tDjangoh;

  cout << "Instance creation.." << endl;
  tDjangoh = new TDjangoh();
  cout << "Instance created !" << endl;

  cout << "Initialization.." << endl;
  tDjangoh->Initialize("mu-", 1, 1, 160.0, 0.0);
  cout << "Initialized !" << endl;

  cout << "Event Generation.." << endl;
  tDjangoh->GenerateEvent();
  cout << "Event Generated !" << endl;

  int nb;

  nb = tDjangoh->GetN();
  cout << "Total number of particles : " << nb << endl;

  return 0;
}
