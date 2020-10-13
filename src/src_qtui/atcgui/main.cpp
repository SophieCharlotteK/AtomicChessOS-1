#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <rpc/rpc.h>
#include <rpcsvc/rusers.h>
#include <stdio.h>
#include "menumanager.h"
int main(int argc, char *argv[])
{
  //  qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
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
    //The following step is important, to make the c++ backend (menumanager.h) working
    //The instance of the mainmenu manager has no parent, but to access/search object in the qmlgraph
    //the menu manager instance need a parent. So this code finds the menumanager instance and assign the parent with the setparent funktion
    //the new parent is the window root object
    QObject *object = view.rootObject();
    qInfo() <<object->objectName();
    QObject *rect = object->findChild<QObject*>("mainmenu");
    if (rect){
        qInfo()<< rect->objectName();
           rect->setParent(object);
    }


    return app.exec();
}

