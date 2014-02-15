#include "PickupSubsystem.h"

void PickupSubsystem::robotInit(void){
	robot.requirePneumatics();
//	SmartDashboard::PutNumber("pickup-speed", .75);
	SmartDashboard::PutNumber("roller-speed", .75);
}
void PickupSubsystem::teleopInit(void){
	robot.compressor->Start();
	
	robot.joystick.register_button("pickup-out", 2, 7, JoystickCache::RISING);
	robot.joystick.register_button("pickup-in", 2, 8, JoystickCache::RISING);
	robot.joystick.register_button("roller-in", 2, 9, JoystickCache::RISING);
	robot.joystick.register_button("roller-out", 2, 10, JoystickCache::RISING);

	pickup_left.Set(DoubleSolenoid::kOff);
	pickup_right.Set(DoubleSolenoid::kOff);
}
void PickupSubsystem::teleop(void){
	double roller_speed = SmartDashboard::GetNumber("roller-speed");
	double roller_output = roller_motor.Get();
	
	if (robot.joystick.button("roller-in")){
		if (roller_motor.Get() >= 0){
			roller_output = -roller_speed;
		} else { 
			roller_output = 0;
		}
	} else if (robot.joystick.button("roller-out")){
		if (roller_motor.Get() <= 0){
			roller_output = roller_speed;
		} else { 
			roller_output = 0;
		}
	}
	
	//pickup_motor.Set(pickup_output);
	if (robot.joystick.button("pickup-out")){
		pickup_left.Set(DoubleSolenoid::kReverse);
		pickup_right.Set(DoubleSolenoid::kReverse);
	}else if (robot.joystick.button("pickup-in")){
		pickup_left.Set(DoubleSolenoid::kForward);
		pickup_right.Set(DoubleSolenoid::kForward);
	} else {
		pickup_left.Set(DoubleSolenoid::kOff);
	    pickup_right.Set(DoubleSolenoid::kOff);

	}
	roller_motor.Set(roller_output);
}
void PickupSubsystem::cylinderOut(void){
	pickup_left.Set(DoubleSolenoid::kReverse);
	pickup_right.Set(DoubleSolenoid::kReverse);
}
	
