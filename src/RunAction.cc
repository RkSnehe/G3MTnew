#include "RunAction.hh"

RunAction::RunAction()
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    analysisManager->CreateH1("Edep", "Energy deposit", 100, 0., 1.1*MeV);

    analysisManager->CreateNtuple("Photons","Photons");
    analysisManager->CreateNtupleIColumn("iEvent");  //I stands for integer
    analysisManager->CreateNtupleDColumn("fx"); //D means double
    analysisManager->CreateNtupleDColumn("fy");
    analysisManager->CreateNtupleDColumn("fyz");
    analysisManager->CreateNtupleDColumn("fGlobalTime"); //Time
    analysisManager->CreateNtupleDColumn("fWlen"); //wavelength
    analysisManager->FinishNtuple(0);

}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run *run)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager:: Instance();

    G4int runID = run->GetRunID();

    std::stringstream strRunID;
    strRunID << runID;

    analysisManager->OpenFile("Mu_1GeV" + strRunID.str() + ".root");
}

void RunAction::EndOfRunAction(const G4Run *run)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager:: Instance();   

    analysisManager->Write();

    analysisManager->CloseFile();

    G4int runID = run->GetRunID();

    G4cout << "Finishing run" << runID << G4endl;
}

