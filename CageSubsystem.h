#ifndef CAGESUBSYSTEM_H
#define CAGESUBSYSTEM_H

#include "CORERobot/CORERobot.h"
#include "WPILib.h"

using namespace CORE;

class CageSubsystem : public CORESubsystem {
	
public:
	
	std::string name(void){
		return "cage";
	}
	
	CageSubsystem(CORERobot& robot):
		CORESubsystem(robot)
	{
		
	}
	
	void robotInit(void);
	void teleopInit(void);
	void teleop(void);
	
};

#endif
