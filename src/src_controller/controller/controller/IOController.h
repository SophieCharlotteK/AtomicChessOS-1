#pragma once
#ifndef __IOCONTROLLER_H__
#define __IOCONTROLLER_H__


#include <iostream>
#include <chrono>
#include <thread>



#include "SPICommunication.h"
#include "ChessPiece.h"


#define NFC_READ_ACK_DELAY 10 ///HOW LONG TO WAIT FOR NFC DATA IN MS
class IOController
{
	
	
public:
	
	enum COIL
	{
		COIL_A = 0,
		COIL_B = 1
	};
	
	enum LED
	{
		STAUS_LED_A = 0,
		STATUS_LED_B = 1
	};
	
	IOController();
	~IOController();
	
	ChessPiece::FIGURE ScanNFC(int _retry_count = 1);
	
	void setCoilState(IOController::COIL _coil, bool _state);
	void setStatusLed(IOController::LED _led, bool _state);
	void setCoilStrenght(IOController::COIL _coil, uint8_t _strenght, bool _invert_field);
	bool isInitialized();
private:
	///THIS ENUM CONTAINS ALL POSSIBLE COMMANDS TO THE IO BOARD
	enum COMMANDS
	{
		NONE = 0,
		SET_STATUS_LED = 1,
		DISABLE_STATUS_LED = 2,
		STATUS_LED = 3,
		RESERVED = 4,
		SET_COIL_A = 5,
		DISABLE_COIL_A = 6,
		SET_COIL_B = 7,
		DISABLE_COIL_B = 8,
		NFC_READ_RQUEST = 9,
		NFC_READ_ANSWER = 10,
		NFC_READ_CANCEL = 0,
	};
	bool board_init_success = false;
	const int SPI_CS_GPIO = 3; //WIRING PI NUMBERING
	SPICommunication::SPI_DEVICE SPI_CS_DEVICE;
};
#endif


