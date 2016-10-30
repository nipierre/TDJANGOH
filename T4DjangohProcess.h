#ifndef T4DJANGOHPROCESS_HH_
#define T4DJANGOHPROCESS_HH_

#include "config.hh"
#ifdef USE_DJANGOH

#include "T4ProcessBackend.hh"
#include "TDjangoh.h"


class T4DjangohProcess : public T4ProcessBackEnd
{
  public:
    T4DjangohProcess(void);
    virtual ~T4DjangohProcess(void);

    G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&);

    void initDjangoh(void);

  private:
    std::string getParticleString(int);
    TPythia6* tDjangoh;
    T4SPythia* djangoh; // See T4SGlobals

    std::string beamParticle;
};


#endif

#endif /* T4PYTHIAPROCESS_HH_ */
