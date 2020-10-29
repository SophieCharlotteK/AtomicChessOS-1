#pragma once
#ifndef __CHESSPIECE_H__
#define __CHESSPIECE_H__
class ChessPiece
{
public:
	///THE FIGURE TYPE OF A CHESS FIGURE
	enum TYPE
	{
		TYPE_UNKNOWN,
		TYPE_INVALID,
		TYPE_KING,
		TYPE_QUEEN,
		TYPE_ROOK,
		TYPE_BISHOP,
		TYPE_KNIGHT,
		TYPE_PAWN
	};
	///COLOR
	enum  COLOR
	{
	        COLOR_UNKNOWN,
			COLOR_BLACK,
			COLOR_WHITE
	};
	/// COLOR, TYPE COMBINED WITH THE FIGURE ID
	struct FIGURE
	{
		ChessPiece::COLOR _color;
		ChessPiece::TYPE _type;
		///THE UPCOUNTING NUMBER OF THE FIGURE TO DETERMN THE PARK POSITION, THE NUMBER IS ONLY UNIQUE COMBINED WITH THE OTHER
		int figure_number;
		///THIS IS THE UNIQUE ID OF THE FIGURE HWICH IS ALSO WRITTEN ON THE NFC TAG
		uint8_t  unique_id;
		
	};
	
	uint8_t figure2NDEF(ChessPiece::FIGURE _figure);
	ChessPiece::FIGURE NDEF2Figure(uint8_t _ndef_id);
};
#endif

