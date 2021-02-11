#ifndef AuxFunctions_HH
#define AuxFunctions_hh
#include "G4ThreeVector.hh"

class G4DisplacedSolid ;
class G4Box;

G4DisplacedSolid * generateBoxUnion(G4String,G4Box*,int,G4ThreeVector);

#endif
