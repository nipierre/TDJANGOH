#ifndef T4DJANGOHPROCESS_HH_
#define T4DJANGOHPROCESS_HH_

#include "T4ProcessBackend.hh"
#include "T4DjangohFile.hh"

class T4DjangohProcess : public T4ProcessBackEnd
{
  public:
    T4DjangohProcess(void) {DjangohFile = T4DjangohFile::getInstance();}
    virtual ~T4DjangohProcess(void) {};

    G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&);

  private:
    T4DjangohFile* DjangohFile;
};

#endif /* T4DJANGOHPROCESS_HH_ */
