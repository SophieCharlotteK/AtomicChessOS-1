#include "menumanager.h"


MenuManager::MenuManager(QObject* _parent){
    qInfo() <<_parent->objectName();
}

MenuManager::MenuManager()
{
qInfo() <<"MenuManager constructor called";


QTimer *timer = new QTimer(this);
 connect(timer, &QTimer::timeout, this, &MenuManager::updateProgress);
 timer->start(500);






guiconnection.start_recieve_thread();

}


void MenuManager::set_visible_element(QString _name, bool _state){

    QObject* recht1 = this->parent()->findChild<QObject*>(_name);
    if(recht1){
        recht1->setProperty("visible",_state);
    }else{
        qInfo()<< "cant get element" << _name;
    }

}
void MenuManager::switch_menu(guicommunicator::GUI_VALUE_TYPE _screen){


    set_visible_element("mm_container",false);
    set_visible_element("ls_container",false);
    set_visible_element("ss_container",false);

    switch (_screen) {
        case guicommunicator::GUI_VALUE_TYPE::MAIN_MENU_SCREEN:{set_visible_element("mm_container",true);break;}
        case guicommunicator::GUI_VALUE_TYPE::LOGIN_SCREEN:{set_visible_element("ls_container",true);break;}
        case guicommunicator::GUI_VALUE_TYPE::SETTINGS_SCREEN:{set_visible_element("ss_container",true);break;}
        default:break;
    }
}

void MenuManager::updateProgress()
{
    guicommunicator::GUI_EVENT ev=  guiconnection.get_gui_update_event();

    if(!ev.is_event_valid){return;}

    //SWITCH MAIN MENU REQUEST
    if(ev.event == guicommunicator::GUI_ELEMENT::SWITCH_MENU){
        switch_menu(ev.type);
    }

}


MenuManager::~MenuManager(){
    qInfo() << "DETRUCTOR CALLED";
    //update_thread->terminate();
    guiconnection.stop_recieve_thread();
}


void MenuManager::show_error(QString _err){
    qInfo() << _err;
    //TODO SHWITCH TO ERR VIEW
}


void MenuManager::lb_settings_btn(){
    qInfo() <<"lb_settings_btn";
};

void MenuManager::ls_login_btn(){
    qInfo() <<"ls_login_btn";
    guiconnection.createEvent(guicommunicator::GUI_ELEMENT::SWITCH_MENU, guicommunicator::GUI_VALUE_TYPE::MAIN_MENU_SCREEN);
    //guiconnection.createEvent(guicommunicator::GUI_ELEMENT::BEGIN_BTN, guicommunicator::GUI_VALUE_TYPE::CLICKED);

}


void MenuManager::sm_open_settings_btn(){
    qInfo() <<"sm_open_settings_btn";
}



void MenuManager::sm_logout_btn(){
    qInfo() << "sm_logout_btn";
    guiconnection.createEvent(guicommunicator::GUI_ELEMENT::LOGOUT_BTN, guicommunicator::GUI_VALUE_TYPE::CLICKED);
}

void MenuManager::sm_init_btn(){
    qInfo() << "sm_init_btn";
    guiconnection.createEvent(guicommunicator::GUI_ELEMENT::INIT_BTN, guicommunicator::GUI_VALUE_TYPE::CLICKED);
    
}
void MenuManager::sm_scan_board_btn(){
    qInfo() << "sm_scan_board_btn";
    guiconnection.createEvent(guicommunicator::GUI_ELEMENT::SCAN_BOARD_BTN, guicommunicator::GUI_VALUE_TYPE::CLICKED);
    
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
        guiconnection.createEvent(guicommunicator::GUI_ELEMENT::MATCHMAKING_BTN, guicommunicator::GUI_VALUE_TYPE::ENABLED);
    }else{
        qInfo() << "0";
        guiconnection.createEvent(guicommunicator::GUI_ELEMENT::MATCHMAKING_BTN, guicommunicator::GUI_VALUE_TYPE::DISBALED);
    }
}
