#include <iostream>
#include <httplib.h>

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
	return 0;
}