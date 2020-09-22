#include "menumanager.h"


MenuManager::MenuManager()
{
qInfo() <<"MenuManager constructor called";


 //        zmq_push = zsock_new_push ("inproc://example");
 //       zmq_pull = zsock_new_pull ("inproc://example");
  // zstr_send (push, "Hello, World ZEROMQ");

 //  char *string = zstr_recv (pull);
 //  qInfo() << string;
  // zstr_free (&string);
    


}

MenuManager::~MenuManager(){
    qInfo() << "DETRUCTOR CALLED";
    
}


//void MenuManager::send_gui_event(guiinterface::GUI_EVENT _event){
 //   if(!zmq_push){
  //      show_error("zmq_push is null");
   //     return;
   // }

  //  QString tmp = guiinterface::Event2String(_event);
   // qInfo() << tmp;
   // zstr_send (zmq_push, tmp.toStdString().c_str());
//}




void MenuManager::show_error(QString _err){
    qInfo() << _err;
    //TODO SHWITCH TO ERR VIEW
}


void MenuManager::lb_settings_btn(){
qInfo() <<"lb_settings_btn";
};

void MenuManager::ls_login_btn(){
qInfo() <<"ls_login_btn";
guiconnection.createEvent(guicommunicator::GUI_ELEMENT::BEGIN_BTN, guicommunicator::GUI_VALUE_TYPE::CLICKED);
}


void MenuManager::sm_open_settings_btn(){
    qInfo() <<"sm_open_settings_btn";
   // guiconnection.createEvent(guicommunicator::GUI_ELEMENT::OPEN_SETTINGS_BTN, guicommunicator::GUI_VALUE_TYPE::CLICKED);
}





void MenuManager::mm_start_ai_btn(){
    qInfo() <<"start_ai";
}

void MenuManager::mm_player_list_btn(){
qInfo() << "goto_search_player_view";
}


void MenuManager::mm_search_for_players_toggled(bool _state){
qInfo() << "search_for_players_toggled";
if(_state){
    qInfo() <<"1";
}else{
qInfo() << "0";
}

}
