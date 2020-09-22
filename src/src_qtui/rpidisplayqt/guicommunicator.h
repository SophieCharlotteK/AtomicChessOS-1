#ifndef GUICOMMUNICATOR_H
#define GUICOMMUNICATOR_H

#include "magic_enum-master/include/magic_enum.hpp"
#include "PROTOCOL_MSG.pb.h"
#include <czmq.h>
#include <string>



//SOME DEBUGGING AND LOGGIN
//ON QT USE THE QT STUFF
#ifdef USES_QT
    #include <QString>
    #include<QDebug>
#else
    #include "loguru-master/loguru.hpp"
#endif
class guicommunicator
{
public:

enum class GUI_ELEMENT{
    UNKNOWN = -1,
   BEGIN_BTN = 0,
   MATCHMAKING_BTN = 1,
   INITFIELD_BTN = 2,

   BEGIN_MATCH_WITH_PLAYER
};


enum class GUI_VALUE_TYPE{
     UNKNOWN = -1,
    CLICKED = 0,
    SELECTED = 1,
    USER_INPUT_STRING = 2,
    USER_INPUT_NUMBER = 3,
    ENABLED = 4,
    DISBALED = 5,


    //SPECIAL TYPES
    AVARIABLED_PLAYERS_ARRAY = 6, //DATA FOR AVARIABLE PLAYER ARRAY VIEW
    CHESS_MOVE = 7, //IS A C
    SWITCH_SCREEN = 8,
    PLAYER_ID = 9 // THE PLAYER ID USED ON BEGIN_MATCH_WITH_PLAYER ELEMENT

};


    struct GUI_EVENT{

    };

    guicommunicator();
    ~guicommunicator();

    void createEvent(GUI_ELEMENT _element, GUI_VALUE_TYPE _type);
    void createEvent(GUI_ELEMENT _element, GUI_VALUE_TYPE _type, std::string _value);
    
    #ifdef USES_QT
    void createEvent(GUI_ELEMENT _element, GUI_VALUE_TYPE _type, QString _value);
    #endif

    void debug_output(std::string _msg);

private:
     zsock_t* zmq_push = nullptr;
     zsock_t* zmq_pull = nullptr;

    void sendEvent(std::string); //SENDS DATATA TO BACKEND USING ZeroMQ


};

#endif // GUICOMMUNICATOR_H
