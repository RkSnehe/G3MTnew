#include "ActionInitialization.hh"
#include "PrimaryGenerator.hh"
#include "RunAction.hh"

ActionInitialization::ActionInitialization()
{} 

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
    RunAction *runAction = new RunAction();
    SetUserAction(runAction);
}

void ActionInitialization::Build() const
{
    PrimaryGenerator* generator = new PrimaryGenerator();
    SetUserAction(generator);

    RunAction *runAction = new RunAction();
    SetUserAction(runAction);
}
