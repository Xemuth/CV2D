#include <Core/Core.h>
#include "CV2DServer.h"

#define LISTENING_PORT 64030

using namespace Upp;

void CV2DServer::ProcessCommand(const Upp::String& cmd){
	
}

void CV2DServer::SendServerState(unsigned int tickRate){
	while(!Thread::IsShutdownThreads() && clientSocket.IsOpen() ){
		
		clientSocket.put( )
		
		sleep(60000/tickRate)
	}
}

void SendMap(const Upp::String& mapName){
	
}


CV2DServer::CV2DServer(){
	TcpSocket server;
	if(!server.Listen(LISTENING_PORT, 1)) {
		Cout() << "Unable to initialize server socket!\n";
		SetExitCode(1);
		return;
	}
	for(;;) {
		Cout() << "Waiting for webServer..\n";
		TcpSocket clientSocket;
		if(s.Accept(server)) {
			String w = clientSocket.GetLine();
			
			
			
			Cout() << "Request: " << w << " from: " << s.GetPeerAddr() << '\n';
			if(w == "time")
				s.Put(AsString(GetSysTime()));
			else
				s.Put(AsString(3 * atoi(~w)));
			s.Put("\n");
		
		
		
		
		}
	}
}

CONSOLE_APP_MAIN
{
}




TcpSocket server;
	if(!server.Listen(LISTENING_PORT)) {
		LOG("Failed to start listening on 64 030..");
		return;
	}

	for(;;) {
		TcpSocket socket;
		WebSocket ws;
		try{
			if(socket.Accept(server) && ws.WebAccept(socket)) {
				LOG("Accepted connection");
				Upp::String cmd = ws.Receive();
				LOG("Commande : " + cmd);
				Upp::String reponse = "";
				if(cmd.IsEqual("GetState")){
					reponse = AsString(X) + ";" + AsString(Y);
				}else if(cmd.IsEqual("MoveLeft")){
					X--;
					reponse = "true";
				}else if(cmd.IsEqual("MoveRight")){
					X++;
					reponse = "true";
				}else if(cmd.IsEqual("MoveUp")){
					Y++;
					reponse = "true";
				}else if(cmd.IsEqual("MoveDown")){
					reponse = "false";
				}
				LOG("Reponse : " + reponse);
				ws.SendText(reponse);
			}
			if(ws.IsError())
				LOG("ERROR: " << ws.GetErrorDesc());
		}catch(Upp::Exc& exception){
			LOG("Exception : " + exception);
		}
	}