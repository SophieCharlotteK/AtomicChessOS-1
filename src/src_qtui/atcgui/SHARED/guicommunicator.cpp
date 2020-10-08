#include "guicommunicator.h"

guicommunicator::guicommunicator()
{
	
//    rpc::server srv(RPC_PORT);
//    srv.bind(RPC_FKT_NAME, &guicommunicator::rpc_callback);
//    ptrsrv = &srv;
//    srv.async_run();
	
	debug_output("guicommunicator started");
}

guicommunicator::~guicommunicator() {
	//STOP REC THREAD IF RUNNING
	    stop_recieve_thread();
	
//	if (ptrsrv)
//	{
//		ptrsrv->stop();
//	}
	//CLEAR PROTOBUF LIB
	google::protobuf::ShutdownProtobufLibrary();
}

void guicommunicator::debug_output(std::string _msg) {
#ifdef USES_QT
	qInfo() << _msg.c_str();
#else
	std::cout << _msg.c_str() << std::endl;
#endif
}

void guicommunicator::debug_event(GUI_EVENT _event, bool _rec) {
	if (_rec)
	{
		debug_output("---RECEIEVED EVENT ---");	
	}
	else
	{
		debug_output("---SEND EVENT ---");
	}
    

	debug_output("RAW: " + std::to_string((int)_event.event) + " " + std::to_string((int)_event.type) + " " + _event.value);
	//https://stackoverflow.com/questions/47525238/why-is-there-no-implicit-conversion-from-stdstring-view-to-stdstring
	std::string e = "";
    e += magic_enum::enum_name(_event.event);
	debug_output("EVENT: " + e);
	e = "";
    e += magic_enum::enum_name(_event.type);
	debug_output("TYPE: " + e);
	e = "";
	debug_output("VALUE: " + _event.value);

	if (_event.is_event_valid) {
		debug_output(" EVENT IS VALID");
	}
	else {
		debug_output("!! EVENT IS NOT VALID !!");
	}
}

guicommunicator::GUI_EVENT guicommunicator::parseEvent(std::string _event) {
	guicommunicator::GUI_EVENT event;
	//CHECK STRING
	if(_event.empty()) {
		debug_output("event string is empty");
		event.is_event_valid = false;
		return event;
	}
	//PARSE STRING TO PROTOBUF
	protocolmsg::gui2backend_msg message;
	message.ParseFromString(_event);

	//CHECK IF PARSING FAILED
	if(!message.IsInitialized()) {
		debug_output("message not initilized");
		event.is_event_valid = false;
		return event;
	}else {
		event.is_event_valid = true;
	}



	//PARSE EVENT BACK
    auto e = magic_enum::enum_cast<GUI_ELEMENT>(message.event());
    if (e.has_value()) {
        event.event = e.value();
    }

	//PARSE TYPE BACK
    auto t = magic_enum::enum_cast<GUI_VALUE_TYPE>(message.type());
    if (t.has_value()) {
        event.type = t.value();
    }
	//PARSE OTHER BACK
    event.value = message.value();

    event.ack = message.ack();

	event.ispageswitchevent = message.ispageswitchevent();

	return event;
}

void guicommunicator::createEvent(GUI_ELEMENT _event, GUI_VALUE_TYPE _type, std::string _value) {


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
 
	debug_event(parseEvent(tmp), false);

    //MAKE REQUEST
    httplib::Client cli(EVENT_URL_COMPLETE);
    cli.Post(EVENT_URL_SETEVENT,tmp,"text/plain");

}

bool guicommunicator::check_guicommunicator_version(){
            httplib::Client cli(EVENT_URL_COMPLETE);
            httplib::Result res =  cli.Get(EVENT_URL_VERSION);
            if(GUICOMMUNICATOR_VERSION == res->body){
                debug_output("CHECK VERSION OK");
                debug_output("GOT:" + res->body + " REQUIRED:" + GUICOMMUNICATOR_VERSION);
                return true;
            }else{
                debug_output("GOT:" + res->body + " REQUIRED:" + GUICOMMUNICATOR_VERSION);
                return false;
            }
}


void guicommunicator::start_recieve_thread() {
  
	thread_running = true;
#ifdef USES_QT
    update_thread = QThread::create([this]{ this->recieve_thread_function(this); });
    update_thread->start();

    if(update_thread->isRunning()){
        debug_output("GUI_COM THREAD isRunning confirmed");
    }
#else
	std::thread t1(recieve_thread_function, this);
	t1.detach();

	update_thread = &t1;     
#endif
	debug_output("GUI_COM THREAD STARTED");
}

void guicommunicator::stop_recieve_thread() {
	thread_running = false;
#ifdef USES_QT
	update_thread->terminate();
#else
	if (update_thread)
	{
		update_thread->join();
	}
#endif
	debug_output("GUI_COM THREAD STOPPED");
}

guicommunicator::GUI_EVENT guicommunicator::get_gui_update_event() {
	GUI_EVENT tmp;
	tmp.is_event_valid = false;
	
	update_thread_mutex.lock();
	//IF A NEW GUI EVENT EXITS GET THEM WITH A THREAD LOCK
	if(!gui_update_event_queue.empty()) {
		tmp = gui_update_event_queue.front();
		gui_update_event_queue.pop();
	}
	
	update_thread_mutex.unlock();
	
	if (tmp.is_event_valid) {
		debug_output("GOT AN EVENT FROM RECIEVE THREAD");
		debug_event(tmp, true);
	}
	
	return tmp;
}


std::string guicommunicator::rpc_callback(std::string _msg)
{
	
}

std::string guicommunicator::event_to_json(GUI_EVENT _ev){

    return "{}";
};

void guicommunicator::enqueue_event(GUI_EVENT _ev){
    update_thread_mutex.lock();
    gui_update_event_queue.push(_ev);
    update_thread_mutex.unlock();
}

void guicommunicator::recieve_thread_function(guicommunicator* _this) {
    using namespace httplib;
    qInfo()<< "thread started";

    //REGISTER WEBSERVER EVENTS
    _this->svr.Get("/", [](const Request& req, Response& res) {
       res.set_content("<html><body>ATC_GUI WEBSERVER. <br>USE /status for last event.<br>USE /status as POST with the EVENT String to update the ui. </body><html>", "text/html");
     });

    _this->svr.Get(EVENT_URL_SETEVENT, [_this](const Request& req, Response& res) {
       res.set_content(_this->event_to_json(_this->last_event_from_webserver),"application/json");
     });

    _this->svr.Get(EVENT_URL_VERSION, [_this](const Request& req, Response& res) {
       res.set_content(GUICOMMUNICATOR_VERSION,"text/plain");
     });



    _this->svr.Post(EVENT_URL_SETEVENT, [_this](const Request& req, Response& res) {

        if(req.body.empty()){
            res.set_content("{\"status\"\"body_is_empty\", \"err\":true}", "application/json");
            return;
        }

         GUI_EVENT ev = _this->parseEvent(req.body);
         _this->last_event_from_webserver = ev;
//        _this->debug_event(ev,false);
         if(!ev.is_event_valid){
             res.set_content("{\"status\"\"event_invalid\",\"err\":true}", "application/json");
             return;
         }

       _this->enqueue_event(ev);
       res.set_content("{\"status\"\"success\",\"err\":false}", "application/json");
       return;
     });

    //START WEBSERVER
    _this->svr.listen(WEBSERVER_BIND_ADDR, WEBSERVER_STAUTS_PORT);

	while (_this->thread_running) {
	
	}
    _this->svr.stop();
	}


	







	//ADDING SOME VARIATIONS OF createEvent
	void guicommunicator::createEvent(GUI_ELEMENT _event, GUI_VALUE_TYPE _type) {
		guicommunicator::createEvent(_event, _type, (std::string)"");
	}
#ifdef USES_QT 
	void guicommunicator::createEvent(GUI_ELEMENT _event, GUI_VALUE_TYPE _type, QString _value) {
		guicommunicator::createEvent(_event, _type, _value.toStdString());
	}
#endif
