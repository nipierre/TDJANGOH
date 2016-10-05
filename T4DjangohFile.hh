#ifndef T4DJANGOHFILE_HH_
#define T4DJANGOHFILE_HH_

#include "T4SettingsFile.hh"
#include "T4Event.hh"

#include "G4RunManager.hh"

class T4DjangohFile
{
  public:
    static T4DjangohFile* getInstance(void);
    ~T4DjangohFile(void);

    T4BeamData* getNextDjangohEvent(void);
    T4BeamData* getCurrentDjangohEvent(void) {return &DjangohCollection.at(DjangohEventNo-1);}

  private:
    T4DjangohFile(void);
    static T4DjangohFile* DjangohFile;

    T4SettingsFile* settingsFile;

    bool readFile(std::ifstream& file, streamsize headerSize, bool firstCall);

    unsigned int DjangohEventNo;
    std::vector<T4BeamData> DjangohCollection;
};

#endif /* T4DJANGOHFILE_HH_ */
