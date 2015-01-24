#include "SetRecyclerPosition.h"
#include "../Robot.h"

SetRecyclerPosition::SetRecyclerPosition(bool setUp)
{
	m_setUp = setUp;
	m_speed = 0;

}

void SetRecyclerPosition::Initialize()
{
	if(m_setUp == true){
		m_speed = .5;
	}
	else{
		m_speed = -.5;
	}
	Robot::m_recycler->SetRecycleMotors(m_speed);

}


void SetRecyclerPosition::Execute()
{

}

bool SetRecyclerPosition::IsFinished()
{
	if(m_setUp == true) {
		return Robot::m_recycler->isUpperSensorTripped();
	}
	else{
		return Robot::m_recycler->isLowerSensorTripped();
	}
}

void SetRecyclerPosition::End()
{
	m_speed = 0;
	Robot::m_recycler->SetRecycleMotors(m_speed);
}

void SetRecyclerPosition::Interrupted()
{
	End();
}
