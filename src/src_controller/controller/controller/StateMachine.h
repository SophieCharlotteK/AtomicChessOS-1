#pragma once
#ifndef __StateMachine__
#define __StateMachine__

#include "BackendConnector.h"

class StateMachine
{
	
	
public:
	enum class SM_STATE
	{
		UNKNOWN
			
	}; 
	
	StateMachine();
	
	
	
	SM_STATE determ_next_state(BackendConnector::PLAYER_STATUS _ps);
	SM_STATE current_next_state(BackendConnector::PLAYER_STATUS _ps);
	SM_STATE switch_to_next_state(BackendConnector::PLAYER_STATUS _ps);
	
	
	
	
	
	
private:
	SM_STATE current_state = SM_STATE::UNKNOWN;
};
#endif

