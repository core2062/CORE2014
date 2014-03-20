#include "ShooterSubsystem.h"

void ShooterSubsystem::robotInit(void){
	SmartDashboard::PutNumber("shoot-delay", .5);
	SmartDashboard::PutNumber("choochoo-speed", .8);
	SmartDashboard::PutBoolean("armed", false);
}
void ShooterSubsystem::teleopInit(void){
//	if (SmartDashboard::GetBoolean("alt-operator-config")){
		robot.joystick.register_button("shoot", 2, 2);
		robot.joystick.register_button("arm", 2, 1);
		robot.joystick.register_button("unwind", 2 ,4);
//	}else{
//		robot.joystick.register_button("shoot", 2, 1);
//		robot.joystick.register_button("arm", 2, 4);
//		robot.joystick.register_button("unwind", 2 ,6);
//	}
	armed = photo.Get();
}
void ShooterSubsystem::teleop(void){
	armed = photo.Get();
	if (photo.Rise()){
		if(shooterWheel.Get() > 0){
			armed = true;
		} else {
			unwound = true;
		}
	} else if (photo.Fall()){
		if(unwound) { 
			unwound = false;
		}
	}
	
	double output = 0;
	double speed = SmartDashboard::GetNumber("choochoo-speed");
	
	if (robot.joystick.button("arm")){
		if (!armed){
			output = 1;
		}else{
			output = 0;
		}
	}
	
	if (robot.joystick.button("shoot") && armed){
		output = 1;
		shooting = true;
	} else if (shooting){
		output = 1;
	}
	if (!photo.Get() && shooting){
		output = 0;
		armed = false;
		shooting = false;
	}
	if(robot.joystick.button("unwind") && !unwound){
		output = -0.5;
		armed = false;
	}
	SmartDashboard::PutBoolean("armed", armed);
	SmartDashboard::PutBoolean("sensor", photo.Get());
	shooterWheel.Set(speed*output);
}
bool ShooterSubsystem::getSwitchRise(void){
	return photo.Rise();
}
bool ShooterSubsystem::getSwitchFall(void){
	return photo.Fall();
}
bool ShooterSubsystem::getSwitchRaw(void){
	return photo.Get();
}
void ShooterSubsystem::setMotor(double speed){
	shooterWheel.Set(speed*SmartDashboard::GetNumber("choochoo-speed"));
}
bool ShooterSubsystem::isArmed(void){
	return armed;
}
void ShooterSubsystem::setArmed(bool value){
	armed = value;
}

