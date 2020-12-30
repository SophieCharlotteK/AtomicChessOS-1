#include "GCodeSender.h"


GCodeSender::GCodeSender(std::string _serialport_file, int _baud)
{
	LOG_SCOPE_F(INFO, "GCodeSender::GCodeSender");
	if (_serialport_file.empty())
	{
		
		LOG_F(ERROR, "serial port _serialport_file is empty");
		return;
	}
		
	if (port == nullptr)
	{
		
		port = new mn::CppLinuxSerial::SerialPort();
		init_serial_port(_serialport_file, _baud,true);
		
		
	}
}


GCodeSender::GCodeSender(std::string _serialport_file)
{
	LOG_SCOPE_F(INFO, "GCodeSender::GCodeSender");
	if (_serialport_file.empty())
	{
		
		LOG_F(ERROR, "serial port _serialport_file is empty");
		return;
	}
		
	if (port == nullptr)
	{
		
		port = new mn::CppLinuxSerial::SerialPort();
		init_serial_port(_serialport_file, MARLIN_SERIAL_BAUD_RATE,true);
		
		
	}
		
}
bool GCodeSender::close_serial_port()
{
	if (port == nullptr)
	{
		LOG_F(ERROR, "close_serial_port serial port instance is null");
		return false;
	}
	
	try
	{
		port->Close();
		return true;
	
	}catch(const char* msg) {
		LOG_F(ERROR, "closing serial port failed", msg);
		return false;
	}
}

bool GCodeSender::init_serial_port(std::string _serial_port_file, int _baud_rate, bool _blocking)
{	
	LOG_SCOPE_F(INFO, "GCodeSender::init_serial_port");
	if (port == nullptr)
	{
		LOG_F(ERROR, "serial port instance is null");
		return false;
	}
	//configure serialport though 3rd party lib
	close_serial_port(); //CLOSE PORT IF OPEN
	port->SetDevice(_serial_port_file);
	port->SetBaudRate(_baud_rate);
	port->SetEcho(false);
	
	if (_blocking)
	{
		LOG_F(INFO, "serial port set to blocking mode -1");
		port->SetTimeout(-1); //INFINITE TIMEOUT
	}
	else
	{
		LOG_F(INFO, "serial port set to NONBLOCKING MODE 0");
		port->SetTimeout(0); //NON BLOCKING
		
	}
	port->Open();
	port->Write("G1 X100 Y100\r\n");
	
	std::string resp = "";
	
	while (resp.empty()) {
		port->Read(resp);
	}
	
	int i = 0;
	
	
}

GCodeSender::~GCodeSender()
{
	//STOP THREAD
	
	//CLOSE PORT
	if (port != nullptr)
	{
		port->Close();
		delete port;
	}
		
}