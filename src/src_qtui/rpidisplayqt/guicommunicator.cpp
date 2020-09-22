#include "guicommunicator.h"

guicommunicator::guicommunicator()
{
    zmq_push = zsock_new_push ("inproc://example");
    zmq_pull = zsock_new_pull ("inproc://example");

//INIT LOGGIN
//   loguru::add_file("guicommunicator.log", loguru::Append, loguru::Verbosity_MAX);
}

guicommunicator::~guicommunicator(){
        //CLEAR ZeroMQ SOCKETS
        zsock_destroy (&zmq_push);
        zsock_destroy (&zmq_pull);
        //CLEAR PROTOBUF LIB
        google::protobuf::ShutdownProtobufLibrary();
}

void guicommunicator::debug_output(std::string _msg){
   // LOG_SCOPE_F(INFO, _msg.c_str());

    #ifdef USES_QT
    qInfo() << _msg.c_str();
    #endif
}

void guicommunicator::createEvent(GUI_ELEMENT _element, GUI_VALUE_TYPE _type, std::string _value){


    //POPULATE THE PROTOBUF MESSAGE
    protocolmsg::gui2backend_msg message;
    //SET FIEDLS IM MESSAGE
    message.set_event(magic_enum::enum_integer(_element));
    message.set_type(magic_enum::enum_integer(_type));
    message.set_value(_value);

    message.set_ispageswitchevent(0);
    message.set_ack(0);

    std::string tmp;
    message.SerializeToString(&tmp);
    debug_output(tmp);

    //SEND OVER ZEROMQ
    if(!zmq_push){
           debug_output("zmq_push is null");
           return;
    }
    zstr_send (zmq_push, tmp.c_str());
}


//ADDING SOME VARIATIONS OF createEvent
void guicommunicator::createEvent(GUI_ELEMENT _element, GUI_VALUE_TYPE _type){
        guicommunicator::createEvent(_element,_type,(std::string)"");
}
#ifdef USES_QT 
void guicommunicator::createEvent(GUI_ELEMENT _element, GUI_VALUE_TYPE _type, QString _value){
    guicommunicator::createEvent(_element,_type,_value.toStdString());
}
#endif
