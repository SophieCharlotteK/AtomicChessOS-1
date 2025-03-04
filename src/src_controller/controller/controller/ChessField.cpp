#include "ChessField.h"


int ChessField::field2Index(ChessField::CHESS_FILEDS _field)
{
	return static_cast<int>(_field);
}

ChessField::CHESS_FILEDS ChessField::Index2Field(int _field){
	return static_cast<ChessField::CHESS_FILEDS>(_field);
}


//https://stackoverflow.com/questions/10058606/splitting-a-string-by-a-character/10058756
std::vector<std::string> ChessField::split(std::string _input, char _char){
    std::stringstream test;
    test.str(_input); //LOAD STRING

    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(test, segment, _char))
    {
        seglist.push_back(segment);
    }

    return seglist;
}



std::string ChessField::field_to_string(ChessField::CHESS_FILEDS _f){

    std::string tmp = std::string(magic_enum::enum_name(_f));
    std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower); //CONVER TO LOWER
    const std::vector<std::string> split_res = split(tmp,'_'); //SPLIT BY _ CHARAKTER

    //CHECK SPLIT RESULT TO GET TYPE OF FIELD PP OR NORMAL
    if(split_res.size() == 3){//NON PARKPOS
        return  split_res[2]; //STANDART FORMAT e2 => e2h4
    }else if(split_res.size() == 6){//PARKPOS
        return "[park" + split_res[4] + split_res[5] + "]"; //non standart so in brackets
    }

    return tmp;
}


bool ChessField::is_row_over_row(ChessField::CHESS_FILEDS _a, ChessField::CHESS_FILEDS _b)
{
	int heigth_a = field2Index(_a) / 8;
	int heigth_b = field2Index(_b) / 8;
	
	
	//CHECK FOR PARK POS
	if (heigth_a < 64 && heigth_b < 64)
	{
		if (heigth_a > heigth_b)
		{
		
			return true;
		}
		return false;	
	}
	
	
}

int ChessField::get_board_index_from_field(ChessField::CHESS_FILEDS _field)
{
	const int LUT[96] = { 
		86,74,62,50,38,26,14,2, //A
		87,75,63,51,39,27,15,3, //B
		88,76,64,52,40,28,16,4, //C
		89,77,65,53,41,29,17,5, //D
		90,78,66,54,42,30,18,6, //E
		91,79,67,55,43,31,19,7, //F
		92,80,68,56,44,32,20,8, //G
		93,81,69,57,45,33,21,9, //H
		
		0,1,12,13,24,25,36,37,48,49,60,61,72,73,84,85, //PARK POSITION WHITE
		10,11,22,23,34,35,46,47,58,59,70,71,82,83,94,95	 //PARK POSITION BLACK
		};
	
	
	return LUT[static_cast<int>(_field)];
	
}

int ChessField::parkpos2IDIndex(ChessField::CHESS_FILEDS _field)
{
	

	int index = field2Index(_field);
	
	if (index >= field2Index(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_1) && index  <= field2Index(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_16))
	{
		return index - field2Index(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_1);
	}
	else 
	if (index >= field2Index(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_1) && index  <= field2Index(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_16))
	{
		return index - field2Index(ChessField::CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_1);
	}
	
	
	return -1;
	
}

ChessField::CHESS_FILEDS ChessField::get_field_from_board_index(int _board_index)
{
	
	const CHESS_FILEDS LUT[96] = { 
		//-------------- ROW 0 ---------------------------- //
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_1,
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_2,
		
		CHESS_FILEDS::CHESS_FIELD_A8,
		CHESS_FILEDS::CHESS_FIELD_B8,
		CHESS_FILEDS::CHESS_FIELD_C8,
		CHESS_FILEDS::CHESS_FIELD_D8,
		CHESS_FILEDS::CHESS_FIELD_E8,
		CHESS_FILEDS::CHESS_FIELD_F8,
		CHESS_FILEDS::CHESS_FIELD_G8,
		CHESS_FILEDS::CHESS_FIELD_H8,
			
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_1,
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_2,
		
		
		
		//-------------- ROW 1 ---------------------------- //
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_3,
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_4,
		
		CHESS_FILEDS::CHESS_FIELD_A7,
		CHESS_FILEDS::CHESS_FIELD_B7,
		CHESS_FILEDS::CHESS_FIELD_C7,
		CHESS_FILEDS::CHESS_FIELD_D7,
		CHESS_FILEDS::CHESS_FIELD_E7,
		CHESS_FILEDS::CHESS_FIELD_F7,
		CHESS_FILEDS::CHESS_FIELD_G7,
		CHESS_FILEDS::CHESS_FIELD_H7,
			
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_3,
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_4,
		
		
		
		//-------------- ROW 2 ---------------------------- //
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_5,
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_6,
		
		CHESS_FILEDS::CHESS_FIELD_A6,
		CHESS_FILEDS::CHESS_FIELD_B6,
		CHESS_FILEDS::CHESS_FIELD_C6,
		CHESS_FILEDS::CHESS_FIELD_D6,
		CHESS_FILEDS::CHESS_FIELD_E6,
		CHESS_FILEDS::CHESS_FIELD_F6,
		CHESS_FILEDS::CHESS_FIELD_G6,
		CHESS_FILEDS::CHESS_FIELD_H6,
			
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_5,
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_6,
		
		
		//-------------- ROW 3 ---------------------------- //
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_7,
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_8,
		
		CHESS_FILEDS::CHESS_FIELD_A5,
		CHESS_FILEDS::CHESS_FIELD_B5,
		CHESS_FILEDS::CHESS_FIELD_C5,
		CHESS_FILEDS::CHESS_FIELD_D5,
		CHESS_FILEDS::CHESS_FIELD_E5,
		CHESS_FILEDS::CHESS_FIELD_F5,
		CHESS_FILEDS::CHESS_FIELD_G5,
		CHESS_FILEDS::CHESS_FIELD_H5,
			
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_7,
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_8,
		
		//-------------- ROW 4 ---------------------------- //
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_9,
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_10,
		
		CHESS_FILEDS::CHESS_FIELD_A4,
		CHESS_FILEDS::CHESS_FIELD_B4,
		CHESS_FILEDS::CHESS_FIELD_C4,
		CHESS_FILEDS::CHESS_FIELD_D4,
		CHESS_FILEDS::CHESS_FIELD_E4,
		CHESS_FILEDS::CHESS_FIELD_F4,
		CHESS_FILEDS::CHESS_FIELD_G4,
		CHESS_FILEDS::CHESS_FIELD_H4,
			
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_9,
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_10,
		
		
		
		//-------------- ROW 5 ---------------------------- //
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_11,
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_12,
		
		CHESS_FILEDS::CHESS_FIELD_A3,
		CHESS_FILEDS::CHESS_FIELD_B3,
		CHESS_FILEDS::CHESS_FIELD_C3,
		CHESS_FILEDS::CHESS_FIELD_D3,
		CHESS_FILEDS::CHESS_FIELD_E3,
		CHESS_FILEDS::CHESS_FIELD_F3,
		CHESS_FILEDS::CHESS_FIELD_G3,
		CHESS_FILEDS::CHESS_FIELD_H3,
			
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_11,
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_12,
		
		
		//-------------- ROW 6 ---------------------------- //
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_13,
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_14,
		
		CHESS_FILEDS::CHESS_FIELD_A2,
		CHESS_FILEDS::CHESS_FIELD_B2,
		CHESS_FILEDS::CHESS_FIELD_C2,
		CHESS_FILEDS::CHESS_FIELD_D2,
		CHESS_FILEDS::CHESS_FIELD_E2,
		CHESS_FILEDS::CHESS_FIELD_F2,
		CHESS_FILEDS::CHESS_FIELD_G2,
		CHESS_FILEDS::CHESS_FIELD_H2,
			
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_13,
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_14,
		
		
		//-------------- ROW 7 ---------------------------- //
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_15,
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_WHITE_16,
		
		CHESS_FILEDS::CHESS_FIELD_A1,
		CHESS_FILEDS::CHESS_FIELD_B1,
		CHESS_FILEDS::CHESS_FIELD_C1,
		CHESS_FILEDS::CHESS_FIELD_D1,
		CHESS_FILEDS::CHESS_FIELD_E1,
		CHESS_FILEDS::CHESS_FIELD_F1,
		CHESS_FILEDS::CHESS_FIELD_G1,
		CHESS_FILEDS::CHESS_FIELD_H1,
			
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_15,
		CHESS_FILEDS::CHESS_FIELD_PARK_POSTION_BLACK_16,
	};
	
	
	return LUT[_board_index];
}