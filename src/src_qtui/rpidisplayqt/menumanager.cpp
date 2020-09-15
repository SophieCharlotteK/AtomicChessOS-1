#include "menumanager.h"

MenuManager::MenuManager()
{
qInfo() <<"MenuManager constructor called";
}


void MenuManager::lb_settings_btn(){
qInfo() <<"lb_settings_btn";
};

void MenuManager::ls_login_btn(){
qInfo() <<"ls_login_btn";
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
