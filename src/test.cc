#include <iostream>
#include <fstream>

#include "TDjangoh.h"

using namespace std;

int main()
{
  TDjangoh* tDjangoh;

  if (tDjangoh != NULL)
    delete tDjangoh;

  tDjangoh = new TDjangoh();

  tDjangoh->Initialize("mu-", 1, 1, 160.0, 0.0);

  tDjangoh->GenerateEvent();

  return 0;
}
