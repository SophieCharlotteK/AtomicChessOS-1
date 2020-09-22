#ifndef GUICOMMUNICATOR_H
#define GUICOMMUNICATOR_H

#include "magic_enum-master/include/magic_enum.hpp"
#include "PROTOCOL_MSG.pb.h"
#include <czmq.h>
#include <string>
#include <queue>


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
   BEGIN_BTN = 0,
   MATCHMAKING_BTN = 1,
   INITFIELD_BTN = 2,
   ATC_LOGO_BTN = 3,
    CLOUD_LOGO_BTN = 4,
    OPEN_SETTINGS_BTN = 5
};


enum class GUI_VALUE_TYPE{
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

    //KEEP IN SYNC WITH THE PROTOCOL_MSG.proto
    struct GUI_EVENT{
        GUI_ELEMENT event;
        GUI_VALUE_TYPE type;
        std::string value;
        int ack;
        int ispageswitchevent;

        bool is_event_valid;
    };


    guicommunicator();
    ~guicommunicator();


    void createEvent(GUI_ELEMENT _event, GUI_VALUE_TYPE _type, std::string _value); //sends a event though ZeroMQ using protocol buffer
    //DERIVATIONS FRom createEvent
    void createEvent(GUI_ELEMENT _event, GUI_VALUE_TYPE _type);
    #ifdef USES_QT
    void createEvent(GUI_ELEMENT _event, GUI_VALUE_TYPE _type, QString _value);
    #endif

    void debug_output(std::string _msg);
    void debug_event(GUI_EVENT _event);
private:
     zsock_t* zmq_push = nullptr;
     zsock_t* zmq_pull = nullptr;

    GUI_EVENT parseEvent(std::string _event); //PARSES A EVENT TO struct GUIEVENT

};

#endif // GUICOMMUNICATOR_H
