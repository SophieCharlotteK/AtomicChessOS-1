#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <QObject>
#include <QMainWindow>
#include <QQuickItem>

#include <QWidget>
#include <QMessageBox>

class main_menu;

class main_menu: public QObject
{
    Q_OBJECT
    Q_PROPERTY(void sfg READ get_sfp_toggle WRITE set_sfp_toggle NOTIFY sfp_toggle_changed)
    Q_PROPERTY(QString get_sfp_timer_text READ set_sfp_timer_text WRITE setName NOTIFY sft_timer_text_changed)

public:
    explicit main_menu (QObject* parent = 0) : QObject(parent) {}


    virtual ~main_menu() {};
public slots:
    void start_ai_match();
    void goto_search_player_view();
    void goto_settings();

signals:
    void sfp_toggle_changed();
    void sft_timer_text_changed();


private:
    bool get_sfp_toggle();
    void set_sfp_toggle(bool _state);
    QString get_sfp_timer_text();
    void set_sfp_timer_text(QString _time);






};

#endif // MAIN_MENU_H
