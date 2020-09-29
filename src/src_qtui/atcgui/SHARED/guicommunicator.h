#ifndef GUICOMMUNICATOR_H
#define GUICOMMUNICATOR_H

//#include "magic_enum-master/include/magic_enum.hpp"
//#include "SHARED/_MODIFIED_rpclib-master/include/rpc/client.h"
//#include "SHARED/_MODIFIED_rpclib-master/include/rpc/server.h"
//nclude "r /include/rpc/server.h"



#include "PROTOCOL_MSG.pb.h"

#include <string>
#include <queue>
#include <mutex>
#include <iostream>
#include <thread>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>




//SOME DEBUGGING AND LOGGIN
//ON QT USE THE QT STUFF
#ifdef USES_QT
    #include <QString>
    #include<QDebug>
    #include <QThread>
    #include <QMutex>
#else

#endif

/*
#ifdef USES_QT
#define RPC_PORT 8000
#define RPC_URL "127.0.0.1"
#define RPC_FKT_NAME "rpc_callback"
#else
#define RPC_PORT 8000
#define RPC_URL "127.0.0.1",
#define RPC_FKT_NAME "rpc_callback"
#endif
*/



class guicommunicator
{
public:

enum class GUI_ELEMENT{
    BEGIN_BTN = 0,
    MATCHMAKING_BTN = 1,
    INITFIELD_BTN = 2,
    ATC_LOGO_BTN = 3,
    CLOUD_LOGO_BTN = 4,
    OPEN_SETTINGS_BTN = 5,
    LOGOUT_BTN = 6,
    INIT_BTN = 7,
    SCAN_BOARD_BTN = 8,
    SWITCH_MENU = 9
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
    PLAYER_ID = 9, // THE PLAYER ID USED ON BEGIN_MATCH_WITH_PLAYER ELEMENT

    RES1 = 10,
    RE2 = 11,
    RES3 =12,
    //MENUS
    LOGIN_SCREEN = 13,
    MAIN_MENU_SCREEN = 14,
    SETTINGS_SCREEN= 15,
    PLAYER_SEARCH_SCREEN = 16,
    GAME_SCREEN = 17,
    CREDIT_SCREEN = 18

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
    void debug_event(GUI_EVENT _event, bool _rec);

    void start_recieve_thread();
    void stop_recieve_thread();


    GUI_EVENT get_gui_update_event();
private:
	  //zmq::context_t zmqctx;
	// zsock_t* zmq_pull = nullptr;
	

#ifdef USES_QT
      QThread* update_thread = nullptr;
      QMutex update_thread_mutex;
#else
    std::thread* update_thread = nullptr;
    std::mutex update_thread_mutex;
	
#endif

    std::queue<GUI_EVENT> gui_update_event_queue;
    
	std::string rpc_callback(std::string _msg);
	
	bool thread_running = false;
    //rpc::server* ptrsrv = nullptr;

     GUI_EVENT parseEvent(std::string _event); //PARSES A EVENT TO struct GUIEVENT

     static void recieve_thread_function(guicommunicator* _this);
};

#endif // GUICOMMUNICATOR_H
