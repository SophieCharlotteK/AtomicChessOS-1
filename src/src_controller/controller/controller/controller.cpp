#include <iostream>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <cstdint>
#include <fstream>
#include <streambuf>
#include <regex>


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


std::string get_interface_mac_address(const string& _ifname) {
	ifstream iface("/sys/class/net/" + _ifname + "/address");
	std::string str((istreambuf_iterator<char>(iface)), istreambuf_iterator<char>());
	if (str.length() > 0) {
		std::string hex = regex_replace(str, std::regex(":"), "");
		return hex;
	}
	else {
		return "1nv4l1dm4c";
	}
} 

std::string read_file_to_string(const std::string& _path) {
	ifstream iface(_path);
	std::string str((istreambuf_iterator<char>(iface)), istreambuf_iterator<char>());
	if (str.length() > 0) {
		return str;
	}
	else {
		return "";
	}
} 


int main(int argc, char *argv[])
{
	
	string iface = "eth0";
	
	
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
	LOG_F(INFO, config.Get("NetworkSettings", "ATCBackendURL", "http://marcelochsendorf.com:3001").c_str());	
		
	
	//TMC5160 motorA = TMC5160(TMC5160::MOTOR_ID::MOTOR_0);
	
	
	//motorA.default_settings();
	
	
	
	
	LOG_F(INFO, "guicommunicator startig ipc thread");
	guicommunicator gui;
	gui.start_recieve_thread();
	//CHECK VERSION ON GUI SIDE
	if(gui.check_guicommunicator_version())
	{
		LOG_F(WARNING, "guicommunicator version check failed");
	}
	//DETERM THE HWID BY USING THE MAC ADDRESS OF THE OUTGOING INTERNFACE NAME
	std::string hwid = get_interface_mac_address(config.Get("GeneralSettings", "HWIDInterface", "eth0"));
	gui.createEvent(guicommunicator::GUI_ELEMENT::INFOSCREEN_HWID_LABEL, guicommunicator::GUI_VALUE_TYPE::USER_INPUT_STRING, hwid);
	LOG_F(INFO, (const char*)hwid.c_str());
	
	
	//DISPLAY FIRMARE VERSION NUMBER
	std::string fwver = read_file_to_string(config.Get("GeneralSettings", "VersionFile", "/VERSION"));
	std::string hwrev = read_file_to_string(config.Get("GeneralSettings", "HWRevFile", "/etc/hwrevision"));
	std::string bootpart = read_file_to_string(config.Get("GeneralSettings", "RunningPartition", "/etc/swupdate/BOOTPART"));
	gui.createEvent(guicommunicator::GUI_ELEMENT::INFOSCREEN_VERSION, guicommunicator::GUI_VALUE_TYPE::USER_INPUT_STRING, fwver + "|" + hwrev + "|" + bootpart);
	LOG_F(INFO, (const char*)fwver.c_str());
	
	
	//CREATE GAME BACKEND INSTANCE
	BackendConnector gamebackend(config.Get("NetworkSettings", "ATCBackendURL", "http://marcelochsendorf.com:3001"), config.Get("NetworkSettings", "OutgoingInterfaceName", "eth0"), hwid);
	
	//CHECK IF GAMESERVER IS REACHABLE
	if(gamebackend.check_connection())
	{
		gui.createEvent(guicommunicator::GUI_ELEMENT::NETWORK_STATUS, guicommunicator::GUI_VALUE_TYPE::ONLINE);	
	}
	else
	{
		gui.createEvent(guicommunicator::GUI_ELEMENT::NETWORK_STATUS, guicommunicator::GUI_VALUE_TYPE::OFFLINE);
		LOG_F(ERROR, "gamebackend - check connection failed");
		gui.show_error_message_on_gui("Cant connect to game server. (ERR01) [" + config.Get("NetworkSettings", "ATCBackendURL", "http://marcelochsendorf.com:3001") + "]");
		return 2;
	}
	
	
	//CHECK IF LOGIN IS VALID AND AN INVALID SESSION ID EXISTS
	//THEN TRY TO LOGOUT
	//IF BOTH VALID WHOE THE MAIN MENU
	if(gamebackend.check_login_state() && gamebackend.get_session_id().empty())
	{
		LOG_F(ERROR, "gamebackend - check loginstate - user already signed in");
		//PERFORM LOGOUT
		if(gamebackend.logout())
		{
			gui.createEvent(guicommunicator::GUI_ELEMENT::SWITCH_MENU, guicommunicator::GUI_VALUE_TYPE::LOGIN_SCREEN);
			LOG_F(ERROR, "gamebackend - LOGIN");	
		}else
		{
			LOG_F(ERROR, "gamebackend - logout failed");
			gui.show_error_message_on_gui("LOGOUT FAILED");

		}
	}
	else if(gamebackend.check_login_state() && !gamebackend.get_session_id().empty())
	{
		//SHOW MAIN MENU
		gui.createEvent(guicommunicator::GUI_ELEMENT::SWITCH_MENU, guicommunicator::GUI_VALUE_TYPE::MAIN_MENU_SCREEN);
		//UPDATE SESSION_ID
		gui.createEvent(guicommunicator::GUI_ELEMENT::INFOSCREEN_SESSIONID_LABEL, guicommunicator::GUI_VALUE_TYPE::USER_INPUT_STRING, gamebackend.get_session_id());
		
	}else
	{
		gui.createEvent(guicommunicator::GUI_ELEMENT::SWITCH_MENU, guicommunicator::GUI_VALUE_TYPE::LOGIN_SCREEN);
	}
	
	
	
	

	
	//ENTERING MIAN LOOP
	while(mainloop_running == 0)
	{
		//HANDLE UI EVENTS
		guicommunicator::GUI_EVENT ev = gui.get_gui_update_event();
		if (ev.is_event_valid)
		{
			gui.debug_event(ev, true);	
			
			//USER PRESSED LOGOUT BUTTON
			if (ev.event == guicommunicator::GUI_ELEMENT::LOGOUT_BTN && ev.type == guicommunicator::GUI_VALUE_TYPE::CLICKED)
			{
				gamebackend.logout();
				gui.createEvent(guicommunicator::GUI_ELEMENT::SWITCH_MENU, guicommunicator::GUI_VALUE_TYPE::LOGIN_SCREEN);
				//TODO RESET ALL STUFF sessid, game field,...
			}
			
			//USER PRESSED LOGIN BUTTON
			if (ev.event == guicommunicator::GUI_ELEMENT::BEGIN_BTN && ev.type == guicommunicator::GUI_VALUE_TYPE::CLICKED)
			{
				if (gamebackend.login())
				{
					//SWITCH TO MAIN MENU
					gui.createEvent(guicommunicator::GUI_ELEMENT::SWITCH_MENU, guicommunicator::GUI_VALUE_TYPE::MAIN_MENU_SCREEN);	
					//UPDATE SESSION_ID
					gui.createEvent(guicommunicator::GUI_ELEMENT::INFOSCREEN_SESSIONID_LABEL, guicommunicator::GUI_VALUE_TYPE::USER_INPUT_STRING, gamebackend.get_session_id());
				}
				else
				{
					gui.show_error_message_on_gui(gamebackend.get_last_error());
				}
			}
		}
		
		//HANDLE GAME STATE 
		if(ev.event == guicommunicator::GUI_ELEMENT::INIT_BTN && ev.type == guicommunicator::GUI_VALUE_TYPE::CLICKED)
		{
//			motorA.goto_position(1203);
		}
		
		//HANDLE BACKEND EVETNS
		//CHECK CONNECTION IN INTERVAL
		//CHECK LOGINSTATE IN INTERVAL
		
		
		
	}
	
	return 0;
}
