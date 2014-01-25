#include "DriveSubsystem.h"

void DriveSubsystem::robotInit(void){
//	robot.requirePneumatics();
}
void DriveSubsystem::teleopInit(void){
	robot.compressor->Start();
	
	robot.joystick.register_axis("mag", 1, 2);
	robot.joystick.register_axis("kaj-rot", 1, 4);
	robot.joystick.register_axis("steer-x", 1, 4);
	robot.joystick.register_axis("steer-y", 1, 5);
	robot.joystick.register_axis("left", 1, 2);
	robot.joystick.register_axis("right", 1, 5);
	robot.joystick.register_button("quickturn", 1, 5);
	robot.joystick.register_axis("quickturn", 1, 3);
	robot.joystick.register_button("shift", 1, 6);
	
	SmartDashboard::PutNumber("ether-a", .75);
	SmartDashboard::PutNumber("ether-b", .4);
	
	SmartDashboard::PutNumber("culver-radius-gain", 1.2);
	SmartDashboard::PutNumber("culver-raw-gain", 1.5);
	
//	SmartDashboard::PutNumber("culver-x", 0);
//	SmartDashboard::PutNumber("culver-y", 0);
//	SmartDashboard::PutNumber("culver-r", 0);
//	SmartDashboard::PutNumber("culver-theta", 0);
//	SmartDashboard::PutNumber("culver-raw", 0);
//	SmartDashboard::PutNumber("culver-radius", 0);
	
	std::string* culver = new std::string("culver");
	std::string* tank = new std::string("tank");
	std::string* ether = new std::string("ether");
	
	driveChooser.AddDefault("ether", ether);
	driveChooser.AddDefault("culver", culver);
	driveChooser.AddDefault("tank", tank);
	SmartDashboard::PutData("drive-chooser", &driveChooser);
}

#include <math.h>
float deadband(float n){
	if(fabs(n) < .1){
		return 0;
	}
	return n;
}

void DriveSubsystem::teleop(void){
	
	if(robot.joystick.button("quickturn") & !quickturn_old){
		quickturn = !quickturn;
	}
	quickturn_old = robot.joystick.button("quickturn");
	if(robot.joystick.axis("quickturn") > .75) { 
		quickturn = true;
	} else if (robot.joystick.axis("quickturn") < .1){
		quickturn = quickturn;
	} else {
		quickturn = false;
	}
	
	std::string choice = * (std::string*) driveChooser.GetSelected();
	
	if(choice == "culver") {
		drive.CulverDrive(deadband(robot.joystick.axis("mag")), deadband(robot.joystick.axis("steer-x")), 
			deadband(robot.joystick.axis("steer-y")), quickturn, 
			SmartDashboard::GetNumber("culver-radius-gain"), SmartDashboard::GetNumber("culver-raw-gain"));
	} else if (choice == "tank") {
		drive.TankDrive(deadband(robot.joystick.axis("left")), deadband(robot.joystick.axis("right")), true);
	} else {
		drive.EtherArcade(deadband(robot.joystick.axis("mag")), deadband(robot.joystick.axis("kaj-rot")), 
			SmartDashboard::GetNumber("ether-b"), SmartDashboard::GetNumber("ether-a"));
	}
}
