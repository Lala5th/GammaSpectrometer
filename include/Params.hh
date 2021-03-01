#ifndef Params_hh
#define Params_hh
#include "globals.hh"
#include "G4SystemOfUnits.hh"

// I define it for the preprocessor to include both geometries in the same codebase.
#define NEW_SETUP 1

const G4double det_X = 50*mm;
const G4double det_YZ = 5*mm;
const G4double spacer_width = 1*mm;
const G4double frontPlate_width = 0*mm;
const G4double world_X = 4.5*m;
const G4double world_Y = 4.5*m;
const G4double world_Z = 4.5*m;
const G4bool check_overlap = false;
#if !NEW_SETUP
const G4int ndet_Y = 33; // Due to the way we simulate y in the thesis is z and z is y
const G4int ndet_Z = 47;
#else
const G4int ndet_Y = 10;
const G4int ndet_Z = 70;
#endif

const G4int pNum = 10000;
const G4int targetPNum = 10000;

#endif
