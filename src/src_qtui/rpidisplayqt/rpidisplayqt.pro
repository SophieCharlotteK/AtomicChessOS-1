QT += quick \
    widgets

# USING C++ 17 IS REQUIRED
CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17
#QMAKE_CXXFLAGS += -std:c++17
#WORKAROUND FOR BUILDROOT COMPILE https://stackoverflow.com/questions/51196806/tell-qmake-std-c17-qmake-cxxflags-overridden
#QMAKE_CXXFLAG  += c++1z


DEFINES += USES_QT

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SHARED/PROTOCOL_MSG.pb.cc \
    SHARED/guicommunicator.cpp \
    main.cpp \
    menumanager.cpp

RESOURCES += qml.qrc



# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    SHARED/PROTOCOL_MSG.pb.h \
    SHARED/guicommunicator.h \
    SHARED/magic_enum-master/include/magic_enum.hpp \ \
    SHARED/rpclib-master/include/rpc/client.h \
    SHARED/rpclib-master/include/rpc/client.inl \
    SHARED/rpclib-master/include/rpc/compatibility.h \
    SHARED/rpclib-master/include/rpc/config.h \
    SHARED/rpclib-master/include/rpc/config.h.in \
    SHARED/rpclib-master/include/rpc/detail/all.h \
    SHARED/rpclib-master/include/rpc/detail/any.h \
    SHARED/rpclib-master/include/rpc/detail/async_writer.h \
    SHARED/rpclib-master/include/rpc/detail/bool.h \
    SHARED/rpclib-master/include/rpc/detail/call.h \
    SHARED/rpclib-master/include/rpc/detail/client_error.h \
    SHARED/rpclib-master/include/rpc/detail/constant.h \
    SHARED/rpclib-master/include/rpc/detail/dev_utils.h \
    SHARED/rpclib-master/include/rpc/detail/func_tools.h \
    SHARED/rpclib-master/include/rpc/detail/func_traits.h \
    SHARED/rpclib-master/include/rpc/detail/if.h \
    SHARED/rpclib-master/include/rpc/detail/invoke.h \
    SHARED/rpclib-master/include/rpc/detail/is_specialization_of.h \
    SHARED/rpclib-master/include/rpc/detail/log.h \
    SHARED/rpclib-master/include/rpc/detail/make_unique.h \
    SHARED/rpclib-master/include/rpc/detail/not.h \
    SHARED/rpclib-master/include/rpc/detail/pimpl.h \
    SHARED/rpclib-master/include/rpc/detail/response.h \
    SHARED/rpclib-master/include/rpc/detail/server_session.h \
    SHARED/rpclib-master/include/rpc/detail/thread_group.h \
    SHARED/rpclib-master/include/rpc/detail/util.h \
    SHARED/rpclib-master/include/rpc/dispatcher.h \
    SHARED/rpclib-master/include/rpc/dispatcher.inl \
    SHARED/rpclib-master/include/rpc/msgpack.hpp \
    SHARED/rpclib-master/include/rpc/rpc_error.h \
    SHARED/rpclib-master/include/rpc/server.h \
    SHARED/rpclib-master/include/rpc/this_handler.h \
    SHARED/rpclib-master/include/rpc/this_handler.inl \
    SHARED/rpclib-master/include/rpc/this_server.h \
    SHARED/rpclib-master/include/rpc/this_session.h \
    SHARED/rpclib-master/include/rpc/version.h \
    SHARED/rpclib-master/include/rpc/version.h.in


LIBS += -pthread -lprotobuf -lpthread

DISTFILES += \
    SHARED/PROTOCOL_MSG.proto
