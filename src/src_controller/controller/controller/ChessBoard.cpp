#include "ChessBoard.h"


	

ChessBoard::ChessBoard(ConfigParser& _configparser) {
	
	///------------ SETUP MOTOR DRIVER /AXIS ------ //
	x_axis = new TMC5160(TMC5160::MOTOR_ID::MOTOR_0);
	y_axis = new TMC5160(TMC5160::MOTOR_ID::MOTOR_1);
	///LOAD DEFAULT SETTINGS
	y_axis->default_settings();
	x_axis->default_settings();
	//OVERRIDE STEPS PER MM IF CONFIG EXISTS
	std::string tmp = _configparser.get(ConfigParser::CFG_ENTRY::MECHANIC_STEPS_PER_MM);
	if (!tmp.empty())
	{
		int spm = atoi(tmp.c_str());
		if (spm >= 0)
		{
			x_axis->steps_per_mm(spm);
			y_axis->steps_per_mm(spm);
		}
	}
	
	
	
	
	///-------- SETUP IO CONTROLLER ------ //
	iocontroller = new IOController();
	if (!iocontroller->isInitialized())
	{
		//TODO LOG ERROR
	}
	//DISABLE COILS
	iocontroller->setCoilState(IOController::COIL::COIL_A, false);
	iocontroller->setCoilState(IOController::COIL::COIL_B, false);
	
	//SET STATUS LED
	iocontroller->setStatusLed(IOController::STAUS_LED_A, true);
	//SET TURN STATE LIGHT
	iocontroller->setTurnStateLight(IOController::TSL_IDLE);
	
	//OVERRIDE POLARITY SETTING OF TH COILS
	tmp = _configparser.get(ConfigParser::CFG_ENTRY::MECHANIC_INVERT_COILS);
	if (!tmp.empty())
	{
		int spm = atoi(tmp.c_str());
		if (spm > 0)
		{
			iocontroller->invertCoilPolarity(IOController::COIL::COIL_A, true);
			iocontroller->invertCoilPolarity(IOController::COIL::COIL_B, true);
		}
	}
	
	
	
	
}
	
ChessBoard::~ChessBoard() {
}


void ChessBoard::log_error(std::string _err)
{
#ifdef USE_STD_LOG
	std::cout << _err << std::endl;
#endif //USE_STD_LOG
//	LOG_F(ERROR, "%s", _err.c_str());
}




ChessBoard::BOARD_ERROR ChessBoard::initBoard()
{
	//CHECK HARDWARE INIT
	if (x_axis == nullptr || y_axis == nullptr || iocontroller == nullptr)
	{
		log_error("ChessBoard::BOARD_ERROR::INIT_NULLPTR_EXECPTION");
		return ChessBoard::BOARD_ERROR::INIT_NULLPTR_EXECPTION;
	}
	
	//FIRST DO HOMEING OF THE AXIS
	x_axis->atc_home_sync();
	y_axis->atc_home_sync();
	
	
}


void ChessBoard::loadBoardPreset(ChessBoard::BOARD_TPYE _target_board, ChessBoard::BOARD_PRESET _preset)
{
}