#pragma once
#ifndef __TMC5160_REGISTERS__
#define __TMC5160_REGISTERS__

//https : //www.trinamic.com/fileadmin/assets/Products/ICs_Documents/TMC5160_Datasheet_V1.01.pdf

#define GCONF        0x00
#define GSTAT        0x01
#define IFCNT        0x02
#define SLAVECONF    0x03
#define INP_OUT      0x04
#define X_COMPARE    0x05
#define IHOLD_IRUN   0x10
#define TPOWERDOWN   0x11
#define TSTEP        0x12
#define TPWMTHRS     0x13
#define TCOOLTHRS    0x14
#define THIGH        0x15
#define RAMPMODE     0x20
#define XACTUAL      0x21
#define VACTUAL      0x22
#define VSTART       0x23
#define A1           0x24
#define V1           0x25
#define AMAX         0x26
#define VMAX         0x27
#define DMAX         0x28
#define D1           0x2A
#define VSTOP        0x2B
#define TZEROWAIT    0x2C
#define XTARGET      0x2D
#define VDCMIN       0x33
#define SWMODE       0x34
#define RAMPSTAT     0x35
#define XLATCH       0x36
#define ENCMODE      0x38
#define XENC         0x39
#define ENC_CONST    0x3A
#define ENC_STATUS   0x3B
#define ENC_LATCH    0x3C
#define MSLUT0       0x60
#define MSLUT1       0x61
#define MSLUT2       0x62
#define MSLUT3       0x63
#define MSLUT4       0x64
#define MSLUT5       0x65
#define MSLUT6       0x66
#define MSLUT7       0x67
#define MSLUTSEL     0x68
#define MSLUTSTART   0x69
#define MSCNT        0x6A
#define MSCURACT     0x6B
#define CHOPCONF     0x6C
#define COOLCONF     0x6D
#define DCCTRL       0x6E
#define DRVSTATUS    0x6F
#define PWMCONF      0x70
#define PWMSTATUS    0x71
#define ENCM_CTRL    0x72
#define LOST_STEPS   0x73

	
#endif