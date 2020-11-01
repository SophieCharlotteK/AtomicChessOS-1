#include "ChessPiece.h"

unsigned char ChessPiece::figure2NDEF(ChessPiece::FIGURE& _figure)
{
	unsigned char unique_id = 0;
	//APPLY TYPE
	unique_id |= (BIT_CHECK(_figure.type, 0) << 5);
	unique_id |= (BIT_CHECK(_figure.type, 1) << 6);
	unique_id |= (BIT_CHECK(_figure.type, 2) << 7);
	//APPLY COLOR
	unique_id |= (BIT_CHECK(_figure.color, 0) << 4);
	//APPLY UPCOUNTING NUMBER
	unique_id |= (BIT_CHECK(_figure.figure_number, 0) << 0);
	unique_id |= (BIT_CHECK(_figure.figure_number, 1) << 1);
	unique_id |= (BIT_CHECK(_figure.figure_number, 2) << 2);
	unique_id |= (BIT_CHECK(_figure.figure_number, 3) << 3);
	//OVERWRITE UNIQUE ID IN THE REFERENCE
	_figure.unique_id = unique_id;
	return unique_id;
}

ChessPiece::FIGURE ChessPiece::NDEF2Figure(unsigned char _ndef_id)
{
	
	ChessPiece::FIGURE tmp;
	tmp.unique_id = _ndef_id;
	
	//SPLIT TYPE
	int type_tmp = 0;
	type_tmp |= (BIT_CHECK(_ndef_id, 5) << 0);
	type_tmp |= (BIT_CHECK(_ndef_id, 6) << 1);
	type_tmp |= (BIT_CHECK(_ndef_id, 7) << 2);
	tmp.type = static_cast<ChessPiece::TYPE>(type_tmp);
	//SPLITE COLOR
	if (BIT_CHECK(_ndef_id, 4)) {
		tmp.color = ChessPiece::COLOR::COLOR_BLACK;
	}else {
		tmp.color = ChessPiece::COLOR::COLOR_WHITE;	
	}
	//SPLIT UPCOUNTING NUMBER
	int num_tmp =0;
	num_tmp |= (BIT_CHECK(_ndef_id, 0) << 0);
	num_tmp |= (BIT_CHECK(_ndef_id, 1) << 1);
	num_tmp |= (BIT_CHECK(_ndef_id, 2) << 2);
	num_tmp |= (BIT_CHECK(_ndef_id, 3) << 2);
	tmp.figure_number = num_tmp;
	
	return tmp;
}


void ChessPiece::FigureDebugPrint(ChessPiece::FIGURE _figure)
{
#ifdef USE_STD_LOG
	std::cout << "=== FIGURE PRINT ===" << std::endl;
	
	if (_figure.color == ChessPiece::COLOR::COLOR_BLACK) {
		std::cout << "COLOR: BLACK (1)" << std::endl;			  
	}
	else if (_figure.color == ChessPiece::COLOR::COLOR_WHITE) {
		std::cout << "COLOR: WHITE (0)" << std::endl;			  
	}else if (_figure.color == ChessPiece::COLOR::COLOR_UNKNOWN) {
		std::cout << "COLOR: COLOR_UNKNOWN (2)" << std::endl;			  
	}
	
	std::cout << "UNIQUE_ID: "<< _figure.unique_id << std::endl;	
	
	std::cout << "FIGURE NUMBER: " << _figure.figure_number << std::endl;	
	
	switch (_figure.type)
	{
	case ChessPiece::TYPE::TYPE_INVALID:
		std::cout << "FIGURE TYPE: TYPE_INVALID (" << _figure.type << ")" << std::endl;	
		break;
	case ChessPiece::TYPE::TYPE_KING:
		std::cout << "FIGURE TYPE: TYPE_KING (" << _figure.type << ")" << std::endl;	
		break;
	case ChessPiece::TYPE::TYPE_QUEEN:
		std::cout << "FIGURE TYPE: TYPE_QUEEN (" << _figure.type << ")" << std::endl;	
		break;
	case ChessPiece::TYPE::TYPE_ROOK:
		std::cout << "FIGURE TYPE: TYPE_ROOK (" << _figure.type << ")" << std::endl;	
		break;
	case ChessPiece::TYPE::TYPE_BISHOP:
		std::cout << "FIGURE TYPE: TYPE_BISHOP (" << _figure.type << ")" << std::endl;	
		break;
	case ChessPiece::TYPE::TYPE_KNIGHT:
		std::cout << "FIGURE TYPE: TYPE_KNIGHT (" << _figure.type << ")" << std::endl;	
		break;
	case ChessPiece::TYPE::TYPE_PAWN:
		std::cout << "FIGURE TYPE: TYPE_PAWN (" << _figure.type << ")" << std::endl;	
		break;
	default:
		std::cout << "FIGURE TYPE: UNKNOWN (" << _figure.type << ")" << std::endl;
		break;
	}
	
	
	std::cout << "====================" << std::endl;
#endif // USE_STD_LOG

}