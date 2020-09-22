#ifndef MAINMENU_H
#define MAINMENU_H

#include <QObject>
#include <QQuickItem>
#include <QDebug> //FOR WRITING TO THE APPLICATION OUTPUT CONSOLE WITH qInfo



#include <QString>


#include "guicommunicator.h"
class MenuManager: public QObject
{
    Q_OBJECT
   // Q_PROPERTY(float best READ best WRITE setBest NOTIFY setBestChanged)
public:
    MenuManager(QObject* _parent);
    MenuManager();
    ~MenuManager();
signals:

    //void setBestChanged();
public slots:
    // ----- HEADLINE BAR ---- //
    void lb_settings_btn();
    // ----- LOGIN/START ----//
    void ls_login_btn();
    //------ MAIN MENU ---- //
    void mm_start_ai_btn();
    void mm_player_list_btn();
    void mm_search_for_players_toggled(bool _state);
private:
    void show_error(QString _err);
    guicommunicator guiconnection;

};

#endif // MAINMENU_H
