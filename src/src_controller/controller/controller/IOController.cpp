#include "IOController.h"


IOController::IOController()
{
	
	if (!SPICommunication::getInstance()->isInitialised()) {
		if (SPICommunication::getInstance()->lastErrorCode() != 0) {
			return;
		}
	}
	
	SPI_CS_DEVICE = SPICommunication::SPI_DEVICE::IO_CONTROLLER;
	SPICommunication::getInstance()->register_cs_gpio(SPI_CS_DEVICE, SPI_CS_GPIO);
}
