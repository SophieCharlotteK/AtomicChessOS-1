#pragma once
#ifndef __CHESSPIECE_H__
#define __CHESSPIECE_H__
class ChessPiece
{
public:
	
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
	
	enum  COLOR
	{
	        COLOR_UNKNOWN,
			COLOR_BLACK,
			COLOR_WHITE
	};
	
	struct FIGURE
	{
		ChessPiece::COLOR _color;
		ChessPiece::TYPE _type;
		int figure_id;
	};
};
#endif

