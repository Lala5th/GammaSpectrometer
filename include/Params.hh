#ifndef Params_hh
#define Params_hh
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>

const G4double det_X = 50*mm;
const G4double det_YZ = 5*mm;
const G4double spacer_width = 1*mm;
const G4double frontPlate_width = 0*mm;
const G4bool check_overlap = false;
const G4int ndet_Y = 10;
const G4int ndet_Z = 70;

const G4double root2 = std::sqrt(2);

const G4double world_X = det_X;
const G4double world_Y = ndet_Y*(det_YZ + spacer_width)*root2; //4.5*m;
const G4double world_Z = 4.5*m;

const G4int pNum = 5000;

#endif
