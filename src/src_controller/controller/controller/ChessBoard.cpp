#include "ChessBoard.h"


	

ChessBoard::ChessBoard() {
	
	///------------ SETUP MOTOR DRIVER /AXIS ------ //
	x_axis = new TMC5160(TMC5160::MOTOR_ID::MOTOR_0);
	y_axis = new TMC5160(TMC5160::MOTOR_ID::MOTOR_1);
	///LOAD DEFAULT SETTINGS
	y_axis->default_settings();
	x_axis->default_settings();
	//OVERRIDE STEPS PER MM IF CONFIG EXISTS
	std::string tmp = ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::MECHANIC_STEPS_PER_MM);
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
	tmp =  ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::MECHANIC_INVERT_COILS);
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
	
	
	travelToField(ChessField::CHESS_FIELD_A1);
	
	
	//scanBoard(board_current);
	//printBoard();
	
	//loadBoardPreset(ChessBoard::TARGET_BOARD, ChessBoard::BOARD_PRESET_ALL_FIGURES_IN_START_POSTITION);
	
//	syncRealWithTargetBoard();
}


void ChessBoard::loadBoardPreset(ChessBoard::BOARD_TPYE _target_board, ChessBoard::BOARD_PRESET _preset)
{
	//CLEAR BOARD
	ChessPiece::FIGURE invlaid_piece;
	invlaid_piece.color = ChessPiece::COLOR_UNKNOWN;
	invlaid_piece.type = ChessPiece::TYPE_INVALID;
	invlaid_piece.unique_id = 0;
	invlaid_piece.figure_number = -1;
	
	
	for(size_t w = 0 ; w < BOARD_WIDTH ; w++)
	{
		for (size_t h = 0; h < BOARD_HEIGHT; h++)
		{
			 
		}		 
	}
	
	
	//LOAD PRESET
	if(_preset == ChessBoard::BOARD_PRESET::BOARD_PRESET_CLEARED) {
		return;
		
	}else if(_preset == ChessBoard::BOARD_PRESET::BOARD_PRESET_ALL_FIGURES_IN_START_POSTITION) {//LOAD DEFAULT FEN TO BOARD
	
		std::string tmp = ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::BOARD_PRESET_START_POSITION_FEN);
		//IF CONFIG FEN IS EMPTY USE A DEFAULT
		if (tmp.empty())
		{
			tmp = DEFAULT_BOARD_FEN;
		}
		if (!boardFromFen(tmp, _target_board))
		{
			log_error("ChessBoard::BOARD_ERROR::INIT_NULLPTR_EXECPTION");
		}
	
		
		
	}else if(_preset == ChessBoard::BOARD_PRESET::BOARD_PRESET_ALL_FIGURES_IN_PARK_POSITION) //LOAD ALL FIGURES IN PARTKIN POSITION BOARD
	{
		//TODO
	}
	
	
}