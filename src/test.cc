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

  for(int i=0; i<5; i++)
  {
    cout << "Initialization.." << endl;
    tDjangoh->Initialize("mu-", 1, 1, 160.0, 0.0);
    cout << "Initialized !" << endl;

    cout << "Event Generation.." << endl;
    tDjangoh->GenerateEvent();
    cout << "Event Generated !" << endl;

    nb = tDjangoh->GetN();
    cout << "Total number of particles : " << nb << endl;
    x = tDjangoh->GetX();
    y = tDjangoh->GetY();
    Q2 = tDjangoh->GetQ2();
    cout << "Xbj : " << x << " y : " << y << " Q2 : " << Q2 << endl;
  }

  return 0;
}
