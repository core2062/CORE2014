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
	timer(),
	armed(false)
	{}
	void init(void){
		cout << "init open arms" << endl;
		timer.Reset();
	}
	ControlFlow call(void){
		if(!armed && !shooter->aArmed()){
			return CONTINUE;
		} else if (!armed && shooter->aArmed()) {
			cout << "resetting armed" << endl;
			shooter->setAArmed(false);
			armed = true;
		}
		if (timer.Get() >= SmartDashboard::GetNumber("cage-delay")){
			timer.Stop();
			shooter->setAArmed(true);
			cout << "ending open arms" << endl;
			return END;
		}
		if(timer.Get() == 0){
			cout << "starting timer" << endl;
			timer.Start();
		}
		cout << "cage fall through" << endl;
		cage->cylinderOut();
		pickup->cylinderOut();
		return CONTINUE;
	}
};

#endif
