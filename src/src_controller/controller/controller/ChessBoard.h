#pragma once
#ifndef __CHESSBOARD_H__
#define __CHESSBOARD_H__

#include <string>
#include <locale>         // std::locale, std::tolower

#include <list>
#include <vector>

#include <queue>


#include <chrono>
#include <stdlib.h>
#include <map>
#define USE_STD_LOG

#ifdef USE_STD_LOG
#include <iostream>
#endif

///CHESS LOGIC RELTATED
#include "ChessPiece.h"
#include "ChessField.h"
//HARDWARE LOGIC RELATED
#include "IOController.h"
#include "TMC5160.h"

//MISC RELATED
#include "ConfigParser.h"

//MISK 3rd PARTY
#include "SHARED/loguru-master/loguru.hpp"
#include "SHARED/magic_enum-master/include/magic_enum.hpp"

///CHESS BOARD DIMENSIONS
///FOR WIDTH 8 normal + 4 for parkposition
/// 2 PARK WHITE + 8 CHESS BOARD + 2 PARK BLACK
#define  BOARD_WIDTH (8+4)
#define BOARD_HEIGHT 8

#define DEFAULT_BOARD_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define WAITITME_FOR_MOTORS_TO_ARRIVE 100 //[ms] to check if motor target position is reached
#define WAITIME_MULTIPLIER_AXIS_ARRIVAL 20 //check x times if target postion is reached
class ChessBoard
{
	
public:
	///WAYPOINT STRUCT - USED BY THE MoveAlongWaypoints Funktion - Generates by makeMoveSync Function
	struct MV_POSITION
	{
		int x;
		int y;
		bool coil_a_state;
		bool coil_b_state;
		MV_POSITION(int _x, int _y, bool _cas,bool _cbs)
		{
			x = _x;
			y = _y;
			coil_a_state = _cas,
			coil_b_state = _cbs;
				
		}
	};
	
	enum class BOARD_TPYE
	{
	         REAL_BOARD = 0,
			TARGET_BOARD = 1,
			TEMP_BOARD = 2
	};
	
	enum class BOARD_ERROR {
		NO_ERROR = 0,
		MOVE_FAILED_FIGURE_MISSING = 1,
		MOVE_FAILED = 2,
		MOVE_FAILED_TARGET_OCCUPIED = 3,
		MOVE_SUCCESS= 4,
		INIT_NULLPTR_EXECPTION = 5,
		AXIS_TRAGET_ARRIVAL_FAILED = 6,
		BAD_FILED_INDEX = 7,
		INIT_CHESS_FIGURES_NOT_COMPLETE = 8,
		INIT_COMPLETE = 9
	};
	
	enum class BOARD_STATUS {
		BOARD_INIT_FAILED,
		BOARD_INIT_OK
		
	};
	
	enum class BOARD_PRESET {
		BOARD_PRESET_ALL_FIGURES_IN_START_POSTITION = 0,
		BOARD_PRESET_ALL_FIGURES_IN_PARK_POSITION = 1,
		BOARD_PRESET_NO_FIGURES_PLACED = 2
	};
	
	struct FigureField
	{
		ChessField::CHESS_FILEDS field;
		ChessPiece::FIGURE figure;
	};
	
	struct MovePiar
	{
		ChessField::CHESS_FILEDS from_field;
		ChessField::CHESS_FILEDS to_field;
		bool is_valid;
	};
		
	void test();
	ChessBoard() ;
	~ChessBoard();
	MovePiar StringToMovePair(std::string _mv);
	std::string board2FEN(ChessBoard::BOARD_TPYE _type); //RETURNS A FEN REPRESENTATION OF THE BOARD
	bool boardFromFen(std::string _fen, ChessBoard::BOARD_TPYE _target_board); //LOADS A BOARD BY FEN
	bool syncRealWithTargetBoard(); ///SNYC THE RealBoard with the Target board and move the figures
	void printBoard(ChessBoard::BOARD_TPYE _target_board);   ///PRINT BOARD TO CONSOLE CURRENT AND TARGET
	ChessBoard::BOARD_ERROR scanBoard(bool _include_park_postion);     ///SCANS THE BOARD WITH THE NFC READER AND STORE THE RESULT IN THE GIVEN REFERENCE BOARD
	
	std::list<FigureField> compareBoards();  ///COMPARE THE REAL AND TARGET BOARD AND GET THE DIFFERENCES
	
	ChessBoard::BOARD_ERROR initBoard(bool _with_scan);   //INIT THE MECHANICS AND SCANS THE BOARD
	
	void loadBoardPreset(ChessBoard::BOARD_TPYE _target_board, ChessBoard::BOARD_PRESET _preset);
	ChessBoard::BOARD_ERROR makeMoveSync(MovePiar _move, bool _with_scan, bool _directly, bool _occupy_check);       //MOVES A FIGURE FROM TO AN FIELD TO AN OTHER _with_scan_scans the figure on start field first; _directly moves figure on direct way, occupy_check ches if target field is alreadey occupied
	
	ChessBoard::BOARD_ERROR travelToField(ChessField::CHESS_FILEDS _field, IOController::COIL _coil,bool _to_field_center);   //TRAVEL HEAD TO A FIELD
	void getFieldCoordinates(ChessField::CHESS_FILEDS _index, int& _x, int& _y, IOController::COIL _coil, bool _get_only_array_index, bool _get_field_center);
	void getParkPositionCoordinates(ChessField::CHESS_FILEDS _index, int& _x, int& _y, IOController::COIL _coil, bool before_parkpostion_entry);
	bool isFieldParkPosition(ChessField::CHESS_FILEDS _field);
	ChessBoard::BOARD_ERROR switch_coil(IOController::COIL _coil, bool _activate_swtiched_coil);
	ChessBoard::BOARD_ERROR get_coil_offset(IOController::COIL _coil, int& _x, int& _y);
	
	std::list<ChessPiece::FIGURE> checkBoardForFullFigureSet(ChessPiece::FIGURE(&board)[BOARD_WIDTH][BOARD_HEIGHT]);
	IOController::COIL getValidCoilTypeParkPosition(ChessField::CHESS_FILEDS _field, IOController::COIL _target);
	ChessBoard::BOARD_ERROR calibrate_home_pos(); //MOVE THE HEAD TO FIELD H1
	
	int get_next_free_park_position(ChessBoard::BOARD_TPYE _target_board, ChessPiece::FIGURE _fig); //RETURNS THE INDEX OF THE NEXT FREE PARK POSITION FO RTHE GIVEN COLOR
	
	std::vector<ChessField::CHESS_FILEDS> get_free_fields_on_the_board(ChessPiece::FIGURE* _board_pointer);
	std::vector<ChessField::CHESS_FILEDS> get_parking_fileld_occupied_by_figure_type(ChessPiece::FIGURE* _board_pointer, ChessPiece::FIGURE _fig);
	
	int get_figure_type_count(ChessBoard::BOARD_TPYE _target_board, char _type_char , bool _board_only);  //RETURNS THE COUNT OF FIGURES PLACED ON THE BOARD
	int get_figure_type_count(ChessPiece::FIGURE* _board_pointer, char _type_char , bool _board_only); 
	
	std::vector<ChessField::CHESS_FILEDS> get_chess_fields_occupied_from_figure(ChessPiece::FIGURE* _board_pointer, ChessPiece::FIGURE _fig, bool _board_only);
	
	ChessPiece::FIGURE* get_board_pointer(ChessBoard::BOARD_TPYE _target_board); //RETURNS THE POINTER TO A SPCIFIED BOARD ARRAY
	
	bool MoveWaypointsAlong(std::queue<MV_POSITION>& _mv); //MOVES THE HEAD ALONG A LIST OF WAYPOINTS
private:
	
	TMC5160* x_axis = nullptr; //X AXIS MOTOR
	TMC5160* y_axis = nullptr; //Y AXIS MOTOR
	IOController* iocontroller = nullptr;//IOCONTROLLER (NFC READER AND MAGNETS)
	IOController::COIL current_selected_coil = IOController::COIL::COIL_A;
	ChessField::CHESS_FILEDS current_field;
	///REPRESENTS THE CHESS BOARD
	ChessPiece::FIGURE board_current[BOARD_WIDTH*BOARD_HEIGHT];  ///REPRESENTS THE CURRENT CHESS BOARD (=> THE MECHANICAL/REAL WORLD)
	ChessPiece::FIGURE board_target[BOARD_WIDTH*BOARD_HEIGHT]; ///REPRESENTS THE TARGETBOARD WHICH SHOULD BE ARCHVIED
	ChessPiece::FIGURE board_temp[BOARD_WIDTH*BOARD_HEIGHT];   ///USED FOR FEN PARSING
	void log_error(std::string _err);
};

#endif //__CHESSBOARD_H__

