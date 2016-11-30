#include <iostream>
#include <fstream>

#include "TDjangoh.h"

using namespace std;

int main()
{
  TDjangoh* tDjangoh;
  int nb;
  float x;
  float y;
  float Q2;

  cout << "Instance creation.." << endl;
  tDjangoh = new TDjangoh();
  cout << "Instance created !" << endl;

  cout << "Initialization.." << endl;
  tDjangoh->Initialize("mu-", 1, 1, 160.0, 0.0);
  cout << "Initialized !" << endl;

  for(int i=0; i<4; i++)
  {
    tDjangoh->Configure("mu-", 1, 1, 160.0, 0.0);

    cout << "\n\nEvent Generation.." << endl;
    tDjangoh->GenerateEvent();
    cout << "Event Generated !\n" << endl;

    nb = tDjangoh->GetN();
    cout << "Total number of particles : " << nb << endl;
    x = tDjangoh->GetX();
    y = tDjangoh->GetY();
    Q2 = tDjangoh->GetQ2();
    cout << "Xbj : " << x << " y : " << y << " Q2 : " << Q2 << "\n\n" << endl;
  }

  return 0;
}
