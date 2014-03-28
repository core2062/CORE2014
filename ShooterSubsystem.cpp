#include "ShooterSubsystem.h"

void ShooterSubsystem::robotInit(void){
	SmartDashboard::PutNumber("shoot-delay", 1);
	SmartDashboard::PutNumber("choochoo-speed", .8);
	SmartDashboard::PutBoolean("armed", false);
}
void ShooterSubsystem::teleopInit(void){
	if (SmartDashboard::GetBoolean("alt-operator-config")){
		robot.joystick.register_button("shoot", 2, 2);
		robot.joystick.register_button("arm", 2, 1);
		robot.joystick.register_button("unwind", 2 ,4);
	}else{
		robot.joystick.register_button("shoot", 2, 1);
		robot.joystick.register_button("arm", 2, 4);
		robot.joystick.register_button("unwind", 2 ,6);
	}
	armed = photo.Get();
}
void ShooterSubsystem::teleop(void){
	if (state == ARMED){
		output = 0;
		if (robot.joystick.button("shoot")){
			state = FIRING;
			shootTimer.Reset();
			shootTimer.Start();
		}else if (robot.joystick.button("unwind")){
			state = UNWIND;
		}
		
		
	}else if (state == FIRING){
		output = 1;
		if (shootTimer.Get()>=SmartDashboard::GetNumber("shoot-delay")){
			shootTimer.Stop();
			shootTimer.Reset();
			output = 0;
			state = IDLE;
		}
	}else if (state == IDLE){
		output = 0;
		if (robot.joystick.button("arm")){
			state = LOADING;
		} else if (robot.joystick.button("unwind")){
			state = UNWIND;
		}
	}else if (state == LOADING){
		output = 1;
		if (photo.Rise()){
			state = ARMED;
		} else if (!robot.joystick.button("arm")){
			state = IDLE;
		}
	}else if (state == UNWIND){
		output = -.5;
		if (!robot.joystick.button("unwind")){
			state = IDLE;
		} else if (photo.Rise()){
			state = UNWOUND;
		}
	}else if (state == UNWOUND){
		output = 0;
		if (robot.joystick.button("arm"))
			state = LOADING;
	}

	cout << state << endl;
	double speed = SmartDashboard::GetNumber("choochoo-speed");
	shooterWheel.Set(speed*output);
}
bool ShooterSubsystem::getSwitchRise(void){
	return photo.Rise();
}
bool ShooterSubsystem::getSwitchFall(void){
	return photo.Fall();
}
bool ShooterSubsystem::getSwitchRaw(void){
	SmartDashboard::PutBoolean("sensor", photo.Get());
	return photo.Get();
}
void ShooterSubsystem::setMotor(double speed){
	shooterWheel.Set(speed*SmartDashboard::GetNumber("choochoo-speed"));
}
bool ShooterSubsystem::isArmed(void){
	SmartDashboard::PutBoolean("armed", armed);
	return armed;
}
void ShooterSubsystem::setArmed(bool value){
	armed = value;
}

