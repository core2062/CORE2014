#ifndef SHOOTERSUBSYSTEM_H
#define SHOOTERSUBSYSTEM_H

#include "CORERobot/CORERobot.h"
#include "WPILib.h"

using namespace CORE;

class ShooterSubsystem : public CORESubsystem {
	
	Victor shooterWheel;
	DigitalInput limitSwitch;
	Timer shootTimer;
	
	bool armed;
	
public:
	std::string name(void){
		return "shooter";
	}	
	ShooterSubsystem(CORERobot& robot):
		CORESubsystem(robot),
		shooterWheel(10),
		limitSwitch(8)
	{
		armed = false;
	}
	
	void robotInit(void);
	void teleopInit(void);
	void teleop(void);
	
};

#endif
