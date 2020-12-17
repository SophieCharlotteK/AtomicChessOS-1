#pragma once
#ifndef __CONFIG_PARSER_H__
#define __CONFIG_PARSER_H__

#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include <map>

///INCLUDE 3rd PARTY
#include "SHARED/inih-master/INIReader.h"
#include "SHARED/magic_enum-master/include/magic_enum.hpp"


#define USE_STD_LOG


#define INI_SETTINGS_CATEGORY_TOKEN "SETTINGS"
class ConfigParser
{
public:
	///PLEAE NOT BY EDIT/ADDING ENTRIES PLEASE EDIT ConfigParser::createConfigFile FUNCTION
	enum class CFG_ENTRY
	{
		NETWORK_BACKEND_URL = 0,
		NETWORK_HEARTBEAT_INTERVAL_SECS = 1,
		
		
		GENERAL_HWID_INTERFACE = 10,
		GENERAL_VERSION_FILE_PATH = 11,
		GENERAL_HWREV_FILE_PATH = 12,
		GENERAL_BOOT_PARTION_INFO_FILE_PATH = 13,
		GENERAL_SYSTEM_TICK_INTERVAL_MS = 14,
			
		MECHANIC_STEPS_PER_MM = 20,
		MECHANIC_INVERT_COILS = 21,
		//OFFSET COORDINATES FROM HOME POS TO H1
		MECHANIC_H1_OFFSET_MM_X = 22,
		MECHANIC_H1_OFFSET_MM_Y = 23,
		//DISTANCE BETWEEN COILS //IN THE MIDDLE IS THE NFC TAG
		MECHANIC_DISTANCE_COILS_MM = 24,
		//X POSITION WHRE TO SWITCH THE COILS NEED TO SWITCH
		MECHANIC_COIL_SWITCH_POSTION_TRIGGER = 25,
		//WIDTH OF EACH CHESS FIELD
		MECHANIC_CHESS_FIELD_WIDTH = 26,
		//WIDTH OF THE 8x8 CHESS BOARD TOTAL = MECHANIC_CHESS_FIELD_WIDTH*8
		MECHANIC_CHESS_BOARD_WIDTH = 27,
		//X AXIS OFFSET FROM HOME POS TO THE RIGHT/BLACK PARKING POS
		MECHANIC_PARK_POS_BLACK_X_LINE = 28,
		//Y AXIS OFFSET FROM HOME POS TO THE FIRST PARKING POS RIGHT/BLACK
		MECHANIC_PARK_POS_BLACK_FIRST_Y_OFFSET = 29,
		
		MECHANIC_PARK_POS_WHITE_X_LINE=30,
		MECHANIC_PARK_POS_WHITE_FIRST_Y_OFFSET=31,
		//DISTANCE BETWEEN EACH PARK POS TO THE NEXT
		MECHANIC_PARK_POS_CELL_SIZE = 32,
		MECHANIC_PARK_POS_CELL_BEFORE_OFFSET = 33,
		MECHANIC_BOARD_SIZE_550MM_WORKAROUND = 34,
		
		
		BOARD_PRESET_START_POSITION_FEN = 50,
		
		
		
		
		//SOME REVERSE ENTRIES FOR CUSTOM  SAVE STUFF
		RESERVED_1 = 100,
		RESERVED_2 = 101,
		RESERVED_3 = 102,
		RESERVED_4 = 103,
		RESERVED_5 = 104,
		RESERVED_6 = 105,
		RESERVED_7 = 106,
		RESERVED_8 = 107,
		RESERVED_9 = 108,
		RESERVED_10 = 109,
		RESERVED_11 = 110,
		RESERVED_12 = 111,
		RESERVED_13 = 112,
		RESERVED_14 = 113,
		RESERVED_15 = 114,
		RESERVED_16 = 115,
		RESERVED_17 = 116,
		RESERVED_18 = 117,
		RESERVED_19 = 118,
		RESERVED_20 = 119
		
	};
	
	static ConfigParser* getInstance();
	
	bool loadConfigFile(std::string _file);
	bool createConfigFile(std::string _file, bool _load_directly); //GENERATES A CONFIG FILE WITH DEFAULTS
	
	bool configLoaded();
	
	std::string get(ConfigParser::CFG_ENTRY _entry);
	void set(ConfigParser::CFG_ENTRY _entry, std::string _value, bool _writeConfigToFile);
	bool writeConfigFile(std::string _file);
	
	bool getInt(ConfigParser::CFG_ENTRY _entry, int& _ret);
	bool getBool(ConfigParser::CFG_ENTRY _entry, bool& _ret);
	void loadDefaults();
private:
	static ConfigParser* instance;
	static std::mutex acces_lock_mutex;
	
	bool cfg_loaded_success = false;
	ConfigParser();
	~ConfigParser();
	//STORES THE ACUTAL CONFIG FILE
	std::map<ConfigParser::CFG_ENTRY, std::string> config_store;
	INIReader reader;
};
#endif
