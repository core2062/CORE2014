#ifndef CAGESUBSYSTEM_H
#define CAGESUBSYSTEM_H

#include "CORERobot/CORERobot.h"
#include "WPILib.h"

using namespace CORE;

class CageSubsystem : public CORESubsystem {
	DoubleSolenoid rightWing;
	DoubleSolenoid leftWing;
	
	bool isOut;
public:
	
	std::string name(void){
		return "cage";
	}
	
	CageSubsystem(CORERobot& robot):
		CORESubsystem(robot),
		rightWing(1,2),
		leftWing(3,4)
	{
		isOut = false;
	}
	
	void robotInit(void);
	void teleopInit(void);
	void teleop(void);
	
};

#endif
