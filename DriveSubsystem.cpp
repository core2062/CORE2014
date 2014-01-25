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
	robot.joystick.register_axis("left", 1, 2);
	robot.joystick.register_axis("right", 1, 3);
	robot.joystick.register_button("quickturn", 1, 6);
	
	SmartDashboard::PutNumber("ether-a", .75);
	SmartDashboard::PutNumber("ether-b", .4);
	
	SmartDashboard::PutNumber("culver-radius-gain", 1.2);
	SmartDashboard::PutNumber("culver-raw-gain", 1.5);
	
	std::string* culver = new std::string("culver");
	std::string* tank = new std::string("tank");
	std::string* ether = new std::string("ether");
	
	driveChooser.AddDefault("ether", ether);
	driveChooser.AddDefault("culver", culver);
	driveChooser.AddDefault("tank", tank);
	SmartDashboard::PutData("drive-chooser", &driveChooser);
}
void DriveSubsystem::teleop(void){
	
	if(robot.joystick.button("quickturn") & !quickturn_old){
		quickturn = !quickturn;
	}
	quickturn_old = robot.joystick.button("quickturn");
	
	std::string choice = * (std::string*) driveChooser.GetSelected();
	
	if(choice == "culver") {
		drive.CulverDrive(robot.joystick.axis("mag"), robot.joystick.axis("steer-x"), robot.joystick.axis("steer-y"), quickturn, 
			SmartDashboard::GetNumber("culver-radius-gain"), SmartDashboard::GetNumber("culver-raw-gain"));
	} else if (choice == "tank") {
		drive.TankDrive(robot.joystick.axis("left"), robot.joystick.axis("right"), true);
	} else {
		drive.EtherArcade(robot.joystick.axis("mag"), robot.joystick.axis("kaj-rot"), 
			SmartDashboard::GetNumber("ether-b"), SmartDashboard::GetNumber("ether-a"));
	}
}
	
