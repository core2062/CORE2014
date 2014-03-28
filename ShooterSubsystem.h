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

enum ShooterState {
	ARMED,
	FIRING,
	IDLE,
	LOADING,
	UNWIND,
	UNWOUND	
};

class ShooterSubsystem : public CORESubsystem {
	
	Victor shooterWheel;
	SensorEdge photo;
	Timer shootTimer;
	
	double output;
	bool armed;
	
	ShooterState state;
	
public:
	std::string name(void){
		return "shooter";
	}	
	ShooterSubsystem(CORERobot& robot):
		CORESubsystem(robot),
		shooterWheel(6),
		photo(2),
		output(0)
	{
		state = ARMED;
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
		cout << "windup start" << endl;
	}
	ControlFlow call(void){
		cout << "windup: " << shooter->isArmed() << " : " << shooter->getSwitchRaw() << endl;
		if(!shooter->getSwitchRaw()){
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
	Timer timer;
public:
	ShootAction(ShooterSubsystem& shooter):
	shooter(&shooter),
	timer()
	{}
	void init(void){
		timer.Reset();
		timer.Start();
	}
	ControlFlow call(void){
		cout << "shoot: " << shooter->isArmed() << " : " << shooter->getSwitchRaw() << endl;		
		if(!shooter->isArmed()){
			return CONTINUE;
		}
		if (timer.Get()>SmartDashboard::GetNumber("shoot-delay")){
			shooter->setMotor(0);
			shooter->setArmed(false);
			return END;
		}
		shooter->setMotor(1);
		return CONTINUE;
	}
};

#endif
