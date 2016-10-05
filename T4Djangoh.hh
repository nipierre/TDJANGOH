#ifndef T4DJANGOH_HH_
#define T4DJANGOH_HH_

#include "T4BeamBackend.hh"
#include "T4DjangohFile.hh"
#include "T4DjangohProcess.hh"

class T4Djangoh : public T4BeamBackEnd
{
  public:
    T4Djangoh(void);
    virtual ~T4Djangoh(void) {}

    void generateEvent(G4Event*);
    void initialize(void);

    T4ProcessBackEnd* getEventGeneratorProcess(void);

  private:
    T4DjangohFile* DjangohFile;
};

#endif /* T4DJANGOH_HH_ */
