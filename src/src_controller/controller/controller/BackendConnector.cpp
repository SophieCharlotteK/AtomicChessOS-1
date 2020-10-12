#include "BackendConnector.h"




BackendConnector::~BackendConnector()
{
}


bool BackendConnector::check_connection()
{
}


BackendConnector::BackendConnector(std::string _backend_base_url, std::string _hwid)
	: backend_base_url()
{
	backend_base_url = _backend_base_url;
	hwid = _hwid;
	
}
