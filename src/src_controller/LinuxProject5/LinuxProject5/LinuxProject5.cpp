#include <iostream>
#include "TMC5160.h"
#include <chrono>
#include <thread>
#include "SHARED/WiringPi-master/wiringPi/wiringPi.h"
using namespace std::this_thread;   // sleep_for, sleep_until
using namespace std::chrono;   // nanoseconds, system_clock, seconds
int main(int argc, char *argv[])
{

	TMC5160 motor =   TMC5160(TMC5160::MOTOR_ID::MOTOR_0);
	//motor
	motor.default_settings();
	motor.steps_per_mm(1140);
	motor.enable_motor();
	motor.stop_motor();
	//QUET SETTINGS FOR ATC
	//motor.move_to_postion_mm_absolute(100, true);
	motor.atc_home_sync();
	
	//PRESET MOVE FIGURE
motor .enable_switch(TMC5160::REF_SWITCH::REF_L, true, true, true);  //ENABLE LIMIT SWICHT
	motor.atc_set_speed_preset(TMC5160::TRAVEL_SPEED_PRESET::TRAVEL);
	motor.move_to_postion_mm_absolute(300, true);
	
	

	motor.atc_set_speed_preset(TMC5160::TRAVEL_SPEED_PRESET::MOVE);
	motor.move_to_postion_mm_absolute(50, true);
	
	std::cout << "SIG" << std::endl;
//	motor.hold_mode();
	//motor.wait_for_target_position_reached();
	motor.stop_motor();
	motor.disable_motor();

}