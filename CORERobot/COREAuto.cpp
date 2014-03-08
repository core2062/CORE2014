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
	background(),
	empty_flag(false){
}

void AutoSequencer::clear(void){
	aqueue.c.clear();
	background.clear();
}

void AutoSequencer::add_action(Action& action){
	add_action(&action);
}

void AutoSequencer::add_action(Action* action){
//	cout<<"adding action"<<endl;
//	cout<<action<<endl;
	aqueue.push(action);
}

void AutoSequencer::init(void){
	aqueue.front()->init();
}

void AutoSequencer::iter(void){
//	cout<<"iter start"<<endl;
	if(aqueue.empty()){
		if(!empty_flag){
			cout << "No remaining auto actions!"<<endl;
			empty_flag = true;
		}
		return;
	}
	Action* a = aqueue.front();
//	cout<<"current action: "<<a<<endl;
	Action::ControlFlow return_val = a->call();
	
//	cout<<"after action"<<endl;
	switch(return_val){
	case Action::CONTINUE:
		break;
	case Action::BACKGROUND:
		cout << "adding to background: " << a << endl;
		background.push_back(a);
	case Action::END:
		aqueue.pop();
		cout << "new task: " << aqueue.front() << endl;
		aqueue.front()->init();
		break;
	}
	std::vector<Action*>::iterator it = background.begin();
	while(it != background.end()){
		cout << "  bkgrnd: " << *it << endl;
		Action::ControlFlow return_val = (*it)->call();
		if (return_val == Action::END){
			it = background.erase(it);
		} else {
			++it;
		}
	}
//	cout<<"after iter"<<endl;
}
