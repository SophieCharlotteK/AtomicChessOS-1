#include <iostream>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


#include "SHARED/guicommunicator.h"
#include "SHARED/inih-master/INIReader.h"
#include "SHARED/loguru-master/loguru.hpp"


#include "BackendConnector.h"
using namespace std;

int mainloop_running = 0;
void signal_callback_handler(int signum)
{
	printf("Caught signal %d\n", signum);
	mainloop_running = signum;
	//exit(signum);
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
	LOG_SCOPE_F(INFO, "LOADING CONFIG FILE ./atcconfig.ini");
	INIReader config("./atcconfig.ini");
	if (config.ParseError() != 0)
	{
		LOG_F(ERROR, "Failed to load ./atcconfig.ini");
		return 1;
	}
	LOG_F(INFO, config.Get("NetworkSettings", "ATCBackendURL","http://marcelochsendorf.com:3001").c_str());	
		
	
	
	LOG_F(INFO, "guicommunicator startig ipc thread");
	guicommunicator gui;
	gui.start_recieve_thread();
	//CHECK VERSION ON GUI SIDE
	if (gui.check_guicommunicator_version())
	{
		LOG_F(WARNING, "guicommunicator version check failed");
	}
	
	//CREATE GAME BACKEND INSTANCE
	BackendConnector gamebackend(config.Get("NetworkSettings", "ATCBackendURL", "http://marcelochsendorf.com:3001"));
	if (gamebackend.check_connection())
	{
		LOG_F(ERROR, "gamebackend - check connection failed");
		gui.createEvent(guicommunicator::GUI_ELEMENT::ERROR, guicommunicator::GUI_VALUE_TYPE::ERROR_MESSAGE, "Cant connect to game server. (ERR01) [" + config.Get("NetworkSettings", "ATCBackendURL", "http://marcelochsendorf.com:3001") + "]");
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