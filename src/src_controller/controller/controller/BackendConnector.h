#pragma once
#ifndef __BackendConnector__
#define __BackendConnector__

#include <string>



#include "SHARED/cpp-httplib-master/httplib.h"
#include "SHARED/json-master/include/tao/json.hpp"




class BackendConnector
{
	
public:
	BackendConnector(std::string _backend_base_url, std::string _interface_name, std::string _hwid);
	~BackendConnector();
	
	bool login();
	bool update_session();
	bool check_connection();
	bool check_login_state();
	bool logout();
	
	
	std::string get_session_id();
private:
	std::string backend_base_url;
	std::string hwid;
	std::string session_id;
	std::string interface_name;
	
	
	const std::string PLAYER_TYPE = "0"; //DEFINED THE PLAYERTYPE 0 IS A HUMAN PLAYER;
	
	
	const std::string URL_CONNECTION_CHECK = "/";
	
};
#endif

