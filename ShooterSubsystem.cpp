#include "ShooterSubsystem.h"

void ShooterSubsystem::robotInit(void){
	SmartDashboard::PutNumber("shoot-delay", .5);
	SmartDashboard::PutNumber("choochoo-speed", .8);
	SmartDashboard::PutBoolean("armed", false);
}
void ShooterSubsystem::teleopInit(void){
	robot.joystick.register_button("shoot", 2, 1);
	robot.joystick.register_button("arm", 2 , 2);
	
}
void ShooterSubsystem::teleop(void){
	if (limitSwitch.Get() && !armed){
			armed = true;
	}
	double output = 0;
	double speed = SmartDashboard::GetNumber("choochoo-speed");
	if (robot.joystick.button("arm")){
		if (!armed){
			output = speed;
		}else{
			output = 0;
		}
	}
	
	if (robot.joystick.button("shoot") && armed){
		output = speed;
		shootTimer.Start();
	} else if (shootTimer.Get() != 0){
		output = speed;
	}
	if (shootTimer.Get() >= SmartDashboard::GetNumber("shoot-delay")){
		shootTimer.Stop();
		shootTimer.Reset();
		output = 0;
		armed = false;
	}
	cout << "timer: " << shootTimer.Get() << " armed: " << armed << endl;
	SmartDashboard::PutBoolean("armed", armed);
	shooterWheel.Set(output);
}
bool ShooterSubsystem::getSwitch(void){
	return limitSwitch.Get();
}
void ShooterSubsystem::setMotor(double speed){
	shooterWheel.Set(speed);
}
void ShooterSubsystem::StartTimer(void){
	shootTimer.Reset();
	shootTimer.Start();
}
bool ShooterSubsystem::WaitForTimer(void){
	if (shootTimer.Get() >= SmartDashboard::GetNumber("shoot-delay")){
		shootTimer.Stop();
		shootTimer.Reset();
		return true;
	} else {
		return false;
	}
}
void ShooterSubsystem::StopTimer(void){
	shootTimer.Stop();
	shootTimer.Reset();
}
