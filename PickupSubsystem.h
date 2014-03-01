#ifndef PICKUPSUBSYSTEM_H
#define PICKUPSUBSYSTEM_H

#include "CORERobot/CORERobot.h"
#include "WPILib.h"

using namespace CORE;

class PickupSubsystem : public CORESubsystem {
	Victor roller_motor;
	DoubleSolenoid pickup_left;
	DoubleSolenoid pickup_right;
	
public:
	std::string name(void){
		return "pickup";
	}
	
	PickupSubsystem(CORERobot& robot):
		CORESubsystem(robot),
		roller_motor(5),
		pickup_left(1,3,4), 
		pickup_right(2,3,4)
	{
		
	}
	
	void robotInit(void);
	void teleopInit(void);
	void teleop(void);
	void cylinderOut(void);
	void putDown(void);
	void putUp(void);
	void intake(float speed);
};

class PickupOut : public Action{
	PickupSubsystem* pickup;
public:
	PickupOut(PickupSubsystem& pickup):
		pickup(&pickup)
	{
	}
	
	void init(void){
	
	}
	
	ControlFlow call(void){
		pickup->putDown();
		return END;
		}
};

class PickupIn : public Action{
	PickupSubsystem* pickup;
public:
	PickupIn(PickupSubsystem& pickup):
		pickup(&pickup)
	{
	}
	
	void init(void){
	
	}
	
	ControlFlow call(void){
		pickup->putUp();
		return END;
		}
};

class rollerIn : public WaitAction{
	PickupSubsystem* pickup;
public:
	rollerIn (PickupSubsystem& pickup, double duration):
		WaitAction(duration),
		pickup(&pickup)
	{	}
	
	void init(void){
	}
	
	ControlFlow call(void){
		ControlFlow flow = WaitAction::call();
		if (flow == CONTINUE){
			pickup->intake(-1);
			return BACKGROUND;
		} else {
			pickup->intake(0);
			return END;
		}
	}
	
};
class pickupBall : public Action {
	PickupSubsystem* pickup;
	Timer pickupTimer;
	bool isUp;
public:
	pickupBall (PickupSubsystem& pickup):
		pickup(&pickup),
		pickupTimer(),
		isUp(false)
	{   }
	void init(void){
		pickupTimer.Reset();
		pickupTimer.Start();
	}
	ControlFlow call(void){
		if (pickupTimer.Get()<1.5){
			pickup->intake(-1);
			if (pickupTimer.Get()>.5 && !isUp){
				pickup->putUp();
				isUp = true;
			}
			return CONTINUE;
		}else{
			pickup->intake(0);
			return END;
		}
	}
};

#endif
