/**
 * \class TMC5160
 *
 * \ingroup ATC_CONTROLLER
 * (Note, this needs exactly one \defgroup somewhere)
 *
 * \brief This class implements a driver for the TMC5160 chip, which provides functions to move a stepper motor to a certan position.
 *
 *
 *
 * \note Uses the LibPiSPI library
 *
 * \author Marcel Ochsendorf marcelochsendorf.com ProDevMo
 *
 * \version 13.10.2020
 *
 * \date $Date: 2020/10/13 21:56:00 $
 *
 * Contact: info@marcelochsendorf.com
 *
 * Created on: 13.10.2020
 *
 *
 */
#pragma once
#ifndef __TMC5160__
#define __TMC5160__

#include "TMC5160_REGISTERS.h"
#include "SHARED/LibPiSPI-master/LibPiSPI/PiSPI.h"


#include "SHARED/loguru-master/loguru.hpp"

class TMC5160
{
	
	
public:
	
	enum MOTOR_ID
	{
			MOTOR_0,
			MOTOR_1
	};
	TMC5160(MOTOR_ID _motor);
	~TMC5160();
	
	void enable_motor();
	void disbale_motor();
	
	void default_settings();
	
	void set_ramp_parameters();
	
	int get_position();
	int get_latched_position();
	int goto_position(int _position);
	void set_current_position(int _position);
	int home();
	int stop();
	
	void hold_mode();
	void position_mode();
	
	bool has_error();
private:
	MOTOR_ID id;
	PiSPI* spi = nullptr;
	
	//SPI SETTINGS
	//THE CHANNEL IS THE /dev/spidev0.x FOR THE CORRESPONDING CS PIN
	const uint8_t SPI_CHANNEL_A = 0;
	const uint8_t SPI_CHANNEL_B = 1;
	
	const int SPI_SPEED = 1000000;
	const int SPI_MODE = 3;
	const uint8_t SPI_BPW = 8;
	
	//MOTOR PARAMETERS
	int RAMP_VSTART = 1;
	int RAMP_A1 = 25000;
	int RAMP_V1 = 250000;
	int RAMP_AMAX = 5000;
	int RAMP_VMAX = 1000000;
	int RAMP_DMAX = 5000;
	int RAMP_D1 = 50000;
	int RAMP_VSTOP = 10;
	
	
	
	int write_spi(int _adress, int _data);
	int read_spi(int _adress);
	int two_complement(int _value, int _bits = 32);
};

#endif

