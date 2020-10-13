#include "TMC5160.h"


TMC5160::TMC5160(MOTOR_ID _motor){
	
	id = _motor;
	int spi_channel = 0;
	if (_motor == TMC5160::MOTOR_1) {
		spi_channel = SPI_CHANNEL_B;
	}
	else {
		spi_channel = SPI_CHANNEL_B;
	}
	
	//OPEN THE SPI PORT
 	PiSPI t = PiSPI(0, SPI_SPEED, SPI_MODE, SPI_BPW);
	spi = &t;
	
	
	
	
}

TMC5160::~TMC5160() {
	
}



void TMC5160::enable_motor()
{
}


void TMC5160::disbale_motor()
{
}


void TMC5160::default_settings()
{
	
	write_spi(GCONF, 0x0000000C); //EN_PWM_MODE
//# TOFF = 3, HSTRT = 4, HEND = 1, TBL = 2, CHM = 0 (spreadCycle)
    write_spi(CHOPCONF, 0x000100C3);
	//# IHOLD = 2, IRUN = 15 (max current), IHOLDDELAY = 8
	write_spi(IHOLD_IRUN, 0x00080F02);
	//# TPOWERDOWN = 10: Delay before powerdown in standstill
	write_spi(TPOWERDOWN, 0x0000000A);
	//# TPWMTHRS = 500
	write_spi(TPWMTHRS, 0x000001F4);
	
	set_ramp_parameters();//WRITE THE DEFAULT RAMP PARAMETERS
	
	
	//# Position mode
	write_spi(RAMPMODE, 0);
	//# Set current positin to 0
	write_spi(XACTUAL, 0);
	//# Set XTARGET to 0, which holds the motor at the current position
	write_spi(XTARGET, 0);
}


void TMC5160::set_ramp_parameters()
{
	write_spi(VSTART, RAMP_VSTART);
	write_spi(A1, RAMP_A1);
	write_spi(V1, RAMP_V1);
	write_spi(AMAX, RAMP_AMAX);
	write_spi(VMAX, RAMP_VMAX);
	write_spi(DMAX, RAMP_DMAX);
	write_spi(D1, RAMP_D1);
	write_spi(VSTOP, RAMP_VSTOP);
	
}


int TMC5160::read_spi(int _adress)
{
}


int TMC5160::write_spi(int _adress, int _data)
{
}


int TMC5160::two_complement(int _value, int _bits /* = 32 */)
{
	signed int signed_value = 0;
	if ((_value & (1 << (_bits - 1))) != 0) {
		signed_value = _value - (1 << _bits);
	}
	else {
		signed_value = _value;
	}
	return signed_value;
}





int TMC5160::get_position()
{
	return two_complement(read_spi(XACTUAL));
}


int TMC5160::get_latched_position()
{
	return two_complement(read_spi(XLATCH));
}
