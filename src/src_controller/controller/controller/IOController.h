#pragma once
#ifndef __IOCONTROLLER_H__
#define __IOCONTROLLER_H__


#include <iostream>
#include "SPICommunication.h"
#include "ChessPiece.h"

class IOController
{
	
	
public:
	
	enum COIL
	{
		COIL_A,
		COIL_B
	};
	
	enum STATUS_LED
	{
		STAUS_LED_A,
		STATUS_LED_B
	};
	
	IOController();
	~IOController();
	
	ChessPiece::TYPE ScanNFC(int _retry_count = 1);
	
	void setCoilState(IOController::COIL _coil, bool _state);
	void setStatusLed(IOController::STATUS_LED _led, bool _state);
	void setCoilStrenght(IOController::COIL _coil, uint8_t _strenght, bool _invert_field);
	
private:
	
	const int SPI_CS_GPIO = 3; //WIRING PI NUMBERING
	SPICommunication::SPI_DEVICE SPI_CS_DEVICE;
};
#endif


