#ifndef __PRIMARY_GENERATOR_ACTION_HH__
#define __PRIMARY_GENERATOR_ACTION_HH__

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4Box.hh"
#include "globals.hh"
#include "Point.hh"

#define PARTICLE_ENERGY 5.2*MeV

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();    
    virtual ~PrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
  
    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
  
  private:
    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
    G4Box* fTarget;

    Point<double> GenerateRndPoint(double max);
};


#endif // __PRIMARY_GENERATOR_ACTION_HH__
