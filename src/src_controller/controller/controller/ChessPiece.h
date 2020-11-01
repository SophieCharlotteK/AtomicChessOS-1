#pragma once
#ifndef __CHESSPIECE_H__
#define __CHESSPIECE_H__

#define BIT_SET(a,b) ((a) |= (1ULL<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b))))        // '!!' to make sure this returns 0 or 1

class ChessPiece
{
public:
	///THE FIGURE TYPE OF A CHESS FIGURE
	enum TYPE
	{
		TYPE_INVALID = 0,
		TYPE_KING = 1,
		TYPE_QUEEN = 2,
		TYPE_ROOK = 3,
		TYPE_BISHOP = 4,
		TYPE_KNIGHT = 5,
		TYPE_PAWN = 6
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
		ChessPiece::COLOR color;
		ChessPiece::TYPE type;
		///THE UPCOUNTING NUMBER OF THE FIGURE TO DETERMN THE PARK POSITION, THE NUMBER IS ONLY UNIQUE COMBINED WITH THE OTHER
		///START COUNTING FROM 0
		int figure_number;
		///THIS IS THE UNIQUE ID OF THE FIGURE HWICH IS ALSO WRITTEN ON THE NFC TAG
		unsigned char  unique_id;
		
	};
	
	static unsigned char figure2NDEF(ChessPiece::FIGURE _figure);
	static ChessPiece::FIGURE NDEF2Figure(unsigned char _ndef_id);
};
#endif

