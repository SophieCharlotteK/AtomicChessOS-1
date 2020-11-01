#include "ChessPiece.h"

unsigned char ChessPiece::figure2NDEF(ChessPiece::FIGURE _figure)
{
	
	int type = _figure.type;
	bool color = _figure.color;
	int num = _figure.figure_number;
	
	unsigned char unique_id = 0;
	//APPLY TYPE
	unique_id |= (BIT_CHECK(_figure.type, 0) << 5);
	unique_id |= (BIT_CHECK(_figure.type, 1) << 6);
	unique_id |= (BIT_CHECK(_figure.type, 2) << 7);
	
	//APPLY COLOR
	unique_id |= (BIT_CHECK(_figure.color, 0) << 4);
	
	unique_id |= (BIT_CHECK(_figure.figure_number, 0) << 0);
	unique_id |= (BIT_CHECK(_figure.figure_number, 1) << 1);
	unique_id |= (BIT_CHECK(_figure.figure_number, 2) << 2);
	unique_id |= (BIT_CHECK(_figure.figure_number, 3) << 3);
	return unique_id;
	int  b = 0;
}

ChessPiece::FIGURE ChessPiece::NDEF2Figure(unsigned char _ndef_id)
{
}