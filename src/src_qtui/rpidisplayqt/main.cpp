#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include "menumanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QWindow window;
    window.setBaseSize(QSize(800,480));

    qmlRegisterType<MenuManager>("MenuManager",1,0,"MenuManager");

    QQuickView view;
    view.setHeight(480);
    view.setWidth(800);
    view.engine()->addImportPath("qrc:/qml/imports");
    view.setSource(QUrl("qrc:/qml/WINDOW.qml"));
    view.engine()->rootContext()->setContextProperty("app", &app);
    view.setResizeMode(QQuickView::SizeRootObjectToView); //SIZE WINDOW TO VIEW SIZE !!
    if(!view.errors().empty()){
           return -1;
    }
    view.show();
    return app.exec();
}
