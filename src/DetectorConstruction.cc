#include "DetectorConstruction.hh"

#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4SDParticleFilter.hh"
#include "G4PSNofSecondary.hh"
#include "G4OpticalSurface.hh"
#include "G4UImanager.hh"
#include "G4VisAttributes.hh"

#include "DetectorMessenger.hh"

#include <fstream>

G4Material* spacerMat = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
G4Material* frontPlateMat = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

G4ThreadLocal G4bool ConstructedSDandField = false;

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(),
                                               constructed(false){
   messenger = new DetectorMessenger(this);
}

DetectorConstruction::~DetectorConstruction(){}

G4VPhysicalVolume* DetectorConstruction::Construct(){

    G4NistManager* nist = G4NistManager::Instance();
    if(constructed)
        return physWorld;
    constructed = true;
    Scnt = new G4Material("Cesium_iodide_scintillator",nist->FindOrBuildMaterial("G4_CESIUM_IODIDE")->GetDensity(),nist->FindOrBuildMaterial("G4_CESIUM_IODIDE"));
    //G4Material* spacerMat = nist->FindOrBuildMaterial("G4_Al");

    G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

    std::ifstream emissionFile("./CsI_data/emission.dat");
    if(!emissionFile){
    G4cerr << "Cannot find emission file!" << G4endl;
    exit(1);
    }
    std::vector<G4double> emission_coeff;
    std::vector<G4double> emission_energy;
    while (!emissionFile.eof())
    {
    G4double wavelength, coeff;
    emissionFile >> wavelength >> coeff;
    emission_coeff.push_back(coeff);
    emission_energy.push_back(wavelength / 1239.84193 * eV);
    }


    std::ifstream absorbFile("./CsI_data/absorbtion.dat");
    if(!absorbFile){
    G4cerr << "Cannot find absorbFile file!" << G4endl;
    exit(1);
    }
    std::vector<G4double> absorb_coeff;
    std::vector<G4double> absorb_energy;
    std::vector<G4double> refract_coeff;
    std::vector<G4double> refract_coeff_air;
    std::vector<G4double> refract_coeff_TiO2;
    std::vector<G4double> absorb_coeff_TiO2;

    while (absorbFile){
        G4double wavelength, coeff;
        absorbFile >> wavelength >> coeff;
        absorb_coeff.push_back(coeff * cm);
        absorb_energy.push_back(wavelength / 1239.84193 * eV);
        refract_coeff.push_back(1.79);
        refract_coeff_air.push_back(1.0);
        refract_coeff_TiO2.push_back(2.4);
        absorb_coeff_TiO2.push_back(3e-5 * cm);
    }

    G4MaterialPropertiesTable* Scnt_MPT = new G4MaterialPropertiesTable();
    Scnt_MPT->AddProperty("RINDEX", absorb_energy.data(), refract_coeff.data(),
    absorb_energy.size())->SetSpline(true);
    Scnt_MPT->AddProperty("ABSLENGTH", absorb_energy.data(), absorb_coeff.data(),
    absorb_energy.size())->SetSpline(true);
    Scnt_MPT->AddProperty("FASTCOMPONENT", emission_energy.data(),
    emission_coeff.data(), emission_coeff.size())->SetSpline(true);
    Scnt_MPT->AddProperty("SLOWCOMPONENT", emission_energy.data(),
    emission_coeff.data(), emission_coeff.size())->SetSpline(true);
    Scnt_MPT->AddConstProperty("SCINTILLATIONYIELD",54.00 / MeV); // 1000x less
    Scnt_MPT->AddConstProperty("RESOLUTIONSCALE",1.0);
    Scnt_MPT->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
    Scnt_MPT->AddConstProperty("SLOWTIMECONSTANT",10.*ns);
    Scnt_MPT->AddConstProperty("YIELDRATIO",1.0);
    Scnt->SetMaterialPropertiesTable(Scnt_MPT);

    G4double ndet_Yhalf = (((G4double)ndet_Y)/2);
    G4double ndet_Zhalf = (((G4double)ndet_Z)/2);

    G4Box* solidWorld = new G4Box("World",0.5*world_X,0.5*world_Y,0.5*world_Z);
    logicWorld = new G4LogicalVolume(solidWorld,world_mat,"World");
    physWorld = new G4PVPlacement(0
                                 ,G4ThreeVector()
                                 ,logicWorld
                                 ,"World"
                                 ,0
                                 ,false
                                 ,0
                                 ,check_overlap);


    detector = new G4Box("Detector",0.5*det_X,0.5*det_YZ,0.5*det_YZ);
    // Even though it is 'front plate' all non x sides will be surronded by this.
    #if !NEW_SETUP
    G4Box* frontPlateBox = new G4Box("FrontPlate",0.5*det_X,ndet_Yhalf*(det_YZ+spacer_width)-0.5*spacer_width,ndet_Zhalf*(det_YZ+spacer_width)-0.5*spacer_width + frontPlate_width);
    G4Box* spacerBox = new G4Box("Spacer",0.5*det_X,ndet_Yhalf*(det_YZ+spacer_width)-0.5*spacer_width,ndet_Zhalf*(det_YZ+spacer_width)-0.5*spacer_width);
    #else
    float xy_halfwidth = 0.5*det_X >= ndet_Yhalf*(det_YZ+spacer_width) ? 0.5*det_X : ndet_Yhalf*(det_YZ+spacer_width);
    G4Box* frontPlateBox = new G4Box("FrontPlate",xy_halfwidth-0.5*spacer_width,xy_halfwidth-0.5*spacer_width,ndet_Zhalf*(det_YZ+spacer_width)-0.5*spacer_width+frontPlate_width);
    G4Box* spacerBox = new G4Box("Spacer",xy_halfwidth-0.5*spacer_width,xy_halfwidth-0.5*spacer_width,ndet_Zhalf*(det_YZ+spacer_width)-0.5*spacer_width);
    #endif
    logicalFrontPlate = new G4LogicalVolume(frontPlateBox,frontPlateMat,"FrontPlate");
    logicalSpacer = new G4LogicalVolume(spacerBox,spacerMat,"Spacer");
    // Add the placements
    physFrontPlate = new G4PVPlacement(0
                                      ,G4ThreeVector(0*m,0,0)
                                      ,logicalFrontPlate
                                      ,"FrontPlate"
                                      ,logicWorld
                                      ,false
                                      ,0
                                      ,check_overlap);
    physSpacer = new G4PVPlacement(0
                                  ,G4ThreeVector(0*m,0,0)
                                  ,logicalSpacer
                                  ,"Spacer"
                                  ,logicalFrontPlate
                                  ,false
                                  ,0
                                  ,check_overlap);
    G4String name;
    G4VisAttributes* crystalAttr = new G4VisAttributes();
    crystalAttr->SetColour(0.2,0.5,0.8,0.25);
    for(int i = 0; i < ndet_Y; i++){
        for(int f = 0; f < ndet_Z; f++){
            name = "Detector_";
            name += std::to_string(i);
            name += "|";
            name += std::to_string(f);
            logicDetectors[i][f] = new G4LogicalVolume(detector, Scnt, name);
            logicDetectors[i][f]->SetVisAttributes(crystalAttr);
            #if !NEW_SETUP
            physDetectors[i][f] = new G4PVPlacement(0
                                                   ,G4ThreeVector(0,(i-(ndet_Yhalf-0.5))*(det_YZ+spacer_width),(f-(ndet_Zhalf-0.5))*(det_YZ+spacer_width))
                                                   ,logicDetectors[i][f]
                                                   ,name
                                                   ,logicalSpacer
                                                   ,false
                                                   ,0
                                                   ,check_overlap);
            #else
            if(f%2 == 0)
                physDetectors[i][f] = new G4PVPlacement(0
                                                       ,G4ThreeVector(0,(i-(ndet_Yhalf-0.5))*(det_YZ+spacer_width),(f-(ndet_Zhalf-0.5))*(det_YZ+spacer_width))
                                                       ,logicDetectors[i][f]
                                                       ,name
                                                       ,logicalSpacer
                                                       ,false
                                                       ,0
                                                       ,check_overlap);
            else
                // Switch around the coordinate displacements (x <-> y) and
                // rotate the crystal 90 degrees along the z axis
                physDetectors[i][f] = new G4PVPlacement(new G4RotationMatrix(0,0,90*deg)
                                                       ,G4ThreeVector((i-(ndet_Yhalf-0.5))*(det_YZ+spacer_width),0,(f-(ndet_Zhalf-0.5))*(det_YZ+spacer_width))
                                                       ,logicDetectors[i][f]
                                                       ,name
                                                       ,logicalSpacer
                                                       ,false
                                                       ,0
                                                       ,check_overlap);
            #endif
            name += "_Surface";
            scintSurfaces[i][f] = new G4OpticalSurface(name);
            scintSurfaces[i][f]->SetType(dielectric_metal);
            scintSurfaces[i][f]->SetFinish(polished);
        }
    }
    ConstructSDandField();
    return physWorld;

}

void DetectorConstruction::ConstructSDandField(){
    if(ConstructedSDandField)
        return;
    ConstructedSDandField = true;
    G4String name;
    for(int i = 0; i < ndet_Y; i++){
        for(int f = 0; f < ndet_Z; f++){
            name = "Detector_";
            name += std::to_string(i);
            name += "|";
            name += std::to_string(f);

            G4SDParticleFilter* gammaFilter = new G4SDParticleFilter("gammaFilter","gamma");
            G4VPrimitiveScorer* scorer;
            G4MultiFunctionalDetector* det = new G4MultiFunctionalDetector(name);

            G4SDManager::GetSDMpointer()->AddNewDetector(det);
            scorer = new G4PSEnergyDeposit("eDep",0);
            det->RegisterPrimitive(scorer);
            scorer = new G4PSNofSecondary("nGamma",0);
            scorer->SetFilter(gammaFilter);
            det->RegisterPrimitive(scorer);
            SetSensitiveDetector(logicDetectors[i][f],det);
            G4SDManager::GetSDMpointer()->SetVerboseLevel(0);

        }
    }

}

void DetectorConstruction::setDetectorPos(G4ThreeVector newPos){
    physSpacer->SetTranslation(newPos);
    G4UImanager::GetUIpointer()->ApplyCommand("/run/geometryModified");
}
