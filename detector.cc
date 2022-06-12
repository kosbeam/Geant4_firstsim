#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    quEff = new G4PhysicsFreeVector();
    std::ifstream datafile;
    datafile.open("eff.dat");

    while (1)
    {
        G4double wlen, queff;
        datafile >> wlen >> queff;
        if (datafile.eof())
            break;
        G4cout << wlen << " " << queff << std::endl;

        quEff->InsertValues(wlen, queff / 100.);
    }
    datafile.close();

    // quEff->SetSpline(false);
}
MySensitiveDetector::~MySensitiveDetector()
{
}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    // track like usrtrack
    G4Track *track = aStep->GetTrack();
    track->SetTrackStatus(fStopAndKill);

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
    G4ThreeVector posPhoton = preStepPoint->GetPosition(); // get position of photon
    G4ThreeVector momPhoton = preStepPoint->GetMomentum(); // get momentum of photon

    G4double wlen = (1.239841939 * eV / momPhoton.mag()) * 1E+03; // convert momentum to wavelength in unit nm

    // Show Terminal
    //  G4cout << "Photon position: " << posPhoton << G4endl;

    // to know the number (index) of the detector hit
    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();

    G4int copyNo = touchable->GetCopyNumber();

    // G4cout << "Copy number:" << copyNo << G4endl;

    G4VPhysicalVolume *physVol = touchable->GetVolume();

    G4ThreeVector posDetector = physVol->GetTranslation();

#ifndef G4MULTITHREADED
    G4cout << "Detector position: " << posDetector << G4endl;
    G4cout << "Photon wavelength: " << wlen << G4endl;
#endif


    // G4AnalysisManager *man = G4AnalysisManager::Instance(); // root analysis
    // G4int evt = G4RunManager::GetRunManager->GetCurrentEvent()->GetEventID();

    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // content of root file Phontons detection
    man->FillNtupleIColumn(0, 0, evt);
    man->FillNtupleDColumn(0, 1, posPhoton[0]); // X posDetector
    man->FillNtupleDColumn(0, 2, posPhoton[1]); // Z posDetector
    man->FillNtupleDColumn(0, 3, posPhoton[2]); // Z posDetector
    man->FillNtupleDColumn(0, 4, wlen);         // wave range of photons

    man->AddNtupleRow(0);

    // content of root file Hits
    if (G4UniformRand() < quEff->Value(wlen))
    {

        // G4cout << "G4UniformRand: " << G4UniformRand() << G4endl;
        // G4cout << "quEff->Value(wlen): " << quEff->Value(wlen) << G4endl;

        man->FillNtupleIColumn(1, 0, evt);
        man->FillNtupleDColumn(1, 1, posDetector[0]); // X posDetector
        man->FillNtupleDColumn(1, 2, posDetector[1]); // Z posDetector
        man->FillNtupleDColumn(1, 3, posDetector[2]); // Z posDetector
        man->AddNtupleRow(1);
    }

    return true;
}