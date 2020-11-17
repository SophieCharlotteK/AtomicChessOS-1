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
		
		
		
		BOARD_PRESET_START_POSITION_FEN = 50,
		
	};
	
	static ConfigParser* getInstance();
	
	bool loadConfigFile(std::string _file);
	bool createConfigFile(std::string _file, bool _load_directly); //GENERATES A CONFIG FILE WITH DEFAULTS

	bool configLoaded();
	
	std::string get(ConfigParser::CFG_ENTRY _entry);
	
	bool get_int(ConfigParser::CFG_ENTRY _entry, int& _ret);
	
private:
	static ConfigParser* instance;
	static std::mutex acces_lock_mutex;
	
	bool cfg_loaded_success = false;
	ConfigParser();
	~ConfigParser();
	
	
	INIReader reader;
};
#endif
