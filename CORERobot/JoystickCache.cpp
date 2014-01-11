
#include <string>
#include <map>

#include "WPILib.h"
#include "JoystickCache.h"

using namespace CORE;

void JoystickCache::register_axis (std::string name, int joystick, int axis){
	axes.insert(j_map::value_type(name, j_key(joystick, axis)));
}

void JoystickCache::register_button(std::string name, int joystick, int button){
	buttons.insert(j_map::value_type(name, j_key(joystick, button)));
}

void JoystickCache::update_cache(void){
	j_map::iterator it;
	cached_axes.clear();
	cached_button.clear();
	for(it = axes.begin(); it != axes.end(); ++it){
//		printf("name: %s\n", it->first.c_str());
		cached_axes.insert(d_cache::value_type(it->first, get_joystick(it->second.joystick).GetRawAxis(it->second.channel))); 
	}
	for(it = buttons.begin(); it != buttons.end(); ++it){
//		printf("name: %s\n", it->first.c_str());
		cached_button.insert(d_cache::value_type(it->first, get_joystick(it->second.joystick).GetRawButton(it->second.channel))); 
	}
}

double JoystickCache::axis(std::string name){
	return cached_axes[name];
}

bool JoystickCache::button(std::string name){
	return cached_button[name];
}

Joystick& JoystickCache::get_joystick(int axis){
	if(axis == 1){
		return joystick1;
	} else {
		return joystick2;
	}
}
