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
    if (_baud_rate <= 0 || !check_baud_rate(_baud_rate))
    {
        LOG_F(ERROR, "serial port baudrate is invalid");
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
		port->SetTimeout(SERIAL_READ_DEFAULT_TIMEOUT); //INFINITE TIMEOUT
	}
	else
	{
		LOG_F(INFO, "serial port set to NONBLOCKING MODE 0");
		port->SetTimeout(0); //NON BLOCKING
		
	}

	//OPEN PORT
	if(!port->isPortOpen()){
        port->Open();
	}

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


bool GCodeSender::wait_for_ack(){
    std::string resp = "";
    while (resp.empty()) {
        port->Read(resp);
    }
    if(resp.rfind("ok") || resp.rfind("")){
        return true;
    }
    return false;
    //TODO CHECK RESULT OF string
    return true;
}

bool GCodeSender::write_gcode(std::string _gcode_line,int _retry_count){
    //check port state
    if (port == nullptr || !port->isPortOpen()){
        return false;
    }
    //check line
    if(_gcode_line.empty()){
        return false;
    }
    //check new line termination
    //(marlin) gcode requires a  /r/n termination
    std::vector<std::string> gcodelines();
    if(!_gcode_line.rfind("\r\n",_gcode_line.length()-2)){
        //IF NOT FOUND SIMPLY ADD AT THE END
        _gcode_line +="\r\n";
        is_single_gcode_line = true;
        gcodelines.push_back(_gcode_line);
    }else{
        //MAYBE THERE ARE MULTIBLE LINES IN THE STRING
        //SPLIT THEM AND SEND THEM OVER
    }

    volatile int ack_counter = 0;
    while (!gcodelines.empty()){
        //write current gcode instruction
        port->Write(gcodelines.front());
        //wait for ack
        while (ack_counter < 3){
            if(wait_for_ack()){
                break;
            }
        }

        //remove line
        gcodelines.pop_back();
    }

}

void GCodeSender::set_speed_preset(int _feedrate){
    if(_feedrate < 0){_feedrate = 0;}
    write_gcode("G0 F" + std::itoa(_feedrate) + "\r\n");
}
void GCodeSender::configure_marlin(){
    write_gcode("G21\r\n"); //SET UNIT TO MM
    write_gcode("G90\r\n"); //ABSOLUTE MODE
    home_sync();
    disable_motors();
}

void GCodeSender::home_sync(){
    write_gcode("G28 X Y\r\n"); //HOME AXIS
}

bool GCodeSender::setServo(int _index, int _pos){
    write_gcode("M280 P"+std::itoa(_index)+" S"+std::itoa(_pos)+"\r\n"); //MOVE SERVO
}

void GCodeSender::disable_motors(){
    write_gcode("M84\r\n"); //DISBBLE MOTOR
}

void GCodeSender::move_to_postion_mm_absolute(int _x,int _y, bool _blocking){
    write_gcode("G0 X"+std::itoa(_x)+" Y"+std::itoa(_y)+"\r\n"); //HOME AXIS
    current_pos_x = _x;
    current_pos_y = _y;

    if(_blocking){
        is_target_position_reached();
    }
}

bool GCodeSender::is_target_position_reached(){
    if (port == nullptr || !port->isPortOpen()){
        return false;
    }
    while(1){
        port->Write("M114\r\n");
        //WAIT FOR REPONSE
        std::string resp = "";
        while (resp.empty()) {
            port->Read(resp);
        }
        //PARSE RESILT
        //x trirs
    }


    //CHECK IF NOT EQUAL REPEAT

}