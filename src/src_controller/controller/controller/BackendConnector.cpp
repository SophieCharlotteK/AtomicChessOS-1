#include "BackendConnector.h"


BackendConnector::~BackendConnector()
{
}




BackendConnector::PLAYER_STATUS BackendConnector::get_player_state()
{
	PLAYER_STATUS status;
	status.requst_success = true;
	
	//DO THE REQUEST
	request_result tmp = make_request(URL_LOGIN + "?hwid=" + hwid + "&sid=" + session_id);
	
	if (tmp.request_failed)
	{
		last_error = "get_player_state - request failed";
		status.requst_success = false;
		return status;
	}
	std::string e = tmp.body;
	//PARSE THE JSON RESULT
	if(tmp.body.empty())
	{
		last_error = "get_player_state - result empty";
		status.requst_success = false;
		return status;
	}
	
}


void BackendConnector::set_https_client_certificate(std::string _path)
{
	ca_client_path = _path;
}

int BackendConnector::get_avariable_ai_player()
{
	request_result tmp = make_request(URL_GET_AI_PLAYERS_AVARIABLE + "?hwid=" + hwid + "&sid=" + session_id);
	if (tmp.request_failed)
	{
		last_error = "get_avariable_ai_player - request failed";
		return-1;
	}
	
	if (tmp.body.empty())
	{
		last_error = "get_avariable_ai_player - result empty";
		return -1;
	}
	
	//PARSE JSON RESULT
	std::string jp_err = "";
	json11::Json t = json11::Json::parse(tmp.body.c_str(), jp_err);
	if (jp_err.empty())
	{
		
		json11::Json::object  tt = t.object_items();
		if (tt.find("count") != tt.end()) {
			
			int count_str = tt["count"].int_value();
			if (count_str >= 0)
			{
				return count_str;
			}
		}
	}
	last_error = "get_avariable_ai_player - result format count invalid";
	return -1;
	
	
}


bool BackendConnector::set_player_state(PLAYER_STATE _ps)
{
	int ps_index = magic_enum::enum_integer(_ps);
	
	
	request_result tmp = make_request(URL_SET_PLAYER_VISIBLE_STATE + "?hwid=" + hwid + "&sid=" + session_id + "&ps=" + std::to_string(ps_index));
	if (!tmp.body.empty())
	{
		//PARSE JSON
		std::string jp_err = "";
		json11::Json t = json11::Json::parse(tmp.body.c_str(), jp_err);
		if (jp_err.empty())
		{
			if (((json11::Json::object)t.object_items())["status"].string_value() == "ok")
			{
				return true;
			}	
		}
	}
	return false;
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
	req_res.uri =  backend_base_url + _url_path;
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

void BackendConnector::set_backend_base_url(std::string _url)
{
	backend_base_url = _url;
}

std::string BackendConnector::get_backend_base_url()
{
	return backend_base_url;
}


BackendConnector::PLAYER_PROFILE BackendConnector::getPlayerProfile()
{
	return player_profile;
}
//CHECKS IF BACKEND REACHABLE
bool BackendConnector::check_connection()
{
	request_result tmp = make_request(URL_CONNECTION_CHECK);
	if (!tmp.body.empty())
	{
		//PARSE JSON
		std::string jp_err = "";
		json11::Json t = json11::Json::parse(tmp.body.c_str(), jp_err);
		if (jp_err.empty())
		{
			if (((json11::Json::object)t.object_items())["status"].string_value() == "ok")
			{
				return true;
			}	
		}
	}
	return false;
}

bool BackendConnector::login(PLAYER_TYPE _pt)
{
	//SET PLAYER TYPE PARAMETER
	std::string pt = "0";
	if (_pt == BackendConnector::PLAYER_TYPE::PT_AI) {
		pt = "1";	
	}
	else if (_pt == BackendConnector::PLAYER_TYPE::PT_HUMAN) {
		pt = "0";	
	}
	//DO THE REQUEST
	request_result tmp = make_request(URL_LOGIN + "?hwid=" + hwid + "&playertype=" + pt);
	
	if (tmp.request_failed)
	{
		last_error = "login - request failed";
		return false;
	}
	//PARSE THE JSON RESULT
	if(!tmp.body.empty())
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
			
			//TRY TO GET PROFILE INFORMATION
			if(tt["profile"].is_object())
			{
				json11::Json::object  po = tt["profile"].object_items();
				//FRIENDLY NAME
				if(po.find("friendly_name") != po.end()) {
					player_profile.friendly_name = po["friendly_name"].string_value();
				}
			
				
				//VIRTUAL PLAYER ID
				if(po.find("virtual_player_id") != po.end()) {
					player_profile.virtual_player_id = po["virtual_player_id"].string_value();
				}
				//ELO_RANK_READABLE
				if(po.find("elo_rank_readable") != po.end()) {
					player_profile.elo_rank_readable = po["elo_rank_readable"].string_value();
				
					//RANK
					if(po.find("rank") != po.end()) {
						player_profile.rank = po["rank"].int_value();
					}
				}
				//FINALLY GET THE SESSION ID
					std::string sid = tt["sid"].string_value();
				if (!sid.empty())
				{
					session_id = sid;
					return true;
				}	
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
	//DESTROY SESSION
	player_profile.rank = -1;
	player_profile.elo_rank_readable = "";
	player_profile.virtual_player_id  = "";
	player_profile.friendly_name = "";
	session_id = "";
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
	if (!get_session_id().empty())
	{
		return true;
	}
	return false;
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