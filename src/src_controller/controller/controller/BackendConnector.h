#pragma once
#ifndef __BackendConnector__
#define __BackendConnector__

#include <string>
#include <list>


#include "SHARED/cpp-httplib-master/httplib.h"
#include "SHARED/json11-master/json11.hpp"
#include "SHARED/loguru-master/loguru.hpp"

//IF A SESSION ID IS SET AND NOT NULL START HEARTBEAR THREAD

//THREAD FOR SESSOON
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
	
	std::list<std::string> get_avariable_player();
	
	bool set_visible_state(bool _state);
	void set_backend_base_url(std::string _url);
	std::string get_backend_base_url();
	std::string get_session_id();
	std::string get_interface_name();
	
	
	void set_https_client_certificate(std::string _path);
	std::string get_last_error();
	
private:
	struct request_result
	{
		bool request_failed;
		std::string body;
		httplib::Error error;
		int status_code;
	};
	
	std::string backend_base_url;
	std::string hwid;
	std::string session_id;
	std::string interface_name;
	
	
	const std::string PLAYER_TYPE = "0"; //DEFINED THE PLAYERTYPE 0 IS A HUMAN PLAYER;
	
	
	const std::string URL_CONNECTION_CHECK = "/";
	const std::string URL_LOGIN = "/rest/login";
	const std::string URL_LOGOUT = "/rest/logout";
	const std::string URL_HEARTBEAT = "/rest/heartbeat";
	std::string ca_client_path = "";
	
	std::string last_error = "";
	
	request_result make_request(std::string _url_path);
	
};
#endif

