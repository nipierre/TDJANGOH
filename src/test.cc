#include <iostream>
#include <fstream>

#include "TDjangoh.h"

using namespace std;

int main()
{
  TDjangoh* tDjangoh;
  TDjangoh* tDjangoh1;
  FILE *f;

  cout << "Instance creation.." << endl;
  tDjangoh = new TDjangoh();
  cout << "Instance created !" << endl;

  cout << "Initialization.." << endl;
  tDjangoh->Initialize("TDjangoh.in","mu-", 1, 1, 160.0, 0.0);
  cout << "Initialized !" << endl;

  cout << "Event Generation.." << endl;
  tDjangoh->GenerateEvent("TDjangoh.in",19);
  cout << "Event Generated !" << endl;

  cout << "Instance creation.." << endl;
  tDjangoh1 = new TDjangoh();
  cout << "Instance created !" << endl;

  cout << "Initialization.." << endl;
  tDjangoh1->Initialize("TDjangoh1.in","mu+", 1, 1, 180.0, 0.0);
  cout << "Initialized !" << endl;

  cout << "Event Generation.." << endl;
  tDjangoh1->GenerateEvent("TDjangoh1.in",97);
  cout << "Event Generated !" << endl;

  int nb;
  float x;
  float y;
  float Q2;

  nb = tDjangoh->GetN();
  cout << "Total number of particles : " << nb << endl;
  x = tDjangoh->GetX();
  y = tDjangoh->GetY();
  Q2 = tDjangoh->GetQ2();
  cout << "Xbj : " << x << " y : " << y << " Q2 : " << Q2 << endl;

  return 0;
}
