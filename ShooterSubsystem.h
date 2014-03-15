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
	
	bool armed;
	bool unwound;
	bool shooting;
	
public:
	std::string name(void){
		return "shooter";
	}	
	ShooterSubsystem(CORERobot& robot):
		CORESubsystem(robot),
		shooterWheel(6),
		photo(2),
		armed(false),
		unwound(false),
		shooting(false)
	{
	}
	
	void robotInit(void);
	void teleopInit(void);
	void teleop(void);
	
	bool getSwitchRise(void);
	bool getSwitchFall(void);
	bool getSwitchRaw(void);
	void setMotor(double speed);
	bool isArmed(void);
	void setArmed(bool value);
};
class WaitArmedAction : public Action{
	ShooterSubsystem* shooter;
	public:
		WaitArmedAction(ShooterSubsystem& shooter):
		shooter(&shooter){
			
		}
		void init(void){
			
		}
		ControlFlow call(void){
			if (!shooter->isArmed()){
				return CONTINUE;
			}
			return END;
		}
};
class WindupAction : public Action{
	ShooterSubsystem* shooter;
	bool sync;
public:
	WindupAction(ShooterSubsystem& shooter, bool sync = true):
	shooter(&shooter),
	sync(sync){
		
	}
	void init(void){
		
	}
	ControlFlow call(void){
		if(!shooter->getSwitchRise()){
			shooter->setMotor(SmartDashboard::GetNumber("choochoo-speed"));
			return sync?CONTINUE:BACKGROUND;
		} else {
			shooter->setMotor(0);
			shooter->setArmed(true);
			return END;
		}
	}
};
class ShootAction : public Action{
	ShooterSubsystem* shooter;
public:
	ShootAction(ShooterSubsystem& shooter):
	shooter(&shooter)
	{}
	void init(void){
	}
	ControlFlow call(void){
		SmartDashboard::PutBoolean("armed", shooter->isArmed());
		SmartDashboard::PutBoolean("sensor", shooter->getSwitchRaw());
		
		if(!shooter->isArmed()){
			return CONTINUE;
		}
		if (!shooter->getSwitchRaw()){
			shooter->setMotor(0);
			return END;
		}
		shooter->setMotor(1);
		return CONTINUE;
	}
};

#endif
