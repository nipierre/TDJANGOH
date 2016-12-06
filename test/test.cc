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
    for(int j=0; j<nb; j++)
    {
	cout 
	<< tDjangoh->GetK(0,j) << " "
	<< tDjangoh->GetK(1,j) << " "
	<< tDjangoh->GetK(2,j) << " "
	<< tDjangoh->GetK(3,j) << " "
	<< tDjangoh->GetK(4,j) << " \n"
	<< tDjangoh->GetP(0,j) << " "
        << tDjangoh->GetP(1,j) << " "
        << tDjangoh->GetP(2,j) << " "
        << tDjangoh->GetP(3,j) << " "
        << tDjangoh->GetP(4,j) << " \n"
	<< tDjangoh->GetV(0,j) << " "
        << tDjangoh->GetV(1,j) << " "
        << tDjangoh->GetV(2,j) << " "
        << tDjangoh->GetV(3,j) << " "
        << tDjangoh->GetV(4,j) << " \n" << endl;
    }
    x = tDjangoh->GetX();
    y = tDjangoh->GetY();
    Q2 = tDjangoh->GetQ2();
    cout << "Xbj : " << x << " y : " << y << " Q2 : " << Q2 << "\n\n" << endl;
  }

  return 0;
}
