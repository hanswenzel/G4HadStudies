
#include "HadDetectorConstruction.hh"
#include "HadDetectorMessenger.hh"
#include "HadSD.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4HadronicProcessStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UnitsTable.hh"
#include "G4ios.hh"

#include "HadAnalysis.hh"
#include "G4NistManager.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadDetectorConstruction::HadDetectorConstruction()
{
  logicTarget = 0;
  logicWorld  = 0;
  detectorMessenger = new HadDetectorMessenger(this);

  radius = 0.3*cm;
  targetZ = 4.0*0.5*cm; 

  targetMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_C");
  worldMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadDetectorConstruction::~HadDetectorConstruction()
{ 
  delete detectorMessenger;
}

G4VPhysicalVolume* HadDetectorConstruction::Construct()
{
  // Cleanup old geometry

  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // Sizes
  G4double worldR  = radius + cm;
  G4double worldZ  = targetZ + cm;

  //
  // World
  //
  G4Tubs* solidW = new G4Tubs("World",0.,worldR,worldZ,0.,twopi);
  logicWorld = new G4LogicalVolume(solidW,worldMaterial,"World");
  G4VPhysicalVolume* world = new G4PVPlacement(0,G4ThreeVector(),
                                       logicWorld,"World",0,false,0);
  //
  // Target volume
  //
  G4Tubs* solidT = new G4Tubs("Target",0.,radius,targetZ,0.,twopi);
  logicTarget = new G4LogicalVolume(solidT,targetMaterial,"Target");
  new G4PVPlacement(0,G4ThreeVector(),logicTarget,"Target",logicWorld,false,0);
  G4String SDname = "TargetSD";
  HadSD* aTrackerSD = new HadSD(SDname, "HitsCollection");
   // Setting aTrackerSD to all logical volumes with the same name 
   // of "Target".
   SetSensitiveDetector("Target", aTrackerSD, true);
  // colors
  G4VisAttributes zero = G4VisAttributes::Invisible;
  logicWorld->SetVisAttributes(&zero);

  G4VisAttributes regCcolor(G4Colour(0., 0.3, 0.7));
  logicTarget->SetVisAttributes(&regCcolor);

  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  //for ntuple:
  HadAnalysis* analysis = HadAnalysis::getInstance();
  std::vector<Double_t> vDCinfo;
  vDCinfo.push_back(2.*targetZ);
  vDCinfo.push_back(radius);
  G4double dens  =  targetMaterial->GetDensity()/(g/cm3);
  G4String nameM =  targetMaterial->GetName();
 
  vDCinfo.push_back(dens);
  
  analysis->SetDetConstInfo(vDCinfo,targetMaterial);
  return world;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HadDetectorConstruction::SetTargetMaterial(const G4String& mat)
{
  // search the material by its name
  G4Material* material = G4NistManager::Instance()->FindOrBuildMaterial(mat);
 
  if (material && material != targetMaterial) {
    targetMaterial = material;
    if(logicTarget) logicTarget->SetMaterial(targetMaterial);
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HadDetectorConstruction::SetWorldMaterial(const G4String& mat)
{
  // search the material by its name
  G4Material* material = G4NistManager::Instance()->FindOrBuildMaterial(mat);

  if (material && material != worldMaterial) {
    worldMaterial = material;
    if(logicWorld) logicWorld->SetMaterial(worldMaterial);
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HadDetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HadDetectorConstruction::SetTargetRadius(G4double val)  
{
  if(val > 0.0) {
    radius = val;
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HadDetectorConstruction::SetTargetLength(G4double val)  
{
  if(val > 0.0) {
    targetZ = val*0.5;
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
  } 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
