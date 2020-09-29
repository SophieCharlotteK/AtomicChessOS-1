#ifndef MAINMENU_H
#define MAINMENU_H

#include <QObject>
#include <QQuickItem>
#include <QDebug> //FOR WRITING TO THE APPLICATION OUTPUT CONSOLE WITH qInfo
#include <QTimer> // FOR REFRESH INTERVAL
#include <QThread>

#include <QString>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>

#include "SHARED/guicommunicator.h"
class MenuManager: public QObject
{
    Q_OBJECT
   // Q_PROPERTY(float best READ best WRITE setBest NOTIFY setBestChanged)
public:
    MenuManager(QObject* _parent);
    MenuManager();
    ~MenuManager();

    static QQuickView* v;

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
    
    //------ SETTINGS MENU ---- //
    void sm_open_settings_btn();

    void sm_init_btn();
    void sm_logout_btn();
    void sm_scan_board_btn();
private:
    void show_error(QString _err);
    guicommunicator guiconnection;
    void switch_menu(guicommunicator::GUI_VALUE_TYPE _screen);
    void set_visible_element(QString _name, bool _state);
    void updateProgress();
};

#endif // MAINMENU_H
