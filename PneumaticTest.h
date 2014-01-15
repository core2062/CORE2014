#ifndef PNEUMATICTEST_H
#define PNEUMATICTEST_H

#include "CORERobot/CORERobot.h"
#include "WPILib.h"

using namespace CORE;

class PneumaticTest : public CORESubsystem {
	DoubleSolenoid solenoid;
public:
	std::string name(void){
		return "pneumatic";
	}
	
	PneumaticTest(CORERobot& robot):
		CORESubsystem(robot),
		solenoid(1,2)
	{

		
	}
	
	void robotInit(void);
	void teleopInit(void);
	void teleop(void);
	
};

#endif
