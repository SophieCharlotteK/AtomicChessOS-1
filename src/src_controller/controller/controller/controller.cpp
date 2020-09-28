#include <iostream>
#include "httplib.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


#include "SHARED/guicommunicator.h"

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
	signal(SIGINT, signal_callback_handler);

	 
		
		
	//httplib::Client cli("http://cpp-httplib-server.yhirose.repl.co");
	//auto res = cli.Get("/hi");
	//res->status;  // 200
	//res->body;    // "Hello World!"
	
	//if(res) {
//		std::cout << res->status << std::endl;
//		std::cout << res->body << std::endl;
//	}

	
	guicommunicator gui;
	
	gui.start_recieve_thread();
  
	
	

	while (mainloop_running == 0)
	{
		guicommunicator::GUI_EVENT ev = gui.get_gui_update_event();
		if (ev.is_event_valid)
		{
			gui.debug_event(ev,true);	
		}
		
		gui.createEvent(guicommunicator::GUI_ELEMENT::SWITCH_MENU, guicommunicator::GUI_VALUE_TYPE::MAIN_MENU_SCREEN);
	}
	
	return 0;
}