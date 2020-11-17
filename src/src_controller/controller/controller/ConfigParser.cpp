#include "ConfigParser.h"

//STATIC INIT
ConfigParser* ConfigParser::instance = nullptr;
std::mutex ConfigParser::acces_lock_mutex;




ConfigParser::ConfigParser()
{
	reader  = INIReader();
}
	
ConfigParser::~ConfigParser()
{
}
ConfigParser* ConfigParser::getInstance()
{
	std::lock_guard<std::mutex> lck(ConfigParser::acces_lock_mutex);
	if (ConfigParser::instance == nullptr)
	{
		ConfigParser::instance = new ConfigParser();
	}
	return ConfigParser::instance;
}
	
bool ConfigParser::loadConfigFile(std::string _file)
{
	reader = INIReader(_file);
	if (reader.ParseError() != 0)
	{
		return false;
	}
	cfg_loaded_success = true;
	return true;
}


bool ConfigParser::createConfigFile(std::string _file, bool _load_directly)
{
	
	std::ofstream myfile;
	myfile.open(_file.c_str());
	
	
	
	myfile << "[SETTINGS]\n";
	
	myfile << "NETWORK_BACKEND_URL=http://192.168.178.125:3001\n";
	myfile << "GENERAL_HWID_INTERFACE=eth0\n";
	myfile << "GENERAL_VERSION_FILE_PATH=/VERSION\n";
	myfile << "GENERAL_HWREV_FILE_PATH=/etc/hwrevision\n";
	myfile << "GENERAL_BOOT_PARTION_INFO_FILE_PATH=/etc/swupdate/BOOTPART\n";
	myfile << "MECHANIC_STEPS_PER_MM=1292\n";
	myfile << "MECHANIC_INVERT_COILS=0\n";
	myfile << "MECHANIC_H1_OFFSET_MM_X=50\n";
	myfile << "MECHANIC_H1_OFFSET_MM_Y=0\n";
	myfile << "MECHANIC_DISTANCE_COILS_MM=110\n";
	myfile << "MECHANIC_COIL_SWITCH_POSTION_TRIGGER=200\n";
	myfile << "MECHANIC_CHESS_FIELD_WIDTH=50\n";
	myfile << "MECHANIC_CHESS_BOARD_WIDTH=400\n";
	
	myfile << "MECHANIC_PARK_POS_BLACK_X_LINE=0\n";
	myfile << "MECHANIC_PARK_POS_BLACK_FIRST_Y_OFFSET=20\n";
	
	myfile << "MECHANIC_PARK_POS_WHITE_X_LINE=350\n";
	myfile << "MECHANIC_PARK_POS_WHITE_FIRST_Y_OFFSET=20\n";
	myfile << "MECHANIC_PARK_POS_CELL_BEFORE_OFFSET=20\n";
	
	
	
	
		
	myfile << "MECHANIC_PARK_POS_CELL_SIZE=30\n";
	
	//MECHANIC_PARK_POS_BLACK_X_LINE = 28,
		//Y AXIS OFFSET FROM HOME POS TO THE FIRST PARKING POS RIGHT/BLACK
		//MECHANIC_PARK_POS_BLACK_FIRST_Y_OFFSET = 29,
		//DISTANCE BETWEEN EACH PARK POS TO THE NEXT
		//SMECHANIC_PARK_POS_CELL_SIZE = 30,
	
	myfile << "BOARD_PRESET_START_POSITION_FEN=rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n";
	myfile.close();
	
	if (_load_directly)
	{
		return loadConfigFile(_file);
		
	}
	return true;

}

bool ConfigParser::configLoaded()
{
	return cfg_loaded_success;
}


bool ConfigParser::get_int(ConfigParser::CFG_ENTRY _entry, int& _ret)
{
	
	if (!ConfigParser::getInstance()->get(_entry).empty()) {
		_ret = atoi(get(_entry).c_str());
		return true;
	}
	return false;
	
}
	
std::string ConfigParser::get(ConfigParser::CFG_ENTRY _entry)
{
	
	//auto color_name = magic_enum::enum_name(_entry);
	//TODO USE MAGIC ENUM
	
	switch(_entry)
	{
	case ConfigParser::CFG_ENTRY::NETWORK_BACKEND_URL: return reader.Get("SETTINGS", "NETWORK_BACKEND_URL", "");break;
	case ConfigParser::CFG_ENTRY::GENERAL_HWID_INTERFACE: return reader.Get("SETTINGS", "GENERAL_HWID_INTERFACE", "");break;
	case ConfigParser::CFG_ENTRY::GENERAL_VERSION_FILE_PATH: return reader.Get("SETTINGS", "GENERAL_VERSION_FILE_PATH", "");break;
	case ConfigParser::CFG_ENTRY::GENERAL_HWREV_FILE_PATH: return reader.Get("SETTINGS", "GENERAL_HWREV_FILE_PATH", "");break;
	case ConfigParser::CFG_ENTRY::GENERAL_BOOT_PARTION_INFO_FILE_PATH: return reader.Get("SETTINGS", "GENERAL_BOOT_PARTION_INFO_FILE_PATH", "");break;
	case ConfigParser::CFG_ENTRY::MECHANIC_STEPS_PER_MM: return reader.Get("SETTINGS", "MECHANIC_STEPS_PER_MM", "");break;
	case ConfigParser::CFG_ENTRY::MECHANIC_INVERT_COILS: return reader.Get("SETTINGS", "MECHANIC_INVERT_COILS", "");break;
	case ConfigParser::CFG_ENTRY::MECHANIC_H1_OFFSET_MM_X: return reader.Get("SETTINGS", "MECHANIC_H1_OFFSET_MM_X", "");break;
	case ConfigParser::CFG_ENTRY::MECHANIC_H1_OFFSET_MM_Y: return reader.Get("SETTINGS", "MECHANIC_H1_OFFSET_MM_Y", "");break;
	case ConfigParser::CFG_ENTRY::MECHANIC_DISTANCE_COILS_MM: return reader.Get("SETTINGS", "MECHANIC_DISTANCE_COILS_MM", "");break;
	case ConfigParser::CFG_ENTRY::MECHANIC_COIL_SWITCH_POSTION_TRIGGER: return reader.Get("SETTINGS", "MECHANIC_COIL_SWITCH_POSTION_TRIGGER", "");break;
	case ConfigParser::CFG_ENTRY::MECHANIC_CHESS_FIELD_WIDTH: return reader.Get("SETTINGS", "MECHANIC_CHESS_FIELD_WIDTH", "");break;
	case ConfigParser::CFG_ENTRY::MECHANIC_CHESS_BOARD_WIDTH: return reader.Get("SETTINGS", "MECHANIC_CHESS_BOARD_WIDTH", "");break;
	case ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_BLACK_X_LINE: return reader.Get("SETTINGS", "MECHANIC_PARK_POS_BLACK_X_LINE", "");break;
	case ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_BLACK_FIRST_Y_OFFSET: return reader.Get("SETTINGS", "MECHANIC_PARK_POS_BLACK_FIRST_Y_OFFSET", "");break;
	case ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_WHITE_X_LINE: return reader.Get("SETTINGS", "MECHANIC_PARK_POS_WHITE_X_LINE", "");break;
	case ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_WHITE_FIRST_Y_OFFSET: return reader.Get("SETTINGS", "MECHANIC_PARK_POS_WHITE_FIRST_Y_OFFSET", "");break;
	case ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_CELL_SIZE: return reader.Get("SETTINGS", "MECHANIC_PARK_POS_CELL_SIZE", "");break;
	case ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_CELL_BEFORE_OFFSET: return reader.Get("SETTINGS", "MECHANIC_PARK_POS_CELL_BEFORE_OFFSET", "");break;
		
		
		

		

	case ConfigParser::CFG_ENTRY::BOARD_PRESET_START_POSITION_FEN: return reader.Get("SETTINGS", "BOARD_PRESET_START_POSITION_FEN", "");break;
		
		
		
	default:
		break;
	}
	
	
	
	
	return "";
}

