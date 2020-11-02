#pragma once
#ifndef __CONFIG_PARSER_H__
#define __CONFIG_PARSER_H__

#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>


///INCLUDE 3rd PARTY
#include "SHARED/inih-master/INIReader.h"
#include "SHARED/magic_enum-master/include/magic_enum.hpp"


#define USE_STD_LOG




class ConfigParser
{
public:
	///PLEAE NOT BY EDIT/ADDING ENTRIES PLEASE EDIT ConfigParser::createConfigFile FUNCTION
	enum CFG_ENTRY
	{
	    NETWORK_BACKEND_URL = 0,
				
		
		
		GENERAL_HWID_INTERFACE = 10,
		GENERAL_VERSION_FILE_PATH = 11,
		GENERAL_HWREV_FILE_PATH = 12,
		GENERAL_BOOT_PARTION_INFO_FILE_PATH = 13,
			
		MECHANIC_STEPS_PER_MM = 20,
		MECHANIC_INVERT_COILS = 21,
	};
	
	static ConfigParser* getInstance();
	
	bool loadConfigFile(std::string _file);
	bool createConfigFile(std::string _file, bool _load_directly); //GENERATES A CONFIG FILE WITH DEFAULTS

	bool configLoaded();
	
	std::string get(ConfigParser::CFG_ENTRY _entry);
	
private:
	static ConfigParser* instance;
	static std::mutex acces_lock_mutex;
	
	bool cfg_loaded_success = false;
	ConfigParser();
	~ConfigParser();
	
	
	INIReader reader;
};
#endif
