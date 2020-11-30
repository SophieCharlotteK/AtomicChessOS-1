#pragma once
#ifndef __BackendConnector__
#define __BackendConnector__

#include <string>
#include <list>


#include "SHARED/cpp-httplib-master/httplib.h"
#include "SHARED/json11-master/json11.hpp"
#include "SHARED/loguru-master/loguru.hpp"
#include "SHARED/magic_enum-master/include/magic_enum.hpp"
//IF A SESSION ID IS SET AND NOT NULL START HEARTBEAR THREAD

//THREAD FOR SESSOON
class BackendConnector
{
	
public:
	struct PLAYER_AVARIABLE{
		std::string visibale_name;
		int rank;
		std::string elo_rank;
		std::string virtual_player_id;
	};
	
	struct MATCHMAKING_STATE
	{
		
	};
	
	struct PLAYER_PROFILE
	{
		std::string friendly_name;
		std::string virtual_player_id;
		std::string elo_rank_readable;
		int rank;
	};

	enum class PLAYER_STATE{
		PS_INVALID=0,
		PS_IDLE=1,
		PS_SEARCHING=4,
		PS_SEARCHING_MANUAL =5,
		PS_PREPARING_INGAME = 6,
		PS_INGAME = 7
	};
	enum class PLAYER_TYPE{
		PT_HUMAN = 0,
		PT_AI = 1
	};
	
	
	struct PLAYER_STATUS
	{
		PLAYER_STATE player_state;
		PLAYER_TYPE player_type;
		
		std::string status;
		std::string err;
		
		bool player_login_state;
		bool requst_success;
		
	};
	BackendConnector(std::string _backend_base_url, std::string _interface_name, std::string _hwid);
	~BackendConnector();
	
	bool login(PLAYER_TYPE _pt);
	bool update_session();
	bool check_connection();
	bool check_login_state();
	bool logout();
	
	std::list<std::string> get_avariable_player();
	int get_avariable_ai_player();
	
	void set_backend_base_url(std::string _url);
	std::string get_backend_base_url();
	std::string get_session_id();
	std::string get_interface_name();
	
	PLAYER_STATUS get_player_state();
	
	bool set_player_state(PLAYER_STATE _ps);
	bool set_player_setup_confirmation(bool _board_ready);
	bool start_match_with_player(std::string _virtual_player_id);
	
	
	
	std::list<PLAYER_AVARIABLE> get_players_avariable();
	
	
	void set_https_client_certificate(std::string _path);
	std::string get_last_error();
	
	
	PLAYER_PROFILE getPlayerProfile();
private:
	struct request_result
	{
		bool request_failed;
		std::string body;
		httplib::Error error;
		int status_code;
		std::string uri;
	};
	
	std::string backend_base_url;
	std::string hwid;
	std::string session_id;
	std::string interface_name;
	
	PLAYER_PROFILE player_profile;
	
	
	const std::string URL_CONNECTION_CHECK = "/rest/client_status";
	const std::string URL_LOGIN = "/rest/login";
	const std::string URL_LOGOUT = "/rest/logout";
	const std::string URL_HEARTBEAT = "/rest/heartbeat";
	const std::string URL_PLAYER_STATE = "/rest/get_player_state";
	const std::string URL_GET_PLAYERS_AVARIABLE = "/rest/get_players_avariable";
	const std::string URL_GET_AI_PLAYERS_AVARIABLE = "/rest/get_avariable_ai_players";
	
	const std::string URL_SET_PLAYER_VISIBLE_STATE = "/rest/set_player_state";
	
	std::string ca_client_path = "";
	
	std::string last_error = "";
	
	request_result make_request(std::string _url_path);
	
};
#endif

