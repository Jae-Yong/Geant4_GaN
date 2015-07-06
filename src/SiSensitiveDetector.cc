#include "AnalysisManager.hh"

#include "SiSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "G4ParticleDefinition.hh"
#include "G4DynamicParticle.hh"
#include "G4TrackVector.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SiSensitiveDetector::SiSensitiveDetector(
							const G4String& name, 
							const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name)
{
	collectionName.insert(hitsCollectionName);

	AnalysisManager* analysisManager = AnalysisManager::GetInstance();
	analysisManager->CreateHisto("Si", "Si (eV)", 100000, 0, 4);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SiSensitiveDetector::~SiSensitiveDetector() 
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiSensitiveDetector::Initialize(G4HCofThisEvent*)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SiSensitiveDetector::ProcessHits(G4Step* step, 
									 G4TouchableHistory*)
{
	G4double totDepEnergy = step->GetTotalEnergyDeposit();
	const G4ThreeVector& pos = step->GetTrack()->GetPosition();

	AnalysisManager* analysisManager = AnalysisManager::GetInstance();
	analysisManager->Fill("Si", totDepEnergy, 1);

	return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SiSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{

}