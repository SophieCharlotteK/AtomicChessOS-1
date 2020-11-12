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

void ChessBoard::getParkPositionCoordinates(int _index, int& _x, int& _y, bool before_parkpostion_entry)
{
	//INDEX IS NOT A PARK POSITION
	if (_index < 64)
	{
		_x = -1;
		_y = -1;
		return;
	}
	
	//TODO
}
//CONVERTS FIELD ID TO THE X Y  COORDINATES 0-7 0-7
void ChessBoard::getFieldCoordinates(int _index, int& _x, int& _y, bool _get_only_array_index, bool _get_field_center)
{
	
	//TODO CHECK PARK POSTION
	if(_index > 63)
	{
		_x = -1;
		_y = -1;
		return;
	}
	
	
	int offset_x = 0;
	int offset_y = 0;
	//LOAD OFFSET FROM CONFIG FILE IF HOME POS IS NOT IN FIELD H1
	if(!ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::MECHANIC_H1_OFFSET_MM_X).empty()) {
		offset_x = atoi(ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::MECHANIC_H1_OFFSET_MM_X).c_str());
	}
	if (!ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::MECHANIC_H1_OFFSET_MM_Y).empty()) {
		offset_x = atoi(ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::MECHANIC_H1_OFFSET_MM_Y).c_str());
	}
	
	
	int board_width = 0;
	//LOAD OFFSET FROM CONFIG FILE IF HOME POS IS NOT IN FIELD H1
	if(!ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::MECHANIC_CHESS_BOARD_WIDTH).empty()) {
		board_width = atoi(ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::MECHANIC_CHESS_BOARD_WIDTH).c_str());
	}
	
	int field_width = 50;
	
	if (!ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::MECHANIC_CHESS_FIELD_WIDTH).empty()) {
		field_width = atoi(ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::MECHANIC_CHESS_FIELD_WIDTH).c_str());
	}
	
	
	///CONVERT TO XY WITH A NORMAL CHESS FIELD WITH 8 by 8 FIELDS
	if(_get_field_center)
	{
		offset_x -= field_width/2;
		
	}else
	{
		offset_y += field_width / 2;
	}
	
	
	if(_get_only_array_index) {
		_x = (_index / 8);
		_y = (_index % 8);
	}else
	{
		_x = board_width-((_index / 8)*field_width) + offset_x;
		_y = ((_index % 8)*field_width) + offset_y;
	}
	

}
	

ChessBoard::BOARD_ERROR ChessBoard::scanBoard(ChessPiece::FIGURE(&board)[BOARD_WIDTH][BOARD_HEIGHT])
{
	//SAVE CURRENT FIELD	
	ChessField::CHESS_FILEDS _original_position = current_field;
	//GOTO EACH CHESS BOARD FIELD
	int x = 0;
	int y = 0;
	for (int i = 0; i < ChessField::CHESS_FIELD_PARK_POSTION_WHITE_1; i++)
	{
		getFieldCoordinates(i, x, y, true, false);
		travelToField(static_cast<ChessField::CHESS_FILEDS>(i), true);
		ChessPiece::FIGURE tmop = iocontroller->ScanNFC(10);
		ChessPiece::FigureDebugPrint(tmop);
		board[x][y] = tmop;
	}
	
	//TODO SCAN PARK POSITION
	
	travelToField(_original_position,true);
	
	return ChessBoard::BOARD_ERROR::NO_ERROR;
}
	
	
	
ChessBoard::BOARD_ERROR ChessBoard::travelToField(ChessField::CHESS_FILEDS _field, bool _to_field_center)
{
	
	
	
	
	//SETUP_MOTORS
	x_axis->atc_set_speed_preset(TMC5160::TRAVEL);
	y_axis->atc_set_speed_preset(TMC5160::TRAVEL);
	
	
	//GET FIELD COORDIANTES
	int field_coordinates_x = -1;
	int field_coordinates_y = -1;
	
	//GET FIELD ID
	int field_index = static_cast<int>(_field);
	if (field_index < 64)
	{
		getFieldCoordinates(field_index, field_coordinates_x, field_coordinates_y, false, _to_field_center);
	}
	else
	{
		getParkPositionCoordinates(field_index, field_coordinates_x, field_coordinates_y, _to_field_center);
	}
	//CONVERSION ERROR HANDLING
	if (field_coordinates_x == -1 && field_coordinates_y == -1)
	{
		return ChessBoard::BOARD_ERROR::AXIS_TRAGET_ARRIVAL_FAILED;
	}
	
	//FINALLY MOVE TO POSITION
	/// NON BLOCKING BECUASE WE WANT TO MOVE SIMULATNIOUSLY
	x_axis->move_to_postion_mm_absolute(field_coordinates_x, false);
	y_axis->move_to_postion_mm_absolute(field_coordinates_y, false);
	//WAIT FOR MOTORS TO REACH POSTION
	int wait_counter = 0;
	while (!(x_axis->is_target_position_reached() && y_axis->is_target_position_reached())) {
		std::this_thread::sleep_for(std::chrono::milliseconds(WAITITME_FOR_MOTORS_TO_ARRIVE));
		wait_counter++;
		if (wait_counter > WAITIME_MULTIPLIER_AXIS_ARRIVAL)
		{
			break;
		}
	}
	
	
	
	//SWITCH COIL IF NESSESSARY
	//WENN FELD > ALS SWITCH FELD
	int coil_switch_threashhold = -1;
	if (!ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::MECHANIC_COIL_SWITCH_POSTION_TRIGGER).empty()) {
		coil_switch_threashhold = atoi(ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::MECHANIC_COIL_SWITCH_POSTION_TRIGGER).c_str());
	}
	
	if (coil_switch_threashhold >= 0)
	{
		if (field_coordinates_x >= coil_switch_threashhold)
		{
			switch_coil(IOController::COIL::COIL_B, false);
		}
		else
		{
			switch_coil(IOController::COIL::COIL_A, false);
		}
	}
	
	
	
	
	
	//DISBALE MOTOR
	x_axis->disable_motor();
	y_axis->disable_motor();
	
	
	//CHECK IF MOTORS ARRIVED TARGET POSITION WITH NO ERROR
 	if(wait_counter > WAITIME_MULTIPLIER_AXIS_ARRIVAL)
	{
		log_error("ChessBoard::BOARD_ERROR::AXIS_TRAGET_ARRIVAL_FAILED");
		return ChessBoard::BOARD_ERROR::AXIS_TRAGET_ARRIVAL_FAILED;
	}
	
	
	
	
	current_field = _field;
	return ChessBoard::BOARD_ERROR::NO_ERROR;
}



ChessBoard::BOARD_ERROR ChessBoard::switch_coil(IOController::COIL _coil, bool _activate_swtiched_coil)
{
	//DO NOTHING IF WE ARE ON THE RIGHT COIL
	if (current_selected_coil == _coil)
	{
		return ChessBoard::BOARD_ERROR::NO_ERROR;
	}
		
	//TURN OFF COILS
	iocontroller->setCoilState(IOController::COIL::COIL_A, false);
	iocontroller->setCoilState(IOController::COIL::COIL_B, false);
	//SWITCH POSITION
	int coil_distance = 0;
	if (!ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::MECHANIC_DISTANCE_COILS_MM).empty()) {
		coil_distance = atoi(ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::MECHANIC_DISTANCE_COILS_MM).c_str());
	}
	;
	coil_distance = coil_distance / 2;
	///IF SWITCHED TO B COIL THE TRAVEL DISTANCE IS NEGATIVE
	if(_coil == IOController::COIL::COIL_A)
	{
		coil_distance = std::abs(coil_distance);
	}else
	{
		coil_distance = -std::abs(coil_distance);
	}
	
	//MOVE RELATIVE TO NEW POSITION
	x_axis->move_to_postion_mm_relative(coil_distance, true);
	
	
	//SWITCH ON OTHER COIL
	iocontroller->setCoilState(_coil, _activate_swtiched_coil);
	//SAVE CURRENT COIL
	current_selected_coil = _coil;
	return ChessBoard::BOARD_ERROR::NO_ERROR;
	
}


ChessBoard::BOARD_ERROR ChessBoard::initBoard()
{
	//CHECK HARDWARE INIT
	if(x_axis == nullptr || y_axis == nullptr || iocontroller == nullptr)
	{
		log_error("ChessBoard::BOARD_ERROR::INIT_NULLPTR_EXECPTION");
		return ChessBoard::BOARD_ERROR::INIT_NULLPTR_EXECPTION;
	}
	iocontroller->setStatusLed(IOController::STAUS_LED_A, false);
	iocontroller->setTurnStateLight(IOController::TSL_PRECCESSING);
	iocontroller->setTurnStateLight(IOController::TSL_PRECCESSING);
	//FIRST DO HOMEING OF THE AXIS
	x_axis->atc_home_sync();
	y_axis->atc_home_sync();
	
	
	//iocontroller->setCoilState(IOController::COIL_A, true);
	current_selected_coil = IOController::COIL_A;
	//MOVE TO A1 FIELD
	travelToField(ChessField::CHESS_FIELD_A1,true);

	
	
	scanBoard(board_current);
	//printBoard();
	iocontroller->setStatusLed(IOController::STAUS_LED_A, true);
	iocontroller->setTurnStateLight(IOController::TSL_IDLE);
	iocontroller->setTurnStateLight(IOController::TSL_IDLE);
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
	
	
	for (size_t w = 0; w < BOARD_WIDTH; w++)
	{
		for (size_t h = 0; h < BOARD_HEIGHT; h++)
		{
			 
		}		 
	}
	
	
	//LOAD PRESET
	if(_preset == ChessBoard::BOARD_PRESET::BOARD_PRESET_CLEARED) {
		return;
		
	}else if(_preset == ChessBoard::BOARD_PRESET::BOARD_PRESET_ALL_FIGURES_IN_START_POSTITION) {
		//LOAD DEFAULT FEN TO BOARD
	
		std::string tmp = ConfigParser::getInstance()->get(ConfigParser::CFG_ENTRY::BOARD_PRESET_START_POSITION_FEN);
		//IF CONFIG FEN IS EMPTY USE A DEFAULT
		if(tmp.empty())
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