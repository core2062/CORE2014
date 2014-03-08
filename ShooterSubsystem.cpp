#include "ShooterSubsystem.h"

void ShooterSubsystem::robotInit(void){
	SmartDashboard::PutNumber("shoot-delay", .5);
	SmartDashboard::PutNumber("choochoo-speed", .7);
	SmartDashboard::PutBoolean("armed", false);
}
void ShooterSubsystem::teleopInit(void){
	robot.joystick.register_button("shoot", 2, 1);
	robot.joystick.register_button("arm", 2 , 2);
	robot.joystick.register_button("unwind",2,6);
}
void ShooterSubsystem::teleop(void){
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
		shootTimer.Start();
	} else if (shootTimer.Get() != 0){
		output = 1;
	}
	if (shootTimer.Get() >= SmartDashboard::GetNumber("shoot-delay")){
		shootTimer.Stop();
		shootTimer.Reset();
		output = 0;
		armed = false;
	}
//	cout << "timer: " << shootTimer.Get() << " armed: " << armed << endl;
	if(robot.joystick.button("unwind") && !unwound){
		shootTimer.Stop();
		shootTimer.Reset();
		output = -0.5;
		armed = false;
	}
	SmartDashboard::PutBoolean("armed", armed);
	SmartDashboard::PutBoolean("sensor", photo.Get());
	shooterWheel.Set(speed*output);
}
bool ShooterSubsystem::getSwitch(void){
	return photo.Rise();
}
void ShooterSubsystem::setMotor(double speed){
	shooterWheel.Set(speed*SmartDashboard::GetNumber("choochoo-speed"));
}
bool ShooterSubsystem::aArmed(void){
	return autoArmed;
}
void ShooterSubsystem::setAArmed(bool value){
	autoArmed = value;
}
