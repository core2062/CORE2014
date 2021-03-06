#include "CageSubsystem.h"

void CageSubsystem::robotInit(void){
	robot.requirePneumatics();
}
void CageSubsystem::teleopInit(void){
	robot.compressor->Start();
	if (SmartDashboard::GetBoolean("alt-operator-config")){
		robot.joystick.register_button("toggle", 2, 6, JoystickCache::RISING);
	}else{
		robot.joystick.register_button("toggle", 2 ,12, JoystickCache::RISING);
	}
}
void CageSubsystem::teleop(void){
	 if (robot.joystick.button("toggle")){
		 if (isOut){
			 leftWing.Set(DoubleSolenoid::kReverse);
			 rightWing.Set(DoubleSolenoid::kReverse);
			 isOut = false;
		 }else{
			 leftWing.Set(DoubleSolenoid::kForward);
			 rightWing.Set(DoubleSolenoid::kForward); 
			 isOut = true;
		 }
	 }

}
	
void CageSubsystem::cylinderOut(void){
	leftWing.Set(DoubleSolenoid::kReverse);
	rightWing.Set(DoubleSolenoid::kReverse);
}

void CageSubsystem::cylinderIn(void){
	leftWing.Set(DoubleSolenoid::kForward);
	rightWing.Set(DoubleSolenoid::kForward);
}
