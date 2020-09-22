QT += quick \
    widgets

# USING C++ 17 IS REQUIRED
CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17



DEFINES += USES_QT

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    guicommunicator.cpp \
    main.cpp \
    menumanager.cpp

RESOURCES += qml.qrc

include($$PWD/czmq/bindings/qt/common.pri)

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    guicommunicator.h \
    magic_enum-master/include/magic_enum.hpp \
    menumanager.h $$PWD/czmq/include/czmq.h

LIBS += -lzmq

DISTFILES += \
    PROTOCOL_MSG.proto
