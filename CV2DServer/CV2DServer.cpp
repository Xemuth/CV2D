#include <Core/Core.h>
#include <csignal>
#include "CV2DServer.h"


#define LISTENING_PORT 64030
#define TICK_RATE 10

using namespace Upp;

void CV2DServer::StartServer(){
	serverThread.Start(THISBACK(ServerRoutine));
	Upp::String command;
	LOG("Command line interface ready");
	while(command != "exit" && !secureStop){
		command	= ToLower(ReadStdIn());
		Cout() << ProcessCommandLine(command);
	}
	server.Close();
	client.Close();
	serverThread.ShutdownThreads();
	serverThread.Wait();
}

void CV2DServer::StopServer(){
	secureStop = true;
	LOG("Shutdown asked..");
}

CV2DServer::~CV2DServer(){
	server.Close();
	client.Close();
	serverThread.ShutdownThreads();
	serverThread.Wait();
}

Upp::String CV2DServer::ProcessCommand(const Upp::String& cmd){
	Value json = ParseJSON(cmd);
	return "";
}

void CV2DServer::ServerRoutine(){
	if(!server.Listen(port, 1)) {
		LOG("Unable to initialize server socket on port " + AsString(port));
		return;
	}
	LOG("Waiting for webServer...");
	if(client.Accept(server)){
		while(client.IsOpen()){
			Upp::String incomingCmd = client.GetLine();
			LOG("Receiving : " + incomingCmd + " from: " + client.GetPeerAddr());
			Upp::String sendingCmd = ProcessCommand(incomingCmd);
			LOG("Sending: " + sendingCmd + " To: " + client.GetPeerAddr());
			client.Put(sendingCmd);
			client.Put("\n");
		}
	}
	LOG("Connection with webServer ended");
}

Upp::String CV2DServer::ProcessCommandLine(const Upp::String& command){
	LOG("Command : " + command);
	return "";
}

void SendMap(const Upp::String& mapName){
	
}


void Instance::SendInstanceState(){
	while(!Thread::IsShutdownThreads() && clientSocket.IsOpen() ){
		
		//clientSocket.put( )
		
		//sleep(60000/tickRate);
	}
}

CV2DServer* serverPtr;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT | LOG_FILE | LOG_TIMESTAMP);
	CV2DServer server(LISTENING_PORT, TICK_RATE);
	serverPtr = &server;
	std::signal(SIGINT,static_cast<__p_sig_fn_t>([](int s)->void{serverPtr->StopServer();}));
	server.StartServer();
}