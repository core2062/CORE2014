#include "DriveSubsystem.h"

void DriveSubsystem::robotInit(void){
//	robot.requirePneumatics();
}
void DriveSubsystem::teleopInit(void){
	robot.compressor->Start();
	
	robot.joystick.register_axis("mag", 1, 2);
	robot.joystick.register_axis("kaj-rot", 1, 4);
	robot.joystick.register_axis("steer-x", 1, 4);
	robot.joystick.register_axis("steer-y", 1, 3);
	robot.joystick.register_button("quickturn", 1, 6);
	
	SmartDashboard::PutNumber("ether-a", .75);
	SmartDashboard::PutNumber("ether-b", .4);
	
	SmartDashboard::PutNumber("gain-radius", 1.2);
	SmartDashboard::PutNumber("gain-raw", 1.5);
	
	SmartDashboard::PutBoolean("use-culver", false);
}
void DriveSubsystem::teleop(void){
	double a = SmartDashboard::GetNumber("ether-a");
	double b = SmartDashboard::GetNumber("ether-b");
	
	if(robot.joystick.button("quickturn") & !quickturn_old){
		quickturn = !quickturn;
	}
	quickturn_old = robot.joystick.button("quickturn");
	
	if(SmartDashboard::GetBoolean("use-culver")){
		drive.CulverDrive(robot.joystick.axis("mag"), robot.joystick.axis("steer-x"),robot.joystick.axis("steer-y"),
				quickturn, 
				SmartDashboard::GetNumber("gain-radius"), SmartDashboard::GetNumber("gain-raw"));
	} else {
		drive.EtherArcade(robot.joystick.axis("mag"), robot.joystick.axis("kaj-rot"), a, b);
	}
}
	
