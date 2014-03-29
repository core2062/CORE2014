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
			Timer vision_timer;
			vision_timer.Start();
			hot = visionMain();
			vision_timer.Stop();
			cout << "vision took " << vision_timer.Get() << endl;
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
			return END;
		}
	};
	
	class WaitIfAction : public Action{
		bool* flag;
		DriverStation* ds;
	public:
		WaitIfAction(bool* flag):
			flag(flag)
		{
			ds = DriverStation::GetInstance();
		}
		void init(void){
			cout << "waitAction" << endl;
		}
		virtual ControlFlow call(void){
			if(*flag){
				cout << "no wait for mid" << endl;
				return END;
			}
			if(ds->GetMatchTime() < 5.5 ){
				cout << "wait for mid" << endl;
				return CONTINUE;
			}
			return END;
		}
	};
}

#endif
