#ifndef SHOOTERSUBSYSTEM_H
#define SHOOTERSUBSYSTEM_H

#include "CORERobot/CORERobot.h"
#include "WPILib.h"

using namespace CORE;

class SensorEdge {
	AnalogChannel	sensor;
	bool		oldRState;
	bool		oldFState;
	
public:
	SensorEdge(uint32_t chan):
		sensor(chan)
	{
		oldRState = false;
		oldFState = true;
	}
	bool Get(){
		return( sensor.GetVoltage() < 3.5 );
	}
	bool Rise(){
		bool state = Get();
		bool rise = state and !oldRState;
		oldRState = state;
		return rise;
	}
	bool Fall(){
		bool state = Get();
		bool fall = !state and oldFState;
		oldFState = state;
		return fall;
	}
	float GetVoltage(){
		return sensor.GetVoltage();
	}
};

class ShooterSubsystem : public CORESubsystem {
	
	Jaguar shooterWheel;
	SensorEdge photo;
	Timer shootTimer;
	
	bool armed;
	bool autoArmed;
	
public:
	std::string name(void){
		return "shooter";
	}	
	ShooterSubsystem(CORERobot& robot):
		CORESubsystem(robot),
		shooterWheel(10),
		photo(1),
		shootTimer(),
		armed(false),
		autoArmed(false)
	{
	}
	
	void robotInit(void);
	void teleopInit(void);
	void teleop(void);
	
	bool getSwitch(void);
	void setMotor(double speed);
	bool aArmed(void);
	void setAArmed(bool value);
};

class Windup : public Action{
	ShooterSubsystem* shooter;
public:
	Windup(ShooterSubsystem& shooter):
	shooter(&shooter){
		shooter.setAArmed(false);
	}
	void init(void){}
	ControlFlow call(void){
		if(!shooter->getSwitch()){
			shooter->setMotor(SmartDashboard::GetNumber("choochoo-speed"));
			return CONTINUE;
		} else {
			shooter->setMotor(0);
			shooter->setAArmed(true);
			return END;
		}
	}
};
class FireShot : public Action{
	ShooterSubsystem* shooter;
	Timer timer;
public:
	FireShot(ShooterSubsystem& shooter):
	shooter(&shooter),
	timer()
	{}
	void init(void){
		timer.Reset();
	}
	ControlFlow call(void){
		if(!shooter->aArmed()){
			return CONTINUE;
		}
		if (timer.Get() >= SmartDashboard::GetNumber("shoot-delay")){
			timer.Stop();
			shooter->setMotor(0);
			return END;
		}
		if(timer.Get() == 0){
			timer.Start();
		}
		shooter->setMotor(SmartDashboard::GetNumber("choochoo-speed"));
		return CONTINUE;
	}
};

#endif
