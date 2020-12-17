#include "ChessBoard.h"


	

ChessBoard::ChessBoard() {
	
	if (!ConfigParser::getInstance()->configLoaded()) {
		LOG_F(ERROR, "CONFIG NOT LOADED");
	}
		
	///------------ SETUP MOTOR DRIVER /AXIS ------ //
	x_axis = new TMC5160(TMC5160::MOTOR_ID::MOTOR_0);
	y_axis = new TMC5160(TMC5160::MOTOR_ID::MOTOR_1);
	///LOAD DEFAULT SETTINGS
	y_axis->default_settings();
	x_axis->default_settings();
	///OVERRIDE STEPS PER MM IF CONFIG EXISTS
	int spm = 0;
	ConfigParser::getInstance()->getInt(ConfigParser::CFG_ENTRY::MECHANIC_STEPS_PER_MM, spm);
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
	ConfigParser::getInstance()->getInt(ConfigParser::CFG_ENTRY::MECHANIC_INVERT_COILS, ivcoils);
	if (ivcoils > 0)
	{
		iocontroller->invertCoilPolarity(IOController::COIL::COIL_A, true);
		iocontroller->invertCoilPolarity(IOController::COIL::COIL_B, true);
	}
}
	
ChessBoard::~ChessBoard() {
}


void ChessBoard::test()
{
	
	//TEST A1-B2 B4-A5
	//TEST A1-H1 (switch detection in mitte tauschen) A1-H8
	//park white ->C4 park black D4 (änderung bis mitte rand ab da dann bis ziel)
	
	//TEST OCCUPY CHECK
	//FIGURE WEGEBWEGEN => wird durch sync gemacht
	MovePiar tmp_pair;
	tmp_pair.from_field = ChessField::CHESS_FILEDS::CHESS_FIELD_G5;
	tmp_pair.to_field =  ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_4;
	
	makeMoveSync(tmp_pair, false, false, false);
		
}
	
	

bool ChessBoard::MoveWaypointsAlong(std::queue<MV_POSITION>& _mv)
{
	//SET MOTORS TO CHESS FIGURE MOVE SPEED
	x_axis->atc_set_speed_preset(TMC5160::TRAVEL_SPEED_PRESET::MOVE);
	y_axis->atc_set_speed_preset(TMC5160::TRAVEL_SPEED_PRESET::MOVE);
	//DISABLE COILS
	iocontroller->setCoilState(IOController::COIL::COIL_A, false);
	iocontroller->setCoilState(IOController::COIL::COIL_B, false);
	int wait_counter = 0;
	bool ca_changed = false;
	bool cb_changed = false;
	while (!_mv.empty())
	{
		const MV_POSITION tmp = _mv.front();
		
		//SOME IMPROVEMENTS => SPI WRITING IS EXPENSIVE
		if (ca_changed != tmp.coil_a_state)
		{
			ca_changed = tmp.coil_a_state;
			iocontroller->setCoilState(IOController::COIL::COIL_A, tmp.coil_a_state);
		}
		if (cb_changed != tmp.coil_b_state)
		{
			cb_changed = tmp.coil_b_state;
			iocontroller->setCoilState(IOController::COIL::COIL_B, tmp.coil_b_state);
		}
			
		
		
			
		x_axis->move_to_postion_mm_absolute(tmp.x, false);   //MOVE TO X AND NOT WAIT
		y_axis->move_to_postion_mm_absolute(tmp.y, false);   //MOVE TO Y AND NOT WAIT
		//WAIT FO BOTH MOTORS
		while(!(x_axis->is_target_position_reached() && y_axis->is_target_position_reached())) {
			std::this_thread::sleep_for(std::chrono::milliseconds(WAITITME_FOR_MOTORS_TO_ARRIVE));
		}
		_mv.pop();
	}
		
	//DISBALE MOTOR
	x_axis->disable_motor();
	y_axis->disable_motor();
	//DISABLE COILS
	iocontroller->setCoilState(IOController::COIL::COIL_A, false);
	iocontroller->setCoilState(IOController::COIL::COIL_B, false);
}

//GET NEXT FIELD TO PARK POS
ChessField::CHESS_FILEDS ChessBoard::getNextHalfFieldFromParkPos(ChessField::CHESS_FILEDS _board_field)
{	
	
	IOController::COIL parkpos_needed_coil = getValidCoilTypeParkPosition(_board_field, IOController::COIL::COIL_A);
	//IF NOT PARK POS RETURN THE BOARD FIELD COODIANTES
	if (!isFieldParkPosition(_board_field))
	{
		return _board_field;
	}
	
	//GET POSITION OF THE PARK POS
	int pp_x = -1;
	int pp_y = -1;
	getParkPositionCoordinates(_board_field, pp_x, pp_y, parkpos_needed_coil,true);

	//NOW FIND THE NEXT BOARD FIELD NEXT TO THIS POSITION
	int bf_x = std::numeric_limits<int>::max();
	int bf_y = std::numeric_limits<int>::max();	
	int tmp_x = std::numeric_limits<int>::max();
	int tmp_y = std::numeric_limits<int>::max();
	float distance = 0.0f;
	float smallest_distance = std::numeric_limits<float>::max();
	ChessField::CHESS_FILEDS nearest_field = _board_field;
	///EDIT START STOP FIELDS HERE
	const int start_index = ChessField::field2Index(ChessField::CHESS_FILEDS::CHESS_FIELD_A1);
	const int stop_index = ChessField::field2Index(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_1);
	
	//FOR EACH BOARD FIELD
	for (int i = start_index; i < stop_index; i++)
	{
		//GET FIELD COODIANTES
		getFieldCoordinates(ChessField::Index2Field(i), tmp_x, tmp_y, parkpos_needed_coil, false, true);
		//USE PYTHAGORAS TO GET THE DISTANCE BEWEEN THE POINTS
		distance = std::hypotl(tmp_x - pp_x, tmp_y - pp_y);
		if (distance < smallest_distance)
		{
			smallest_distance = distance;
			nearest_field = ChessField::Index2Field(i);
		}
	}
		

	return nearest_field;
}


bool ChessBoard::makeMoveFromBoardToParkPosition(ChessField::CHESS_FILEDS _park_pos, std::queue<MV_POSITION>& _generated_waypoint_list, int _current_x, int _current_y)
{	
	//IF NOT PARK POS RETURN ADD THE FINAL TARGET FIELD POSITION TO THE QUEUE
	if(!isFieldParkPosition(_park_pos))
	{
		IOController::COIL parkpos_needed_coil = getValidCoilTypeParkPosition(_park_pos, IOController::COIL::COIL_A);
		getFieldCoordinates(_park_pos, _current_x, _current_y, parkpos_needed_coil, false, true);
		_generated_waypoint_list.push(MV_POSITION(_current_x, _current_y, parkpos_needed_coil == IOController::COIL::COIL_A, parkpos_needed_coil == IOController::COIL::COIL_B));
		return false;
	}
	
	int pp_pos_inside_x = 0;
	int pp_pos_inside_y = 0;  
	
	int pp_pos_before_x = 0;
	int pp_pos_before_y = 0;  
	IOController::COIL parkpos_needed_coil = getValidCoilTypeParkPosition(_park_pos, IOController::COIL::COIL_A);

	
	//GET PARK POS COORIDNATES IN THE PARK POS
	getParkPositionCoordinates(_park_pos, pp_pos_inside_x, pp_pos_inside_y, parkpos_needed_coil, false);
	//GET PARK POS COORIDNATES IN THE BEFORE THE PARK POS
	getParkPositionCoordinates(_park_pos, pp_pos_before_x, pp_pos_before_y, parkpos_needed_coil, true);
	
	
	//MOVE OUT THE PARK POS COILS
	///DONT CHANGE Y AXIS 
	_generated_waypoint_list.push(MV_POSITION(pp_pos_before_x, _current_y, parkpos_needed_coil == IOController::COIL::COIL_A, parkpos_needed_coil == IOController::COIL::COIL_B));
//	MoveWaypointsAlong(_generated_waypoint_list);
	//MOVE TO PARK POS BEFORE ENTRY
	_generated_waypoint_list.push(MV_POSITION(pp_pos_before_x, pp_pos_before_y, parkpos_needed_coil == IOController::COIL::COIL_A, parkpos_needed_coil == IOController::COIL::COIL_B));
//	MoveWaypointsAlong(_generated_waypoint_list);
	
	//FINALLY MOVE IN
	_generated_waypoint_list.push(MV_POSITION(pp_pos_inside_x, pp_pos_inside_y, parkpos_needed_coil == IOController::COIL::COIL_A, parkpos_needed_coil == IOController::COIL::COIL_B));
//	MoveWaypointsAlong(_generated_waypoint_list);
	//TURN OFF COILS
	_generated_waypoint_list.push(MV_POSITION(pp_pos_inside_x, pp_pos_inside_y, false, false));
//	MoveWaypointsAlong(_generated_waypoint_list);
	return true;
}
	
	
bool ChessBoard::makeMoveFromParkPositionToBoard(ChessField::CHESS_FILEDS _park_pos, ChessField::CHESS_FILEDS _dest_pos, std::queue<MV_POSITION>& _generated_waypoint_list, int& _dest_pos_x, int& _dest_pos_y,bool _EN_BOARD_SIZE_WORKAROUND)
{
	if (!isFieldParkPosition(_park_pos))
	{
		return false;
	}
	//GET FIELD WITDTH /2
	int field_width = 50;
	if (!ConfigParser::getInstance()->getInt(ConfigParser::CFG_ENTRY::MECHANIC_CHESS_FIELD_WIDTH, field_width))
	{
		field_width = 50;    //SET TO DEFAULT WIDTH
	}
	field_width /= 2;    //WANT TO TRAVEL BETWEEN TWO FIELDS
	//MOVE TO PARK START POSTION
	int pp_pos_inside_x = 0;
	int pp_pos_inside_y = 0;  
	
	int pp_pos_before_x = 0;
	int pp_pos_before_y = 0;  
	IOController::COIL parkpos_needed_coil = getValidCoilTypeParkPosition(_park_pos, IOController::COIL::COIL_A);
	IOController::COIL target_coil = getValidCoilTypeParkPosition(_dest_pos, IOController::COIL::COIL_A);
	
	//GET PARK POS COORIDNATES IN THE PARK POS
	getParkPositionCoordinates(_park_pos, pp_pos_inside_x, pp_pos_inside_y, parkpos_needed_coil, false);
	//GET PARK POS COORIDNATES IN THE BEFORE THE PARK POS
	getParkPositionCoordinates(_park_pos, pp_pos_before_x, pp_pos_before_y, parkpos_needed_coil, true);
	
	//TRAVEL TO PARK POS
	_generated_waypoint_list.push(MV_POSITION(pp_pos_inside_x, pp_pos_inside_y,false,false));
	//MoveWaypointsAlong(_generated_waypoint_list);
	//ACTIVATE COILS
	_generated_waypoint_list.push(MV_POSITION(pp_pos_inside_x, pp_pos_inside_y, parkpos_needed_coil == IOController::COIL::COIL_A, parkpos_needed_coil == IOController::COIL::COIL_B));
	//MoveWaypointsAlong(_generated_waypoint_list);
	//MOVE OUT THE PARK POS COILS
	_generated_waypoint_list.push(MV_POSITION(pp_pos_before_x, pp_pos_before_y, parkpos_needed_coil == IOController::COIL::COIL_A, parkpos_needed_coil == IOController::COIL::COIL_B));
	//MoveWaypointsAlong(_generated_waypoint_list);
	
	//MOVE THE FIGURE TO THE _target_field y position
	int target_pos_x = 0;
	int target_pos_y = 0;
	getFieldCoordinates(_dest_pos, target_pos_x, target_pos_y, target_coil, false, true);
	
	//TARGET POS IS EXACLTY BETWEEN TWO FIELDS
	if(_EN_BOARD_SIZE_WORKAROUND){
	_generated_waypoint_list.push(MV_POSITION(pp_pos_before_x, target_pos_y - field_width, parkpos_needed_coil == IOController::COIL::COIL_A, parkpos_needed_coil == IOController::COIL::COIL_B));
	}else {
		_generated_waypoint_list.push(MV_POSITION(pp_pos_before_x, target_pos_y + field_width, parkpos_needed_coil == IOController::COIL::COIL_A, parkpos_needed_coil == IOController::COIL::COIL_B));
		
	}
	//MoveWaypointsAlong(_generated_waypoint_list);
	_dest_pos_x = pp_pos_before_x;
	_dest_pos_y = target_pos_y;
	
	return true;
}


ChessBoard::BOARD_ERROR ChessBoard::makeMoveSync(ChessBoard::MovePiar _move, bool _with_scan, bool _directly, bool _occupy_check)
{
	//IF FIELDS EQUAL NOTHING TODO
	if (_move.to_field == _move.from_field) {
		iocontroller->setCoilState(IOController::COIL::COIL_A, false);
		iocontroller->setCoilState(IOController::COIL::COIL_B, false);
		return ChessBoard::BOARD_ERROR::NO_ERROR;
	}
	
	int coil_switch_pos = 200;
	if (!ConfigParser::getInstance()->getInt(ConfigParser::CFG_ENTRY::MECHANIC_COIL_SWITCH_POSTION_TRIGGER, coil_switch_pos))
	{
		coil_switch_pos = 200;
	}
	
	
	//------------------- BOARD SIZE LIMIT WORKAROUND ---------------------- //
	bool EN_BSIZE_WORKAROUND = false;
	if (!ConfigParser::getInstance()->getBool(ConfigParser::CFG_ENTRY::MECHANIC_BOARD_SIZE_550MM_WORKAROUND, EN_BSIZE_WORKAROUND))
	{
		EN_BSIZE_WORKAROUND = true;
	}
	//GET FIELD WITDTH /2
	int field_width = 50;
	if (!ConfigParser::getInstance()->getInt(ConfigParser::CFG_ENTRY::MECHANIC_CHESS_FIELD_WIDTH, field_width))
	{
		field_width = 50;   //SET TO DEFAULT WIDTH
	}
	field_width /= 2;   //WANT TO TRAVEL BETWEEN TWO FIELDS
	//GET COIL OFFSET FOR THE LATER COIL SWITCH
	int coil_distance = 0;
	ConfigParser::getInstance()->getInt(ConfigParser::CFG_ENTRY::MECHANIC_DISTANCE_COILS_MM, coil_distance);
	//COLLECTED THE MOVEMENT STEPS WITH COIL INFORMATION
	std::queue<MV_POSITION> position_queue;

	//DISABLE COILS
	iocontroller->setCoilState(IOController::COIL::COIL_A, false);
	iocontroller->setCoilState(IOController::COIL::COIL_B, false);
	//GET THE NEDDED COILS TO REACH THE POSITONS
	IOController::COIL start_coil = getValidCoilTypeParkPosition(_move.from_field, IOController::COIL::COIL_A);
	IOController::COIL end_coil = getValidCoilTypeParkPosition(_move.to_field, IOController::COIL::COIL_B);
	//GET THE POSITIONS FROM START / END
	//WITHIN THE BOARD !!!
	int x_start = -1;
	int y_start = -1;
	int x_end = -1;
	int y_end = -1;
	bool is_start_park_pos = false;
	bool is_end_park_pos = false;
	
	
	//WORKAROUND FOR TOO SMALL BOARD
	bool EN_START_WORKAROUND = false;
	bool EN_END_WORKAROUND = false;
	int INVERT_FIELD_OFFSET = 1;
	int INVERT_FIELD_OFFSET_END = 1;
	//------------------- BOARD SIZE LIMIT WORKAROUND ---------------------- //
	if(EN_BSIZE_WORKAROUND && (_move.from_field == ChessField::CHESS_FILEDS::CHESS_FIELD_A8 || _move.from_field == ChessField::CHESS_FILEDS::CHESS_FIELD_B8 || _move.from_field == ChessField::CHESS_FILEDS::CHESS_FIELD_C8 || _move.from_field == ChessField::CHESS_FILEDS::CHESS_FIELD_D8 || _move.from_field == ChessField::CHESS_FILEDS::CHESS_FIELD_E8 || _move.from_field == ChessField::CHESS_FILEDS::CHESS_FIELD_F8 || _move.from_field == ChessField::CHESS_FILEDS::CHESS_FIELD_G8 || _move.from_field == ChessField::CHESS_FILEDS::CHESS_FIELD_H8))
	{
		EN_START_WORKAROUND = true;
		INVERT_FIELD_OFFSET = -1;
	}
	if (EN_BSIZE_WORKAROUND && (_move.to_field == ChessField::CHESS_FILEDS::CHESS_FIELD_A8 || _move.to_field == ChessField::CHESS_FILEDS::CHESS_FIELD_B8 || _move.to_field == ChessField::CHESS_FILEDS::CHESS_FIELD_C8 || _move.to_field == ChessField::CHESS_FILEDS::CHESS_FIELD_D8 || _move.to_field == ChessField::CHESS_FILEDS::CHESS_FIELD_E8 || _move.to_field == ChessField::CHESS_FILEDS::CHESS_FIELD_F8 || _move.to_field == ChessField::CHESS_FILEDS::CHESS_FIELD_G8 || _move.to_field == ChessField::CHESS_FILEDS::CHESS_FIELD_H8))
	{
		EN_END_WORKAROUND = true;
		INVERT_FIELD_OFFSET_END = -1;
	}
	
	//FIST GET THE MOTOR COORINATES
	if(isFieldParkPosition(_move.from_field))
	{
		makeMoveFromParkPositionToBoard(_move.from_field, _move.to_field, position_queue, x_start, y_start, EN_END_WORKAROUND);
		is_start_park_pos = true;
		//------------------- BOARD SIZE LIMIT WORKAROUND ---------------------- //
		if(EN_END_WORKAROUND)
		{
			EN_START_WORKAROUND = true;
			INVERT_FIELD_OFFSET = -1;
		}
			
	}else{
		getFieldCoordinates(_move.from_field, x_start, y_start, start_coil, false, true);
		start_coil = getValidCoilTypeParkPosition(_move.from_field, IOController::COIL::COIL_B);
		
	}
	
	if (isFieldParkPosition(_move.to_field))
	{
		ChessField::CHESS_FILEDS end_field_tmp = getNextHalfFieldFromParkPos(_move.to_field); // GET NEAREST FIELD NEXT TO PARK POS
		getFieldCoordinates(end_field_tmp, x_end, y_end, end_coil, false, true); //CALCULATE NEW COORDINATES FOR THE CHANGED FIELD
		end_coil = getValidCoilTypeParkPosition(end_field_tmp, IOController::COIL::COIL_B);
		is_end_park_pos = true;
	}
	else {
		getFieldCoordinates(_move.to_field, x_end, y_end, end_coil, false, true);
	}
	
	//FIRST TRAVEL TO START POSITON
	if(!is_start_park_pos) {
		position_queue.push(MV_POSITION(x_start, y_start, false, false));
	}
	
	
	//SECOND TRAVEL 1/2 FIELD UP OR DOWN IF START WORKAROUND
	position_queue.push(MV_POSITION(x_start, y_start + field_width*INVERT_FIELD_OFFSET, start_coil == IOController::COIL::COIL_A, start_coil == IOController::COIL::COIL_B));
	
	
//	MoveWaypointsAlong(position_queue);

	//CHECK IF COIL_SWITCH NEEDED
	//INERT A BREAKPOINT BEWEEN FIELD D AND E ON THE SAME Y LINE
	
	if(start_coil != end_coil) {
		int coil_offset = 0;
		
		//MoveWaypointsAlong(position_queue);
		//MOVE TO THE COIL SWITCH POSITION WITH THE START COIL
		if(start_coil == IOController::COIL::COIL_A) {
			coil_offset = coil_switch_pos;
		}
		else if(start_coil == IOController::COIL::COIL_B) {
			coil_offset = coil_switch_pos - coil_switch_pos / 2;	
		}
		
		
		position_queue.push(MV_POSITION(coil_offset, y_start + field_width*INVERT_FIELD_OFFSET, start_coil == IOController::COIL::COIL_A, start_coil == IOController::COIL::COIL_B));
	
		
		
		//MoveWaypointsAlong(position_queue);
		//TURN COILS OFF
		iocontroller->setCoilState(IOController::COIL::COIL_A, false);
		iocontroller->setCoilState(IOController::COIL::COIL_B, false);
	//	MoveWaypointsAlong(position_queue);
		//GET COORDINATES FOR THE COIL SWITCH
		//=> SWITCH TO B COIL => -coil_offset
		if(start_coil == IOController::COIL::COIL_A && end_coil == IOController::COIL::COIL_B)
		{
			position_queue.push(MV_POSITION(coil_offset - coil_switch_pos / 2, y_start + field_width*INVERT_FIELD_OFFSET, false, false));
			position_queue.push(MV_POSITION(coil_offset - coil_switch_pos / 2, y_start + field_width*INVERT_FIELD_OFFSET, start_coil == IOController::COIL::COIL_A, start_coil == IOController::COIL::COIL_B));
		}else if(start_coil == IOController::COIL::COIL_B && end_coil == IOController::COIL::COIL_A)
		{
			position_queue.push(MV_POSITION(coil_offset + coil_switch_pos / 2, y_start + field_width*INVERT_FIELD_OFFSET, false, false));
			position_queue.push(MV_POSITION(coil_offset + coil_switch_pos / 2, y_start + field_width*INVERT_FIELD_OFFSET, start_coil == IOController::COIL::COIL_A, start_coil == IOController::COIL::COIL_B));
			
		}
	//	MoveWaypointsAlong(position_queue);
		
	}
	
	//MOVE TO TARGET X COORDINATES WITH 1/2 FIELD OFFSET
	position_queue.push(MV_POSITION(x_end + field_width, y_start + field_width*INVERT_FIELD_OFFSET, start_coil == IOController::COIL::COIL_A, start_coil == IOController::COIL::COIL_B));

	//MoveWaypointsAlong(position_queue);
	//MOVE TO TARGET Y COORDINATES WITH 1/2 FIELD OFFSET
	position_queue.push(MV_POSITION(x_end + field_width, y_end + field_width*INVERT_FIELD_OFFSET_END, start_coil == IOController::COIL::COIL_A, start_coil == IOController::COIL::COIL_B));

	//MoveWaypointsAlong(position_queue);
	//MOVE TO FIELD CENTER OR FURTHER TO MARK POS
	if(!is_end_park_pos) {
		position_queue.push(MV_POSITION(x_end, y_end, start_coil == IOController::COIL::COIL_A, start_coil == IOController::COIL::COIL_B));
	}else
	{
		//TODO MOVE FROM THERE TO FINAL PARK POS
		makeMoveFromBoardToParkPosition(_move.to_field, position_queue, x_end + field_width, y_end + field_width);
	}

	
	//FINALLY EXECUTE MOVE
	MoveWaypointsAlong(position_queue);
	

	
	return ChessBoard::BOARD_ERROR::NO_ERROR;
}
	

ChessBoard::MovePiar ChessBoard::StringToMovePair(std::string _mv)
{
	MovePiar tmp;
	//CHECK STRING POPULATES
	if(_mv.empty())
	{
		tmp.is_valid = false;
	}
	//CHECK STRING LENGHT OK
	if(_mv.size() != 4)
	{
		tmp.is_valid = false;
	}
	
	//STRING TO LOWER CASE	
	transform(_mv.begin(), _mv.end(), _mv.begin(), ::tolower);
	
	int ft = std::atoi(&_mv.at(1)) + (_mv.at(0) - 97) * 8;
	int tf = std::atoi(&_mv.at(3)) + (_mv.at(2) - 97) * 8;
	
	
	
	tmp.from_field = ChessField::Index2Field(ft);
	tmp.to_field = ChessField::Index2Field(tf);
	
	return tmp;
}





bool ChessBoard::syncRealWithTargetBoard() {
	//TODO
   //USE MAKE MOVE TO GENERATE A MOVE LIST THEN EXECUTE THE MOVES
   std::vector<MovePiar> move_list;
	ChessPiece::FIGURE*board_current = get_board_pointer(ChessBoard::BOARD_TPYE::REAL_BOARD);
	ChessPiece::FIGURE*board_target = get_board_pointer(ChessBoard::BOARD_TPYE::TARGET_BOARD);
	
	if (!board_current || !board_target)
	{
		return false;
	}
	
	
	//NOW CHECK BOARD DIFFERENCES
	for(size_t w = 0 ; w < BOARD_WIDTH ; w++) {
		for (size_t h = 0; h < BOARD_HEIGHT; h++) {
			size_t index = w + (h*BOARD_WIDTH);
			ChessPiece::FIGURE tmp_curr = board_current[index];
			ChessPiece::FIGURE tmp_target = board_target[index];
			//CHECK IF EQUAL FIGURES
			if(ChessPiece::compareFigures(tmp_curr, tmp_target))
			{
				continue;
			}
			//FIGURE POSITIONS NOT EQUAL
			//CHECK HOW MANY SOURCE AND HOW MANY DST PIVECES ON THE BOARD
			//THE SMALLEST NUMBERS LETS BE ON THE BOARD THE HIGHES ARE SORTED OUT TO THE PARK POSITIONS
			
		}		 
	}
	
	
	const size_t FIGURE_COUNT = 12;
	const char FIGURES[FIGURE_COUNT] = { 'r', 'n', 'b', 'q', 'k', 'p', 'R', 'N', 'B', 'Q', 'K', 'P' };
	//CHECK THE COUNT OF THE PIECES
	for(int i = 0 ; i < FIGURE_COUNT ; i++)
	{
		int bA_count = get_figure_type_count(board_current, FIGURES[i], true);  //CURRENT 
		int bB_count = get_figure_type_count(board_target, FIGURES[i], true);  //TARGET
		
		//IF THE AMOUNT ARE THE SAME OF BOTH BOARD => NOTHING TODO HERE
		if(bA_count == bB_count)
		{
			continue;
		
			//CURRENT BOARD HAS MORE OF THE PIECES THAN TARGET
			//=> REMOVE THE AMOUNT FROM THE CURRENT BOARD
		}else if(bA_count > bB_count)
		{
			//GET THE FILEDS OCCUPEID BY FIGURE TYPE
			std::vector<ChessField::CHESS_FILEDS> figure_field_list = get_chess_fields_occupied_from_figure(board_current, ChessPiece::getFigureByCharakter(FIGURES[i]), true);
			
			//USE THE GET PARK POSITION FUNCTION
			//GET THE FIELD BY FUNCTION INDEX TO FIELDS
			int difference = bA_count - bB_count;
			//CHECK OCCUPIED SIZE
			if(figure_field_list.size() < difference)
			{
				difference = figure_field_list.size();
			}
				
			for (int dc = 0; dc < difference; dc++) { 
				int parking_slot_index = get_next_free_park_position(ChessBoard::BOARD_TPYE::REAL_BOARD, ChessPiece::getFigureByCharakter(FIGURES[i]));
				
				//CREATE MOVE AND ADD TO MOVE LIST
				MovePiar tmp_pair;
				tmp_pair.from_field = figure_field_list.at(dc);
				tmp_pair.to_field = ChessField::get_field_from_board_index(parking_slot_index);
				move_list.push_back(tmp_pair);
			}
			
			//ELSE ADD SOME
		}else if(bA_count < bB_count)
		{
			int difference = bB_count - bA_count;
			std::vector<ChessField::CHESS_FILEDS> figure_park_field_list = get_parking_fileld_occupied_by_figure_type(board_current, ChessPiece::getFigureByCharakter(FIGURES[i]));
			std::vector<ChessField::CHESS_FILEDS> free_fields_list = get_free_fields_on_the_board(board_current);
			//SOME BOUNDRY CHECKS
			if(figure_park_field_list.size() < difference)
			{
				difference = figure_park_field_list.size();
			}
			if (free_fields_list.size() < difference)
			{
				difference = free_fields_list.size();
			}
			
			//ADD MOVE TO MOVE THE PIECES FROM PARKING TO THE BOARD
			for(int dc = 0 ; dc < difference ; dc++) { 
				//CREATE MOVE AND ADD TO MOVE LIST
				MovePiar tmp_pair;
				tmp_pair.to_field = free_fields_list.at(dc);
				tmp_pair.from_field = figure_park_field_list.at(dc);
				move_list.push_back(tmp_pair);
			}

		}
		
		
	}
	
	
	
	//NOW ALL NEEDED PIECES ARE PLACED INSIDE THE BOARD
	//NEXT STEP IS TO MOVE THEM AROUND
	
	//-> MAKE OCCUPY CHECK IF OCCUPIED ADD AN OTHER MOVE TO GET THE FIGURE TO PARK POSITION
	
	
	
	
	
	
	
	//FINALLY EXECUTE MOVES
	for(size_t i = 0 ; i < move_list.size() ; i++)
	{
		makeMoveSync(move_list.at(i), false, false, false);
	}
		
		
	//FINALLY TRAVEL BACK TO HOME POS
travelToField(ChessField::CHESS_FILEDS::CHESS_FIELD_H1, IOController::COIL::COIL_A, true);
	
	
	//NOW COPY NEW POSITIONS OVER
	for(size_t w = 0 ; w < BOARD_WIDTH*BOARD_HEIGHT ; w++) {
		board_current[w] = board_target[w];
	}
	
	return true;	
}


std::vector<ChessField::CHESS_FILEDS> ChessBoard::get_free_fields_on_the_board(ChessPiece::FIGURE* _board_pointer)
{
	std::vector<ChessField::CHESS_FILEDS> tmp;
	if (_board_pointer == nullptr)
	{
		return tmp;
	}
	
	for (size_t h = 0; h < BOARD_HEIGHT; h++) {
		
		for (size_t w = 2; w < 10; w++) {
			size_t index = w + (h*BOARD_WIDTH);
			ChessPiece::FIGURE tmp_fig = _board_pointer[index];
			if (tmp_fig.is_empty)
			{
				tmp.push_back(ChessField::get_field_from_board_index(index));
				
			}
		}
	}
	
	return tmp;
}



std::vector<ChessField::CHESS_FILEDS> ChessBoard::get_parking_fileld_occupied_by_figure_type(ChessPiece::FIGURE* _board_pointer, ChessPiece::FIGURE _fig)
{
	std::vector<ChessField::CHESS_FILEDS> tmp;
	if (_board_pointer == nullptr)
	{
		return tmp;
	}
	//DETERM SITE OFFSET
	int side_offset = -1;
	if (_fig.color == ChessPiece::COLOR_WHITE) {
		side_offset = 0;   //LEFT BOARD COL 0+1
	}
	else if (_fig.color == ChessPiece::COLOR_BLACK) {
		side_offset = 10;   //RIGHT BOARD COL 10+11
	}
	else {
		return tmp;
	}
	
	
	//NOW CHECK FIELDS FOR NEXT FREE SPACE	
	for(size_t h = 0 ; h < BOARD_HEIGHT ; h++) {
		
		for (size_t w = 0 + side_offset; w < 2 + side_offset; w++) {
			size_t index = w + (h*BOARD_WIDTH);
			ChessPiece::FIGURE tmp_fig = _board_pointer[index];
			
			//CHECK IF FIELD FREE
			if(tmp_fig.is_empty)
			{
				continue;
			}
			//OVERRIDE INVALID FIGURES
			if(!ChessPiece::compareFigures(tmp_fig, _fig))
			{
				continue;
			}
			
			tmp.push_back(ChessField::get_field_from_board_index(index));
			
		}		 
	}
	return tmp;
}

std::vector<ChessField::CHESS_FILEDS> ChessBoard::get_chess_fields_occupied_from_figure(ChessPiece::FIGURE* _board_pointer, ChessPiece::FIGURE _fig, bool _board_only)
{
	
	std::vector<ChessField::CHESS_FILEDS> tmp;
	
	if (_board_pointer == nullptr || !ChessPiece::IsFigureValid(_fig))
	{
		return tmp;
	}
		
	int w_offset = 0;
	if (_board_only)
	{
		w_offset = 2;   //BOARD WIDTH - 2*2 FOR PARKING
	}
	
	for (size_t w = 0 + w_offset; w < BOARD_WIDTH - w_offset; w++) {
		for (size_t h = 0; h < BOARD_HEIGHT; h++) {
			size_t index = w + (h*BOARD_WIDTH);
			ChessPiece::FIGURE tmp_fig = _board_pointer[index];
			
			
			//CHECK IF EQUAL FIGURES
			if(ChessPiece::compareFigures(_fig, tmp_fig))
			{
				//ADD TO THE LIST
				tmp.push_back(ChessField::get_field_from_board_index(index));
			}
			
		}		 
	}
	
	
	return tmp;

}







int ChessBoard::get_next_free_park_position(ChessBoard::BOARD_TPYE _target_board, ChessPiece::FIGURE _fig)
{

	ChessPiece::FIGURE*cb = get_board_pointer(_target_board);
	if (cb == nullptr)
	{
		return -1;
	}
	
	
	
	//DETERM SITE OFFSET
	int side_offset = -1;
	if (_fig.color == ChessPiece::COLOR_WHITE) {
		side_offset = 0;  //LEFT BOARD COL 0+1
	}
	else if (_fig.color == ChessPiece::COLOR_BLACK) {
		side_offset = 10;  //RIGHT BOARD COL 10+11
	}
	else {
		return -1;
	}
		
	//NOW CHECK FIELDS FOR NEXT FREE SPACE	
	for(size_t h = 0 ; h < BOARD_HEIGHT ; h++) {
		
		for (size_t w = 0 + side_offset; w < 2 + side_offset; w++) {
			size_t index = w + (h*BOARD_WIDTH);
			ChessPiece::FIGURE tmp = cb[index];
			
			//CHECK IF FIELD FREE
			if(!tmp.is_empty)
			{
				continue;
			}
			//OVERRIDE INVALID FIGURES
			if(ChessPiece::IsFigureValid(tmp))
			{
				continue;
			}
			
			return index;
			
		}		 
	}
	return -1; //RETURN INVALID
}


ChessPiece::FIGURE* ChessBoard::get_board_pointer(ChessBoard::BOARD_TPYE _target_board)
{
	if (_target_board == ChessBoard::BOARD_TPYE::TARGET_BOARD)
	{
		return board_target;
	}
	else if (_target_board == ChessBoard::BOARD_TPYE::REAL_BOARD)
	{
		return board_current;
	}
	else if (_target_board == ChessBoard::BOARD_TPYE::TEMP_BOARD)
	{
		return board_temp;
	}
	else
	{
		return nullptr;//INVALID SELECTD BOARD
	}
}


int ChessBoard::get_figure_type_count(ChessBoard::BOARD_TPYE _target_board, char _type_char, bool _board_only)
{
	return get_figure_type_count(get_board_pointer(_target_board), _type_char, _board_only);

}

int ChessBoard::get_figure_type_count(ChessPiece::FIGURE* _board_pointer, char _type_char, bool _board_only) {
	if (_board_pointer == nullptr)
	{
		return -1;
	}
	
	
	int count = 0;
	int w_offset = 0;
	if (_board_only)
	{
		w_offset = 2;  //BOARD WIDTH - 2*2 FOR PARKING
	}
		
		
	for (size_t w = 0 + w_offset; w < BOARD_WIDTH - w_offset; w++) {
		for (size_t h = 0; h < BOARD_HEIGHT; h++) {
			size_t index = w + (h*BOARD_WIDTH);
			ChessPiece::FIGURE tmp_fig = _board_pointer[index];

			//CHECK IF EQUAL FIGURES
			if(ChessPiece::Figure2Charakter(tmp_fig) == _type_char && ChessPiece::IsFigureValid(tmp_fig))
			{
				count++;
			}
		}		 
	}
	
	
	return count;
}

bool ChessBoard::boardFromFen(std::string _fen, ChessBoard::BOARD_TPYE _target_board) {
	
	
	_fen += " ";  //THE BOARD BLOCK (CONTAINING FROM START OF THE STRING) TO THE FIRST WHITESPACE => EVERYTHING AFTER IS NOT IMPORTANT
	//CREATE AN EMPTY FIGURE
	
	ChessPiece::FIGURE empty_firegure;
	empty_firegure.is_empty = true;
	empty_firegure.figure_number = -1;
	empty_firegure.color = ChessPiece::COLOR_UNKNOWN;
	empty_firegure.type = ChessPiece::TYPE_INVALID;
	
	ChessPiece::FIGURE*cb = get_board_pointer(_target_board);
	if (cb == nullptr)
	{
		return false;
	}
	
	
	//WE NEED COUNTER FOR EACH TYPE OF FIGURE PLACED IN THE BOARD
	//TO DETERM THE AMOUNT OF GIRES IN THE PARKING SLOTS
	// INIT WIHT -1 INDICATED THAT NO FIGURE OF THIS TYPE IS SET
	std::map<char, int> figure_counter;
	//BLACK
	figure_counter['r'] = -1;
	figure_counter['n'] = -1;
	figure_counter['b'] = -1;
	figure_counter['q'] = -1;
	figure_counter['k'] = -1;
	figure_counter['p'] = -1;
	
	//WHITE
	figure_counter['R'] = -1;
	figure_counter['N'] = -1;
	figure_counter['B'] = -1;
	figure_counter['Q'] = -1;
	figure_counter['K'] = -1;
	figure_counter['P'] = -1;
	
	
	
	//CLEAR TARGET BOARD
	for(size_t h = 0 ; h < BOARD_HEIGHT ; h++) {
		for (size_t w = 0; w < BOARD_WIDTH; w++) {
			size_t index = w + (h*BOARD_WIDTH);
			cb[index] = empty_firegure;
		}		 
	}
	//	printBoard(_target_board);
		//ALGORITHM PARTLY MODIFIED FRO  POST 12550 SITE:
		//http://www.cplusplus.com/forum/general/181525/
		//CHECK FEN STRING
		int num_spaces = 0;
	int num_rows = 1;
	//FOR EACH CHARACTER IN FEN STRING
	
	for(const char *p =  _fen.c_str() ; *p != 0 ; ++p) {
		if (*p == ' ') {
			++num_spaces;
		}
		else if (*p == '/') {
			++num_rows; 
		}
		
	}
	//INVALID FEN
	if(num_rows > 8) //DISBALE SPACE CHECK => ONLY USED FOR EXTENDED FEN STRINGS num_spaces != 5 || 
	{			
		return false;
	}
	

	
	
	int board_position_x = 0;  //WIDTH
	int board_position_y = 0;  //HEIGHT
	const char *p =  _fen.c_str();
	for (; *p != ' '; ++p)
	{
		const char ch = *p;

		if (isdigit(ch)) {
			 //IS A DIGIT => EMPTY SPACE FOR THE VALUE OF THE DIGIT
			for(int i = 0 ; i < (ch - '0') ; ++i) {
				board_position_x++;  //STEP OVER FIELD	
			}
		}
		else if (ch == '/') {
			 //NEW ROW
			printf("\n\n");
			board_position_y++;  //GOTO NEXT ROW
			board_position_x = 0;   //AND STARTING A NEW LINE WITH X COUNTER AT 0
		}
		else if ((ch > 65 && ch < 90) || (ch > 97 && ch < 122)) {
			//IS A FIGURE 
			printf(" %c", ch); 
			ChessPiece::FIGURE tmp = ChessPiece::getFigureByCharakter(ch);
			//CHECK FIGURE IS VLAID
			if(!ChessPiece::IsFigureValid(tmp))
			{
				continue;
			}
				
				
			//ASSIGN FIGURE NUMBER
			figure_counter[ch]++;
			tmp.figure_number = figure_counter[ch];
			
			//PLACE THE FIGURE
			size_t index = board_position_x + (board_position_y*BOARD_WIDTH) + 2;  //+2 FOR THE 2 SPACE FOR THE PARKING POSITIONS
			cb[index] = tmp;
			//STEP TO NEXT FIELD
			board_position_x++;  
		}
			
	}

	//printBoard(_target_board);
	
	//DETERMN FIGURES IN PARK POSITION
	const size_t FIGURE_COUNT = 12;
	const char FIGURES[FIGURE_COUNT] = { 'r', 'n', 'b', 'q', 'k', 'p', 'R', 'N', 'B', 'Q', 'K', 'P' };
	int park_pos_counter_white = 0;
	int park_pos_counter_black = 0;
	
	//FOR EACH FIGURE
	for(int i = 0 ; i < FIGURE_COUNT ; i++)
	{
		//GET TARGET COUNT
		const int target_count = ChessPiece::getFigureCountByChrakter(FIGURES[i]);
		const int current_count = figure_counter[FIGURES[i]] + 1;
		const int missing_count = llabs(current_count - target_count);
		//IF TARGET COUNT REACHED
		if(missing_count <= 0)
		{
			continue;
		}
		//ELSE FILL UP
		for(int curr_missing_count = 0 ; curr_missing_count < missing_count ; curr_missing_count++)
		{
			//CREATE FIGURE WITH NUMBER
			ChessPiece::FIGURE tmp = ChessPiece::getFigureByCharakter(FIGURES[i]);
			figure_counter[FIGURES[i]]++;
			tmp.figure_number = figure_counter[FIGURES[i]];
			//CHECK FIGURE IS VLAID
			if(!ChessPiece::IsFigureValid(tmp))
			{
				return false;
			}
			
			int pos_index = get_next_free_park_position(_target_board, tmp);
			//INVALID POSITION
			if(pos_index < 0)
			{
				continue;
			}
			//FINALLY PLACE FIGURE
			cb[pos_index] = tmp;
			//printBoard(_target_board);
		}
			
	}
	
	printBoard(_target_board);
	return true;
}
	


void ChessBoard::printBoard(ChessBoard::BOARD_TPYE _target_board)
{
	ChessPiece::FIGURE*cb;
	
	if (_target_board == ChessBoard::BOARD_TPYE::TARGET_BOARD)
	{
		cb = board_target;
		std::cout << "----- BOARD - TARGET -----------" << std::endl;
		
	}
	else if (_target_board == ChessBoard::BOARD_TPYE::REAL_BOARD)
	{
		cb = board_current;
		std::cout << "----- BOARD - REAL -------------" << std::endl;
		
	}
	else if (_target_board == ChessBoard::BOARD_TPYE::TEMP_BOARD)
	{
		cb = board_temp;
		std::cout << "----- BOARD - TEMP -------------" << std::endl;
		
	}
	else
	{
		return;
	}
	std::cout << "x = INVALID -----. = EMPTY FIELD" << std::endl;
	
	for (size_t h = 0; h < BOARD_HEIGHT; h++) {
		for (size_t w = 0; w < BOARD_WIDTH; w++) {
			if (w == 2 || w == 10)
			{
				std::cout << "| ";
			}
				
			size_t index = w + (h*BOARD_WIDTH);
			//PRINT BLANK IF FIGURE IS INVALID
			if(cb[index].is_empty)
			{
				std::cout << "." << " ";
				continue;
			}
			//PRINT X FOR AN INVALID FIGURE
			if(!ChessPiece::IsFigureValid(cb[index]))
			{
				std::cout << "x" << " ";
				continue;
			}
			//ELSE PRINT THE CHARAKTER
			std::cout << ChessPiece::Figure2Charakter(cb[index]) << " ";
		}	
		std::cout << std::endl;
	}
	std::cout << "----- END BOARD -------------------" << std::endl;
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
	
	if (is_black_park_position == 2) //BLACK
		{
			ConfigParser::getInstance()->getInt(ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_BLACK_X_LINE, line_offset);
			ConfigParser::getInstance()->getInt(ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_BLACK_FIRST_Y_OFFSET, y_offset);
		}
	else
	{
		ConfigParser::getInstance()->getInt(ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_WHITE_X_LINE, line_offset);
		ConfigParser::getInstance()->getInt(ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_WHITE_FIRST_Y_OFFSET, y_offset);
	}
		
		
	//LOAD PARKING SLOT CELL SIZE
	ConfigParser::getInstance()->getInt(ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_CELL_SIZE, cell_size);
	ConfigParser::getInstance()->getInt(ConfigParser::CFG_ENTRY::MECHANIC_PARK_POS_CELL_BEFORE_OFFSET, before_line_offset);
	
	
	//GET PARK POSITON INDEX 1-16
	int pp_entry = 0;
	if (is_black_park_position == 2) //BLACK
		{
			pp_entry = (magic_enum::enum_integer(_index) - magic_enum::enum_integer(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_1)) % 16;
		}
	else
	{
		pp_entry = (magic_enum::enum_integer(_index) - magic_enum::enum_integer(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_1)) % 16;
	}
	
	if (before_parkpostion_entry)
	{
		if (is_black_park_position == 2) //BLACK
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
	ConfigParser::getInstance()->getInt(ConfigParser::CFG_ENTRY::MECHANIC_H1_OFFSET_MM_X, offset_x);
	ConfigParser::getInstance()->getInt(ConfigParser::CFG_ENTRY::MECHANIC_H1_OFFSET_MM_Y, offset_y);
	int board_width = 0;
	//LOAD OFFSET FROM CONFIG FILE IF HOME POS IS NOT IN FIELD H1
	ConfigParser::getInstance()->getInt(ConfigParser::CFG_ENTRY::MECHANIC_CHESS_BOARD_WIDTH, board_width);
	int field_width = 50;
	ConfigParser::getInstance()->getInt(ConfigParser::CFG_ENTRY::MECHANIC_CHESS_FIELD_WIDTH, field_width);
	
	
	
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
	

ChessBoard::BOARD_ERROR ChessBoard::scanBoard(bool _include_park_postion)
{
	//SAVE CURRENT FIELD	
	ChessField::CHESS_FILEDS original_position = current_field;
	IOController::COIL original_coil = current_selected_coil;
	
	iocontroller->setCoilState(IOController::COIL::COIL_A, false);
	iocontroller->setCoilState(IOController::COIL::COIL_B, false);
	
	int x = 0;
	int y = 0;
	for (int i = 0; i < magic_enum::enum_integer(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_1); i++)
	{
		//getFieldCoordinates((ChessField::CHESS_FILEDS)i, x, y, IOController::COIL::COIL_NFC, true, true);        //GET INDEX FOR ARRAY
		travelToField(static_cast<ChessField::CHESS_FILEDS>(i), IOController::COIL::COIL_NFC, true);         //TRAVEL TO NEXT FIELD
		ChessPiece::FIGURE tmop = iocontroller->ScanNFC(10);        //SCAN NFC TAG IF PRESENT
		ChessPiece::FigureDebugPrint(tmop);        //DEBUG PRINT FIGURE IF FOUND
		if(tmop.type == ChessPiece::TYPE::TYPE_INVALID) {
			tmop.is_empty = true;	
		}else
		{
			tmop.is_empty = false;
		}
		
		board_current[ChessField::get_board_index_from_field(static_cast<ChessField::CHESS_FILEDS>(i))] = tmop;               //STORE FIGURE ON BOARD
	}
	
	//TODO SCAN PARK POSITION FOR WHITE
	for(int i = magic_enum::enum_integer(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_1) ; i < magic_enum::enum_integer(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_16) ; i++)
	{
		
		//GOOT
		//getFieldCoordinates((ChessField::CHESS_FILEDS)i, x, y, IOController::COIL::COIL_NFC, true, true);   //GET INDEX FOR ARRAY
		travelToField(static_cast<ChessField::CHESS_FILEDS>(i), IOController::COIL::COIL_NFC, true);          //TRAVEL TO NEXT FIELD
		//ACTIVATE COIL FOR THE BLACK SITE COIL A IS NESSESSARY
		iocontroller->setCoilState(IOController::COIL_A, false);
		//MOVE OUT SLOW
		//MOVE NFC
		
		//SCAN POSSBILE NFC TAG
		ChessPiece::FIGURE tmop = iocontroller->ScanNFC(10);         //SCAN NFC TAG IF PRESENT
		ChessPiece::FigureDebugPrint(tmop);         //DEBUG PRINT FIGURE IF FOUND
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
	int field_index = static_cast<int>(_field);        //CAST FIELD ENUM TO A INDEX/NUMBER
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
	
	//if (!isFieldParkPosition(_field))
//	{
//		return _target;
//	}
	
	if(_target == IOController::COIL::COIL_NFC)
	{
		return _target;
	}
		
	bool is_black_park_position = true;
	int field_index = static_cast<int>(_field);
	
	
	if (isFieldParkPosition(_field))
	{
		if (field_index >= static_cast<int>(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_1) && field_index <= static_cast<int>(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_16))
		{
			return IOController::COIL::COIL_B;
		}
		else
		{
			return IOController::COIL::COIL_A;	
		}	
	}
	else
	{
		if (field_index >= static_cast<int>(ChessField::CHESS_FILEDS::CHESS_FIELD_A1) && field_index <= static_cast<int>(ChessField::CHESS_FILEDS::CHESS_FIELD_D8))
		{
			return IOController::COIL::COIL_B;
		}
		else
		{
			return IOController::COIL::COIL_A;	
		}
	}
	
	
	
	
}
	
ChessBoard::BOARD_ERROR ChessBoard::travelToField(ChessField::CHESS_FILEDS _field, IOController::COIL _coil, bool _to_field_center)
{
	
	//SHUT COILS OFF
//	iocontroller->setCoilState(IOController::COIL_A, false);
//	iocontroller->setCoilState(IOController::COIL_B, false);
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
	ConfigParser::getInstance()->getInt(ConfigParser::CFG_ENTRY::MECHANIC_DISTANCE_COILS_MM, coil_distance);
	
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

	_y = 0;        //THE COILS ARE ONLY MOVING IN X DIRECTION
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

ChessBoard::BOARD_ERROR ChessBoard::initBoard(bool _with_scan)
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
	//travelToField(ChessField::CHESS_FILEDS::CHESS_FIELD_A1, IOController::COIL_A, true);
	/// AFTER HOME AND SET COIL A
	/// THE A MAGNET HAVE TO BE AT POSTION H1
	

	
	
	//LOAD PRESETS
//	loadBoardPreset(ChessBoard::BOARD_TPYE::REAL_BOARD, ChessBoard::BOARD_PRESET::BOARD_PRESET_ALL_FIGURES_IN_PARK_POSITION);
//	loadBoardPreset(ChessBoard::BOARD_TPYE::TARGET_BOARD, ChessBoard::BOARD_PRESET::BOARD_PRESET_ALL_FIGURES_IN_START_POSTITION);
	
	//NEXT SCAN THE FIELD WITH PARK POSTIONS
	if(_with_scan)
	{
		scanBoard(true);
		printBoard(ChessBoard::BOARD_TPYE::REAL_BOARD);
	}
		
		
	
	
	//makeMoveSync(ChessField::CHESS_FILEDS::CHESS_FIELD_H1, ChessField::CHESS_FILEDS::CHESS_FIELD_A1, true, false, true);   //WITH SCAN //DIRECTLY //OCCUPY CHECK
	
	//TODO SCAN FIELD ROUTINE
	
	//MOVE FIGURES TO STANDART START POSTION
	
		//syncRealWithTargetBoard();
	

//	iocontroller->setStatusLed(IOController::STAUS_LED_A, false);
//	iocontroller->setTurnStateLight(IOController::TSL_IDLE);

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
	travelToField(ChessField::CHESS_FILEDS::CHESS_FIELD_H1, IOController::COIL_A, true);
	
	
	//ENABLE COIL A
	iocontroller->setCoilState(IOController::COIL_A, true);
	iocontroller->setCoilState(IOController::COIL_B, false);
	iocontroller->setTurnStateLight(IOController::TSL_IDLE);
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
	
	
	if (_preset == ChessBoard::BOARD_PRESET::BOARD_PRESET_ALL_FIGURES_IN_START_POSTITION) {
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
	
		
		
	}
	else if (_preset == ChessBoard::BOARD_PRESET::BOARD_PRESET_ALL_FIGURES_IN_PARK_POSITION) //LOAD ALL FIGURES IN PARTKIN POSITION BOARD
		{
			boardFromFen("8/8/8/8/8/8/8/8", _target_board);
		
		
		}
	else if (_preset == ChessBoard::BOARD_PRESET::BOARD_PRESET_NO_FIGURES_PLACED) //LOAD ALL FIGURES IN PARTKIN POSITION BOARD
		{
			boardFromFen("8/8/8/8/8/8/8/8", _target_board);
		}
	
	
}