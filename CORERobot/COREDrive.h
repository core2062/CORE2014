#ifndef COREDRIVE_H
#define COREDRIVE_H

#include "WPILib.h"

namespace CORE {

class DoubleSpeed : public SpeedController {
public:
	SpeedController& one;
	SpeedController& two;
	DoubleSpeed(SpeedController& one, SpeedController& two):
		one(one),
		two(two)
	{}
	
	virtual void Set(float speed, uint8_t syncGroup=0){
		one.Set(speed, syncGroup);
		two.Set(speed, syncGroup);
	}
	
	virtual float Get(){
		float n = 0;
		n += one.Get();
		n += two.Get();
		return n/2;
	}
	
	virtual void Disable(){
		one.Disable();
		two.Disable();
	}
	
	virtual void PIDWrite(float output){
		Set(output);
	}

};

class COREDrive : public RobotDrive {
public:
	COREDrive::COREDrive(SpeedController &leftMotor, SpeedController &rightMotor):
			RobotDrive(leftMotor, rightMotor){
			
			}
	
	void EtherArcade(double mag, double rotate, double a, double b);
	void ArcadeDrive(float moveValue, float rotateValue, bool squaredInputs = false);
};

class CORERateLimiter{
	float increment;
	float old;
public:

	CORERateLimiter(float increment){
		old = 0;
		increment = increment;
	}
	
	float limit(float input){
		float diff = input - old;
		
		if(diff < 0){
			if(diff < -increment){
				old -= increment;	
			}else{
				old = input;
			}
		} else if( diff > 0){
			if(diff > increment){
				old += increment;
			} else {
				old = input;
			}
		}
		return old;
	}
};

}
#endif
