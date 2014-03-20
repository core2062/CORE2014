#include "PickupSubsystem.h"

void PickupSubsystem::robotInit(void){
	robot.requirePneumatics();
//	SmartDashboard::PutNumber("pickup-speed", .75);
	SmartDashboard::PutNumber("roller-speed", .75);
}
void PickupSubsystem::teleopInit(void){
	robot.compressor->Start();
	
//	if (SmartDashboard::GetBoolean("alt-operator-config")){
		robot.joystick.register_axis("roller-axis", 2, 2);
		robot.joystick.register_button("pickup-toggle", 2, 5, JoystickCache::RISING);
//	}else{
//		robot.joystick.register_button("pickup-out", 2, 7, JoystickCache::RISING);
//		robot.joystick.register_button("pickup-in", 2, 8, JoystickCache::RISING);
//		robot.joystick.register_button("roller-in", 2, 3);
//		robot.joystick.register_button("roller-out", 2, 5);
//		robot.joystick.register_button("pickup-toggle", 2, 11, JoystickCache::RISING);	
//	}
	


	pickup_left.Set(DoubleSolenoid::kOff);
	pickup_right.Set(DoubleSolenoid::kOff);
}
void PickupSubsystem::teleop(void){
	double roller_speed = SmartDashboard::GetNumber("roller-speed");
	double roller_output = roller_motor.Get();
	
	/*if (robot.joystick.button("roller-in")){
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
	}*/
	
	if (SmartDashboard::GetBoolean("alt-operator-config")){
		if(robot.joystick.axis("roller-axis") > .5){
			roller_output = -roller_speed;
		}else if(robot.joystick.axis("roller-axis") < -.5){
			roller_output = roller_speed;
		}else{
			roller_output = 0;
		}
	}else{
		if (robot.joystick.button("roller-in")){
				roller_output = -roller_speed;
		} else if (robot.joystick.button("roller-out")){
				roller_output = roller_speed;
		} else { 
			roller_output = 0;
		}
	}
	if (!SmartDashboard::GetBoolean("alt-operator-config")){
		//pickup_motor.Set(pickup_output);
		if (robot.joystick.button("pickup-out")){
			pickup_left.Set(DoubleSolenoid::kReverse);
			pickup_right.Set(DoubleSolenoid::kReverse);
		}else if (robot.joystick.button("pickup-in")){
			pickup_left.Set(DoubleSolenoid::kForward);
			pickup_right.Set(DoubleSolenoid::kForward);
		}/* else {
			pickup_left.Set(DoubleSolenoid::kOff);
			pickup_right.Set(DoubleSolenoid::kOff);
	
		}*/
	}
	if (robot.joystick.button("pickup-toggle")){
		DoubleSolenoid::Value v = pickup_left.Get();
		if (v == DoubleSolenoid::kForward ){
			cout << "p reverse" <<endl;
			pickup_left.Set(DoubleSolenoid::kReverse);
			pickup_right.Set(DoubleSolenoid::kReverse);
		}else{
			cout << "p forward" <<endl;
			pickup_left.Set(DoubleSolenoid::kForward);
			pickup_right.Set(DoubleSolenoid::kForward);
		}
	}
	roller_motor.Set(roller_output);
}
void PickupSubsystem::putDown(void){
	pickup_left.Set(DoubleSolenoid::kReverse);
	pickup_right.Set(DoubleSolenoid::kReverse);
}
void PickupSubsystem::putUp(void){
	pickup_left.Set(DoubleSolenoid::kForward);
	pickup_right.Set(DoubleSolenoid::kForward);
}
void PickupSubsystem::intake(float speed = 0){
	roller_motor.Set(SmartDashboard::GetNumber("roller-speed")*speed);

}
