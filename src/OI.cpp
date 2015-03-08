#include "OI.h"
#include "Robot.h"
#include "Commands/BinJugglerCommand.h"
#include "Commands/Shift.h"
#include "Commands/ShiftHigh.h"
#include "Commands/SetElevatorPosition.h"
#include "Commands/LoadMagazine.h"
#include "Commands/Intake_FrontTote.h"
#include "Commands/AcquireBins.h"
#include "Commands/DriveStraight.h"
#include "Commands/Auto.h"
#include "Commands/DatCurve.h"
#include "Commands/AutoDrive.h"
#include "Commands/SetPneumatics.h"

OI::OI()
{
	driverJoystick = new Joystick(0);
	operatorJoystick = new Joystick(1);
	debugJoystick = new Joystick(2);
	debugJoystick2 = new Joystick(3);

	driver_A = new JoystickButton(driverJoystick, 1);
	driver_B = new JoystickButton(driverJoystick, 2);
	driver_X = new JoystickButton(driverJoystick, 3);
	driver_Y = new JoystickButton(driverJoystick, 4);
	driver_LB = new JoystickButton(driverJoystick, 5);
	driver_RB = new JoystickButton(driverJoystick, 6);
	driver_Back = new JoystickButton(driverJoystick, 7);
	driver_Start = new JoystickButton(driverJoystick, 8);
	driver_LJoy = new JoystickButton(driverJoystick, 9);
	driver_RJoy = new JoystickButton(driverJoystick, 10);

	operator_A = new JoystickButton(operatorJoystick, 1);
	operator_B = new JoystickButton(operatorJoystick, 2);
	operator_X = new JoystickButton(operatorJoystick, 3);
	operator_Y = new JoystickButton(operatorJoystick, 4);
	operator_LB = new JoystickButton(operatorJoystick, 5);
	operator_RB = new JoystickButton(operatorJoystick, 6);
	operator_Back = new JoystickButton(operatorJoystick, 7);
	operator_Start = new JoystickButton(operatorJoystick, 8);
	operator_LJoy = new JoystickButton(operatorJoystick, 9);
	operator_RJoy = new JoystickButton(operatorJoystick, 10);

	debug_11 = new JoystickButton(debugJoystick, 7);
	debug_12 = new JoystickButton(debugJoystick, 8);

	operator_Y->WhenPressed(new BinJugglerCommand(Bin_LoadCenter));
	operator_X->WhenPressed(new BinJugglerCommand(Bin_LoadRight));
	operator_A->WhenPressed(new AutoDrive(999,.35,0));
	operator_B->WhenPressed(new BinJugglerCommand(Bin_LoadLeft));

	operator_LB->WhenPressed(new SetPneumatics(cyl_leftHook, true));
	operator_LB->WhenReleased(new SetPneumatics(cyl_leftHook, false));
	operator_RB->WhenPressed(new SetPneumatics(cyl_rightHook, true));
	operator_RB->WhenReleased(new SetPneumatics(cyl_rightHook, false));

	operator_Start->WhenPressed(new SetPneumatics(cyl_handleHolder, false));
	operator_Back->WhenPressed(new SetPneumatics(cyl_handleHolder, true));

	debug_12->WhenPressed(new AcquireBins());
	//debug_11->WhenPressed(new SetElevatorPosition(1.0));

	debug_11->WhenPressed(new LoadMagazine());

	m_allowElevatorInput = false;
	m_allowIntakeInput = false;
}

Joystick* OI::getDriverJoystick()
{
	return 	driverJoystick;
}
Joystick* OI::getOperatorJoystick()
{
	return operatorJoystick;
}
Joystick* OI::getDebugJoystick()
{
	return debugJoystick;
}

Joystick* OI::getDebugJoystick2()
{
	return debugJoystick2;
}
void OI::checkInput(){
	bool param = getDriverJoystick()->GetRawAxis(Axis_LTrigger)>0.2 ? false : true; // shift with the trigger - Switched to left trigger by chris 3/3
	Robot::m_drive->ShiftGear(param);

	double throttle = driverJoystick->GetRawAxis(Axis_LY);
	double turn = driverJoystick->GetRawAxis(Axis_RX);
	if(fabs(throttle) < .075){ throttle = 0; }
	if(fabs(turn) < .075){turn = 0; }
	//RobotMap::m_robotDrive->ArcadeDrive(throttle, turn);

	//Intakes
	if(!Robot::m_intake->IsIntakeSystemInUse()) // if the intake subsystem isn't utilizing the motors
	{
		//Rear Intake
		operatorJoystick->GetRawAxis(Axis_RTrigger) < .05 ? Robot::m_intake->SetRearIntake(0) : Robot::m_intake->SetRearIntake(operatorJoystick->GetRawAxis(Axis_RTrigger));

		//Front Intake
		if(driverJoystick->GetRawAxis(Axis_RTrigger) > .05)
		{
			Robot::m_intake->SetFrontIntake(-driverJoystick->GetRawAxis(Axis_RTrigger));
		}
		else if(driverJoystick->GetRawButton(6))
		{
			Robot::m_intake->SetFrontIntake(.5);
		}
		else
		{
			Robot::m_intake->SetFrontIntake(0);
		}
	}

	if(operatorJoystick->GetRawAxis(Axis_LTrigger) > .2){ // left trigger
		Robot::m_elevator->lockTotes(false);
	}
	if(operatorJoystick->GetRawAxis(Axis_RTrigger) > .2){ // right trigger
		Robot::m_elevator->lockTotes(true);
	}

	if(GetDebugJoystickButton(1)){
		Robot::m_binJuggler->loadSelection(Bin_LeftLock, true);
	}
	if(GetDebugJoystickButton(2)){
		Robot::m_binJuggler->loadSelection(Bin_LeftHook, true);
	}
	if(GetDebugJoystickButton(3)){
		Robot::m_binJuggler->loadSelection(Bin_RightHook, true);
	}
	if(GetDebugJoystickButton(4)){
		Robot::m_binJuggler->loadSelection(Bin_RightLock, true);
	}
	if(GetDebugJoystickButton(5)){
		Robot::m_binJuggler->loadSelection(Bin_LeftLock, false);
	}
	if(GetDebugJoystickButton(6)){
		Robot::m_binJuggler->loadSelection(Bin_LeftHook, false);
	}
	if(GetDebugJoystickButton(7)){
		Robot::m_binJuggler->loadSelection(Bin_RightHook, false);
	}
	if(GetDebugJoystickButton(8)){
		Robot::m_binJuggler->loadSelection(Bin_RightLock, false);
	}
	if(GetDebugJoystickButton(9)){
		Robot::m_binJuggler->loadSelection(Bin_LiftCylinder, true);
	}
	if(GetDebugJoystickButton(10)){

	}
	if(GetDebugJoystickButton(11)){

	}
	if(GetDebugJoystickButton(12)){

	}
	if(GetDebugJoystickButton(13)){
		Robot::m_binJuggler->loadSelection(Bin_LiftCylinder, false);
	}
	if(GetDebugJoystickButton(14)){
		Robot::m_recycler->HandleHold(true);
	}
	if(GetDebugJoystickButton(15)){
		Robot::m_recycler->HandleHold(false);
	}
	if(GetDebugJoystickButton(16)){

	}
	if(GetDebugJoystickButton(16)){
		operatorJoystick->SetRumble(Joystick::kLeftRumble,debugJoystick->GetRawAxis(4)); // left rumble = 0, right rumble = 1
	}
	if(GetDebugJoystickButton(10)){
		operatorJoystick->SetRumble(Joystick::kRightRumble,debugJoystick->GetRawAxis(4)); // left rumble = 0, right rumble = 1
	}

	//Re-add when we have a working sensor
	if(Robot::m_intake->IsAligned()){ // controlling rumble for the driver when aligned
		driverJoystick->SetRumble(Joystick::kLeftRumble,1); // left rumble = 0, right rumble = 1
		driverJoystick->SetRumble(Joystick::kRightRumble,1); // left rumble = 0, right rumble = 1
	}
	else{ // turn off the rumble
		driverJoystick->SetRumble(Joystick::kLeftRumble,0); // left rumble = 0, right rumble = 1
		driverJoystick->SetRumble(Joystick::kRightRumble,0); // left rumble = 0, right rumble = 1
	}

	/*elevator*/
	bool outsideElevatorDeadband = fabs(operatorJoystick->GetRawAxis(Axis_LY)) > 0.15;
	if(outsideElevatorDeadband)
	{
		m_allowElevatorInput = true;
		Robot::m_elevator->disablePID();
	}
	else if(m_allowElevatorInput && !outsideElevatorDeadband)
	{
		//Turn motors off then don't allow human input again until joysticks are moved again
		m_allowElevatorInput = false;

		//Robot::m_elevator->setAbsoluteHeight(Robot::m_elevator->getCurrentHeight());
		Robot::m_elevator->setElevatorMotors(0);
	}
	if(m_allowElevatorInput)
	{
		//The elevator output is bounds checked in the Elevator class
		Robot::m_elevator->setElevatorMotors(operatorJoystick->GetRawAxis(Axis_LY));
	}
	//Recycler Track
	bool outsideRecyclerDeadband = fabs(operatorJoystick->GetRawAxis(Axis_RY)) > 0.15;
	if(outsideRecyclerDeadband)
	{
		m_allowRecyclerInput = true;
	}
	else if(m_allowRecyclerInput && !outsideRecyclerDeadband)
	{
		m_allowRecyclerInput = false;
		Robot::m_recycler->SetRecycleMotors(0);
	}
	if(m_allowRecyclerInput)
	{
		//The elevator output is bounds checked in the Recycler class
		Robot::m_recycler->SetRecycleMotors(-operatorJoystick->GetRawAxis(Axis_RY));
	}

	if(driverJoystick->GetRawButton(1) || driverJoystick->GetRawButton(2) ||
			driverJoystick->GetRawButton(3) || driverJoystick->GetRawButton(4)){ // if one of the paddles are pressed
		Robot::m_intake->SetControlledIntake();
	}

}

bool OI::GetDebugJoystickButton(int number){
	switch(number){
	case 1:
		return debugJoystick2->GetRawButton(1);
		break;
	case 2:
		return debugJoystick2->GetRawButton(2);
		break;
	case 3:
		return debugJoystick2->GetRawButton(3);
		break;
	case 4:
		return debugJoystick2->GetRawButton(4);
		break;
	case 5:
		return debugJoystick->GetRawButton(9);
		break;
	case 6:
		return debugJoystick->GetRawButton(10);
		break;
	case 7:
		return debugJoystick->GetRawButton(11);
		break;
	case 8:
		return debugJoystick->GetRawButton(12);
		break;
	case 9:
		return debugJoystick->GetRawButton(5);
		break;
	case 10:
		return debugJoystick->GetRawButton(6);
		break;
	case 11:
		return debugJoystick->GetRawButton(7);
		break;
	case 12:
		return debugJoystick->GetRawButton(8);
		break;
	case 13:
		return debugJoystick->GetRawButton(1);
		break;
	case 14:
		return debugJoystick->GetRawButton(2);
		break;
	case 15:
		return debugJoystick->GetRawButton(3);
		break;
	case 16:
		return debugJoystick->GetRawButton(4);
		break;
	case 17:
		return debugJoystick2->GetRawButton(5);
		break;
	default:
		return false;
		break;
	}
}


