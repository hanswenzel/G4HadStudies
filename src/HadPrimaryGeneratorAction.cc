#include "HadPrimaryGeneratorAction.hh"
#include "HadAnalysis.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include <time.h>
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadPrimaryGeneratorAction::HadPrimaryGeneratorAction()
{
  particleGun  = new G4ParticleGun(1);

  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  G4double zVertex = -5.*mm;
  particleGun->SetParticlePosition(G4ThreeVector(0.,0.,zVertex));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadPrimaryGeneratorAction::~HadPrimaryGeneratorAction()
{
  delete particleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HadPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  particleGun->GeneratePrimaryVertex(anEvent);
  HadAnalysis* analysis = HadAnalysis::getInstance();
  G4double ener = particleGun->GetParticleEnergy();
  G4ParticleDefinition* part  = particleGun->GetParticleDefinition();

  analysis->SetPrimGenInfo(ener,part);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
