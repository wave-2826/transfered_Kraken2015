// RobotBuilder Version: 1.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in th future.


#ifndef WAVEWAIT_H
#define WAVEWAIT_H


#include "../CommandBase.h"
#include "WPILib.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class WaveWait: public Command {
public:
	WaveWait();
	WaveWait(double seconds);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
