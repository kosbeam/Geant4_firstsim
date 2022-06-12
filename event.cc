#include "event.hh"

   

MyEventAction::MyEventAction(MyRunAction*)
{
    fEdep = 0.; // initial value when start
}

MyEventAction::~MyEventAction()
{
}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
    fEdep = 0.; // reset value fEdep when new event start
}
void MyEventAction::EndOfEventAction(const G4Event *)
{
    // fEdep = AddEdep(G4double edep)
    G4cout << "Energy deposition: " << fEdep << G4endl;

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->FillNtupleDColumn(2, 0, fEdep);

    man->AddNtupleRow(2);
}
