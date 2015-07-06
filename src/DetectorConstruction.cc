#include "DetectorConstruction.hh"

#include "G4SystemOfUnits.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4Box.hh"
#include "G4Hype.hh"

#include "G4VisAttributes.hh"

#include "AlGaNSensitiveDetector.hh"
#include "GaNSensitiveDetector.hh"
#include "BufferLayerSensitiveDetector.hh"
#include "SiSensitiveDetector.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{ }

DetectorConstruction::~DetectorConstruction()
{ }

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	// Get nist material manager
	G4NistManager* nist = G4NistManager::Instance();

	// Option to switch on/off checking of volumes overlaps
	//
	G4bool checkOverlaps = true;

	//
	// World
	//
	G4double world_sizeXY = 10*cm;
	G4double world_sizeZ  = 10*cm;
	G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

	G4VSolid* solidWorld =
	new G4Box("World",                          //its name
	world_sizeXY, world_sizeXY, world_sizeZ);   //its size

	G4LogicalVolume* logicWorld =                         
	new G4LogicalVolume(solidWorld,     //its solid
			world_mat,                //its material
			"World");                 //its name

	G4VPhysicalVolume* physWorld = 
	new G4PVPlacement(0,                    //no rotation
			G4ThreeVector(),              //at (0,0,0)
			logicWorld,                   //its logical volume
			"World",                      //its name
			0,                            //its mother  volume
			false,                        //no boolean operation
			0,                            //copy number
			checkOverlaps);               //overlaps checking

	//
	// AlGaN
	//

	G4Material* matAlGaN = MakeAlGaN();

	G4double AlGaN_xy = 1. / 2. * cm;
	G4double AlGaN_z  = 25. / 2. * nm;
	G4VSolid* AlGaN_LV_box = new G4Box("AlGaN_box", AlGaN_xy, AlGaN_xy, AlGaN_z);
	G4LogicalVolume* AlGaN_LV =
		new G4LogicalVolume(AlGaN_LV_box,       //its solid
						  matAlGaN,   //its material
						  "AlGaN_LV");             //its name

	G4VisAttributes* AlGaNVisAtt = new G4VisAttributes(G4Colour::Red());
	AlGaN_LV->SetVisAttributes(AlGaNVisAtt);

	G4ThreeVector AlGaN_pos(0, 0, AlGaN_z);
	new G4PVPlacement(0,				///no rotation
		  AlGaN_pos,					//at position
		  AlGaN_LV,						//its logical volume
		  "AlGaN_PV",                   //its name
		  logicWorld,    				///its mother  volume
		  false,						///no boolean operation
		  0,							///copy number
		  checkOverlaps);				///overlaps checking

	//
	// GaN
	//

	G4Material* matGaN = MakeGaN();

	G4double GaN_xy = AlGaN_xy;
	G4double GaN_z  = 1.5 / 2. * um;
	G4VSolid* GaN_LV_box = new G4Box("GaN_box", GaN_xy, GaN_xy, GaN_z);
	G4LogicalVolume* GaN_LV =
		new G4LogicalVolume(GaN_LV_box,		//its solid
						  matGaN,			//its material
						  "GaN_LV");		//its name

	G4VisAttributes* GaNVisAtt = new G4VisAttributes(G4Colour::Blue());
	GaN_LV->SetVisAttributes(GaNVisAtt);

	G4ThreeVector GaN_pos(0, 0, 2 * AlGaN_z + GaN_z);
	new G4PVPlacement(0,				///no rotation
		  GaN_pos,						//at position
		  GaN_LV,						//its logical volume
		  "GaN_PV",						//its name
		  logicWorld,    				///its mother  volume
		  false,						///no boolean operation
		  0,							///copy number
		  checkOverlaps);				///overlaps checking

	//
	// Buffer (AlGaN)
	//

	G4double bufferLayer_z  = 0.8 / 2. * um;
	G4VSolid* BufferLayer_LV_box = new G4Box("BufferLayer_box", AlGaN_xy, AlGaN_xy, bufferLayer_z);
	G4LogicalVolume* BufferLayer_LV =
		new G4LogicalVolume(BufferLayer_LV_box,		//its solid
						  matAlGaN,					//its material
						  "BufferLayer_LV");		//its name

	G4VisAttributes* bufferVisAtt = new G4VisAttributes(G4Colour::Cyan());
	BufferLayer_LV->SetVisAttributes(bufferVisAtt);

	G4ThreeVector BufferLayer_pos(0, 0, 2 * AlGaN_z + 2 * GaN_z + bufferLayer_z);
	new G4PVPlacement(0,				///no rotation
		  BufferLayer_pos,					//at position
		  BufferLayer_LV,						//its logical volume
		  "BufferLayer_PV",                   //its name
		  logicWorld,    				///its mother  volume
		  false,						///no boolean operation
		  0,							///copy number
		  checkOverlaps);				///overlaps checking

	//
	// Substrate (Silicon(111))
	//

	G4Material* matSi = nist->FindOrBuildMaterial("G4_Si");

	G4double substrate_xy = GaN_xy;
	G4double substrate_z  = 500. / 2 * um;
	G4VSolid* substarte_box = new G4Box("Substrate_box", substrate_xy, substrate_xy, substrate_z);
	G4LogicalVolume* substrate_LV = new G4LogicalVolume(substarte_box,
															matSi,
															"Substrate_LV");

	G4VisAttributes* SiVisAtt = new G4VisAttributes(G4Colour::Gray());
	substrate_LV->SetVisAttributes(SiVisAtt);

	G4ThreeVector substrate_pos(0, 0, 2 * AlGaN_z + 2 * GaN_z + 2 * bufferLayer_z + substrate_z);
	new G4PVPlacement(0,
						substrate_pos,
						substrate_LV,
						"Substrate_PV",
						logicWorld,
						false,
						0,
						checkOverlaps);

	//
	//always return the physical World
	//
	return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
	AlGaNSensitiveDetector* AlGaN_sd = new AlGaNSensitiveDetector("AlGaN_SD", "AlGaN_SDHitsCollection");
	SetSensitiveDetector("AlGaN_LV", AlGaN_sd);

	GaNSensitiveDetector* GaN_sd = new GaNSensitiveDetector("GaN_SD", "GaN_SDHitsCollection");
	SetSensitiveDetector("GaN_LV", GaN_sd);

	BufferLayerSensitiveDetector* buff_sd = new BufferLayerSensitiveDetector("buff_SD", "buff_SDHitsCollection");
	SetSensitiveDetector("BufferLayer_LV", buff_sd);

	SiSensitiveDetector* Si_sd = new SiSensitiveDetector("Si_sd", "Si_SDHitsCollection");
	SetSensitiveDetector("Substrate_LV", Si_sd);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* DetectorConstruction::MakeAlGaN()
{
	G4String name, symbol;
	G4double a, z, density;

	G4int ncomponents;
	G4double fractionmass;

	// Aluminum
	a = 26.981538 * g/mole;
	G4Element* elAl = new G4Element(name="Aluminum", symbol="Al", z = 13, a);

	// Gallium
	a = 69.723 * g/mole;
	G4Element* elGa = new G4Element(name="Gallium", symbol="Ga", z = 31, a);

	// Nitrogen
	a = 14.0067 * g/mole;
	G4Element* elN = new G4Element(name="Nitrogen", symbol="N", z = 7, a);

	density = 5.39 * g/cm3;
	G4Material* matAlGaN = new G4Material(name="AlGaN", density, ncomponents = 3);
	matAlGaN->AddElement(elAl, fractionmass = 0.125);
	matAlGaN->AddElement(elGa, fractionmass = 0.375);
	matAlGaN->AddElement(elN,  fractionmass = 0.5);

	return matAlGaN;
}

G4Material* DetectorConstruction::MakeGaN()
{
	G4String name, symbol;
	G4double a, z, density;

	G4int ncomponents;
	G4double fractionmass;

	// Gallium
	a = 69.723 * g/mole;
	G4Element* elGa = new G4Element(name="Gallium", symbol="Ga", z = 31, a);

	// Nitrogen
	a = 14.0067 * g/mole;
	G4Element* elN = new G4Element(name="Nitrogen", symbol="N", z = 7, a);

	density = 6.25 * g/cm3;
	G4Material* matGaN = new G4Material(name="GaN", density, ncomponents = 2);
	matGaN->AddElement(elGa, fractionmass = 0.5);
	matGaN->AddElement(elN,  fractionmass = 0.5);

	return matGaN;
}