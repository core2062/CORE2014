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

class PickupAction : public WaitAction{
	PickupSubsystem* pickup;
	int direction;
public:
	PickupAction(PickupSubsystem& pickup, int dir = -1, double duration = 0.1):
		WaitAction(duration),
		pickup(&pickup),
		direction(dir)
	{
	}
	
	void init(void){
	
	}
	
	ControlFlow call(void){
		ControlFlow flow = WaitAction::call();
		if (flow == CONTINUE){
			if (direction == -1){
				pickup->putDown();
			}else{
				pickup->putUp();			
			}
			return CONTINUE;
		}else{
			return END;
		}
	}	
};


class RollerAction : public WaitAction{
	PickupSubsystem* pickup;
	int direction;
public:
	RollerAction (PickupSubsystem& pickup, int dir, double duration):
		WaitAction(duration),
		pickup(&pickup),
		direction(dir)
	{	}
	
	void init(void){
	}
	
	ControlFlow call(void){
		ControlFlow flow = WaitAction::call();
		if (flow == CONTINUE){
			pickup->intake(direction);
			return CONTINUE;
		} else {
			pickup->intake(0);
			return END;
		}
	}
	
};
class PickupRollerAction : public Action {
	PickupSubsystem* pickup;
	Timer pickupTimer;
	bool isChanged;
	int direction;
	double duration;
public:
	PickupRollerAction(PickupSubsystem& pickup, int dir, double dur):
		pickup(&pickup),
		pickupTimer(),
		isChanged(false),
		direction(dir),
		duration(dur)
	{   }
	void init(void){
		pickupTimer.Reset();
		pickupTimer.Start();
	}
	ControlFlow call(void){
		if (!isChanged){
			if (direction = -1){
				pickup->putUp();
			}else{
			pickup->putDown();			
			}

		}		
		if (pickupTimer.Get()<duration){
			pickup->intake(-1);

			return CONTINUE;
		}else{
			pickup->intake(0);
			pickupTimer.Stop();
			return END;
		}
	}
};

#endif
