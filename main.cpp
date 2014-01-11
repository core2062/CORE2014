#include "WPILib.h" 
#include "CORERobot/CORERobot.h"
#include "Subsystems.h"

using namespace CORE;


class CORE2014 : public SimpleRobot {
	
	CORERobot robot;
	
	DriveSubsystem drive;
	CageSubsystem cage;
	ShooterSubsystem shooter;
	PickupSubsystem pickup;
	
	
public:
	CORE2014():
		robot(),
		drive(robot),
		cage(robot),
		shooter(robot),
		pickup(robot)
	{
		robot.add(drive);
		robot.add(shooter);
		robot.add(pickup);
		robot.add(cage);
	}

	void RobotInit(){
		robot.robotInit();
	}
	
	void Autonomous(){
		
	}


	void OperatorControl()
	{
		
		while (IsOperatorControl() && !IsDisabled())
		{
			robot.teleop();
			Wait(0.005);				// wait for a motor update time
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
};

START_ROBOT_CLASS(CORE2014);

