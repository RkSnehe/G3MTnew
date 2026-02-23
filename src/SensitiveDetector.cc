#include "SensitiveDetector.hh"


SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name)

{
    fTotalEnergyDeposited = 0.;

}

SensitiveDetector::~SensitiveDetector()
{
}

void SensitiveDetector::Initialize(G4HCofThisEvent *)
{
    fTotalEnergyDeposited = 0.;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{  
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
     
    analysisManager->FillH1(0, fTotalEnergyDeposited);

    G4cout << "Deposited energy: " << fTotalEnergyDeposited << G4endl;
}

G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();

    G4double fGlobalTime = preStepPoint->GetGlobalTime();
    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();

    G4double fMomPhotonMag = momPhoton.mag();

    G4double fWlen = (1.239341939 * eV / fMomPhotonMag) * 1E+03;

    analysisManager->FillNtupleIColumn(0, 0, eventID);
    analysisManager->FillNtupleDColumn(0,1,posPhoton[0]);
    analysisManager->FillNtupleDColumn(0,2,posPhoton[1]);
    analysisManager->FillNtupleDColumn(0,3,posPhoton[2]);
    analysisManager->FillNtupleDColumn(0,4,fGlobalTime);
    analysisManager->FillNtupleDColumn(0,5,fWlen);
    analysisManager->AddNtupleRow(0);


    G4double fEnergyDeposited = aStep->GetTotalEnergyDeposit();

    if(fEnergyDeposited >0)
    {
        fTotalEnergyDeposited +=fEnergyDeposited;
    }

    return true;
}