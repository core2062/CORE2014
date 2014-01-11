#ifndef SHOOTERSUBSYSTEM_H
#define SHOOTERSUBSYSTEM_H

#include "CORERobot/CORERobot.h"
#include "WPILib.h"

using namespace CORE;

class ShooterSubsystem : public CORESubsystem {
	
public:
	std::string name(void){
		return "shooter";
	}	
	ShooterSubsystem(CORERobot& robot):
		CORESubsystem(robot)
	{

	}
	
	void robotInit(void);
	void teleopInit(void);
	void teleop(void);
	
};

#endif
