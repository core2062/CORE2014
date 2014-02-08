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
		shooterWheel(6),
		limitSwitch(8),
		shootTimer()
	{
		armed = false;
	}
	
	void robotInit(void);
	void teleopInit(void);
	void teleop(void);
	bool getSwitch(void);
	void setMotor(double speed);
	void StartTimer(void);
	bool WaitForTimer(void);
	void StopTimer(void);
};

class Windup : public Action{
	ShooterSubsystem* shooter;
public:
	Windup(ShooterSubsystem& shooter):
	shooter(&shooter){

	}
	void init(void){}
	ControlFlow call(void){
		if(!shooter->getSwitch()){
			shooter->setMotor(1);
			return CONTINUE;
		} else {
			shooter->setMotor(0);
			return END;
		}
	}
};
class FireShot : public Action{
	ShooterSubsystem* shooter;
	bool isArmed;
public:
	FireShot(ShooterSubsystem& shooter):
	shooter(&shooter),
	isArmed(true)
	{}
	void init(void){}
	ControlFlow call(void){
		if (isArmed){
			shooter->setMotor(1);
			isArmed = false;
			return CONTINUE;
		} else {
			shooter->StartTimer();
			if (shooter->WaitForTimer()){
				shooter->setMotor(0);
				shooter->StopTimer();
				return END;
			} else {
				return CONTINUE;
			}
		}
		shooter->setMotor(1);
		
	}
};

#endif
