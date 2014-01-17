#ifndef PICKUPSUBSYSTEM_H
#define PICKUPSUBSYSTEM_H

#include "CORERobot/CORERobot.h"
#include "WPILib.h"

using namespace CORE;

class PickupSubsystem : public CORESubsystem {
	Victor roller_motor;
	Victor pickup_motor;
	DigitalInput pickup_limit_in;
	DigitalInput pickup_limit_out;

	bool old_roller_in;
	bool old_roller_out;
	
public:
	std::string name(void){
		return "pickup";
	}
	
	PickupSubsystem(CORERobot& robot):
		CORESubsystem(robot),
		roller_motor(21),
		pickup_motor(23),
		pickup_limit_in(25),
		pickup_limit_out(24)
	{
		old_roller_in = false;
		old_roller_out = false;
	}
	
	void robotInit(void);
	void teleopInit(void);
	void teleop(void);
};

#endif
