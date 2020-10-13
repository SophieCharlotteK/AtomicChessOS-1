#include "BackendConnector.h"

#include "SHARED/cpp-httplib-master/httplib.h"


BackendConnector::~BackendConnector()
{
}

//CHECKS IF LOGGED IN
bool BackendConnector::check_connection()
{
	
	httplib::Client cli(backend_base_url.c_str());
	cli.set_interface(interface_name.c_str()); 
	if (auto res = cli.Get(URL_CONNECTION_CHECK.c_str())) {
		if (res->status >= 200 && res->status < 300) {
			return true;
		}
	}
	else {
		auto err = res.error();
		return false;
	}
}

bool BackendConnector::login()
{
	
	//SAVE SESSION
	return true;
}
bool BackendConnector::logout()
{
	
	//SAVE SESSION
	return true;
}

BackendConnector::BackendConnector(std::string _backend_base_url,std::string _interface_name, std::string _hwid)
	: backend_base_url()
{
	backend_base_url = _backend_base_url;
	hwid = _hwid;
	
}


bool BackendConnector::check_login_state()
{
	
}

std::string BackendConnector::get_session_id()
{
	return session_id;
}