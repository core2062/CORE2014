#include "COREAuto.h"
#include "WPILib.h"
using namespace CORE;

Action::Action(void){};

WaitAction::WaitAction(float duration):
	timer(){
	m_duration	= duration;
}

Action::ControlFlow WaitAction::call(void){
	if(timer.Get() == 0){
		timer.Start();
	}
	if(timer.Get()<=m_duration){
		return CONTINUE;
	}else{
		return END;
	}
}

AutoSequencer::AutoSequencer():
	aqueue(),
	background(){
}

void AutoSequencer::add_action(Action& action){
	add_action(&action);
}

void AutoSequencer::add_action(Action* action){
//	cout<<"adding action"<<endl;
//	cout<<action<<endl;
	aqueue.push(action);
}

void AutoSequencer::iter(void){
//	cout<<"iter start"<<endl;
	if(aqueue.empty()){
		cout << "No remaining auto actions!"<<endl;	
		return;
	}
	Action* a = aqueue.front();
	cout <<a<<endl;
	Action::ControlFlow return_val = a->call();
	
//	cout<<"after action"<<endl;
	switch(return_val){
	case Action::CONTINUE:
//		cout<<" in countinue"<<endl;
		break;
	case Action::BACKGROUND:
//		cout<<"in background"<<endl;
		background.push_back(a);
		aqueue.pop();
		break;
	case Action::END:
//		cout<<"in end"<<endl;
		aqueue.pop();
		break;
	}
//	cout<<"before backgrond loop"<<endl;
	for(std::vector<Action*>::iterator it = background.begin(); it != background.end(); ++it){
		Action::ControlFlow return_val = (*it)->call();
		if (return_val == Action::END){
			background.erase(it);
		}
	}
//	cout<<"after iter"<<endl;
}
