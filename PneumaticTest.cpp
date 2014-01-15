#include "PneumaticTest.h"

void PneumaticTest::robotInit(void){
	robot.requirePneumatics();
	
}
void PneumaticTest::teleopInit(void){
	robot.compressor->Start();
	robot.joystick.register_button("solenoid", 1 ,1);
	
}
void PneumaticTest::teleop(void){
	if (robot.joystick.button("solenoid")){
		solenoid.Set(DoubleSolenoid::kReverse);
	}else{
		solenoid.Set(DoubleSolenoid::kForward);
	}
}
	
