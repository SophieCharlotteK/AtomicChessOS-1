#include <iostream>
#include "httplib.h"
#include "SHARED/guicommunicator.h"
using namespace std;

int main(int argc, char *argv[])
{
	
	httplib::Client cli("http://cpp-httplib-server.yhirose.repl.co");
	auto res = cli.Get("/hi");
	res->status;  // 200
	res->body;    // "Hello World!"
	
	if(res) {
		std::cout << res->status << std::endl;
		std::cout << res->body << std::endl;
	}
	char sz[] = "Hello, World!";
	cout << sz << endl;
	
	
	guicommunicator gui;
	gui.createEvent(guicommunicator::GUI_ELEMENT::SWITCH_MENU, guicommunicator::GUI_VALUE_TYPE::MAIN_MENU_SCREEN);
	return 0;
}