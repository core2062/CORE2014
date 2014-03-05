#ifndef VISION_H
#define VISION_H

#include "WPILib.h"
#include "CORERobot/CORERobot.h"

namespace CORE{
	bool visionMain();
	void TestSubtraction();
	
	class VisionAction : public Action{
		bool hot;
		DriverStation* ds;
	public:
		VisionAction(){
			hot = false;
			ds = DriverStation::GetInstance();
		}
		void init(void){
//			hot = visionMain();
			hot = SmartDashboard::GetBoolean("hot-debug");
			if(hot) {
				cout << "hot, no wait" <<endl;
			} else {
				cout << "not hot, waiting 5 sec." << endl;
			}
		}
		virtual ControlFlow call(void){
			if(!hot and ds->GetMatchTime() < 5 ){
				return CONTINUE;
			}
			cout << "wait over" << endl;
			return END;
		}
		virtual ~VisionAction(void){};
	};
	class WaitMidpointAction : public Action{
		DriverStation* ds;
	public:
		WaitMidpointAction(){
			ds = DriverStation::GetInstance();
		}
		void init(void){
		}
		virtual ControlFlow call(void){
			if(ds->GetMatchTime() < 5 ){
				return CONTINUE;
			}
//			cout << "wait over" << endl;
			return END;
		}
	};
}

#endif
