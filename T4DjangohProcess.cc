#include "T4PythiaProcess.hh"

<<<<<<< HEAD
#ifdef USE_PYTHIA

#ifdef USE_LHAPDF
#include "LHAPDF/LHAPDF.h"
#endif

static int pyStatusCode;

extern "C"
{
  void pystop_(int *mcod)
  {
    T4SMessenger::getInstance()->printfMessage(T4SWarning, __LINE__, __FILE__,
        "Pythia6 has thrown an error (%i) while execution! Resetting tPythia6 \n",
        *mcod);
    pyStatusCode = *mcod;
  }
}

void T4PythiaProcess::initPythia(void)
{
  if (tPythia6 != NULL)
    delete tPythia6;

  pythia = settingsFile->getStructManager()->getPythia();

  tPythia6 = new TPythia6();

  //process list
  tPythia6->SetMSEL(pythia->genericFlags.msel);

  //mdme flags
  for (unsigned int i = 0; i < pythia->flagsMDME.size(); i++) {
=======
#ifdef USE_DJANGOH
// Might come in handy but need to code in Djangoh
// extern "C"
// {
//   void pystop_(int *mcod)
//   {
//     T4SMessenger::getInstance()->printfMessage(T4SWarning, __LINE__, __FILE__,
//         "Pythia6 has thrown an error (%i) while execution! Resetting tPythia6 \n",
//         *mcod);
//     pyStatusCode = *mcod;
//   }
// }

void T4PythiaProcess::initDjangoh(void)
{
  if (tDjangoh != NULL)
    delete tDjangoh;

  djangoh = settingsFile->getStructManager()->getDjangoh();

  tDjangoh = new TDjangoh();

  // Again might implement it this way of not, to see
  //process list
  // tPythia6->SetMSEL(pythia->genericFlags.msel);

  //mdme flags
  // for (unsigned int i = 0; i < pythia->flagsMDME.size(); i++) {
>>>>>>> af1c408f6449350c72e66701e26235bbdd1030ad
//    T4SMessenger::getInstance()->printfMessage(T4SVerboseMore, __LINE__,
//        __FILE__, "T4PythiaProcess setting MDME num: %i flag1: %e flag2: %e \n",
//        pythia->flagsMDME.at(i).number, pythia->flagsMDME.at(i).flag1,
//        pythia->flagsMDME.at(i).flag2);
<<<<<<< HEAD
    tPythia6->SetMDME(pythia->flagsMDME.at(i).number,
        pythia->flagsMDME.at(i).flag1, pythia->flagsMDME.at(i).flag2);
  }
  //other generic flags
  for (unsigned int i = 0; i < pythia->flagsMSBD.size(); i++) {
=======
  //   tPythia6->SetMDME(pythia->flagsMDME.at(i).number,
  //       pythia->flagsMDME.at(i).flag1, pythia->flagsMDME.at(i).flag2);
  // }
  //other generic flags
  // for (unsigned int i = 0; i < pythia->flagsMSBD.size(); i++) {
>>>>>>> af1c408f6449350c72e66701e26235bbdd1030ad
//    T4SMessenger::getInstance()->printfMessage(T4SVerboseMore, __LINE__,
//        __FILE__,
//        "T4PythiaProcess adding tuning type: %i cutNum: %i setTo: %e \n",
//        pythia->flagsMSBD.at(i).type, pythia->flagsMSBD.at(i).number,
//        pythia->flagsMSBD.at(i).flag1);
<<<<<<< HEAD
    switch (pythia->flagsMSBD.at(i).type) {
      case MSTP:
        tPythia6->SetMSTP(pythia->flagsMSBD.at(i).number,
            pythia->flagsMSBD.at(i).flag1);
        break;
      case MSTU:
        tPythia6->SetMSTU(pythia->flagsMSBD.at(i).number,
            pythia->flagsMSBD.at(i).flag1);
        break;
      case PARU:
        tPythia6->SetPARU(pythia->flagsMSBD.at(i).number,
            pythia->flagsMSBD.at(i).flag1);
        break;
      case PARP:
        tPythia6->SetPARP(pythia->flagsMSBD.at(i).number,
            pythia->flagsMSBD.at(i).flag1);
        break;
      case CKIN:
        tPythia6->SetCKIN(pythia->flagsMSBD.at(i).number,
            pythia->flagsMSBD.at(i).flag1);
        break;
    };
  }

  int pdfNum;
  string pdfName;
  string pdfType;
  int pdfMemSet;

#ifdef USE_LHAPDF
  LHAPDF::SetType pdfTypeEnum;

  for (unsigned int i = 0; i < pythia->pdfList.size(); i++) {
    if (pythia->pdfList.at(i).pdfType == "EVOLVE")
      pdfTypeEnum = LHAPDF::EVOLVE;
    else if (pythia->pdfList.at(i).pdfType == "LHPDF")
      pdfTypeEnum = LHAPDF::LHPDF;
    else if (pythia->pdfList.at(i).pdfType == "LHGRID")
      pdfTypeEnum = LHAPDF::LHGRID;
    else if (pythia->pdfList.at(i).pdfType == "INTERPOLATE")
      pdfTypeEnum = LHAPDF::INTERPOLATE;
    else
      T4SMessenger::getInstance()->printfMessage(T4SWarning, __LINE__, __FILE__,
          "T4PythiaProcess: LHAPDF-pdfType is NOT set correctly! Please use EVOLVE,LHPDF,INTERPOLATE or LHGRID! You used: %s\n",
          pythia->pdfList.at(i).pdfType.c_str());

    LHAPDF::setVerbosity(LHAPDF::SILENT);
    LHAPDF::initPDFByName(pythia->pdfList.at(i).pdfNum,
        pythia->pdfList.at(i).pdfName.c_str(), pdfTypeEnum,
        pythia->pdfList.at(i).pdfMemSet);
  }
#endif

  // Random seed
  tPythia6->SetMRPY(1, static_cast<int>(CLHEP::RandFlat::shoot(settingsFile->getRandom()->getTheEngine(),0, 900000001)));
}

T4PythiaProcess::T4PythiaProcess(void)
{
  pyStatusCode = 0;
  tPythia6 = NULL;
  initPythia();
=======
    // switch (pythia->flagsMSBD.at(i).type) {
    //   case MSTP:
    //     tPythia6->SetMSTP(pythia->flagsMSBD.at(i).number,
    //         pythia->flagsMSBD.at(i).flag1);
    //     break;
    //   case MSTU:
    //     tPythia6->SetMSTU(pythia->flagsMSBD.at(i).number,
    //         pythia->flagsMSBD.at(i).flag1);
    //     break;
    //   case PARU:
    //     tPythia6->SetPARU(pythia->flagsMSBD.at(i).number,
    //         pythia->flagsMSBD.at(i).flag1);
    //     break;
    //   case PARP:
    //     tPythia6->SetPARP(pythia->flagsMSBD.at(i).number,
    //         pythia->flagsMSBD.at(i).flag1);
    //     break;
    //   case CKIN:
    //     tPythia6->SetCKIN(pythia->flagsMSBD.at(i).number,
    //         pythia->flagsMSBD.at(i).flag1);
    //     break;
    // };
  // }
//
//   int pdfNum;
//   string pdfName;
//   string pdfType;
//   int pdfMemSet;
//
// #ifdef USE_LHAPDF
//   LHAPDF::SetType pdfTypeEnum;
//
//   for (unsigned int i = 0; i < pythia->pdfList.size(); i++) {
//     if (pythia->pdfList.at(i).pdfType == "EVOLVE")
//       pdfTypeEnum = LHAPDF::EVOLVE;
//     else if (pythia->pdfList.at(i).pdfType == "LHPDF")
//       pdfTypeEnum = LHAPDF::LHPDF;
//     else if (pythia->pdfList.at(i).pdfType == "LHGRID")
//       pdfTypeEnum = LHAPDF::LHGRID;
//     else if (pythia->pdfList.at(i).pdfType == "INTERPOLATE")
//       pdfTypeEnum = LHAPDF::INTERPOLATE;
//     else
//       T4SMessenger::getInstance()->printfMessage(T4SWarning, __LINE__, __FILE__,
//           "T4PythiaProcess: LHAPDF-pdfType is NOT set correctly! Please use EVOLVE,LHPDF,INTERPOLATE or LHGRID! You used: %s\n",
//           pythia->pdfList.at(i).pdfType.c_str());
//
//     LHAPDF::setVerbosity(LHAPDF::SILENT);
//     LHAPDF::initPDFByName(pythia->pdfList.at(i).pdfNum,
//         pythia->pdfList.at(i).pdfName.c_str(), pdfTypeEnum,
//         pythia->pdfList.at(i).pdfMemSet);
//   }
// #endif

}

T4DjangohProcess::T4DjangohProcess(void)
{
  pyStatusCode = 0;
  tPythia6 = NULL;
  initDjangoh();
>>>>>>> af1c408f6449350c72e66701e26235bbdd1030ad
  beamParticle = getParticleString(
      settingsFile->getStructManager()->getBeam()->beamParticle);
}

<<<<<<< HEAD
T4PythiaProcess::~T4PythiaProcess(void)
{
  delete tPythia6;
}

G4VParticleChange* T4PythiaProcess::PostStepDoIt(const G4Track& aTrack,
=======
T4DjangohProcess::~T4DjangohProcess(void)
{
  delete tDjangoh;
}

G4VParticleChange* T4DjangohProcess::PostStepDoIt(const G4Track& aTrack,
>>>>>>> af1c408f6449350c72e66701e26235bbdd1030ad
    const G4Step& aStep)
{
  aParticleChange.Initialize(aTrack);

  G4double energy = aStep.GetPostStepPoint()->GetKineticEnergy() / CLHEP::GeV;
<<<<<<< HEAD
  tPythia6->Initialize("FIXT", beamParticle.c_str(), pythia->target.c_str(),
      energy); // needs GeV

  //if we get a bad status from pythia, we reset the status, kill the particle and commence as if anything has happened
  if (pyStatusCode != 0) {
    pyStatusCode = 0;
    aParticleChange.ProposeTrackStatus(fStopAndKill);
    targetTracking->processCalled();
    targetTracking->dontSaveThisEvent();
    initPythia();
    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }

  tPythia6->GenerateEvent();

  if (settingsFile->getStructManager()->getGeneral()->verboseLevel > 2)
    tPythia6->Pylist(1);
=======
  tDjangoh->Initialize(beamParticle.c_str(), pythia->nuc_A, pythia->nuc_Z,
      energy, 0); // needs GeV

  //if we get a bad status from pythia, we reset the status, kill the particle and commence as if anything has happened
  // if (pyStatusCode != 0) {
  //   pyStatusCode = 0;
  //   aParticleChange.ProposeTrackStatus(fStopAndKill);
  //   targetTracking->processCalled();
  //   targetTracking->dontSaveThisEvent();
  //   initPythia();
  //   return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  // }

  tDjangoh->GenerateEvent();

  if (settingsFile->getStructManager()->getGeneral()->verboseLevel > 2)
    tDjangoh->Lulist(1);
>>>>>>> af1c408f6449350c72e66701e26235bbdd1030ad

  std::pair<G4double, G4ThreeVector> rotation = getRotation(
      aStep.GetPostStepPoint()->GetMomentumDirection());

  T4BeamData* beamData = T4EventManager::getInstance()->getBeamData();
  beamData->setDefault();

<<<<<<< HEAD
  Pyjets_t* pythiaParticle = tPythia6->GetPyjets();

  std::vector<G4int> lineList;
  for (G4int i = 0; i < tPythia6->GetNumberOfParticles(); i++) {
    if (pythiaParticle->K[0][i] != 1)
      continue;
    G4int id = pythiaParticle->K[1][i];
=======
  Pyjets_t* djangohParticle = tDjangoh->GetLujets();

  std::vector<G4int> lineList;
  for (G4int i = 0; i < tPythia6->GetNumberOfParticles(); i++) {
    if (djangohParticle->K[0][i] != 1)
      continue;
    G4int id = djangohParticle->K[1][i];
>>>>>>> af1c408f6449350c72e66701e26235bbdd1030ad

    if (std::abs(id) == 12 || std::abs(id) == 14 || std::abs(id) == 16) // skip neutrinos
      continue;
    else if ((std::abs(id) >= 1 && std::abs(id) <= 8) || id == 21)
      T4SMessenger::getInstance()->printMessage(T4SWarning, __LINE__, __FILE__,
<<<<<<< HEAD
          "T4PythiaProcess::PostStepDoIt: Pythia6 output has a quarks/gluon (id = "
              + intToStr(id)
              + ") in final state! Activate jet fragmentation MSTJ(1, 1).");
=======
          "T4DjangohProcess::PostStepDoIt: Djangoh output has a quarks/gluon (id = "
              + intToStr(id)
              + ") in final state! Activate jet fragmentation MSTJ(1, 1).
              (Should be activated per default though)");
>>>>>>> af1c408f6449350c72e66701e26235bbdd1030ad
    else
      lineList.push_back(i);
  }

  aParticleChange.SetNumberOfSecondaries(lineList.size());

  G4ThreeVector vertexPosition = aStep.GetPostStepPoint()->GetPosition();
  G4double vertexTime = aStep.GetPostStepPoint()->GetGlobalTime();

<<<<<<< HEAD
  for (G4int i = 0; i < tPythia6->GetNumberOfParticles(); i++) {
    G4int id = pythiaParticle->K[1][i];
    G4ThreeVector helper = G4ThreeVector(pythiaParticle->P[0][i],
        pythiaParticle->P[1][i], pythiaParticle->P[2][i]);
=======
  for (G4int i = 0; i < tDjangoh->GetNumberOfParticles(); i++) {
    G4int id = djangohParticle->K[1][i];
    G4ThreeVector helper = G4ThreeVector(djangohParticle->P[0][i],
        djangohParticle->P[1][i], djangohParticle->P[2][i]);
>>>>>>> af1c408f6449350c72e66701e26235bbdd1030ad

    if (rotation.first != 0)
      helper.rotate(rotation.first, rotation.second);

    T4BeamParticle particle;
    if (std::find(lineList.begin(), lineList.end(), i) != lineList.end()) {

      G4ThreeVector momentumDirection = helper.unit();
<<<<<<< HEAD
      G4double kinEnergy = pythiaParticle->P[3][i] * CLHEP::GeV; //pythia convention(?!): p[3] is set to max(kin energy, mass)

      if (kinEnergy > pythiaParticle->P[4][i] * CLHEP::GeV) {
=======
      G4double kinEnergy = djangohParticle->P[3][i] * CLHEP::GeV; //pythia convention(?!): p[3] is set to max(kin energy, mass)

      if (kinEnergy > djangohParticle->P[4][i] * CLHEP::GeV) {
>>>>>>> af1c408f6449350c72e66701e26235bbdd1030ad
        G4DynamicParticle* aParticle = new G4DynamicParticle(getParticleByID(id),
            momentumDirection, kinEnergy);

        G4ThreeVector vertexHelper = G4ThreeVector(
<<<<<<< HEAD
            pythiaParticle->V[0][i] * CLHEP::mm,
            pythiaParticle->V[1][i] * CLHEP::mm,
            pythiaParticle->V[2][i] * CLHEP::mm);
=======
            djangohParticle->V[0][i] * CLHEP::mm,
            djangohParticle->V[1][i] * CLHEP::mm,
            djangohParticle->V[2][i] * CLHEP::mm);
>>>>>>> af1c408f6449350c72e66701e26235bbdd1030ad
        if (rotation.first != 0)
          vertexHelper.rotate(rotation.first, rotation.second);

        G4Track* aSecondaryTrack = new G4Track(aParticle,
            vertexTime + pythiaParticle->V[3][i] * CLHEP::mm / CLHEP::c_light,
            vertexPosition + vertexHelper);
        aSecondaryTrack->SetTouchableHandle(aTrack.GetTouchableHandle());
        aSecondaryTrack->SetParentID(aTrack.GetTrackID());
        aParticleChange.AddSecondary(aSecondaryTrack);
      }
    }

<<<<<<< HEAD
    particle.k[0] = pythiaParticle->K[0][i];
    particle.k[1] = id;
    particle.k[2] = pythiaParticle->K[2][i];
    particle.k[3] = pythiaParticle->K[3][i];
    particle.k[4] = pythiaParticle->K[4][i];
=======
    particle.k[0] = djangohParticle->K[0][i];
    particle.k[1] = id;
    particle.k[2] = djangohParticle->K[2][i];
    particle.k[3] = djangohParticle->K[3][i];
    particle.k[4] = djangohParticle->K[4][i];
>>>>>>> af1c408f6449350c72e66701e26235bbdd1030ad

    particle.p[0] = helper.x();
    particle.p[1] = helper.y();
    particle.p[2] = helper.z();
<<<<<<< HEAD
    particle.p[3] = pythiaParticle->P[3][i];
    particle.p[4] = pythiaParticle->P[4][i];
=======
    particle.p[3] = djangohParticle->P[3][i];
    particle.p[4] = djangohParticle->P[4][i];
>>>>>>> af1c408f6449350c72e66701e26235bbdd1030ad

    beamData->beamParticles.push_back(particle);
  }

  beamData->nBeamParticle = beamData->beamParticles.size();
  beamData->generator = eventGenId;
  setThreeVector<double>(beamData->vertexPosition, vertexPosition.x(),
      vertexPosition.y(), vertexPosition.z());
  beamData->vertexTime = vertexTime;
<<<<<<< HEAD
  beamData->x_bj = tPythia6->GetVINT(41);

  std::copy(tPythia6->GetPypars()->MSTP, tPythia6->GetPypars()->MSTP + 200,
      beamData->pypars.mstp);
  std::copy(tPythia6->GetPypars()->PARP, tPythia6->GetPypars()->PARP + 200,
      beamData->pypars.parp);
  std::copy(tPythia6->GetPypars()->MSTI, tPythia6->GetPypars()->MSTI + 200,
      beamData->pypars.msti);
  std::copy(tPythia6->GetPypars()->PARI, tPythia6->GetPypars()->PARI + 200,
      beamData->pypars.pari);
=======
  beamData->x_bj = tDjangoh->GetVINT(41);

// Should see what we need
  // std::copy(tPythia6->GetPypars()->MSTP, tPythia6->GetPypars()->MSTP + 200,
  //     beamData->pypars.mstp);
  // std::copy(tPythia6->GetPypars()->PARP, tPythia6->GetPypars()->PARP + 200,
  //     beamData->pypars.parp);
  // std::copy(tPythia6->GetPypars()->MSTI, tPythia6->GetPypars()->MSTI + 200,
  //     beamData->pypars.msti);
  // std::copy(tPythia6->GetPypars()->PARI, tPythia6->GetPypars()->PARI + 200,
  //     beamData->pypars.pari);
>>>>>>> af1c408f6449350c72e66701e26235bbdd1030ad

  aParticleChange.ProposeTrackStatus(fStopAndKill);
  targetTracking->processCalled();

  T4SMessenger::getInstance()->printMessage(T4SVerboseMore, __LINE__, __FILE__,
<<<<<<< HEAD
      "T4PythiaProcess::PostStepDoIt: T4PythiaProcess was executed.");
=======
      "T4DjangohProcess::PostStepDoIt: T4DjangohProcess was executed.");
>>>>>>> af1c408f6449350c72e66701e26235bbdd1030ad

  return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}

<<<<<<< HEAD
std::string T4PythiaProcess::getParticleString(int particleId)
=======
std::string T4DjangohProcess::getParticleString(int particleId)
>>>>>>> af1c408f6449350c72e66701e26235bbdd1030ad
{
  if (particleId == 13)
    return "mu-";
  else if (particleId == -13)
    return "mu+";
  else if (particleId == 11)
    return "e-";
  else if (particleId == -11)
    return "e+";
  else if (particleId == -2212)
    return "pbar";
  else if (particleId == 2212)
    return "p";
  else if (particleId == -211)
    return "pi-";
  else if (particleId == 211)
    return "pi+";
  else if (particleId == 2112)
    return "n";
  else {
    T4SMessenger::getInstance()->printMessage(T4SErrorNonFatal, __LINE__,
    __FILE__,
<<<<<<< HEAD
        "T4PythiaProcess::getParticleString: Unknown TGEANT::BeamParticle type. Return mu+ as default.");
=======
        "T4DjangohProcess::getParticleString: Unknown TGEANT::BeamParticle type. Return mu+ as default.");
>>>>>>> af1c408f6449350c72e66701e26235bbdd1030ad
    return "mu+";
  }
}

#endif
