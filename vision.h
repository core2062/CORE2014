#ifndef VISION_H
#define VISION_H

#include "WPILib.h"
#include "CORERobot/CORERobot.h"

namespace CORE{
	bool visionMain();
	void TestSubtraction();
	
	class HotAction : public Action{
		bool hot;
		DriverStation* ds;
	public:
		HotAction(){
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
		virtual ~HotAction(void){};
	};
}

#endif
