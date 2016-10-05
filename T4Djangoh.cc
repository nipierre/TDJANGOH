#include "T4Djangoh.hh"

static T4Djangoh* t4Djangoh = new T4Djangoh();

T4Djangoh::T4Djangoh(void)
{
  pluginName = "DJANGOH";
  eventGenId = 2;
  registerPlugin();

  DjangohFile = NULL;
}

void T4Djangoh::initialize(void)
{
  DjangohFile = T4DjangohFile::getInstance();
  T4BeamBackEnd::initialize();
}

void T4Djangoh::generateEvent(G4Event* event)
{
  T4BeamFileParticle beamFileParticle;
  T4BeamData* beamData;
  while (true) {
    beamData = DjangohFile->getNextDjangohEvent();
    beamFileParticle.posX = beamData->uservar[0] * CLHEP::mm;
    beamFileParticle.posY = beamData->uservar[1] * CLHEP::mm;
    beamFileParticle.posZ = settingsFile->getStructManager()->getBeam()->beamFileZConvention;
    beamFileParticle.energy = (beamData->beamParticles.at(0).p[3]
        - beamData->beamParticles.at(0).p[4]) * CLHEP::GeV;
    beamFileParticle.momentumDirection = G4ThreeVector(
        beamData->beamParticles.at(0).p[0], beamData->beamParticles.at(0).p[1],
        beamData->beamParticles.at(0).p[2]);
    beamFileParticle.momentumDirection.setMag(beamFileParticle.energy);
    beamFileParticle.beamFileType = TGEANT::Normal;

    if (settingsFile->getStructManager()->getBeam()->useTargetExtrap) {
      if (extrapolate->extrapAndCalcTargetDist(beamFileParticle))
        break;
    } else {
      extrapolate->extrap(beamFileParticle);
      break;
    }
  }

  // incoming beam particle
  G4int incomingId = beamData->beamParticles.at(0).k[1];
  if (incomingId != settingsFile->getStructManager()->getBeam()->beamParticle) {
    T4SMessenger::getInstance()->printMessage(T4SErrorNonFatal, __LINE__,
            __FILE__,
            "T4Djangoh::generateEvent: Particle ID missmatch: Djangoh file ("
                + intToStr(incomingId) + ") and beamParticle ("
		+ intToStr(settingsFile->getStructManager()->getBeam()->beamParticle)
                + ").");
  }
  particleGun->SetParticleDefinition(getParticleByID(incomingId));

  particleGun->SetParticleEnergy(
      beamFileParticle.energy + extrapolate->getAdditionalKineticEnergy());

  particleGun->SetParticlePosition(
      G4ThreeVector(beamFileParticle.posX, beamFileParticle.posY,
          beamFileParticle.posZ));
  particleGun->SetParticleMomentumDirection(beamFileParticle.momentumDirection);
  particleGun->SetNumberOfParticles(1);
  particleGun->SetParticleTime(0);
  particleGun->GeneratePrimaryVertex(event);

  // The pile up code must stay below the code for the "real" primary particle
  T4SBeam* beam = settingsFile->getStructManager()->getBeam();
  if (beam->useBeamfile && beam->usePileUp)
    generatePileUp(event);
}

T4ProcessBackEnd* T4Djangoh::getEventGeneratorProcess(void)
{
  if (process == NULL) {
    process = new T4DjangohProcess();
    process->setEventGenId(eventGenId);
  }
  return process;
}
