#include "Core/Core.h"

using namespace Upp;
int X = 0;
int Y = 0;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	TcpSocket server;
	if(!server.Listen(64030)) {
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
}
