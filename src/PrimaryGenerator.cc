#include "PrimaryGenerator.hh"

PrimaryGenerator::PrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);

    // Particle position
    G4double x = -3. * m;
    G4double y = 6. * m;
    G4double z = 0. * m;
    
    G4ThreeVector pos(x, y, z);

    // Particle direction
    G4double theta = 45.*degree;
    G4double phi = 0.*degree;

    G4double px = std::sin(theta)*std::cos(phi);
    G4double py = -std::cos(theta);
    G4double pz = std::sin(theta)*std::sin(phi);

    G4ThreeVector mom(px, py, pz);

    // Particle type
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle = particleTable->FindParticle("gamma");

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleEnergy(5* GeV);
    fParticleGun->SetParticleDefinition(particle);
}

PrimaryGenerator::~PrimaryGenerator()
{
    delete fParticleGun;
}

void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
