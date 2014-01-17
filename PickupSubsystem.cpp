#include "PickupSubsystem.h"

void PickupSubsystem::robotInit(void){
	robot.requirePneumatics();
	
}
void PickupSubsystem::teleopInit(void){
	robot.compressor->Start();
	
}
void PickupSubsystem::teleop(void){
	
}
	
