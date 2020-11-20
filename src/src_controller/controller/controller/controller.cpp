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

#include "SHARED/loguru-master/loguru.hpp"

#include "TMC5160.h"
#include "BackendConnector.h"
#include "ChessPiece.h"
#include "ConfigParser.h"
#include "ChessBoard.h"
#include "IOController.h"



//---------------------- CONFIG DEFINED --------------------------- //
#define CONFIG_FILE_PATH "./atccontrollerconfig.ini"


using namespace std;

//!Reads the system HWID File from the location
std::string readHWID(std::string _file)
{
	std::ifstream t(_file.c_str());
	std::string str;

	t.seekg(0, std::ios::end);   
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
}


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
	
	
	
	//REGISTER SIGNAL HANDLER
	signal(SIGINT, signal_callback_handler);
	
	//SETUP LOGGER
	loguru::init(argc, argv);
	loguru::add_file("./log.log", loguru::Append, loguru::Verbosity_MAX);
	loguru::g_stderr_verbosity = 1;
	LOG_SCOPE_F(INFO, "ATC CONTROLLER STARTED");
	
	//READ CONFIG FILE
	LOG_SCOPE_F(INFO, "LOADING CONFIG FILE ./atccontrollerconfig.ini");
	ConfigParser::getInstance()->createConfigFile(CONFIG_FILE_PATH, false);
	if (!ConfigParser::getInstance()->loadConfigFile(CONFIG_FILE_PATH))
	{
		LOG_F(ERROR, "Failed to load atccontrollerconfig.ini");
		return 1;
	}
	LOG_F(INFO, "CONFIG FILE LOADED");	
	
	
	
		
	//SARTING GUI COMMUNICATOR PROCESS
	LOG_F(INFO, "guicommunicator startig ipc thread");
	guicommunicator gui;
	gui.start_recieve_thread();
	
	//WAIT FOR GUI TO BECOME REACHABLE
	int gui_wait_counter = 0;
	//TRY X TIMES BEFORE RETURN AN ERROR
	const int GUI_WAIT_COUNTER_MAX = 100;
	while (!gui.check_guicommunicator_reachable())
	{
		gui_wait_counter++;
		if (gui_wait_counter > GUI_WAIT_COUNTER_MAX)
		{
			break;
		}
	}
	//
	if(gui_wait_counter > GUI_WAIT_COUNTER_MAX)
	{
		LOG_F(WARNING, "guicommunicator check_guicommunicator_reachable check failed");
		return 2;
	}
	
	
	gui.createEvent(guicommunicator::GUI_ELEMENT::SWITCH_MENU, guicommunicator::GUI_VALUE_TYPE::PROCESSING_SCREEN);
	IOController io;
	io.setTurnStateLight(IOController::TURN_STATE_LIGHT::TSL_PRECCESSING);
	//CHECK VERSION ON GUI SIDE
	if(gui.check_guicommunicator_version())
	{
		LOG_F(WARNING, "guicommunicator version check failed");
	}
	//DETERM THE HWID BY USING THE MAC ADDRESS OF THE OUTGOING INTERNFACE NAME
	std::string hwid = get_interface_mac_address(ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::GENERAL_HWID_INTERFACE));
	gui.createEvent(guicommunicator::GUI_ELEMENT::INFOSCREEN_HWID_LABEL, guicommunicator::GUI_VALUE_TYPE::USER_INPUT_STRING, hwid);
	LOG_F(INFO, (const char*)hwid.c_str());
	
	
	//DISPLAY FIRMARE VERSION NUMBER
	std::string fwver = ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::GENERAL_VERSION_FILE_PATH);
	std::string hwrev = ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::GENERAL_HWREV_FILE_PATH);
	std::string bootpart = ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::GENERAL_BOOT_PARTION_INFO_FILE_PATH);
	gui.createEvent(guicommunicator::GUI_ELEMENT::INFOSCREEN_VERSION, guicommunicator::GUI_VALUE_TYPE::USER_INPUT_STRING, fwver + "|" + hwrev + "|" + bootpart);
	LOG_F(INFO, (const char*)fwver.c_str());
	
	
	//INIT CHESSBOARD
	ChessBoard board;
	//INIT THE CHESS BOARD MECHANIC
	//=> HOME, SETUP COILS
	while (board.initBoard() != ChessBoard::BOARD_ERROR::INIT_COMPLETE)
	{
		if (gui.show_message_box(guicommunicator::GUI_MESSAGE_BOX_TYPE::MSGBOX_A_OK_CANCEL, "BOARD_INIT_FAILED RETRY?", 10000) != guicommunicator::GUI_MESSAGE_BOX_RESULT::MSGBOX_RES_OK) {
			break;
		}
	}
	


	
	
	
	
//CREATE GAME BACKEND INSTANCE
BackendConnector gamebackend(ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::NETWORK_BACKEND_URL), ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::GENERAL_HWID_INTERFACE), hwid);
	
//CHECK IF GAMESERVER IS REACHABLE
if(gamebackend.check_connection())
{
	gui.createEvent(guicommunicator::GUI_ELEMENT::NETWORK_STATUS, guicommunicator::GUI_VALUE_TYPE::ONLINE);	
}
else
{
	gui.createEvent(guicommunicator::GUI_ELEMENT::NETWORK_STATUS, guicommunicator::GUI_VALUE_TYPE::OFFLINE);
	LOG_F(ERROR, "gamebackend - check connection failed");
	gui.show_error_message_on_gui("Cant connect to game server. (ERR01) [" + ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::NETWORK_BACKEND_URL) + "]");
	return 3;
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
io.setTurnStateLight(IOController::TURN_STATE_LIGHT::TSL_IDLE);
while (mainloop_running == 0)
{
	//HANDLE UI EVENTS UI LOOP
	guicommunicator::GUI_EVENT ev = gui.get_gui_update_event();
	if (ev.is_event_valid)
	{
		gui.debug_event(ev, true);	
			
		if (ev.event == guicommunicator::GUI_ELEMENT::INIT_BTN && ev.type == guicommunicator::GUI_VALUE_TYPE::CLICKED) {
			gui.createEvent(guicommunicator::GUI_ELEMENT::SWITCH_MENU, guicommunicator::GUI_VALUE_TYPE::PROCESSING_SCREEN);
			if (board.initBoard() != ChessBoard::BOARD_ERROR::INIT_COMPLETE)
			{
				gui.show_error_message_on_gui("board.initBoard() FAILED");
			}
			gui.createEvent(guicommunicator::GUI_ELEMENT::SWITCH_MENU, guicommunicator::GUI_VALUE_TYPE::SETTINGS_SCREEN);
		}
			
			
			
			
		if ((ev.event == guicommunicator::GUI_ELEMENT::SCAN_BOARD_BTN || ev.event == guicommunicator::GUI_ELEMENT::DEBUG_FUNCTION_A) && ev.type == guicommunicator::GUI_VALUE_TYPE::CLICKED) {
			gui.createEvent(guicommunicator::GUI_ELEMENT::SWITCH_MENU, guicommunicator::GUI_VALUE_TYPE::PROCESSING_SCREEN);
			if (board.calibrate_home_pos() == ChessBoard::BOARD_ERROR::NO_ERROR)
			{
				gui.show_message_box(guicommunicator::GUI_MESSAGE_BOX_TYPE::MSGBOX_B_OK, "TABLE REACHED HOME POSITION", 10000);
			}
			else
			{
				gui.show_error_message_on_gui("board.initBoard() FAILED");
			}
			gui.createEvent(guicommunicator::GUI_ELEMENT::SWITCH_MENU, guicommunicator::GUI_VALUE_TYPE::SETTINGS_SCREEN);
		}
			
			
			
			
			
			
		//USER PRESSED LOGOUT BUTTON
		//if (ev.event == guicommunicator::GUI_ELEMENT::LOGOUT_BTN && ev.type == guicommunicator::GUI_VALUE_TYPE::CLICKED)
		//{
			//gamebackend.logout();
//	gui.createEvent(guicommunicator::GUI_ELEMENT::SWITCH_MENU, guicommunicator::GUI_VALUE_TYPE::LOGIN_SCREEN);
	//TODO RESET ALL STUFF sessid, game field,...
//}
			
//USER PRESSED LOGIN BUTTON
//if (ev.event == guicommunicator::GUI_ELEMENT::BEGIN_BTN && ev.type == guicommunicator::GUI_VALUE_TYPE::CLICKED)
//{
	//if (gamebackend.login())
	//{
		//SWITCH TO MAIN MENU
	//	gui.createEvent(guicommunicator::GUI_ELEMENT::SWITCH_MENU, guicommunicator::GUI_VALUE_TYPE::MAIN_MENU_SCREEN);	
		//UPDATE SESSION_ID
	//	gui.createEvent(guicommunicator::GUI_ELEMENT::INFOSCREEN_SESSIONID_LABEL, guicommunicator::GUI_VALUE_TYPE::USER_INPUT_STRING, gamebackend.get_session_id());
	//}
	//else
	//{
	//	gui.show_error_message_on_gui(gamebackend.get_last_error());
	//}
//}
//}
		
//HANDLE GAME STATE 
//if(ev.event == guicommunicator::GUI_ELEMENT::INIT_BTN && ev.type == guicommunicator::GUI_VALUE_TYPE::CLICKED)
//{
//			motorA.goto_position(1203);
	}
		
	//HANDLE BACKEND EVETNS
	//CHECK CONNECTION IN INTERVAL
	//CHECK LOGINSTATE IN INTERVAL
		
		
		
}
	
return 0;
}
