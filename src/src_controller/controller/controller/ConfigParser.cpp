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
	//PARSE GIVEN INI FILE
	reader = INIReader(_file);
	if (reader.ParseError() != 0)
	{
		return false;
	}
	
	//LOAD VALUES TO MAP
	constexpr auto& entries = magic_enum::enum_values<ConfigParser::CFG_ENTRY>();
	
	for (size_t i = 0; i < entries.size(); i++)
	{
		std::string_view cfg_name = magic_enum::enum_name(entries.at(i));
		if (std::string(cfg_name).empty()) {
			continue;
		}
		config_store[entries.at(i)] = reader.Get(INI_SETTINGS_CATEGORY_TOKEN, std::string(cfg_name), "");
	}
	
	
	cfg_loaded_success = true;
	return true;
}


bool ConfigParser::writeConfigFile(std::string _file)
{
	//OPEN TEXT FILE
	std::ofstream myfile;
	myfile.open(_file.c_str());
	if (!myfile.is_open()) {
		return false;
	}
	//WRITE INI FILE CATEGORY
	myfile << "[" << INI_SETTINGS_CATEGORY_TOKEN << "]\n";
	//FOR EACH ENTRY IN ENUM
	constexpr auto& entries = magic_enum::enum_values<ConfigParser::CFG_ENTRY>();
	
	for (size_t i = 0; i < entries.size(); i++)
	{
		std::string_view cfg_name = magic_enum::enum_name(entries.at(i));
		if (std::string(cfg_name).empty()) {
			continue;
		}
		myfile << std::string(cfg_name) << "=" << config_store[entries.at(i)] << "\n";
	}
	
	//FINALLY WRITE FILE
	myfile.close();
	return true;
}



void ConfigParser::loadDefaults() {
	//DEFAULT CONFIG HARD CODED :)
	config_store[ConfigParser::CFG_ENTRY::NETWORK_BACKEND_URL] = "http://atomicchess.de:3000";
	config_store[ConfigParser::CFG_ENTRY::NETWORK_HEARTBEAT_INTERVAL_SECS] = "5";
	
	
	
	config_store[ConfigParser::CFG_ENTRY::GENERAL_HWID_INTERFACE] = "eth0";
	config_store[ConfigParser::CFG_ENTRY::GENERAL_VERSION_FILE_PATH] = "/VERSION";
	config_store[ConfigParser::CFG_ENTRY::GENERAL_HWREV_FILE_PATH] = "/etc/hwrevision";
	config_store[ConfigParser::CFG_ENTRY::GENERAL_BOOT_PARTION_INFO_FILE_PATH] = "/etc/swupdate/BOOTPART";
	config_store[ConfigParser::CFG_ENTRY::GENERAL_SYSTEM_TICK_INTERVAL_MS] = "1000";
	
	
	config_store[ConfigParser::CFG_ENTRY::MECHANIC_STEPS_PER_MM] = "1292";
	config_store[ConfigParser::CFG_ENTRY::MECHANIC_INVERT_COILS] = "1";
	config_store[ConfigParser::CFG_ENTRY::MECHANIC_H1_OFFSET_MM_X] = "50";
	config_store[ConfigParser::CFG_ENTRY::MECHANIC_H1_OFFSET_MM_Y] = "0";
	config_store[ConfigParser::CFG_ENTRY::MECHANIC_DISTANCE_COILS_MM] = "110";
	config_store[ConfigParser::CFG_ENTRY::MECHANIC_COIL_SWITCH_POSTION_TRIGGER] = "200";
	config_store[ConfigParser::CFG_ENTRY::MECHANIC_CHESS_FIELD_WIDTH] = "50";
	config_store[ConfigParser::CFG_ENTRY::MECHANIC_CHESS_BOARD_WIDTH] = "400";
	config_store[ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_BLACK_X_LINE] = "0";
	config_store[ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_BLACK_FIRST_Y_OFFSET] = "10";
	config_store[ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_WHITE_X_LINE] = "350";
	config_store[ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_WHITE_FIRST_Y_OFFSET] = "5";
	config_store[ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_CELL_BEFORE_OFFSET] = "30";	
	config_store[ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_CELL_SIZE] = "26";
	
	
	config_store[ConfigParser::CFG_ENTRY::BOARD_PRESET_START_POSITION_FEN] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
}

bool ConfigParser::createConfigFile(std::string _file, bool _load_directly) {
		
	//LOAD DEFAULTS
	loadDefaults();

	
	//WRITE SETTINGS TO FILE
	writeConfigFile(_file);
	//LOAD FILE IF FLAG SET
	if(_load_directly)
	{
		return loadConfigFile(_file) ;
		
	}
	return true ;
}

bool ConfigParser::configLoaded()
{
	return cfg_loaded_success;
}


bool ConfigParser::getInt(ConfigParser::CFG_ENTRY _entry, int& _ret)
{
	
	if (!ConfigParser::getInstance()->get(_entry).empty()) {
		_ret = atoi(get(_entry).c_str());
		return true;
	}
	return false;
	
}
	
std::string ConfigParser::get(ConfigParser::CFG_ENTRY _entry)
{
	if (!cfg_loaded_success) {
		return "";
	}
	
	return config_store[_entry];
}

