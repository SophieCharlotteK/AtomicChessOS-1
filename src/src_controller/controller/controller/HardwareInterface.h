#ifndef __HARDWAREINTERFACE_H__
#define __HARDWAREINTERFACE_H__


//THIS CLASS WAS CREATED FOR THE NEW V2 ELECTROIC TO ENABLE USAGE OF THE NEW MOTOR CONTROLLER ELECTRONICS
//IT WAS CREATED TO COLLECT EVERY HARDWARE ACCES THOUGH ONE CLASS AND MAKE V1 AND V2 ELECTRONIC USABLE

#include <thread>
#include <mutex>

//3rd party
#include "SHARED/loguru-master/loguru.hpp"

//ATC CLASSES
#include "ConfigParser.h"
#include "ChessPiece.h"
//--- V1 HARDWARE MODULES --------- //
//USING SPI FOR TMC5160 AND ARDUINO NANO FOR COILS
#include "IOController.h"
#include "TMC5160.h"

//--- V2 HARDWARE MODULES -------- //
#include "GCodeSender.h"
#include "UserBoardController.h" //READ ARDUINO BASED NFC TAG USING PN532



//------ GENERAL HARDWARE ------ //
#include "BoardUserMoveWatcher.h"


class HardwareInterface
{
public:
	enum HI_HARDWARE_REVISION {
		HI_HWREV_UNKNOWN = 0,
		HI_HWREV_DK   = 1, //FIRST 55x55cm ATC TABLE WITH TWO COILS
		HI_HWREV_PROD = 2, //SECONDS GENERATION BASED ON SKR1.3 3D PRINT CONTROLLER
		HI_HWREV_PROD_V2 =3,  //THIRD GENERATION WITH SKR 1.4 WITH CORE XY MECHANIC
		HI_HWREV_VIRT=4, //SIMULATED HW FOR TESTING USING THE DOCKERFILE
	};
	enum HI_TRAVEL_SPEED_PRESET {
		HI_TSP_MOVE   = 0,
		HI_TSP_TRAVEL = 1
	};
	enum HI_TURN_STATE_LIGHT
	{
		HI_TSL_OFF               = 0,
		HI_TSL_IDLE              = 1,
		HI_TSL_PLAYER_WHITE_TURN = 2,
		HI_TSL_PLAYER_BLACK_TURN = 3,
		HI_TSL_PRECCESSING       = 4
	};
	enum HI_COIL
	{
		HI_COIL_A   = 0,
		HI_COIL_B   = 1,
		HI_COIL_NFC = 2
	};

	enum HI_FAN{
	    HI_FAN_MOTOR_DRIVER = 0
	};


	static HardwareInterface* getInstance(); 
	
	
	
	//USED FUNCTIONS FROM THE IO CONTROLLER  (CONTROL COILS,LED, NFC READER)
	bool setTurnStateLight(HI_TURN_STATE_LIGHT _state);
	bool setCoilState(HI_COIL _coil, bool _state);
	ChessPiece::FIGURE ScanNFC();
	
	
	
	
	//USED FUNCTION FOR MOTORS
	//NOW THERE IS AN INTERFACE FOR BOTH X AND Y MOTORS COMBINED
	void enable_motors();
	void disable_motors();
	void set_fan_state(HI_FAN _fan, bool _state);
	bool is_target_position_reached();
	void move_to_postion_mm_absolute(int _x,int _y, bool _blocking);
	void home_sync();
	
	void set_speed_preset(HI_TRAVEL_SPEED_PRESET _preset);
	
	bool init_hardware(HI_HARDWARE_REVISION _hwrev);
	
	//GENERAL FUNCTIONS
	bool check_hw_init_complete();
	
	
	bool is_production_hardware();
	bool is_simulates_hardware();
private:
	
	
	static HardwareInterface* instance;
	static std::mutex acces_lock_mutex;
	
	HI_HARDWARE_REVISION hwrev;
	bool init_complete = false;
	//------ V1 HARDWARE ------ //
	TMC5160* tmc5160_x = nullptr;
	TMC5160* tmc5160_y = nullptr;
	IOController* iocontroller = nullptr;
	//----- V2 HARDWARE ------- //
	GCodeSender* gcode_interface = nullptr;
	UserBoardController* userboardcontroller_interface = nullptr;
	HardwareInterface();
	~HardwareInterface();
};

#endif