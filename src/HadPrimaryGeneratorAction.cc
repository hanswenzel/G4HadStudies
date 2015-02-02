
#include "HadPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "HadAnalysis.hh"
#include <time.h>
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadPrimaryGeneratorAction::HadPrimaryGeneratorAction()
{
  particleGun  = new G4ParticleGun(1);

  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadPrimaryGeneratorAction::~HadPrimaryGeneratorAction()
{
  delete particleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HadPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4double zVertex = -5.*mm;
  particleGun->SetParticlePosition(G4ThreeVector(0.,0.,zVertex));
  particleGun->GeneratePrimaryVertex(anEvent);
  HadAnalysis* analysis = HadAnalysis::getInstance();
  Double_t ener = particleGun->GetParticleEnergy();
  G4ParticleDefinition* part  = particleGun->GetParticleDefinition();

  analysis->GetPrimGenInfo(ener,part);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
