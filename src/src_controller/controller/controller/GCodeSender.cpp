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
		
		port = new mn::CppLinuxSerial::SerialPort(_serialport_file, B115200);
		init_serial_port(_serialport_file, _baud, true);
		
		
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
		
		port = new mn::CppLinuxSerial::SerialPort(_serialport_file, B115200);
		init_serial_port(_serialport_file, MARLIN_SERIAL_BAUD_RATE, true);
		
		
	}
		
}

bool GCodeSender::check_baud_rate(int _baudrate_to_check) {
	switch (_baudrate_to_check) {
	case 9600:
		return B9600;
	case 19200:
		return B19200;
	case 38400:
		return B38400;
	case 57600:
		return B57600;
	case 115200:
		return B115200;
	case 230400:
		return B230400;
	case 460800:
		return B460800;
	case 500000:
		return B500000;
	case 576000:
		return B576000;
	case 921600:
		return B921600;
	case 1000000:
		return B1000000;
	case 1152000:
		return B1152000;
	case 1500000:
		return B1500000;
	case 2000000:
		return B2000000;
	case 2500000:
		return B2500000;
	case 3000000:
		return B3000000;
	case 3500000:
		return B3500000;
	case 4000000:
		return B4000000;
	default:
		return 0;
	}
}

mn::CppLinuxSerial::BaudRate GCodeSender::get_baud_rate(int _baudrate_to_check) {
	switch (_baudrate_to_check) {
	case 9600:
		return mn::CppLinuxSerial::BaudRate::B_9600;
	case 19200:
		return mn::CppLinuxSerial::BaudRate::B_19200;
	case 38400:
		return mn::CppLinuxSerial::BaudRate::B_38400;
	case 57600:
		return mn::CppLinuxSerial::BaudRate::B_57600;
	case 115200:
		return mn::CppLinuxSerial::BaudRate::B_115200;
	case 230400:
		return mn::CppLinuxSerial::BaudRate::B_230400;
	case 460800:
		return mn::CppLinuxSerial::BaudRate::B_460800;
	default:
		return mn::CppLinuxSerial::BaudRate::B_0;
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
	
	}
	catch (const char* msg) {
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
	if (_baud_rate <= 0 || !check_baud_rate(_baud_rate))
	{
		LOG_F(ERROR, "serial port baudrate is invalid");
		return false;
	}
	//configure serialport though 3rd party lib
	close_serial_port();   //CLOSE PORT IF OPEN
	port->SetDevice(_serial_port_file);
	port->SetBaudRate(get_baud_rate(_baud_rate));
	port->SetEcho(false);
	
	if (_blocking)
	{
		LOG_F(INFO, "serial port set to blocking mode -1");
		port->SetTimeout(SERIAL_READ_DEFAULT_TIMEOUT);   //INFINITE TIMEOUT
	}
	else
	{
		LOG_F(INFO, "serial port set to NONBLOCKING MODE 0");
		port->SetTimeout(0);   //NON BLOCKING
		
	}

	//OPEN PORT
	if(!port->isPortOpen()) {
		port->Open();
	}
	
	dummy_read();
	write_gcode("\n");
	write_gcode("G28\n");
	write_gcode("G1 X150 Y125.3 F3000\n");
	write_gcode("G1 X50 Y25.3 F3000\n");
	//PEROM A DUMMY READ
	dummy_read();
	
	
	
	
}

void GCodeSender::dummy_read()
{
	std::string dummy = "DUMMY";
	std::string complete = "";
	while (!dummy.empty())
	{
		port->Read(dummy); 
		if (!dummy.empty())
		{
			complete += dummy;
		}
	}
	
	std ::cout << complete.c_str() << std::endl;
}

GCodeSender::~GCodeSender()
{
	//STOP THREAD
	
	//CLOSE PORT
	if(port != nullptr)
	{
		port->Close();
		delete port;
	}
		
}


bool GCodeSender::wait_for_ack() {
    
	volatile int wait_counter = 0;
	std::string read_data = "";
	
	while (true) {
		std::string resp = "";
		port->Read(resp);
		if (!resp.empty())
		{
			read_data += resp;
		}
		else
		{
			wait_counter++;
		}
		
		if (read_data.rfind("ok") != std::string::npos)
		{
			break;
		}else if(read_data.rfind("echo:busy: processing") != std::string::npos){
			wait_counter = 0;
			read_data = "";
			std::cout << "wait_for_ack: busy_processing" << std::endl;
		}

		if (wait_counter > 10)
		{
			break;
		}
	}
	std::cout << "wait_for_ack: " << read_data << std::endl;
	//SPLIT RESULT BY NEW LINE
		
	
		//CHECK RESULT
	//	if(read_data.rfind("ok") != std::string::npos) {
		
//		}else if(read_data.rfind("echo:Unknown") != std::string::npos) {

	//	}else if(read_data.rfind("echo:busy") != std::string::npos) {

	    
	
    
	if (wait_counter >= 5)
	{
		return false;
	}
   
	return true;
}

bool GCodeSender::write_gcode(std::string _gcode_line, bool _ack_check) {
	//check port state
	if(port == nullptr || !port->isPortOpen()) {
		return false;
	}
	//check line
	if(_gcode_line.empty()) {
		return false;
	}

	if (!port->isPortOpen()) {
		LOG_F(ERROR, _gcode_line.c_str(), "  SERIAL PORT CLOSED");
		return false;
	}

	//write current gcode instruction
	port->Write(_gcode_line);
	if (!_ack_check)
	{
		return true;
	}
		
	//wait for ack
	
	if(!wait_for_ack())
	{
		LOG_F(ERROR, _gcode_line.c_str(), " ACK FAILED");
		return false;
	}
		
	return true;

}

void GCodeSender::set_speed_preset(int _feedrate) {
	if (_feedrate < 0){_feedrate = 0; }
	write_gcode("G0 F" + std::to_string(_feedrate));
}
void GCodeSender::configure_marlin() {
	write_gcode("G21");    //SET UNIT TO MM
    write_gcode("G90");    //ABSOLUTE MODE
  //  home_sync();
//	disable_motors();
}

void GCodeSender::home_sync() {
	write_gcode("G28");   //HOME AXIS
}

bool GCodeSender::setServo(int _index, int _pos) {
	write_gcode("M280 P" + std::to_string(_index) + " S" + std::to_string(_pos));   //MOVE SERVO
}

void GCodeSender::disable_motors() {
	write_gcode("M84");   //DISBBLE MOTOR
}


bool GCodeSender::write_gcode(std::string _gcode_line)
{
	write_gcode(_gcode_line, true);
}


void GCodeSender::move_to_postion_mm_absolute(int _x, int _y, bool _blocking) {
	write_gcode("G0 X" + std::to_string(_x) + " Y" + std::to_string(_y));   //HOME AXIS
	current_pos_x = _x;
	current_pos_y = _y;

	if (_blocking) {
		write_gcode("M400");    //HOME AXIS
		//is_target_position_reached();
	}
}

bool GCodeSender::is_target_position_reached() {
	if (port == nullptr || !port->isPortOpen()) {
		return false;
	}
	//	while (1) {
		//	write_gcode("M114", -1, true);  //NO ACK CHECK 
			//WAIT FOR REPONSE
			std::string resp = "";
	//	while (resp.empty()) {
	//		port->Read(resp);
	//	}
		//PARSE RESILT
		//x trirs
	//}


	//CHECK IF NOT EQUAL REPEAT

}