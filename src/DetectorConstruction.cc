#include "DetectorConstruction.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"
#include "SensitiveDetector.hh"
DetectorConstruction::DetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()

{ G4bool checkOverlaps = true;


auto nist = G4NistManager::Instance();
auto worldMat = nist->FindOrBuildMaterial("G4_AIR");

auto steel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
auto tungsten = nist->FindOrBuildMaterial("G4_W");

auto Ar = nist->FindOrBuildMaterial("G4_Ar");
auto CH4 = nist->FindOrBuildMaterial("G4_METHANE");


//concrete
auto SiO2 = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
auto CaO = nist->FindOrBuildMaterial("G4_CALCIUM_OXIDE");
auto Al2O3 = nist->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
auto Fe2O3 = nist->FindOrBuildMaterial("G4_FERRIC_OXIDE");
auto H2O = nist->FindOrBuildMaterial("G4_WATER");
auto P10 = new G4Material("P10", 1.74*mg/cm3, 2);


P10->AddMaterial(Ar, 90*perCent);
P10->AddMaterial(CH4,10*perCent);

auto solidWorld = new G4Box("World", 8*m, 8*m, 8*m);
auto logicWorld = new G4LogicalVolume(solidWorld, worldMat, "World");
auto physWorld = new G4PVPlacement( nullptr, {}, logicWorld, "World", nullptr, false, 0,
checkOverlaps);
logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());


G4double length = 600*cm;
G4double width = 10*cm;
G4double height = 10*cm;

//layer0

auto solidTube = new G4Box("PRC_Tube",length/2,width/2,height/2);
auto logicTube = new G4LogicalVolume(solidTube, steel, "PRC_Tube");

//GAS
G4double gasGap = 2*mm;

auto solidGas = new G4Box("Gas",length/2 - gasGap, width/2 - gasGap, height/2 - gasGap);
logicGas = new G4LogicalVolume(solidGas, P10, "Gas");
new G4PVPlacement(nullptr,{},logicGas,"Gas",logicTube,false,0,checkOverlaps);

//WIRE

G4double wireRadius = 25*micrometer;
auto solidWire = new G4Tubs("Wire",0,wireRadius,length/2-gasGap,0*deg,360*deg);
auto logicWire = new G4LogicalVolume(solidWire, tungsten, "Wire");

//Layer 0
// rotate wire so its axis matches tube length

auto wireRot = new G4RotationMatrix();
wireRot->rotateY(90*deg); // Z → X
new G4PVPlacement( wireRot,G4ThreeVector(),logicWire,"Wire",logicGas,false,0,checkOverlaps);

const G4int nCounters = 59;
const G4double pitch = width;
for (G4int i = 0; i < nCounters; i++)
{ G4double zPos = (i - (nCounters - 1)/2.0) * pitch;
new G4PVPlacement( nullptr, G4ThreeVector( 0,0,zPos), logicTube, "PRC_Tube", logicWorld, false,
i, checkOverlaps);
}

//concrete 0

G4double blockX = 6*m;
G4double blockY = 15*cm;
G4double blockZ = 6*m;

auto concrete = new G4Material("Concrete", 2.3*g/cm3, 5);

concrete->AddMaterial(SiO2, 52*perCent);
concrete->AddMaterial(CaO, 33*perCent);
concrete->AddMaterial(Al2O3, 7*perCent);
concrete->AddMaterial(Fe2O3, 3*perCent);
concrete->AddMaterial(H2O, 5*perCent);


auto solidConcrete = new G4Box( "ConcreteBlock", blockX/2, blockY/2, blockZ/2);
auto logicConcrete = new G4LogicalVolume( solidConcrete, concrete, "ConcreteBlock");
new G4PVPlacement(nullptr,G4ThreeVector(0, 12.5*cm,
0),logicConcrete,"ConcreteBlock",logicWorld,false,0, checkOverlaps);


//layer1
auto rotY90 = new G4RotationMatrix();
rotY90->rotateY(90*deg);

G4double yLayer1 = 12.5*cm + (blockY/2) + (height/2);

const G4int nCounters2 = 59;
const G4double pitch2 = width;


for (G4int i = 0; i < nCounters2; i++)
{
G4double xPos = (i - (nCounters2 - 1)/2.0) * pitch2;
new G4PVPlacement(rotY90,G4ThreeVector(xPos, yLayer1,
0),logicTube,"PRC_Tube",logicWorld,false,1000 + i,checkOverlaps);}

// concrete 2
G4double yConcrete2 = 37.5*cm;
new G4PVPlacement(nullptr,G4ThreeVector(0, yConcrete2, 0),logicConcrete,
"ConcreteBlock2",logicWorld,false,1,checkOverlaps);

//layer 2
G4double yLayer2 = 50.0*cm;
for (G4int i = 0; i < nCounters; i++)
{
G4double zPos = (i - (nCounters - 1)/2.0) * pitch;
new G4PVPlacement(nullptr, G4ThreeVector(0, yLayer2, zPos),
logicTube,"PRC_Tube",logicWorld,false,2000 + i,checkOverlaps);
}

//concrete 3
G4double yConcrete3 = 62.5*cm;
new G4PVPlacement(nullptr,G4ThreeVector(0,yConcrete3,0),logicConcrete, "ConcreteBlock3",
logicWorld,false,2,checkOverlaps);

//layer 3
G4double yLayer3 = 62.5*cm + (blockY/2) + (height/2);
for (G4int i = 0; i < nCounters2; i++)
{
G4double xPos = (i - (nCounters2 - 1)/2.0) * pitch2;
new G4PVPlacement(rotY90,G4ThreeVector(xPos, yLayer3,
0),logicTube,"PRC_Tube",logicWorld,false,3000 + i,checkOverlaps);
}

//concrete of 2m inverted pyramid.
G4double nInvertedConcrete = 7;
G4double heightY = 15*cm;
G4double yPyramidBase = yLayer3 + height/2;

//slant degree w.r.t vert

G4double theta = 60*deg;
G4double delta = heightY*std::tan(theta);

for (G4int i = 0; i < nInvertedConcrete; i++)

{
G4double resize = i * delta;
G4double lengthX = blockX + 2 * resize;
G4double heighty = heightY;
G4double breadthZ = blockZ + 2* resize;
auto solidPyramid = new G4Box("PyramidConcrete",lengthX/2,heighty/2,breadthZ/2);
auto logicPyramid = new G4LogicalVolume(solidPyramid,logicConcrete->GetMaterial(),"PyramidConcrete");
G4double yPos =yPyramidBase +i * heightY +heighty/2;
new G4PVPlacement( nullptr,G4ThreeVector(0, yPos,  0),logicPyramid,"PyramidConcrete",logicWorld,false,5000 + i,checkOverlaps);
logicPyramid->SetVisAttributes(new G4VisAttributes(G4Colour(0.6, 0.6, 0.6)));
}

logicTube->SetVisAttributes(new G4VisAttributes(G4Colour(0.55,0.27,0.07)));
logicGas->SetVisAttributes(new G4VisAttributes(G4Colour(0.7,0.7,0.7,0.4)));
logicWire->SetVisAttributes(new G4VisAttributes(G4Colour::Yellow()));
logicConcrete->SetVisAttributes(new G4VisAttributes(G4Colour(0.6,0.6,0.6)));
return physWorld;
}
void DetectorConstruction::ConstructSDandField()
{
auto sensDet = new SensitiveDetector("GasSD");
logicGas->SetSensitiveDetector(sensDet);
G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
}