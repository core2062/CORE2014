#ifndef CAGESUBSYSTEM_H
#define CAGESUBSYSTEM_H

#include "CORERobot/CORERobot.h"
#include "WPILib.h"
#include "PickupSubsystem.h"
#include "ShooterSubsystem.h"

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
		rightWing(2,5,6),
		leftWing(1,1,2)
	{
		isOut = false;
	}
	
	void robotInit(void);
	void teleopInit(void);
	void teleop(void);
	void cylinderOut(void);
	void cylinderIn(void);
};

class OpenArms : public Action{
	ShooterSubsystem* shooter;
	PickupSubsystem* pickup;
	CageSubsystem* cage;
	Timer timer;
	bool armed;
public:
	OpenArms(ShooterSubsystem& shooter, PickupSubsystem& pickup, CageSubsystem& cage):
	shooter(&shooter),
	pickup(&pickup),
	cage(&cage),
	timer()
	{}
	void init(void){
		timer.Reset();
	}
	ControlFlow call(void){
		if(!shooter->isArmed()){
			return CONTINUE;
		}
		if (timer.Get() >= SmartDashboard::GetNumber("auto-cage-delay")){
			timer.Stop();
			return END;
		}
		if(timer.Get() == 0){
			timer.Start();
		}
		cage->cylinderOut();
		pickup->putDown();
		return CONTINUE;
	}
};

class CageAction : public Action{
	CageSubsystem* cage;
	Timer timer;
	int direction;
	double delay;
public:
	CageAction(CageSubsystem& cage, int direction, double delay):
	cage(&cage),
	timer(),
	direction(direction),
	delay(delay)
	{}
	void init(void){
		timer.Reset();
	}
	ControlFlow call(void){
		if (timer.Get() >= delay){
			timer.Stop();
			return END;
		}
		if(timer.Get() == 0){
			timer.Start();
		}
		if (direction > 0){
			cage->cylinderOut();
		} else {
			cage->cylinderIn();
		}
		return CONTINUE;
	}
};

#endif
