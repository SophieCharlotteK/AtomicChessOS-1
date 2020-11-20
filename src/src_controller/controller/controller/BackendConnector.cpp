#include "BackendConnector.h"


BackendConnector::~BackendConnector()
{
}

void BackendConnector::set_https_client_certificate(std::string _path)
{
	ca_client_path = _path;
}




BackendConnector::request_result BackendConnector::make_request(std::string _url_path)
{
	//PORT AND PROTOCOL IS ALREADY INCLUDED IN BASE URL LIKE http://127.0.0.1:3000
	httplib::Client cli(backend_base_url.c_str());
	
	cli.set_follow_location(true);  //AUTO REDIRECT IF GOT AN 300
	//SEND REQUEST OVER SELECTED INTERFACE
	if(!interface_name.empty())
	{
		cli.set_interface(interface_name.c_str());
	}
	//SET CLIENT CERTIFICATE FOR HTTP SERVER
	if(!ca_client_path.empty())
	{
		//cli.set_(ca_client_path.c_str());
	}
	
	request_result req_res;
	req_res.request_failed = false;
	//PERFORM REQUEST
	if(auto res = cli.Get(_url_path.c_str())) {
		req_res.status_code = res->status;
		//CHECK STATUS CODE 200 IS VALID
		
			req_res.body = res->body;
		
		
	}
	else {
		req_res.error = res.error();
		req_res.request_failed = true;
	}
	
	return req_res;
}



//CHECKS IF BACKEND REACHABLE
bool BackendConnector::check_connection()
{
	request_result tmp = make_request(URL_CONNECTION_CHECK);
	return !tmp.request_failed;
}

bool BackendConnector::login()
{
	request_result tmp = make_request(URL_LOGIN + "?hwid=" + hwid + "&playertype=" + PLAYER_TYPE);
	
	if (tmp.request_failed)
	{
		last_error = "login - request failed";
		return false;
	}
	
	if (!tmp.body.empty())
	{
		//PARSE JSON
		std::string jp_err = "";
		json11::Json t = json11::Json::parse(tmp.body.c_str(), jp_err);
		
		if (jp_err.empty())
		{
			json11::Json::object  tt = t.object_items();
			std::string status = tt["status"].string_value();
			if (status != "ok")
			{
				last_error = tt["status"].string_value();
				return false;
			}
		
			std::string sid = tt["sid"].string_value();
			if (!sid.empty())
			{
				session_id = sid;
				return true;
			}			
		}
	}
		//PARSE ID
		//SAVE SESSION ID
		//SAVE IMPORTANT PROFILE DATA
		last_error = "login - body empty";
		return false;
}
bool BackendConnector::logout()
{
	request_result tmp = make_request(URL_LOGOUT + "?hwid=" + hwid);
	
	if (tmp.request_failed)
	{
		last_error = "login - request failed";
		return false;
	}
	//SAVE SESSION
	return true;
}

BackendConnector::BackendConnector(std::string _backend_base_url, std::string _interface_name, std::string _hwid)
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

std::string BackendConnector::get_interface_name()
{
	return interface_name;
}

std::string BackendConnector::get_last_error()
{
	return last_error;
}