#ifndef MAINMENU_H
#define MAINMENU_H

#include <QObject>
#include <QQuickItem>
#include <QDebug> //FOR WRITING TO THE APPLICATION OUTPUT CONSOLE WITH qInfo
class MenuManager: public QObject
{
    Q_OBJECT
   // Q_PROPERTY(float best READ best WRITE setBest NOTIFY setBestChanged)
public:
    MenuManager(QObject* _parent);
    MenuManager();
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

};

#endif // MAINMENU_H
