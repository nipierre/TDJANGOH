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
  float eele[4] = {145.0,161.0,148.0,160.5};

  cout << "Instance creation.." << endl;
  tDjangoh = new TDjangoh();
  cout << "Instance created !" << endl;

  cout << "Initialization.." << endl;
  tDjangoh->Initialize("mu-", 1.0, 1.0, 160.0, 0.0);
  cout << "Initialized !" << endl;

  for(int i=0; i<1000000; i++)
  {

    tDjangoh->Configure("mu-", 1.0, 1.0, eele[i%4], 0.0);

    cout << "\n\nEvent Generation (N°" << i << ").." << endl;
    tDjangoh->GenerateEvent();
    cout << "Event Generated !\n" << endl;

    nb = tDjangoh->GetN();
    cout << "Total number of particles : " << nb << endl;
    for(int j=0; j<nb; j++)
    {
    	cout
    	<< tDjangoh->GetK(j,1) << " "
    	<< tDjangoh->GetK(j,2) << " "
    	<< tDjangoh->GetK(j,3) << " "
    	<< tDjangoh->GetK(j,4) << " "
    	<< tDjangoh->GetK(j,5) << " \n"
    	<< tDjangoh->GetP(j,1) << " "
      << tDjangoh->GetP(j,2) << " "
      << tDjangoh->GetP(j,3) << " "
      << tDjangoh->GetP(j,4) << " "
      << tDjangoh->GetP(j,5) << " \n"
      << tDjangoh->GetV(j,1) << " "
      << tDjangoh->GetV(j,2) << " "
      << tDjangoh->GetV(j,3) << " "
      << tDjangoh->GetV(j,4) << " "
      << tDjangoh->GetV(j,5) << " \n" << endl;
    }
    x = tDjangoh->GetX();
    y = tDjangoh->GetY();
    Q2 = tDjangoh->GetQ2();
    cout << "Xbj : " << x << " y : " << y << " Q2 : " << Q2 << "\n\n" << endl;
  }

  tDjangoh->EndRecap();

  return 0;
}
