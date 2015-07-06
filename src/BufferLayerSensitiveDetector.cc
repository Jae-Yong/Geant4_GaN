#include "AnalysisManager.hh"

#include "BufferLayerSensitiveDetector.hh"
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

BufferLayerSensitiveDetector::BufferLayerSensitiveDetector(
							const G4String& name, 
							const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name)
{
	collectionName.insert(hitsCollectionName);

	AnalysisManager* analysisManager = AnalysisManager::GetInstance();
	analysisManager->CreateHisto("BufferLayer", "BufferLayer (eV)", 10000, 0, 0.1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BufferLayerSensitiveDetector::~BufferLayerSensitiveDetector() 
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BufferLayerSensitiveDetector::Initialize(G4HCofThisEvent*)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool BufferLayerSensitiveDetector::ProcessHits(G4Step* step, 
									 G4TouchableHistory*)
{
	G4double totDepEnergy = step->GetTotalEnergyDeposit();
	const G4ThreeVector& pos = step->GetTrack()->GetPosition();

	AnalysisManager* analysisManager = AnalysisManager::GetInstance();
	analysisManager->Fill("BufferLayer", totDepEnergy);

	return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BufferLayerSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{

}