#include "T4DjangohFile.hh"

T4DjangohFile* T4DjangohFile::DjangohFile = NULL;

T4DjangohFile* T4DjangohFile::getInstance(void)
{
  if (DjangohFile == NULL) {
    DjangohFile = new T4DjangohFile();
  }
  return DjangohFile;
}

T4DjangohFile::T4DjangohFile(void)
{
  settingsFile = T4SettingsFile::getInstance();
  std::string filename = settingsFile->getStructManager()->getExternal()
      ->localGeneratorFile;

  std::ifstream file;
  file.open(filename.c_str(), std::ios::in | std::ios::binary);

  if (!readFile(file, sizeof(G4int), true)) {
    file.seekg(0, ios::beg);
    readFile(file, sizeof(int64_t), false);
  }

  file.close();

  T4SMessenger::getInstance()->printMessage(T4SNotice, __LINE__, __FILE__,
      "T4DjangohProcess. Reading file is finished. Number of collected events: "
          + intToStr(DjangohCollection.size()) + ".");

  if (settingsFile->getStructManager()->getBeam()->numParticles
      > DjangohCollection.size()) {
    T4SMessenger::getInstance()->printMessage(T4SWarning, __LINE__, __FILE__,
        "T4DjangohProcess: Desired number of events ("
            + intToStr(
                settingsFile->getStructManager()->getBeam()->numParticles)
            + ") is larger than the number of loaded Djangoh events and will be adjusted.");

    settingsFile->getStructManager()->getBeam()->numParticles = DjangohCollection
        .size();
  }

  DjangohEventNo = 0;
}

T4DjangohFile::~T4DjangohFile(void)
{
  DjangohFile = NULL;
}

T4BeamData* T4DjangohFile::getNextDjangohEvent(void)
{
  DjangohEventNo++;
  if (DjangohEventNo == DjangohCollection.size()) {
    T4SMessenger::getInstance()->printMessage(T4SWarning,__LINE__,__FILE__,
    "T4DjangohFile::getNextDjangohEvent: No more Djangoh events available! Most likely some of them missed the target. Aborting Run to keep up system stability!");
    G4RunManager::GetRunManager()->AbortRun(true);
  }
  return getCurrentDjangohEvent();
}

bool T4DjangohFile::readFile(std::ifstream& file, streamsize headerSize,
    bool firstCall)
{
  G4int byteInt = 0;
  G4float byteFloat;

  std::string filename = settingsFile->getStructManager()->getExternal()
      ->localGeneratorFile;

  file.read((char*) &byteInt, headerSize);
  if (byteInt == 228) {
    if (firstCall)
      T4SMessenger::getInstance()->printMessage(T4SNotice, __LINE__, __FILE__,
          "T4DjangohProcess::readFile: Header of the Djangoh file " + filename
              + " is readable.");
  } else {
    T4SMessenger::getInstance()->printMessage(T4SFatalError, __LINE__, __FILE__,
        "T4DjangohProcess::readFile: Header of the Djangoh file " + filename
            + " is not readable.");
  }
  file.read((char*) &byteInt, sizeof(G4int)); // Djangoh binary file version = 2

  T4BeamData beamData;
  beamData.setDefault();
  beamData.generator = 2;

  for (unsigned int i = 0; i < 14; i++) {
    file.read((char*) &byteFloat, sizeof(G4float));
    beamData.cut[i] = byteFloat;
  }
  for (unsigned int i = 0; i < 20; i++) {
    file.read((char*) &byteInt, sizeof(G4int));
    beamData.lst[i] = byteInt;
  }

  file.read((char*) &byteInt, sizeof(G4int)); // unidentified
  file.read((char*) &byteInt, sizeof(G4int)); // unidentified

  for (unsigned int i = 0; i < 19; i++) {
    file.read((char*) &byteFloat, sizeof(G4float));
    beamData.parl[i] = byteFloat;
  }

  file.read((char*) &byteFloat, sizeof(G4float)); // unidentified = 0.1
  file.read((char*) &byteInt, headerSize); // end flag of header = 228

  if (byteInt != 228) {
    if (firstCall) {
      T4SMessenger::getInstance()->printMessage(T4SNotice, __LINE__, __FILE__,
          "T4DjangohProcess::readFile: Stop flag of the Djangoh file header: "
              + intToStr(byteInt)
              + " is not 228 as expected. Try again with 64bit header words.");
      return false;
    } else {
      T4SMessenger::getInstance()->printMessage(T4SFatalError, __LINE__,
          __FILE__,
          " T4DjangohProcess::readFile: Again wrong stop flag of the Djangoh file header: "
              + intToStr(byteInt) + " instead of 228.");
    }
  }

  // the varibales until now are equal for all events...
  // now the event loop stats

  while (!file.eof()) {
    file.read((char*) &byteInt, headerSize); // start flag (432 for DVCS, 512 for rho0)
    G4int start = byteInt;
    if (file.eof())
      break;

    // num particles
    file.read((char*) &byteInt, sizeof(G4int));
    beamData.nBeamParticle = byteInt;

    for (unsigned int i = 0; i < 11; i++) {
      file.read((char*) &byteInt, sizeof(G4int));
      beamData.lst[i + 20] = byteInt;
    }

    for (unsigned int i = 0; i < 11; i++) {
      file.read((char*) &byteFloat, sizeof(G4float));
      beamData.parl[i + 19] = byteFloat;
    }

    file.read((char*) &byteFloat, sizeof(G4float));
    beamData.x_bj = byteFloat;
    file.read((char*) &byteFloat, sizeof(G4float));
    beamData.y = byteFloat;
    file.read((char*) &byteFloat, sizeof(G4float));
    beamData.w2 = byteFloat;
    file.read((char*) &byteFloat, sizeof(G4float));
    beamData.q2 = byteFloat;
    file.read((char*) &byteFloat, sizeof(G4float));
    beamData.nu = byteFloat;

    for (unsigned int i = 0; i < 20; i++) {
      file.read((char*) &byteFloat, sizeof(G4float));
      beamData.uservar[i] = byteFloat;
    }

    beamData.beamParticles.clear();
    beamData.beamParticles.resize(beamData.nBeamParticle);

    for (unsigned int i = 0; i < 5; i++) {
      for (unsigned int j = 0; j < beamData.nBeamParticle; j++) {
        file.read((char*) &byteInt, sizeof(G4int));
        beamData.beamParticles.at(j).k[i] = byteInt;
      }
    }

    for (unsigned int i = 0; i < 5; i++) {
      for (unsigned int j = 0; j < beamData.nBeamParticle; j++) {
        file.read((char*) &byteFloat, sizeof(G4float));
        beamData.beamParticles.at(j).p[i] = byteFloat;
      }
    }

    DjangohCollection.push_back(beamData);

    file.read((char*) &byteInt, headerSize); // stop flag
    if (byteInt != start) {
      if (firstCall) {
        T4SMessenger::getInstance()->printMessage(T4SNotice, __LINE__, __FILE__,
            "T4DjangohProcess::readFile: Stop flag of the Djangoh event: "
                + intToStr(byteInt) + " is not " + intToStr(start)
                + " as expected. Try again with 64bit header words.");
        return false;
      } else {
        T4SMessenger::getInstance()->printMessage(T4SFatalError, __LINE__,
            __FILE__,
            "T4DjangohProcess::readFile: Stop flag of the Djangoh event: "
                + intToStr(byteInt) + " is not " + intToStr(start)
                + " as expected.");
        return false;
      }
    }
  }

  return true;
}
