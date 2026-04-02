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

    G4cout << "Total Energy Deposited all particles: " 
           << fTotalEnergyDeposited << G4endl;
}
G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
    G4Track *track = aStep->GetTrack();

    // Get particle definition
    const G4ParticleDefinition *particle = track->GetParticleDefinition();
    G4String particleName = particle->GetParticleName();
    G4int pdgCode = particle->GetPDGEncoding();

    G4int eventID = G4RunManager::GetRunManager()
                    ->GetCurrentEvent()->GetEventID();

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();

    G4double globalTime = preStepPoint->GetGlobalTime();

    G4ThreeVector pos = preStepPoint->GetPosition();
    G4ThreeVector mom = preStepPoint->GetMomentum();

    const G4VTouchable* touchable = preStepPoint->GetTouchable();
    G4int tubeCopyNumber = touchable->GetCopyNumber(1);

    G4int layerNumber = tubeCopyNumber / 1000;
    G4int counterNumber = tubeCopyNumber % 1000;

    // Only consider particles entering volume
    if(preStepPoint->GetStepStatus() != fGeomBoundary)
        return false;

    G4double kineticEnergy = track->GetKineticEnergy();
    G4double energyDeposited = aStep->GetTotalEnergyDeposit();

    // Print info
    G4cout << "Hit by particle: " << particleName
           << " (PDG: " << pdgCode << ")"
           << " Layer: " << layerNumber
           << " Counter: " << counterNumber
           << " Edep: " << energyDeposited
           << G4endl;

    
    analysisManager->FillNtupleIColumn(0,0,eventID);
    analysisManager->FillNtupleDColumn(0,1,pos[0]);
    analysisManager->FillNtupleDColumn(0,2,pos[1]);
    analysisManager->FillNtupleDColumn(0,3,pos[2]);
    analysisManager->FillNtupleDColumn(0,4,globalTime);
    analysisManager->FillNtupleDColumn(0,5,kineticEnergy);
    analysisManager->FillNtupleIColumn(0,6,layerNumber);
    analysisManager->FillNtupleIColumn(0,7,counterNumber);
    analysisManager->FillNtupleDColumn(0,8,energyDeposited);
    analysisManager->FillNtupleIColumn(0,9,pdgCode);    
    analysisManager->FillNtupleSColumn(0,10,particleName);
    analysisManager->AddNtupleRow(0);

    if(energyDeposited > 0)
    {
        fTotalEnergyDeposited += energyDeposited;
    }

    return true;
}