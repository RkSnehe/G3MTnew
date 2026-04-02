#include "SensitiveDetector.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

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

    G4cout << "Total Energy Deposited by Muons: " 
           << fTotalEnergyDeposited << G4endl;
}

G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
    G4Track *track = aStep->GetTrack();

    // Particle definition
    const G4ParticleDefinition *particle = track->GetParticleDefinition();

    // Select only muons
    if (particle != G4MuonPlus::MuonPlusDefinition() &&
        particle != G4MuonMinus::MuonMinusDefinition())
    {
        return false;
    }

    G4int eventID = G4RunManager::GetRunManager()
                    ->GetCurrentEvent()->GetEventID();

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();

    G4double globalTime = preStepPoint->GetGlobalTime();

    G4ThreeVector posMuon = preStepPoint->GetPosition();
    G4ThreeVector momMuon = preStepPoint->GetMomentum();

    const G4VTouchable* touchable = preStepPoint->GetTouchable();
    G4int tubeCopyNumber = touchable->GetCopyNumber(1);

    G4int layerNumber = tubeCopyNumber / 1000;
    G4int counterNumber = tubeCopyNumber % 1000;

    G4cout << "Muon hit -> Layer: " << layerNumber
       << " Counter: " << counterNumber
       << G4endl;
    
    if(aStep->GetPreStepPoint()->GetStepStatus()!=fGeomBoundary)
    return false;

    G4double kineticEnergy = track->GetKineticEnergy();

    analysisManager->FillNtupleIColumn(0,0,eventID);
    analysisManager->FillNtupleDColumn(0,1,posMuon[0]);
    analysisManager->FillNtupleDColumn(0,2,posMuon[1]);
    analysisManager->FillNtupleDColumn(0,3,posMuon[2]);
    analysisManager->FillNtupleDColumn(0,4,globalTime);
    analysisManager->FillNtupleDColumn(0,5,kineticEnergy);
    analysisManager->FillNtupleIColumn(0,6,layerNumber);
    analysisManager->FillNtupleIColumn(0,7,counterNumber);
    analysisManager->AddNtupleRow(0);

    G4double energyDeposited = aStep->GetTotalEnergyDeposit();

    if(energyDeposited > 0)
    {
        fTotalEnergyDeposited += energyDeposited;
    }

    return true;
}