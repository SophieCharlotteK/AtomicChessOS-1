#pragma once
#ifndef __CONFIG_PARSER_H__
#define __CONFIG_PARSER_H__

#include <thread>
#include <mutex>


#include "SHARED/inih-master/INIReader.h"



#define USE_STD_LOG

#ifdef USE_STD_LOG
#include <iostream>
#endif

class ConfigParser
{
public:
	static ConfigParser* getInstance();
	
	bool loadConfigFile(std::string _file);
	bool createConfigFile(std::string _file);
private:
	ConfigParser();
	~ConfigParser();
	
};
#endif
