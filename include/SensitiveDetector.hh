#ifndef SENSITIVEDETECTOR_HH
#define SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"

#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

class SensitiveDetector : public G4VSensitiveDetector
{

public:

    SensitiveDetector(G4String name);
    virtual ~SensitiveDetector();

private:

    G4double fTotalEnergyDeposited;

    virtual void Initialize(G4HCofThisEvent *hce) override;

    virtual void EndOfEvent(G4HCofThisEvent *hce) override;

    virtual G4bool ProcessHits(G4Step *step,
                               G4TouchableHistory *history) override;

};

#endif