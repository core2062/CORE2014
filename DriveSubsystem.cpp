#include "DriveSubsystem.h"

void DriveSubsystem::robotInit(void){
	
}
void DriveSubsystem::teleopInit(void){
	robot.joystick.register_axis("mag", 1, 2);
	robot.joystick.register_axis("kaj-rot", 1, 4);
	
	SmartDashboard::PutNumber("ehter-a", .75);
	SmartDashboard::PutNumber("ehter-b", .4);
}
void DriveSubsystem::teleop(void){
	double a = SmartDashboard::GetNumber("ether-a");
	double b = SmartDashboard::GetNumber("ether-b");
	
	drive.EtherArcade(robot.joystick.axis("mag"), robot.joystick.axis("kaj-rot"), a, b);
}
	
