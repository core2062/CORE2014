#include "PickupSubsystem.h"

void PickupSubsystem::robotInit(void){
	robot.requirePneumatics();
	SmartDashboard::PutNumber("pickup-speed", .75);
	SmartDashboard::PutNumber("roller-speed", .75);
	
}
void PickupSubsystem::teleopInit(void){
	robot.compressor->Start();
	
	robot.joystick.register_button("pickup-out", 2, 7);
	robot.joystick.register_button("pickup-in", 2, 8);
	robot.joystick.register_button("roller-in", 2, 9);
	robot.joystick.register_button("roller-out", 2, 10);

}
void PickupSubsystem::teleop(void){
	double pickup_speed = SmartDashboard::GetNumber("pickup-speed");
	double roller_speed = SmartDashboard::GetNumber("roller-speed");
	double pickup_output = 0;
	double roller_output = roller_motor.Get();
	
	if (robot.joystick.button("pickup-out")){
		pickup_output = pickup_speed;
	}
	else if (robot.joystick.button("pickup-in")){
		pickup_output = -pickup_speed;
	}
	
	if (robot.joystick.button("roller-in") && !old_roller_in){
		if (roller_motor.Get() <= 0){
			roller_output = roller_speed;
		} else { 
			roller_output = 0;
		}
	} else if (robot.joystick.button("roller-out") && !old_roller_out){
		if (roller_motor.Get() >= 0){
			roller_output = -roller_speed;
		} else { 
			roller_output = 0;
		}
	}
	
	old_roller_out = robot.joystick.button("roller-out");
	old_roller_in = robot.joystick.button("roller-in");
	
	pickup_motor.Set(pickup_output);
	roller_motor.Set(roller_output);
}
	
