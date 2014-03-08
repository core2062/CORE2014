#include "CageSubsystem.h"

void CageSubsystem::robotInit(void){
	robot.requirePneumatics();
	SmartDashboard::PutNumber("auto-cage-delay", 0.2);
}
void CageSubsystem::teleopInit(void){
	robot.compressor->Start();
	robot.joystick.register_button("toggle", 2 ,12, JoystickCache::RISING);
	leftWing.Set(DoubleSolenoid::kForward);
	rightWing.Set(DoubleSolenoid::kForward);
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
