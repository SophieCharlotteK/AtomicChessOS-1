//
// Created by Marcel Ochsendorf on 18.10.20.
//

#include "SPICommunication.h"


static SPICommunication* SPICommunication::getInstance()
{
    {
        std::lock_guard<std::mutex> lck (mtx);
        if (instance == nullptr)
        {
            instance = new SPICommunication();
        }
        return instance;
    }
}

SPICommunication::SPICommunication()
{
    //SETUP WIRING PI
    //ITS POSSIBLE TP CALL THE SETUP ROUTINE MULTIBLE TIMES
    //WIRINGPI CHECK FOR THIS
    wiringPiSetup();

    //TRY TO GET OPEN THE SPI BUF (FILE)
	int fd = wiringPiSPISetup(0, SPI_SPEED);
	if (fd < 0) {
		std::cout << "ERROR ----SPI FD IS" << fd << std::endl;
        last_error = 1;
        return;
	}
	
    is_initialised = true;
}


int SPICommunication::lastErrorCode(){
    int tmp = last_error;
    last_error = 0;
    return tmp;
}

int SPICommunication::spi_send(SPICommunication::SPI_DEVICE _device, uint8_t* _data, int _len){

    int res = 0;
    disable_device(_device);
    wiringPiSPIDataRW(0, _data, _len);
    enable_device(_device);
    //deaktivate alle cs pins bis auf device
    //sende
    //deaktiviere alle
return res;
}

int SPICommunication::spi_write(SPICommunication::SPI_DEVICE _device, uint8_t* _data, int _len){
return 0;
}




bool SPICommunication::isInitialised(){
    return is_initialised;
}


void SPICommunication::disable_devices(){

disable_device(SPICommunication::SPI_DEVICE::MOTOR_0);
disable_device(SPICommunication::SPI_DEVICE::MOTOR_1);
disable_device(SPICommunication::SPI_DEVICE::MAGNET_CONTROLLER);
disable_device(SPICommunication::SPI_DEVICE::RESERVED_1);
disable_device(SPICommunication::SPI_DEVICE::RESERVED_2);

}

void SPICommunication::disable_device(SPICommunication::SPI_DEVICE _device){

}

void SPICommunication::enable_device(SPICommunication::SPI_DEVICE _device){

}
