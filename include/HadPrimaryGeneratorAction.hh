
#ifndef HadPrimaryGeneratorAction_h
#define HadPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HadPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    HadPrimaryGeneratorAction();
    virtual ~HadPrimaryGeneratorAction();

    void GeneratePrimaries(G4Event*);

    G4ParticleGun* GetParticleGun() {
        return particleGun;
    };
private:

    HadPrimaryGeneratorAction & operator=(const HadPrimaryGeneratorAction &right);
    HadPrimaryGeneratorAction(const HadPrimaryGeneratorAction&);
    G4ParticleGun* particleGun;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


