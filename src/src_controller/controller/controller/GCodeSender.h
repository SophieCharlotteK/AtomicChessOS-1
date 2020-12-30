#ifndef __GCODESENDER_H__
#define __GCODESENDER_H__


#include <string>
#include <queue>
#include <mutex>
#include <iostream>
#include <thread>

//3rd PARTY INCLUDES
#include "SHARED/loguru-master/loguru.hpp"
#include "SHARED/CppLinuxSerial-master/include/CppLinuxSerial/SerialPort.hpp"


#include "ConfigParser.h"

class GCodeSender
{
	
public:
	
	GCodeSender(std::string _serialport_file);
	GCodeSender(std::string _serialport_file,int _baud);
	~GCodeSender();
	//THREAD ZUM SENDEN
	
	
		
private :
	mn::CppLinuxSerial::SerialPort*  port = nullptr;
	const int MARLIN_SERIAL_BAUD_RATE = 115200;
	bool init_serial_port(std::string _serial_port_file, int _baud_rate, bool _blocking);
	bool close_serial_port();
	
};

#endif


