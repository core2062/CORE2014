#include "WPILib.h" 
#include "CORERobot/CORERobot.h"
#include "Subsystems.h"

using namespace CORE;


class CORE2014 : public SimpleRobot {
	
	CORERobot robot;
	
//	DriveSubsystem drive;
//	CageSubsystem cage;
//	ShooterSubsystem shooter;
//	PickupSubsystem pickup;
	
public:
	CORE2014():
		robot()
//		drive(robot),
//		cage(robot),
//		shooter(robot)
//		pickup(robot)
	{
//		robot.add(drive);
//		robot.add(shooter); 
//		robot.add(pickup);
//		robot.add(cage);
	}

	void RobotInit(){
		robot.robotInit();
	}
	
	PixelValue pixVal(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
		PixelValue ret;
		ret.grayscale = 0;
		RGBValue rgb = {
			(unsigned char) b,
			(unsigned char) g,
			(unsigned char) r,
			(unsigned char) a
		};
		ret.rgb = rgb;
		cout << (int)ret.rgb.R << " " << (int)ret.rgb.G << " " << (int)ret.rgb.B << endl;
		return ret;
	}
	
#define chk(input) {if(!(input)) {goto error;}}
#define log(image) {cout<<"writing "<<step<<endl; image.Write(("/."+step+".jpg").c_str());}
	
	void Autonomous(){
		cout << "starting autonomous" << endl;
//		AxisCamera& camera = AxisCamera::GetInstance();
		ColorImage image (IMAQ_IMAGE_RGB);
		ColorImage output (IMAQ_IMAGE_RGB);
//		camera.GetImage(&image);
		Image* imaqImage = image.GetImaqImage();
		Image* imaqOutput = output.GetImaqImage();
		
		std::string step = "start";
		chk(imaqReadFile(imaqImage, "/Center.jpg", NULL, NULL));
		chk(imaqReadFile(imaqOutput, "/Center.jpg", NULL, NULL));
		log(image);
		
		PixelValue darkening_cnst;
		
		step = "red";
		darkening_cnst = pixVal(255, 0, 0, 0);
		chk(imaqSubtractConstant(imaqImage, imaqOutput, darkening_cnst));
		log(image);
		
		step = "green";
		darkening_cnst = pixVal(0, 255, 0, 0);
		chk(imaqSubtractConstant(imaqImage, imaqOutput, darkening_cnst));
		log(image);
		
		step = "blue";
		darkening_cnst = pixVal(0, 0, 255, 0);
		chk(imaqSubtractConstant(imaqImage, imaqOutput, darkening_cnst));
		log(image);
		
		step = "alpha";
		darkening_cnst = pixVal(0, 0, 0, 255);
		chk(imaqSubtractConstant(imaqImage, imaqOutput, darkening_cnst));
		log(image);
		return;
		step = "thresh";
		BinaryImage* thresholdImage = image.ThresholdRGB(0, 23, 22, 50, 0, 43);
		log((*thresholdImage));
		
		step = "filter";
		Image* imaqThresh = thresholdImage->GetImaqImage();
		{
			ParticleFilterCriteria2 criteria = {
					IMAQ_MT_AREA,
					150,
					65535,
					false,
					true
			};
			int numParticles;
			ParticleFilterOptions2 filterOptions = {0, 0, 0, 1};
			chk(imaqParticleFilter4(imaqThresh, imaqThresh, &criteria, 1,
					&filterOptions, NULL, &numParticles));
		}
		log((*thresholdImage));
		
		error:
			delete thresholdImage;
			cout << "Error in step " << step << endl;
	}


	void OperatorControl()
	{
		robot.teleopInit();
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

