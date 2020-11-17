#include "ChessBoard.h"


	

ChessBoard::ChessBoard() {
	///------------ SETUP MOTOR DRIVER /AXIS ------ //
	x_axis = new TMC5160(TMC5160::MOTOR_ID::MOTOR_0);
	y_axis = new TMC5160(TMC5160::MOTOR_ID::MOTOR_1);
	///LOAD DEFAULT SETTINGS
	y_axis->default_settings();
	x_axis->default_settings();
	///OVERRIDE STEPS PER MM IF CONFIG EXISTS
	int spm = 0;
	ConfigParser::getInstance()->get_int(ConfigParser::CFG_ENTRY::MECHANIC_STEPS_PER_MM, spm);
	if (spm > 0)
	{
		x_axis->steps_per_mm(spm);
		y_axis->steps_per_mm(spm);
	}
	///-------- SETUP IO CONTROLLER ------ //
	iocontroller = new IOController();
	if (!iocontroller->isInitialized())
	{
		//TODO LOG ERROR
	}
	///DISABLE COILS
	iocontroller->setCoilState(IOController::COIL::COIL_A, false);
	iocontroller->setCoilState(IOController::COIL::COIL_B, false);
	
	///SET STATUS LED
	iocontroller->setStatusLed(IOController::STAUS_LED_A, true);
	///SET TURN STATE LIGHT
	iocontroller->setTurnStateLight(IOController::TSL_IDLE);
	//OVERRIDE POLARITY SETTING OF TH COILS
	int ivcoils = 0;
	ConfigParser::getInstance()->get_int(ConfigParser::CFG_ENTRY::MECHANIC_INVERT_COILS, ivcoils);
	if (ivcoils > 0)
	{
		iocontroller->invertCoilPolarity(IOController::COIL::COIL_A, true);
		iocontroller->invertCoilPolarity(IOController::COIL::COIL_B, true);
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

void ChessBoard::getParkPositionCoordinates(ChessField::CHESS_FILEDS _index, int& _x, int& _y, IOController::COIL _coil, bool before_parkpostion_entry)
{
	//INDEX IS NOT A PARK POSITION
	if(!isFieldParkPosition(_index))
	{
		_x = -1;
		_y = -1;
		return;
	}
	//DETERM IF PARKING POSITION IS BLACK OR WHITE SLOT
	int is_black_park_position = 0;
	int field_index = static_cast<int>(_index);
	if (field_index >= static_cast<int>(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_1) && field_index <= static_cast<int>(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_16)) {
		is_black_park_position = 1;
	}
	else if (field_index >= static_cast<int>(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_1) && field_index <= static_cast<int>(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_16)) {
		is_black_park_position = 2;
	}
	
	if (is_black_park_position == 0) //NOT WHITE OR BLACK
	{
		return; //ERROR
	}
		
	//NOW LOAD THE OFFSETS OF THE PARKPOSTION WHITE OR BLACK SITE
	int line_offset = 0;
	int cell_size = 0;
	int y_offset = 0;
	int before_line_offset = 0;
	
	if (is_black_park_position ==2) //BLACK
	{
		ConfigParser::getInstance()->get_int(ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_BLACK_X_LINE, line_offset);
		ConfigParser::getInstance()->get_int(ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_BLACK_FIRST_Y_OFFSET, y_offset);
	}
	else
	{
		ConfigParser::getInstance()->get_int(ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_WHITE_X_LINE, line_offset);
		ConfigParser::getInstance()->get_int(ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_WHITE_FIRST_Y_OFFSET, y_offset);
	}
		
		
	//LOAD PARKING SLOT CELL SIZE
	ConfigParser::getInstance()->get_int(ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_CELL_SIZE, cell_size);
	ConfigParser::getInstance()->get_int(ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_CELL_BEFORE_OFFSET, before_line_offset);
	
	
	//GET PARK POSITON INDEX 1-16
	int pp_entry = 0;
	if (is_black_park_position == 2) //BLACK
	{
		pp_entry = (_index - ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_1) % 16;
	}
	else
	{
		pp_entry = (_index - ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_1) % 16;
	}
	
	if (before_parkpostion_entry)
	{
		if (is_black_park_position ==2) //BLACK
		{
			_x = line_offset + before_line_offset;
		}
		else
		{
			_x = line_offset - before_line_offset;
		}
			
			
		_y = (pp_entry*cell_size) + y_offset;
	}
	else
	{
		_x = line_offset;
		_y = (pp_entry*cell_size) + y_offset;
	}
	
	//TODO -> lade offsets
	//hole seite
	//
}
//CONVERTS FIELD ID TO THE X Y  COORDINATES 0-7 0-7
void ChessBoard::getFieldCoordinates(ChessField::CHESS_FILEDS  _index, int& _x, int& _y, IOController::COIL _coil, bool _get_only_array_index, bool _get_field_center)
{
	
	//TODO CHECK PARK POSTION
	if(isFieldParkPosition(_index))
	{
		_x = -1;
		_y = -1;
		return;
	}
	
	
	int offset_x = 0;
	int offset_y = 0;
	//LOAD OFFSET FROM CONFIG FILE IF HOME POS IS NOT IN FIELD H1
	ConfigParser::getInstance()->get_int(ConfigParser::CFG_ENTRY::MECHANIC_H1_OFFSET_MM_X, offset_x);
	ConfigParser::getInstance()->get_int(ConfigParser::CFG_ENTRY::MECHANIC_H1_OFFSET_MM_Y, offset_y);
	int board_width = 0;
	//LOAD OFFSET FROM CONFIG FILE IF HOME POS IS NOT IN FIELD H1
	ConfigParser::getInstance()->get_int(ConfigParser::CFG_ENTRY::MECHANIC_CHESS_BOARD_WIDTH, board_width);
	int field_width = 50;
	ConfigParser::getInstance()->get_int(ConfigParser::CFG_ENTRY::MECHANIC_CHESS_FIELD_WIDTH, field_width);
	
	
	
	//GET COIL OFFSET
	int coil_offset_x = 0;
	int coil_offset_y = 0;
	get_coil_offset(_coil, coil_offset_x, coil_offset_y);
	
	
	
	///CONVERT TO XY WITH A NORMAL CHESS FIELD WITH 8 by 8 FIELDS
	int field_index = static_cast<int>(_index);
	if (_get_field_center)
	{
		
		
	}
	else
	{
		offset_x += field_width / 2;
		offset_y += field_width / 2;
	}
	
	
	if (_get_only_array_index) {
		_x = (field_index / 8);
		_y = (field_index % 8);
	}
	else
	{
		_x = (board_width - ((field_index / 8)*field_width) - offset_x) - coil_offset_x;
		_y = (((field_index % 8)*field_width) + offset_y) + coil_offset_y;
	}
	

}
	

ChessBoard::BOARD_ERROR ChessBoard::scanBoard(ChessPiece::FIGURE(&board)[BOARD_WIDTH][BOARD_HEIGHT], bool _include_park_postion)
{
	//SAVE CURRENT FIELD	
	ChessField::CHESS_FILEDS original_position = current_field;
	IOController::COIL original_coil = current_selected_coil;
	
	iocontroller->setCoilState(IOController::COIL::COIL_A, false);
	iocontroller->setCoilState(IOController::COIL::COIL_B, false);
	
	int x = 0;
	int y = 0;
	for (int i = 0; i < ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_1; i++)
	{
		getFieldCoordinates((ChessField::CHESS_FILEDS)i, x, y, IOController::COIL::COIL_NFC, true, true);    //GET INDEX FOR ARRAY
		travelToField(static_cast<ChessField::CHESS_FILEDS>(i), IOController::COIL::COIL_NFC, true);    //TRAVEL TO NEXT FIELD
		ChessPiece::FIGURE tmop = iocontroller->ScanNFC(10);   //SCAN NFC TAG IF PRESENT
		ChessPiece::FigureDebugPrint(tmop);   //DEBUG PRINT FIGURE IF FOUND
		if(tmop.type == ChessPiece::TYPE::TYPE_INVALID) {
			tmop.is_empty = true;	
		}
		board[x][y] = tmop;     //STORE FIGURE ON BOARD
	}
	
	//TODO SCAN PARK POSITION FOR WHITE
	for(int i = ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_1 ; i < ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_16 ; i++)
	{
		
		//GOOT
		//getFieldCoordinates((ChessField::CHESS_FILEDS)i, x, y, IOController::COIL::COIL_NFC, true, true);   //GET INDEX FOR ARRAY
		travelToField(static_cast<ChessField::CHESS_FILEDS>(i), IOController::COIL::COIL_NFC, true);     //TRAVEL TO NEXT FIELD
		//ACTIVATE COIL FOR THE BLACK SITE COIL A IS NESSESSARY
		iocontroller->setCoilState(IOController::COIL_A, false);
		//MOVE OUT SLOW
		//MOVE NFC
		
		//SCAN POSSBILE NFC TAG
		ChessPiece::FIGURE tmop = iocontroller->ScanNFC(10);    //SCAN NFC TAG IF PRESENT
		ChessPiece::FigureDebugPrint(tmop);    //DEBUG PRINT FIGURE IF FOUND
		if(tmop.type == ChessPiece::TYPE::TYPE_INVALID) {
			tmop.is_empty = true;	
		}
		//ACTIVTE COIL A AGAIN
		iocontroller->setCoilState(IOController::COIL::COIL_A, true);
		//MOVE IN
		//DEACTIVATE COIL
		iocontroller->setCoilState(IOController::COIL::COIL_A, false);
	}
	
	
	//TRAVEL BACK TO ORIGINAL STARTING POSITION
	travelToField(original_position, original_coil, true);
	
	return ChessBoard::BOARD_ERROR::NO_ERROR;
}
	

bool ChessBoard::isFieldParkPosition(ChessField::CHESS_FILEDS _field)
{	
	int field_index = static_cast<int>(_field);   //CAST FIELD ENUM TO A INDEX/NUMBER
	if(field_index < 64) //ALL PARKPOSITIONS ARE INDEX >=64
	{
		return false;
	}
	else
	{
		return true;
	}	
}
	
std::list<ChessPiece::FIGURE> checkBoardForFullFigureSet(ChessPiece::FIGURE(&board)[BOARD_WIDTH][BOARD_HEIGHT])
{
	//list missing or to much figures
}

IOController::COIL ChessBoard::getValidCoilTypeParkPosition(ChessField::CHESS_FILEDS _field, IOController::COIL _target)
{
	
	if (!isFieldParkPosition(_field))
	{
		return _target;
	}
	
	if (_target == IOController::COIL::COIL_NFC)
	{
		return _target;
	}
		
	bool is_black_park_position = true;
	int field_index = static_cast<int>(_field);
	
	if (field_index >= static_cast<int>(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_1) && field_index <= static_cast<int>(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_1)) {
		return IOController::COIL::COIL_B;
	}
	
	return IOController::COIL::COIL_A;
}
	
ChessBoard::BOARD_ERROR ChessBoard::travelToField(ChessField::CHESS_FILEDS _field, IOController::COIL _coil, bool _to_field_center)
{
	
	//SHUT COILS OFF
	iocontroller->setCoilState(IOController::COIL_A, false);
	iocontroller->setCoilState(IOController::COIL_B, false);
	//SETUP_MOTORS
	x_axis->atc_set_speed_preset(TMC5160::TRAVEL_SPEED_PRESET::MOVE);
	y_axis->atc_set_speed_preset(TMC5160::TRAVEL_SPEED_PRESET::MOVE);
	
	
	//GET FIELD COORDIANTES
	int field_coordinates_x = -1;
	int field_coordinates_y = -1;
	//GET FIELD TYPE NORMAL BOARD FIELD OR PARK POSITION 
	if(!isFieldParkPosition(_field))
	{
		getFieldCoordinates(_field, field_coordinates_x, field_coordinates_y, _coil, false, _to_field_center);
	}
	else
	{
		getParkPositionCoordinates(_field, field_coordinates_x, field_coordinates_y, _coil, _to_field_center);
	}
	//CONVERSION ERROR HANDLING
	if(field_coordinates_x == -1 && field_coordinates_y == -1)
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
	//DISBALE MOTOR
	x_axis->disable_motor();
	y_axis->disable_motor();
	//CHECK IF MOTORS ARRIVED TARGET POSITION WITH NO ERROR
 	if(wait_counter > WAITIME_MULTIPLIER_AXIS_ARRIVAL)
	{
		log_error("ChessBoard::BOARD_ERROR::AXIS_TRAGET_ARRIVAL_FAILED");
		return ChessBoard::BOARD_ERROR::AXIS_TRAGET_ARRIVAL_FAILED;
	}	
	//SAVE CURRENT FIELD POSITION
	current_field = _field;
	return ChessBoard::BOARD_ERROR::NO_ERROR;
}


ChessBoard::BOARD_ERROR ChessBoard::get_coil_offset(IOController::COIL _coil, int& _x, int& _y)
{
	//LOAD COIL DISTANCE COIL_A <--- NFC ---> COIL_B THE DISTANCE IS COIL_A to COIL_B THE NFC COIL IS IN THE MIDDLE
	int coil_distance = 0;
	ConfigParser::getInstance()->get_int(ConfigParser::CFG_ENTRY::MECHANIC_DISTANCE_COILS_MM, coil_distance);
	
	int mv_distance = 0;
	//coil_distance = coil_distance / 2;
	///IF SWITCHED TO B COIL THE TRAVEL DISTANCE IS NEGATIVE
	if(_coil == IOController::COIL::COIL_A)
	{
		mv_distance = -std::abs(coil_distance / 2);
	}else if(_coil == IOController::COIL::COIL_B)
	{
		mv_distance = std::abs(coil_distance / 2);
	}else if(_coil == IOController::COIL::COIL_NFC)
	{
		//	mv_distance = std::abs(coil_distance / 2);
	}

	_y = 0;   //THE COILS ARE ONLY MOVING IN X DIRECTION
	_x = mv_distance;
}
	

//ChessBoard::BOARD_ERROR ChessField::makeMoveSync(ChessField::CHESS_FILEDS _from, ChessField::CHESS_FILEDS _to, bool _with_scan, bool _directly, bool _occupy_check)
//{
	
	//TRAVEL TO FIELD WITH COILS OFF 
	//SWITCH TO SCAN NFC
	//MATCH WITH CURRENT BOARD
	//CHECK IF TARGET FIELD IS OCCUPEID -> MOVE FIGURE TO PARK
	//MOVE BACK TO START AND MOVE FIGURE TO DESTIANTION
//}

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

	//FIRST DO HOMEING OF THE AXIS
	x_axis->atc_home_sync();
	y_axis->atc_home_sync();
	
	//TRAVEL TO THE "HOME FIELD H1" AND SWITCH ALL COILS OFF
	iocontroller->setCoilState(IOController::COIL_A, false);
	iocontroller->setCoilState(IOController::COIL_B, false);
	travelToField(ChessField::CHESS_FILEDS::CHESS_FIELD_H1, IOController::COIL_A, true);
	/// AFTER HOME AND SET COIL A
	/// THE A MAGNET HAVE TO BE AT POSTION H1
	

	travelToField(ChessField::CHESS_FILEDS::CHESS_FIELD_D5, getValidCoilTypeParkPosition(ChessField::CHESS_FILEDS::CHESS_FIELD_D5, IOController::COIL_B), true);
	travelToField(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_1, getValidCoilTypeParkPosition(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_1, IOController::COIL::COIL_B), true);

	//NEXT SCAN THE FIELD WITH PARK POSTIONS
	//scanBoard(board_current, true);
//	printBoard();
	//CHECK IF ALL FIGURES ARE PRESENT
	//	std::list<ChessPiece::FIGURE> checkBoardForFullFigureSet(ChessPiece::FIGURE(&board)[BOARD_WIDTH][BOARD_HEIGHT]);
	//return ChessBoard::BOARD_ERROR::INIT_CHESS_FIGURES_NOT_COMPLETE;
	
	//makeMoveSync(ChessField::CHESS_FILEDS::CHESS_FIELD_H1, ChessField::CHESS_FILEDS::CHESS_FIELD_A1, true, false, true);   //WITH SCAN //DIRECTLY //OCCUPY CHECK
	
	//TODO SCAN FIELD ROUTINE
	
	//MOVE FIGURES TO STANDART START POSTION
	loadBoardPreset(ChessBoard::TARGET_BOARD, ChessBoard::BOARD_PRESET_ALL_FIGURES_IN_START_POSTITION);
	//	syncRealWithTargetBoard();
	

		iocontroller->setStatusLed(IOController::STAUS_LED_A, false);
	iocontroller->setTurnStateLight(IOController::TSL_IDLE);

	return ChessBoard::BOARD_ERROR::INIT_COMPLETE;
}

ChessBoard::BOARD_ERROR ChessBoard::calibrate_home_pos()
{
	//CHECK HARDWARE INIT
	if(x_axis == nullptr || y_axis == nullptr || iocontroller == nullptr)
	{
		log_error("ChessBoard::BOARD_ERROR::INIT_NULLPTR_EXECPTION");
		return ChessBoard::BOARD_ERROR::INIT_NULLPTR_EXECPTION;
	}
	//SOME USER NOTIFICATION
	iocontroller->setStatusLed(IOController::STAUS_LED_A, false);
	iocontroller->setTurnStateLight(IOController::TSL_PRECCESSING);

	//FIRST DO HOMEING OF THE AXIS
	x_axis->atc_home_sync();
	y_axis->atc_home_sync();
	//MOVE TO H1 WITH COILA_ACTIVE
	travelToField(ChessField::CHESS_FIELD_H1, IOController::COIL_A, true);
	
	
	//ENABLE COIL A
	iocontroller->setCoilState(IOController::COIL_A, true);
	iocontroller->setCoilState(IOController::COIL_B, false);
	return ChessBoard::BOARD_ERROR::NO_ERROR;
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
		//		if (!boardFromFen(tmp, _target_board))
		//		{
		//			log_error("ChessBoard::BOARD_ERROR::INIT_NULLPTR_EXECPTION");
		//		}
	
		
		
	}else if(_preset == ChessBoard::BOARD_PRESET::BOARD_PRESET_ALL_FIGURES_IN_PARK_POSITION) //LOAD ALL FIGURES IN PARTKIN POSITION BOARD
	{
		//TODO
	}
	
	
}