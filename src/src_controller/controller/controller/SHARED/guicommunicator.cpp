#include "guicommunicator.h"

guicommunicator::guicommunicator()
{
    zmq_push = zsock_new_push ("inproc://example");


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

void guicommunicator::debug_event(GUI_EVENT _event){
    debug_output("--- EVENT ---");

    debug_output("RAW: " + std::to_string((int)_event.event)+ " " +std::to_string((int)_event.type) + " " + _event.value);
    //https://stackoverflow.com/questions/47525238/why-is-there-no-implicit-conversion-from-stdstring-view-to-stdstring
    std::string e = "";
    e += magic_enum::enum_name(_event.event);
    debug_output("EVENT: " + e);
    e = "";
    e += magic_enum::enum_name(_event.type);
    debug_output("TYPE: " + e);
    e = "";
    debug_output("VALUE: " + _event.value);

    if(_event.is_event_valid){
        debug_output(" EVENT IS VALID");
    }else{
        debug_output("!! EVENT IS NOT VALID !!");
    }
}

guicommunicator::GUI_EVENT guicommunicator::parseEvent(std::string _event){
        guicommunicator::GUI_EVENT event;
        //CHECK STRING
        if(_event.empty()){
            debug_output("event string is empty");
            event.is_event_valid =false;
            return event;
        }
        //PARSE STRING TO PROTOBUF
        protocolmsg::gui2backend_msg message;
        message.ParseFromString(_event);

        //CHECK IF PARSING FAILED
        if(!message.IsInitialized()){

            debug_output("message not initilized");
            event.is_event_valid =false;
            return event;
        }else{
            event.is_event_valid = true;
        }



        //PARSE EVENT BACK
        auto e = magic_enum::enum_cast<GUI_ELEMENT>(message.event());
        if(e.has_value()){
            event.event = e.value();
        }

        //PARSE TYPE BACK
        auto t = magic_enum::enum_cast<GUI_VALUE_TYPE>(message.type());
        if(t.has_value()){
            event.type = t.value();
        }
        //PARSE OTHER BACK
        event.value = message.value();

        event.ack = message.ack();

        event.ispageswitchevent = message.ispageswitchevent();

    return event;
}

void guicommunicator::createEvent(GUI_ELEMENT _event, GUI_VALUE_TYPE _type, std::string _value){


    //POPULATE THE PROTOBUF MESSAGE
    protocolmsg::gui2backend_msg message;
    //SET FIEDLS IM MESSAGE
    message.set_event(magic_enum::enum_integer(_event));
    message.set_type(magic_enum::enum_integer(_type));
    message.set_value(_value);

 //   message.set_ispageswitchevent(0);
 //   message.set_ack(0);

    std::string tmp;
    message.SerializeToString(&tmp);
   // debug_output(tmp);

 //   debug_event(parseEvent(tmp));
    //SEND OVER ZEROMQ
    if(!zmq_push){
           debug_output("zmq_push is null");
           return;
    }
    zstr_send (zmq_push, tmp.c_str());
}



void guicommunicator::start_recieve_thread(){
    //  std::thread t1(recieve_thread_function, this);
      // receieve_thread = &t1;
     //  debug_output("started thread");

    #ifdef USES_QT
    update_thread = QThread::create([this]{ this->recieve_thread_function(this); });
    update_thread->start();
    #else
    //  std::thread t1(recieve_thread_function, this);
      // receieve_thread = &t1;
     //  debug_output("started thread");
    #endif



}

void guicommunicator::stop_recieve_thread(){
     #ifdef USES_QT
    update_thread->terminate();
    #endif
    //if(receieve_thread == nullptr){
    //    debug_output("receieve_thread is null");
    //    return;
    //}
    //receieve_thread->join();

}

guicommunicator::GUI_EVENT guicommunicator::get_gui_update_event(){
    GUI_EVENT tmp;
    tmp.is_event_valid = false;
    update_thread_mutex.lock();
    //IF A NEW GUI EVENT EXITS GET THEM WITH A THREAD LOCK
    if(!gui_update_event_queue.empty()){
        tmp = gui_update_event_queue.front();
        gui_update_event_queue.pop();
    }
    update_thread_mutex.unlock();
    if(tmp.is_event_valid){

        debug_output("GOT AN EVENT FROM RECIEVE THREAD");
        debug_event(tmp);
}
    return tmp;
}

void guicommunicator::recieve_thread_function(guicommunicator* _this){
     _this->zmq_pull = zsock_new_pull ("inproc://example");
    bool thread_running = true;
    std::string tmp = "";
    while(thread_running){

    tmp =_this->czmq_getmessage();
    //GOT AN MESSAGE
    if(!tmp.empty()){
        //LOCK RESSOURCES = QUEUE
#ifdef USES_QT
        if(_this->update_thread_mutex.tryLock()){
#else
		if(_this->update_thread_mutex.try_lock()){
#endif

            //PARSE EVENT
            guicommunicator::GUI_EVENT ev = _this->parseEvent(tmp);
            if(ev.is_event_valid){
              _this->gui_update_event_queue.push( ev);
            }

            //UNLOCK MUTEX
            _this->update_thread_mutex.unlock();
        }
    }
    //_this->debug_output(".");
    }
}


std::string guicommunicator::czmq_getmessage(){
        //READ FROM ZeroMQ
        std::string tmp = "";
        char *string = zstr_recv (zmq_pull);
        if(!string){return "";}
        tmp = std::string(string);
        debug_output(tmp);
        zstr_free (&string);
            return tmp;
}









//ADDING SOME VARIATIONS OF createEvent
void guicommunicator::createEvent(GUI_ELEMENT _event, GUI_VALUE_TYPE _type){
        guicommunicator::createEvent(_event,_type,(std::string)"");
}
#ifdef USES_QT 
void guicommunicator::createEvent(GUI_ELEMENT _event, GUI_VALUE_TYPE _type, QString _value){
    guicommunicator::createEvent(_event,_type,_value.toStdString());
}
#endif
