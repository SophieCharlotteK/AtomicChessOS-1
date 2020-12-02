#pragma once
#ifndef __BackendConnector__
#define __BackendConnector__

#include <string>
#include <list>
//THREAD STUFF
#include <queue>
#include <mutex>
#include <iostream>
#include <thread>
#include <chrono>
//INCLUDE SHARED DEPENDENCIES
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
	
	struct GAME_STATE_SIMPLIFIED
	{
	};
		
	struct MATCHMAKING_STATE
	{
		bool waiting_for_game; //IF THE PLAYER HAS MATCHMAKING ENABLED
	};
	
	struct GAME_STATE
	{
		bool game_running;
		GAME_STATE_SIMPLIFIED simplified;
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
		
		
		MATCHMAKING_STATE matchmaking_state;
		GAME_STATE game_state;
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
	
	/*
	int? test1 = listID;
	//SO
	if(!s3c.List.WaitForListIdle(out test1, 1000))
	
	//ODER SO ?
	if(!s3c.List.WaitForListIdle(out test1, 0))
	
	//ODER SOOO ?
		while(!s3c.List.WaitForListIdle(out test1, 0))
	{
		//TODO NOTHIING
	}
	*/
	int get_avariable_ai_player();
	bool get_heartbeat();
	
	void set_backend_base_url(std::string _url);
	std::string get_backend_base_url();
	std::string get_session_id();
	std::string get_interface_name();
	
	PLAYER_STATUS get_player_state();
	
	bool set_player_state(PLAYER_STATE _ps);
	bool set_player_setup_confirmation(bool _board_ready);
	bool start_match_with_player(std::string _virtual_player_id);
	
	
	
	std::list<PLAYER_AVARIABLE> get_players_avariable();
	//DATA STRUCT SEND TO THE HEARTBEAT THREAD FUNCTION
	
	
	
	
	bool start_heartbeat_thread();   //CHECK IF RUNNING
	bool stop_heartbeat_thread();

	
	
	
	void setHearbeatCallInterval(int _int);

	
	void set_https_client_certificate(std::string _path);
	std::string get_last_error();
	
	
	PLAYER_PROFILE getPlayerProfile(); //GETTER FOR private plaqyer_profile
private:
	
	//STRUCT FOR HOLDING IMPORTANT INFORMATION ABOUT A REQUEST RESPONSE USED AS RETURN IN THE make_request FUNCTION
	struct request_result
	{
		bool request_failed;
		std::string body;
		httplib::Error error;
		int status_code;
		std::string uri;
	};
	
	std::string backend_base_url; //BASE URL OF THE BACKEND (http:// + URL + : + PORT ) SET BY CONSTRUCTOR
	std::string hwid; //HWID OF THE USER (SET BY CONTSTRUCTOR) USED FOR ALL REQUESTS
	std::string session_id; //THE CURRENT USER SESSION ID SET AFTER LOGIN(); cleared after logout()
	std::string interface_name; //NAME OF THE OUTGOING TRAFIIC INTERFACE NAME (eth0)
	
	PLAYER_PROFILE player_profile; //HOLDS THE CURRENT USERPROFILE AFTER LOGIN (FILLED WITH login() call)
	
	
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
	
	
	//--------------------- THREAD VARIABLES ---------------------- //
	//------------------ USED FOR THE HEARTBEAT ------------------ //
	struct HB_THREAD_DATA {
		std::string hwid;
		std::string sid;
		int heartbeat_interval;
	};
	int heartbeat_call_interval = 5;
	bool heartbeat_thread_running = false; //IS THE THREAD RUNNING
	HB_THREAD_DATA heartbeat_thread_data; //THREAD READ ALL VARS IN THIS STRUCT; SET BY START_HEARBEAT THREAD FUNCTION
	std::thread* heartbeat_thread = nullptr;
	std::mutex heartbeat_thread_mutex;
	bool get_heartbeat(HB_THREAD_DATA _data);  //FUNCTION VARIANT TO USE WITH A THREAD
	//-------------------- FUNCTIONS ----------------------------- //
	
	request_result make_request(std::string _url_path);
	static void heartbeat_thread_function(BackendConnector* _this);
	
	
	
};
#endif

