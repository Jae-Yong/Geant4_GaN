#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include <cmath>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),
  fTarget(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
	= particleTable->FindParticle(particleName="proton");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(PARTICLE_ENERGY);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	//this function is called at the begining of ecah event
	//
	G4double targetZ = 0;
	if (!fTarget)
	{
		G4LogicalVolume* targetLV = NULL;
		if ((targetLV = G4LogicalVolumeStore::GetInstance()->GetVolume("AlGaN_LV")) != NULL)
		{
			if ((fTarget = dynamic_cast<G4Box*>(targetLV->GetSolid())) != NULL)
				targetZ = fTarget->GetZHalfLength();
		}
	}

	Point<double> pos = GenerateRndPoint(1 * cm);

	G4double x0 = pos.GetX() - 0.5 * cm;
	G4double y0 = pos.GetY() - 0.5 * cm;

	G4double z0 = -targetZ - 5*cm;

	fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

	fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Point<double> PrimaryGeneratorAction::GenerateRndPoint(double max)
{
	double x = G4UniformRand() * max;
	double y = G4UniformRand() * max;

	return Point<double>(x, y);
}