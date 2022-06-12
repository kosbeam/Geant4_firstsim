#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
    nCols = 10; // default
    nRows = 10; // default

    xWorld = 0.5 * m;
    yWorld = 0.5 * m;
    zWorld = 0.5 * m;

    fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");
    fMessenger->DeclareProperty("nCols", nCols, "Number of columns");                           // User input
    fMessenger->DeclareProperty("nRows", nRows, "Number of rows");                              // User input
    fMessenger->DeclareProperty("isCherenkov", isCherenkov, "Toggle Cherenkov setup");          // option
    fMessenger->DeclareProperty("isScintillator", isScintillator, "Toggle Scintillator setup"); // option

    DefineMaterial();

    isCherenkov = false;
    isScintillator = true;
}

MyDetectorConstruction::~MyDetectorConstruction()
{
}

// Class for define material of geometry
void MyDetectorConstruction::DefineMaterial()
{
    //! XCOM database (nist)

    G4NistManager *nist = G4NistManager::Instance();
    //! defined compound and mixture
    SiO2 = new G4Material("SiO2", 2.201 * g / cm3, 2);
    SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
    SiO2->AddElement(nist->FindOrBuildElement("O"), 2);

    H2O = new G4Material("H2O", 1.000 * g / cm3, 2);
    H2O->AddElement(nist->FindOrBuildElement("H"), 2);
    H2O->AddElement(nist->FindOrBuildElement("O"), 1);

    NaI = new G4Material("NaI", 3.67 * g / cm3, 2);
    NaI->AddElement(nist->FindOrBuildElement("Na"), 1);
    NaI->AddElement(nist->FindOrBuildElement("I"), 1);

    C = nist->FindOrBuildElement("C");

    Aerogel = new G4Material("Aerogel", 0.200 * g / cm3, 3);
    Aerogel->AddMaterial(SiO2, 62.5 * perCent);
    Aerogel->AddMaterial(H2O, 37.4 * perCent);
    Aerogel->AddElement(C, 0.1 * perCent);

    worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4double energy[2] = {1.239841939 * eV, 1.239841939 * eV / 0.2};
    G4double rindexAerogel[2] = {1.1, 1.1};
    G4double rindexWorld[2] = {1.0, 1.0};

    //! reflective Index of Aerogel
    G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
    mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2);
    Aerogel->SetMaterialPropertiesTable(mptAerogel);

    //! reflective Index of AIR
    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);

    worldMat->SetMaterialPropertiesTable(mptWorld);
}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{

    //! blk and void

    solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);

    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    //! option simulation
    if (isCherenkov)
    {
        ConstructCherenkov();
    }

    if (isScintillator)
    {
        ConstructScintillator();
    }

    return physWorld;
}

void MyDetectorConstruction::ConstructCherenkov()
{
    //! Radiator
    solidRadiator = new G4Box("solidRadiator", 0.4 * m, 0.4 * m, 0.01 * m);

    logicRadiator = new G4LogicalVolume(solidRadiator, Aerogel, "logicalRadiator");

    fScoringVolume = logicRadiator; // Scoring Edep of Aerogel

    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.25 * m), logicRadiator, "physRadiator", logicWorld, false, 0, true); // physRadiator

    //! Detectors
    solidDetector = new G4Box("solidDetector", xWorld / nRows, yWorld / nCols, 0.01 * m);

    logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector"); // set this detector

    //! like usrbin in FLUKA
    for (G4int i = 0; i < nRows; i++)
    {
        for (G4int j = 0; j < nCols; j++)
        {
            physDetector = new G4PVPlacement(0, G4ThreeVector(-0.5 * m + (i + 0.5) * m / nRows, -0.5 * m + (j + 0.5) * m / nCols, 0.49 * m), logicDetector, "physDetector", logicWorld, false, j + i * nCols, true);
        }
    }
}

void MyDetectorConstruction::ConstructScintillator()
{
    solidScintillator = new G4Tubs("solidScintillator", 10 * cm, 20 * cm, 30 * cm, 0 * deg, 360 * cm);
    logicScintillator = new G4LogicalVolume(solidScintillator, NaI, "logicScintillator");
    physScintillator = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicScintillator, "physScintillator", logicWorld, false, 0, true);

    fScoringVolume = logicScintillator; // Scoring Edep of NaI
}

void MyDetectorConstruction::ConstructSDandField()
{
    MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

    if (isCherenkov)
    {
        logicDetector->SetSensitiveDetector(sensDet);
    }
}