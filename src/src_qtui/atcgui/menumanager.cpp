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
guiconnection.check_guicommunicator_version();
guiconnection.createEvent(guicommunicator::GUI_ELEMENT::QI_START_EVENT,guicommunicator::GUI_VALUE_TYPE::ENABLED);
}


void MenuManager::set_label_text(QString _container_name, QString _labelname,QString _text){
    QObject* recht1 = this->parent()->findChild<QObject*>(_container_name);
    if(recht1){
        QObject* recht2 = recht1->findChild<QObject*>(_labelname);
        if(recht2){
            recht2->setProperty("text",_text);
        }else{
            qInfo()<< "cant get element" << "is_container";
        }
    }else{
        qInfo()<< "cant get element" << "is_container";
    }
}

void MenuManager::set_visible_element(QString _name, bool _state){

    QObject* recht1 = this->parent()->findChild<QObject*>(_name);
    if(recht1){
        recht1->setProperty("visible",_state);
    }else{
        qInfo()<< "cant get element" << _name;
    }

}


void MenuManager::go_menu_back(){
switch_menu(last_menu_opened);
}

void MenuManager::lb_info_btn(){
    MenuManager::switch_menu(guicommunicator::GUI_VALUE_TYPE::INFO_SCREEN);
}

void MenuManager::switch_menu(QString _screen){

    last_menu_opened = current_menu_opened;
    current_menu_opened = _screen;
    set_visible_element("mm_container",false);
    set_visible_element("ls_container",false);
    set_visible_element("ss_container",false);
    set_visible_element("is_container",false);

    set_visible_element(_screen,true);
}

void MenuManager::switch_menu(guicommunicator::GUI_VALUE_TYPE _screen){
    switch (_screen) {
        case guicommunicator::GUI_VALUE_TYPE::MAIN_MENU_SCREEN:{switch_menu("mm_container");break;}
        case guicommunicator::GUI_VALUE_TYPE::LOGIN_SCREEN:{switch_menu("ls_container");break;}
        case guicommunicator::GUI_VALUE_TYPE::SETTINGS_SCREEN:{switch_menu("ss_container");break;}
        case guicommunicator::GUI_VALUE_TYPE::INFO_SCREEN:{switch_menu("is_container");break;}
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

    if(ev.event == guicommunicator::GUI_ELEMENT::INFOSCREEN_HWID_LABEL){
        set_label_text("is_container","is_hwid_label",QString::fromStdString(ev.value));
    }
    if(ev.event == guicommunicator::GUI_ELEMENT::INFOSCREEN_SESSIONID_LABEL){
        set_label_text("is_container","is_sessionid_label",QString::fromStdString(ev.value));
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
    //qInfo() <<"lb_settings_btn";
   // MenuManager::switch_menu(guicommunicator::GUI_VALUE_TYPE::SETTINGS_SCREEN);
};

void MenuManager::ls_login_btn(){
    qInfo() <<"ls_login_btn";
    guiconnection.createEvent(guicommunicator::GUI_ELEMENT::BEGIN_BTN, guicommunicator::GUI_VALUE_TYPE::CLICKED);

}

void MenuManager::is_open_is_screen_btn(){
    MenuManager::switch_menu(guicommunicator::GUI_VALUE_TYPE::INFO_SCREEN);
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
    guiconnection.createEvent(guicommunicator::GUI_ELEMENT::MAINMENU_START_AI_MATCH_BTN, guicommunicator::GUI_VALUE_TYPE::CLICKED);
}

void MenuManager::mm_player_list_btn(){
    qInfo() << "goto_search_player_view";
    guiconnection.createEvent(guicommunicator::GUI_ELEMENT::MAINMENU_PLAYER_LIST_BTN, guicommunicator::GUI_VALUE_TYPE::CLICKED);
}


void MenuManager::mm_search_for_players_toggled(bool _state){
    qInfo() << "search_for_players_toggled";
    if(_state){
        qInfo() <<"1";
        guiconnection.createEvent(guicommunicator::GUI_ELEMENT::MAINMENU_START_AI_MATCH_BTN, guicommunicator::GUI_VALUE_TYPE::ENABLED);
    }else{
        qInfo() << "0";
        guiconnection.createEvent(guicommunicator::GUI_ELEMENT::MAINMENU_START_AI_MATCH_BTN, guicommunicator::GUI_VALUE_TYPE::DISBALED);
    }
}
