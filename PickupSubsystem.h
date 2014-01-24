#ifndef PICKUPSUBSYSTEM_H
#define PICKUPSUBSYSTEM_H

#include "CORERobot/CORERobot.h"
#include "WPILib.h"

using namespace CORE;

class PickupSubsystem : public CORESubsystem {
	Victor roller_motor;
	DoubleSolenoid pickup_solenoid_left;
	DoubleSolenoid pickup_solenoid_right;
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
		roller_motor(10),
		pickup_solenoid_left(1,2), //module 2 removed for testing
		pickup_solenoid_right(2,3,4),
		pickup_limit_in(7),
		pickup_limit_out(8)
	{
		old_roller_in = false;
		old_roller_out = false;
	}
	
	void robotInit(void);
	void teleopInit(void);
	void teleop(void);
};

#endif
