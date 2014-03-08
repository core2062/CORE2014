#ifndef SHOOTERSUBSYSTEM_H
#define SHOOTERSUBSYSTEM_H

#include "CORERobot/CORERobot.h"
#include "WPILib.h"

using namespace CORE;

class SensorEdge {
	DigitalInput	sensor;
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
//		return( sensor.GetVoltage() < 3.5 );
		return !sensor.Get();
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
//	float GetVoltage(){
//		return sensor.GetVoltage();
//	}
};

class ShooterSubsystem : public CORESubsystem {
	
	Victor shooterWheel;
	SensorEdge photo;
	Timer shootTimer;
	
	bool armed;
	bool unwound;
	bool autoArmed;
	
public:
	std::string name(void){
		return "shooter";
	}	
	ShooterSubsystem(CORERobot& robot):
		CORESubsystem(robot),
		shooterWheel(6),
		photo(2),
		shootTimer(),
		armed(false),
		unwound(false)
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

class WindupAction : public Action{
	ShooterSubsystem* shooter;
public:
	WindupAction(ShooterSubsystem& shooter):
	shooter(&shooter){
		shooter.setAArmed(false);
	}
	void init(void){
		shooter->getSwitch();
	}
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
class ShootAction : public Action{
	ShooterSubsystem* shooter;
	Timer timer;
public:
	ShootAction(ShooterSubsystem& shooter):
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
		shooter->setMotor(1);
		return CONTINUE;
	}
};

#endif
