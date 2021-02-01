#ifndef DetectorConstruction_hh
#define DetectorConstruction_hh
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

#include "Params.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4OpticalSurface;
class G4Box;
class G4Material;

class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction {
    public:
        DetectorConstruction();
        ~DetectorConstruction();
        G4VPhysicalVolume* Construct();
        void ConstructSDandField();
        void DefineMaterials();
        void setDetectorPos(G4ThreeVector);
        //void CloneSD();
        //void CloneF();
    private:
        G4bool constructed;
        G4Box* detector;
        G4Material* Scnt;
        G4LogicalVolume* logicWorld;
        G4LogicalVolume* logicalSpacer;
        G4LogicalVolume* logicDetectors[ndet_Y][ndet_Z];
        G4VPhysicalVolume* physWorld;
        G4VPhysicalVolume* physSpacer;
        G4VPhysicalVolume* physDetectors[ndet_Y][ndet_Z];
        G4OpticalSurface* scintSurfaces[ndet_Y][ndet_Z];
        DetectorMessenger* messenger;
};



#endif
