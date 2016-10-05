#include "T4DjangohProcess.hh"

G4VParticleChange* T4DjangohProcess::PostStepDoIt(const G4Track& aTrack,
    const G4Step& aStep)
{
  aParticleChange.Initialize(aTrack);

  T4BeamData* beamData = DjangohFile->getCurrentDjangohEvent();
  beamData->generator = eventGenId;
  if (beamData->beamParticles.size() == 0)
    T4SMessenger::getInstance()->printMessage(T4SFatalError, __LINE__, __FILE__,
        "T4DjangohProcess::PostStepDoIt: DJANGOH file error: beamParticles == 0.");

  G4int nSecondaries = 0;
  for (unsigned int i = 0; i < beamData->nBeamParticle; i++)
    if (beamData->beamParticles.at(i).k[0] == 1)
      nSecondaries++;

  aParticleChange.SetNumberOfSecondaries(nSecondaries);

  std::pair<double, G4ThreeVector> rotation = getRotation(
      aStep.GetPostStepPoint()->GetMomentumDirection(),
      G4ThreeVector(beamData->beamParticles.at(0).p[0],
          beamData->beamParticles.at(0).p[1],
          beamData->beamParticles.at(0).p[2]));

  G4ThreeVector vertexPosition = aStep.GetPostStepPoint()->GetPosition();
  G4double vertexTime = aStep.GetPostStepPoint()->GetGlobalTime();

  for (unsigned int i = 0; i < beamData->nBeamParticle; i++) {
    T4BeamParticle* secondary = &beamData->beamParticles.at(i);

    if (rotation.first != 0.) {
      G4ThreeVector helper = G4ThreeVector(secondary->p[0], secondary->p[1],
          secondary->p[2]);
      helper.rotate(rotation.first, rotation.second);
      secondary->p[0] = helper.x();
      secondary->p[1] = helper.y();
      secondary->p[2] = helper.z();
    }

    if (beamData->beamParticles.at(i).k[0] == 1) {
      G4double kinEnergy = (beamData->beamParticles.at(i).p[3]
          - beamData->beamParticles.at(i).p[4]) * CLHEP::GeV;
      G4ThreeVector momentumDirection = G4ThreeVector(
          beamData->beamParticles.at(i).p[0],
          beamData->beamParticles.at(i).p[1],
          beamData->beamParticles.at(i).p[2]).unit();
      G4DynamicParticle* aParticle = new G4DynamicParticle(
          getParticleByID(beamData->beamParticles.at(i).k[1]),
          momentumDirection, kinEnergy);
      G4Track* aSecondaryTrack = new G4Track(aParticle, vertexTime,
          vertexPosition);
      aSecondaryTrack->SetTouchableHandle(aTrack.GetTouchableHandle());
      aSecondaryTrack->SetParentID(aTrack.GetTrackID());
      aParticleChange.AddSecondary(aSecondaryTrack);
    }
  }

  setThreeVector<double>(beamData->vertexPosition, vertexPosition.x(),
      vertexPosition.y(), vertexPosition.z());

  beamData->vertexTime = vertexTime;

  aParticleChange.ProposeTrackStatus(fStopAndKill);
  targetTracking->processCalled();

  T4SMessenger::getInstance()->printMessage(T4SVerboseMore, __LINE__, __FILE__,
      "T4DjangohProcess::PostStepDoIt: T4DjangohProcess was executed.");

  T4EventManager::getInstance()->setBeamData(beamData);

  return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}
