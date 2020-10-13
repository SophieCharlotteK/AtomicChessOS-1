#include <iostream>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


#include "SHARED/guicommunicator/guicommunicator.h"
#include "SHARED/inih-master/INIReader.h"
#include "SHARED/loguru-master/loguru.hpp"

#include "TMC5160.h"
#include "BackendConnector.h"
using namespace std;

//!Reads the system HWID File from the location
//std::string readHWID(std::string _file)
//{
//	std::ifstream t(_file.c_str());
//	std::string str;

//	t.seekg(0, std::ios::end);   
//	str.reserve(t.tellg());
//	t.seekg(0, std::ios::beg);

//	str.assign((std::istreambuf_iterator<char>(t)),
//		std::istreambuf_iterator<char>());
//}


int mainloop_running = 0;
void signal_callback_handler(int signum)
{
	printf("Caught signal %d\n", signum);
	mainloop_running = signum;
	//exit(signum);
}

/*
#include <cstdint>
#include <fstream>
#include <streambuf>
#include <regex>

using namespace std;

uint64_t getIFMAC(const string &ifname) {
  ifstream iface("/sys/class/net/" + ifname + "/address");
  string str((istreambuf_iterator<char>(iface)), istreambuf_iterator<char>());
  if (str.length() > 0) {
    string hex = regex_replace(str, std::regex(":"), "");
    return stoull(hex, 0, 16);
  } else {
    return 0;
  }
} 
int main()
{
  string iface = "eth0";
  printf("%s: mac=%016llX\n", iface.c_str(), getIFMAC(iface));
}
*/


int main(int argc, char *argv[])
{
	//REGISTER SIGNAL HANDLER
	signal(SIGINT, signal_callback_handler);
	//SETUP LOGGER
	loguru::init(argc, argv);
	loguru::add_file("./log.log", loguru::Append, loguru::Verbosity_MAX);
	loguru::g_stderr_verbosity = 1;
	LOG_SCOPE_F(INFO, "ATC CONTROLLER STARTED");
	//READ CONFIG FILE
	LOG_SCOPE_F(INFO, "LOADING CONFIG FILE ./atccontrollerconfig.ini");
	INIReader config("./atccontrollerconfig.ini");
	if (config.ParseError() != 0)
	{
		LOG_F(ERROR, "Failed to load atccontrollerconfig.ini");
		return 1;
	}
	LOG_F(INFO, config.Get("NetworkSettings", "ATCBackendURL","http://marcelochsendorf.com:3001").c_str());	
		
	
	//TMC5160 motorA = TMC5160(TMC5160::MOTOR_ID::MOTOR_0);
	
	
	//motorA.default_settings();
	
	
	
	
	//LOG_F(INFO, "guicommunicator startig ipc thread");
	guicommunicator gui;
	gui.start_recieve_thread();
	//CHECK VERSION ON GUI SIDE
	if (gui.check_guicommunicator_version())
	{
		LOG_F(WARNING, "guicommunicator version check failed");
	}
	//CREATE GAME BACKEND INSTANCE
	BackendConnector gamebackend(config.Get("NetworkSettings", "ATCBackendURL", "http://marcelochsendorf.com:3001"), config.Get("NetworkSettings", "OutgoingInterfaceName", "eth0"), "");
	
	
	if (gamebackend.check_connection())
	{
		LOG_F(ERROR, "gamebackend - check connection failed");
		gui.show_error_message_on_gui("Cant connect to game server. (ERR01) [" + config.Get("NetworkSettings", "ATCBackendURL", "http://marcelochsendorf.com:3001") + "]");
		return 2;
	}
	
	
	
	if (gamebackend.check_login_state())
	{
		LOG_F(ERROR, "gamebackend - check loginstate - user already signed in");
		//PERFORM LOGOUT
		if(gamebackend.logout())
		{
			gui.createEvent(guicommunicator::GUI_ELEMENT::SWITCH_MENU, guicommunicator::GUI_VALUE_TYPE::LOGIN_SCREEN, "");
			LOG_F(ERROR, "gamebackend - LOGIN");	
		}else
		{
			LOG_F(ERROR, "gamebackend - logout failed");
			gui.show_error_message_on_gui("LOGOUT FAILED");
			return 3;
		}
		//SHOW LOGIN SCREEN
		
		//gui.show_error_message_on_gui("Cant connect to game server. (ERR01) [" + config.Get("NetworkSettings", "ATCBackendURL", "http://marcelochsendorf.com:3001") + "]");
	}
	else
	{
		gui.createEvent(guicommunicator::GUI_ELEMENT::SWITCH_MENU, guicommunicator::GUI_VALUE_TYPE::LOGIN_SCREEN, "");
	}
	
	
	
	
	//INIT SPI
	//INIT FILED
	
	//LOGOUT
	
	//ENTERING MIAN LOOP
	while (mainloop_running == 0)
	{
		guicommunicator::GUI_EVENT ev = gui.get_gui_update_event();
		if (ev.is_event_valid)
		{
			gui.debug_event(ev,true);	
		}
		
		
	}
	
	return 0;
}
