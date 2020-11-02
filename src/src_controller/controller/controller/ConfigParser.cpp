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
	myfile << "MECHANIC_STEPS_PER_MM=1140\n";
	myfile << "MECHANIC_INVERT_COILS=0\n";
	

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
	default:
		break;
	}
	
	
	
	
	return "";
}

