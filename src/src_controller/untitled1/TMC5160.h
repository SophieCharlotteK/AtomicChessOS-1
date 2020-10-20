//
// Created by Marcel Ochsendorf on 17.10.20.
//

#ifndef UNTITLED1_TMC5160_H
#define UNTITLED1_TMC5160_H



#include <cstdint>
#include <chrono>
#include <thread>
class TMC5160 {


public:
    TMC5160();

    enum MOTOR_ID
    {
        MOTOR_0,  //CS0
        MOTOR_1 //CS1
    };
    TMC5160(MOTOR_ID _motor);

    enum DIRECTION{
        LEFT = 0,
        RIGHT = 1
    };

    void enable_motor();
    void disable_motor();
    void default_settings();
    void set_VSTART(int _value);
    void set_A1(int _value);
    void set_V1(int _value);
    void set_AMAX(int _value);
    void set_VMAX(int _value);
    void set_DMAX(int _value);
    void set_D1(int _value);
    void set_VSTOP(int _value);
    void write_ramp_params();
    void reset_ramp_defaults();
    void enable_switch(DIRECTION _direction);
    int get_latched_position();
    int get_velocity();
    void go_to(int _position);
    void calibrate_home(int _direction);
    void move_velocity(DIRECTION _dir,int  _v_max,int _a_max);
    void stop_motor();
    void hold_mode();
    void position_mode();
    void get_ramp_status();
    int read(int _address);
    int write(int _address,int _data);

    int twos_comp(int _value,int _bits = 32);

    int get_position();

    int steps_per_mm();
    void steps_per_mm(int _steps);

    void move_to_postion_mm_relative(bool _blocking, DIRECTION _dir);
    void move_to_postion_mm_absolute(bool _blocking);
private:
    //RESOLUTION SETTINGS
    int spm = 25600;
    int current_postion = 0;
    //GPIO SETTINGS
    const int MOTOR_ENABLE_MOTOR_0 = 0;
    const int MOTOR_ENABLE_MOTOR_1 = 0;

    //SPI SETTINGS
    const int SPI_SPEED = 1000000;
    const int SPI_MODE = 3;
    const uint8_t SPI_BPW = 8;

    const int CS_GPIO_NUMBER_MOTOR_0 = 10; //USING GPIO 10 = CE_0
    const int CS_GPIO_NUMBER_MOTOR_1 = 11; //USING GPIO 10 = CE_0
    int CS_GPIO = -1;

    //TMC5160 RAMP PARAMETERS
    int RAMP_VSTART = 0;
    int RAMP_A1 = 0;
    int RAMP_V1 = 0;
    int RAMP_AMAX = 0;
    int RAMP_VMAX = 0;
    int RAMP_DMAX = 0;
    int RAMP_D1 = 0;
    int RAMP_VSTOP = 0;
    //TMC5160 REGISTER SPI ADRESSES
    const int REGDEF_GCONF      = 0x00;
    const int REGDEF_GSTAT      = 0x01;
    const int REGDEF_IFCNT      = 0x02;
    const int REGDEF_SLAVECONF  = 0x03;
    const int REGDEF_INP_OUT    = 0x04;
    const int REGDEF_X_COMPARE  = 0x05;
    const int REGDEF_IHOLD_IRUN = 0x10;
    const int REGDEF_TPOWERDOWN = 0x11;
    const int REGDEF_TSTEP      = 0x12;
    const int REGDEF_TPWMTHRS   = 0x13;
    const int REGDEF_TCOOLTHRS  = 0x14;
    const int REGDEF_THIGH      = 0x15;
    const int REGDEF_RAMPMODE   = 0x20;
    const int REGDEF_XACTUAL    = 0x21;
    const int REGDEF_VACTUAL    = 0x22;
    const int REGDEF_VSTART     = 0x23;
    const int REGDEF_A1         = 0x24;
    const int REGDEF_V1         = 0x25;
    const int REGDEF_AMAX       = 0x26;
    const int REGDEF_VMAX       = 0x27;
    const int REGDEF_DMAX       = 0x28;
    const int REGDEF_D1         = 0x2A;
    const int REGDEF_VSTOP      = 0x2B;
    const int REGDEF_TZEROWAIT  = 0x2C;
    const int REGDEF_XTARGET    = 0x2D;
    const int REGDEF_VDCMIN     = 0x33;
    const int REGDEF_SWMODE     = 0x34;
    const int REGDEF_RAMPSTAT   = 0x35;
    const int REGDEF_XLATCH     = 0x36;
    const int REGDEF_ENCMODE    = 0x38;
    const int REGDEF_XENC       = 0x39;
    const int REGDEF_ENC_CONST  = 0x3A;
    const int REGDEF_ENC_STATUS = 0x3B;
    const int REGDEF_ENC_LATCH  = 0x3C;
    const int REGDEF_MSLUT0     = 0x60;
    const int REGDEF_MSLUT1     = 0x61;
    const int REGDEF_MSLUT2     = 0x62;
    const int REGDEF_MSLUT3     = 0x63;
    const int REGDEF_MSLUT4     = 0x64;
    const int REGDEF_MSLUT5     = 0x65;
    const int REGDEF_MSLUT6     = 0x66;
    const int REGDEF_MSLUT7     = 0x67;
    const int REGDEF_MSLUTSEL   = 0x68;
    const int REGDEF_MSLUTSTART = 0x69;
    const int REGDEF_MSCNT      = 0x6A;
    const int REGDEF_MSCURACT   = 0x6B;
    const int REGDEF_CHOPCONF   = 0x6C;
    const int REGDEF_COOLCONF   = 0x6D;
    const int REGDEF_DCCTRL     = 0x6E;
    const int REGDEF_DRVSTATUS  = 0x6F;
    const int REGDEF_PWMCONF    = 0x70;
    const int REGDEF_PWMSTATUS  = 0x71;
    const int REGDEF_ENCM_CTRL  = 0x72;
    const int REGDEF_LOST_STEPS = 0x73;
};


#endif //UNTITLED1_TMC5160_H
