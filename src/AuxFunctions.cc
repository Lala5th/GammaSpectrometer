#include "AuxFunctions.hh"

#include "G4Box.hh"
#include "G4MultiUnion.hh"
#include "G4DisplacedSolid.hh"

#include "Params.hh"

#include <cassert>

G4DisplacedSolid * generateBoxUnion(G4String name,G4Box* box,int number,G4ThreeVector offset){
	assert(number >= 1);
	G4MultiUnion* unio = new G4MultiUnion(name+"unio");
	for(int i = 0; i < number; i++)
		unio->AddNode(*box, *(new G4Transform3D(G4RotationMatrix(),offset*i)));
	unio->Voxelize();
	G4DisplacedSolid* ret = new G4DisplacedSolid(name,unio,new G4RotationMatrix(),-offset*((G4double) number -1)/2);

	return ret;
}
