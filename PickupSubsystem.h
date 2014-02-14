#ifndef PICKUPSUBSYSTEM_H
#define PICKUPSUBSYSTEM_H

#include "CORERobot/CORERobot.h"
#include "WPILib.h"

using namespace CORE;

class PickupSubsystem : public CORESubsystem {
	Victor roller_motor;
	DoubleSolenoid pickup_solenoid_left;
	DoubleSolenoid pickup_solenoid_right;
	
public:
	std::string name(void){
		return "pickup";
	}
	
	PickupSubsystem(CORERobot& robot):
		CORESubsystem(robot),
		roller_motor(22), //not being tested
		pickup_solenoid_left(1,3,4), //module 2 removed for testing,
		pickup_solenoid_right(2,3,4)
	{
		
	}
	
	void robotInit(void);
	void teleopInit(void);
	void teleop(void);
	void goOut(void);
};

class PickupOut : public Action{
	PickupSubsystem* pickup;
public:
	PickupOut(PickupSubsystem& pickup):
	pickup(&pickup){

	}
	void init(void){}
	ControlFlow call(void){
		pickup->goOut();
		return END; 
		}
	
};

#endif
