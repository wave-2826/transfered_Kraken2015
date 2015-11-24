#include "AutoMode_SecondAndThird.h"
#include "Auto.h"
#include "WaveConstants.h"
#include "SetPneumatics.h"
#include "BinJugglerCommand.h"
#include "SetRecyclerPosition.h"
#include "DriveStraight.h"
#include "SetElevatorPosition.h"
#include "Intake_FrontTote.h"
#include "LoadMagazine.h"
#include "ResetRecycler.h"
#include "AutoDrive.h"
#include "AutoCurve.h"
#include "AutoRecycle.h"
#include "SetFrontIntake.h"
#include "KillDriveStraight.h"
#include "AutoDistance.h"

namespace{
bool up = true;
//bool down = false;
//bool grab = false;
//bool release = true;
//bool open = true;
bool close = false;
}

AutoMode_SecondAndThird::AutoMode_SecondAndThird()
{
	//first(second) set
	AddSequential(new WaveWait(.00));
	AddParallel(new AutoDrive(65,.3, 0)); // drive 24 inches at 30 percent power // .3
	AddParallel(new AutoRecycle(Bin_LoadRight,cyl_leftHook));//lift, grab, un-lift, load to the side
	AddSequential(new Intake_FrontTote(.33));//Run intake until a tote is seated in the elevator // .4
	AddSequential(new SetElevatorPosition(stackClearanceElevatorPosition)); //elevator up
	AddSequential(new Intake_FrontTote(.33));//wait for a tote / .4

	//second(third) set
	AddParallel(new SetElevatorPosition(lowElevatorPosition));
	AddSequential(new AutoDrive(4, .27, 0/* 1.5*/)); // move some distance forwards instead of time // 5in
	AddParallel(new SetPneumatics(cyl_handleHolder, close)); // grab on to the bin with the track
	AddSequential(new KillDriveStraight());
	AddParallel(new SetRecyclerPosition(up,1, false)); // bring the bin up to the top
	AddParallel(new LoadMagazine(elevatorEngagedPosition)); // [for swapping wheels] AddParallel(new LoadMagazine(autoElevatorPosition));

	//Turn
	AddSequential(new AutoCurve(90));

	//Drive'n'drop
	AddSequential(new AutoDrive(72, .75, 90));
	AddParallel(new SetElevatorPosition(lowElevatorPosition));
	AddSequential(new AutoDrive(58, .18, 90)); // 52in


	//AddSequential(new AutoDrive(0, 0, 90));
	//	AddSequential(new WaveWait(0.5));
//	AddParallel(new SetFrontIntake(.5,3));
//	AddSequential(new AutoDrive(-12, -.35, 90));
//	//	AddSequential(new AutoDrive(0, 0, 90));
//	AddSequential(new AutoDrive(-.5,1, 90));
//	AddSequential(new AutoDrive(-20, .18, 90)); // -10in
//	AddSequential(new SetFrontIntake(0,3));


	//the curve at the end
	//AddParallel(new AutoDrive(-72,-.3));
	//AddSequential(new WaveWait(.5)); // wait for things to settle for the first test
	//AddSequential(new AutoDrive(-20,-.25));
	//AddSequential(new AutoCurve(-.85,.85,20,20));
	//	AddSequential(new AutoDrive(96,.75));
	//	AddParallel(new SetElevatorPosition(lowElevatorPosition));
	//	AddParallel(new SetFrontIntake(1, 3));
	//	AddSequential(new WaveWait(.5));
	//	AddSequential(new AutoCurve(-.65,.65,7,7));
	//	AddSequential(new AutoDrive(-30,-.5));
	//	AddSequential(new AutoDrive(0,0));



	//massive block of old auto moved to new file, "TooValuableToDeleteButNotNeeded.h"



}