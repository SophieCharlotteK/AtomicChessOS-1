#pragma once
#ifndef __BackendConnector__
#define __BackendConnector__

#include <string>



#include "SHARED/cpp-httplib-master/httplib.h"
#include "SHARED/json-master/include/tao/json.hpp"

class BackendConnector
{
	
public:
	BackendConnector(std::string _backend_base_url, std::string _hwid);
	~BackendConnector();

	bool check_connection();
private:
	std::string backend_base_url;
	
};
#endif

