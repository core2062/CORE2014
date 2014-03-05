#ifndef VISION_H
#define VISION_H

#include "WPILib.h"
#include "CORERobot/CORERobot.h"

namespace CORE{
	bool visionMain();
	void TestSubtraction();
	
	class VisionAction : public Action{
		bool* right_hot;
	public:
		VisionAction(bool* right_hot):
		right_hot(right_hot){
			
		}
		void init(void){
			
		}
		virtual ControlFlow call(void){
			(*right_hot) = visionMain();
			return END;
		}
		virtual ~VisionAction(void){};
	};
	class VisionWaitAction : public Action{
		bool hot;
		DriverStation* ds;
	public:
		VisionWaitAction(){
			hot = false;
			ds = DriverStation::GetInstance();
		}
		void init(void){
			hot = visionMain();
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
		virtual ~VisionWaitAction(void){};
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
