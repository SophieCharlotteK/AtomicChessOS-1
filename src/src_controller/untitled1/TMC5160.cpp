//
// Created by Marcel Ochsendorf on 17.10.20.
//

#include "TMC5160.h"

TMC5160::TMC5160(MOTOR_ID _motor) {

    wiringPiSetup();
    int fd = wiringPiSPISetup(0,SPI_SPEED);
    if(fd<0){
        std::cout << "ERROR ----SPI FD IS" << fd << std::endl;
    }else{
        std::cout << "SPI FD IS " << fd << std::endl;
    }



    if(_id == MOTOR_ID::MOTOR_0){
        CS_GPIO = CS_GPIO_NUMBER_MOTOR_0;

    }else  if(_id == MOTOR_ID::MOTOR_1) {
        CS_GPIO = CS_GPIO_NUMBER_MOTOR_1;
    }

    pinMode(CS_GPIO,OUTPUT);
    digitalWrite(CS_GPIO,HIGH);
}

void TMC5160::enable_motor() {

}

void TMC5160::disable_motor() {

}

void TMC5160::default_settings() {
//# MULTISTEP_FILT = 1, EN_PWM_MODE = 1 enables stealthChop
    write(REGDEF_GCONF, 0x0000000C);
//# TOFF = 3, HSTRT = 4, HEND = 1, TBL = 2, CHM = 0 (spreadCycle)
    write(REGDEF_CHOPCONF, 0x000100C3);
//# IHOLD = 2, IRUN = 15 (max current), IHOLDDELAY = 8
    write(REGDEF_IHOLD_IRUN, 0x00080F02);
//# TPOWERDOWN = 10: Delay before powerdown in standstill
    write(REGDEF_TPOWERDOWN, 0x0000000A);
//# TPWMTHRS = 500
    write(REGDEF_TPWMTHRS, 0x000001F4);

    reset_ramp_defaults();

//# Position mode
    write(REGDEF_RAMPMODE, 0);
//# Set current position to 0
    write(REGDEF_XACTUAL, 0);
//# Set XTARGET to 0, which holds the motor at the current position
    write(REGDEF_XTARGET, 0);
}

void TMC5160::set_VSTART(int _value) {
    write(REGDEF_VSTART, _value);
    RAMP_VSTART = _value;
}

void TMC5160::set_A1(int _value) {
    write(RAMP_A1, _value):
    RAMP_A1 = _value;
}

void TMC5160::set_V1(int _value) {
    write(RAMP_V1, _value);
    RAMP_V1 = _value;
}

void TMC5160::set_AMAX(int _value) {
    write(RAMP_AMAX, _value):
    RAMP_AMAX = _value;
}

void TMC5160::set_VMAX(int _value) {
    write(RAMP_VMAX, _value);
    RAMP_VMAX = _value;
}

void TMC5160::set_DMAX(int _value) {
    write(RAMP_DMAX, _value);
    RAMP_DMAX = _value;
}

void TMC5160::set_D1(int _value) {
    write(RAMP_D1, _value);
    RAMP_D1 = _value;
}

void TMC5160::set_VSTOP(int _value) {
    write(RAMP_VSTOP, _value);
    RAMP_VSTOP = _value;
}

void TMC5160::write_ramp_params() {
    set_VSTART(RAMP_VSTART);
    set_A1(RAMP_A1);
    set_V1(RAMP_V1);
    set_AMAX(RAMP_AMAX);
    set_VMAX(RAMP_VMAX);
    set_DMAX(RAMP_DMAX);
    set_D1(RAMP_D1);
    set_VSTOP(RAMP_VSTOP);
}

void TMC5160::reset_ramp_defaults() {
    RAMP_VSTART = 1;
    RAMP_A1 = 25000;
    RAMP_V1 = 250000;
    RAMP_AMAX = 5000;
    RAMP_VMAX = 1000000;
    RAMP_DMAX = 5000;
    RAMP_D1 = 50000;
    RAMP_VSTOP = 10;
    write_ramp_params();
}

void TMC5160::enable_switch(DIRECTION _direction) {

}

int TMC5160::get_latched_position() {
    int current_position = read(REGDEF_XLATCH);
    current_position = twos_comp(current_position);
    return current_position;
}

int TMC5160::get_velocity() {
    int current_velocity = read(REGDEF_VACTUAL);
    //# Convert 2's complement to get signed number
    //# VACTUAL is valid for +-(2^23)-1, so 24 bits
    //# 24 bits optional argument
    current_velocity = twos_comp(current_velocity, 24);
    return current_velocity;
}

void TMC5160::go_to(int _position) {
    position_mode();
    write_ramp_params();
    write(REGDEF_XTARGET, _position);
}

void TMC5160::calibrate_home(int _direction) {

}


void TMC5160::move_velocity(DIRECTION _dir, int _v_max, int _a_max) {

    write(REGDEF_VMAX, _v_max);
    write(REGDEF_AMAX, _a_max);

    bool error = false;
    int velocity_mode = 0;
    if (_dir == DIRECTION::LEFT){
        velocity_mode = 1;
        error = false;
    }else if(_dir == DIRECTION::RIGHT){
        velocity_mode = 2;
        error = false;
    }else{
        error = true;
    }

    if(!error){
        write(REGDEF_RAMPMODE, velocity_mode);
    }

}

void TMC5160::stop_motor() {
    move_velocity(DIRECTION::LEFT, 0,RAMP_AMAX);
    while (get_velocity() != 0){
        std::this_thread::sleep_for(std::chrono::microseconds (1));
    }
    hold_mode();
    set_VMAX(RAMP_VMAX);
}

void TMC5160::hold_mode() {
    write(REGDEF_RAMPMODE, 3);
}

void TMC5160::position_mode() {
    write(REGDEF_RAMPMODE, 0);
}

void TMC5160::get_ramp_status() {

}

int TMC5160::read(int _address) {
    const size_t DATA_LEN = 5;
    uint8_t address_buffer[] = {0,0,0,0,0};
    uint8_t read_buffer[] = {0,0,0,0,0};
    int value = 0;
    int res = 0;
    address_buffer[0] = _address & 0x7F;
    read_buffer[0] = _address & 0x7F;

    //SEND DATA
    digitalWrite(CS_GPIO,LOW);
    res = wiringPiSPIDataRW(0,address_buffer,DATA_LEN);
    digitalWrite(CS_GPIO,HIGH);
    //It will look like [address, 0, 0, 0, 0]

    //READ DATA
    digitalWrite(CS_GPIO,LOW);
    res = wiringPiSPIDataRW(0,read_buffer,DATA_LEN);
    digitalWrite(CS_GPIO,HIGH);

    //PARSE RETURNED DATA
    value = read_buffer[1];
    value = value << 8;
    value |= read_buffer[2];
    value = value << 8;
    value |= read_buffer[3];
    value = value << 8;
    value |= read_buffer[4];

    return value;
}

int TMC5160::write(int _address, int _data) {
    const size_t DATA_LEN = 5;
    uint8_t write_buffer[] = {0,0,0,0,0};


    write_buffer[0] = _address | 0x80;

    write_buffer[1] = 0xFF & (_data >> 24);
    write_buffer[2] = 0xFF & (_data >> 16);
    write_buffer[3] = 0xFF & (_data >> 8);
    write_buffer[4] = 0xFF & _data;

    digitalWrite(CS_GPIO,LOW);
    int res = wiringPiSPIDataRW(0,write_buffer,DATA_LEN);
    std::cout <<"wb "<< res<<std::endl;
    digitalWrite(CS_GPIO,HIGH);


    return res;
}



int TMC5160::twos_comp(int _value, int _bits) {
    signed int signed_value = 0;
    if ((_value & (1 << (_bits - 1))) != 0) {
        signed_value = _value - (1 << _bits);
    }
    else {
        signed_value = _value;
    }
    return signed_value;
}

int TMC5160::get_position() {
    int current_position = read(REGDEF_XACTUAL);
    current_position = twos_comp(current_position);
    return current_position;
}

int TMC5160::steps_per_mm() {
    return spm;
}

void TMC5160::steps_per_mm(int _steps) {
spm = _steps;
}
