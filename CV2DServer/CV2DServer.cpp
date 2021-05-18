#include <Core/Core.h>
#include <csignal>
#include "CV2DServer.h"


#define LISTENING_PORT 64030
#define TICK_RATE 10
#define AllMapDirectories "C:\\Upp\\allMap\\"

using namespace Upp;


String ReadCmd(int timeout)
{
	String r;
	int time = msecs();
	
	while(msecs(time) < timeout){
		int c = 0;
		int n = read(STDIN_FILENO, (char*) &c, 1);
		if(c == '\n')
			break;
		if(n > 0) {
			r.Cat(c);
			time = msecs();
		}
	}
	return r.GetCount() ? r : String::GetVoid();
}
Upp::String GetErrorJson(const Upp::String& commandName, const Upp::String& errorMessage){
	return "{\"command\":\""+ commandName +"\",\n\"error\":\"" + errorMessage +"\"}";
}

Upp::String GetMap(const String& mapName){
	if(FileExists(AllMapDirectories + mapName)){
		TiledMapJson tiledMap(AllMapDirectories + mapName);
		Upp::String ret;
		ret << "{\n\t\"mapname\":\""+ mapName +"\",\n\t\"data\":\""+ Base64Encode(LoadFile(AllMapDirectories + mapName)) +"\",\n\t\"tilessets\":[\n\t\t";
		for(const TiledTilesSet& set: tiledMap.GetTilesSets()){
			ret << "\t{\n\"name\":\""+ Replace(set.GetSource(),{"tsx"},{"png"}) +"\",\n\t\t\"data\":\""+ Base64Encode(LoadFile(AllMapDirectories + Replace(set.GetSource(),{"tsx"},{"png"}))) +"\"\n\t},\n\t";
		}
		ret << "\n\t\t]\n}";
		return ret;
	}
	return {};
}


void CV2DServer::StartServer(){
	serverThread.Start(THISBACK(ServerRoutine));
	Upp::String command;
	LOG("Command line interface ready");
	while(command != "exit" && !secureStop){
		command	= ToLower(ReadCmd(1000));
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



void CV2DServer::ServerRoutine(){
	if(!server.Listen(port, 1)) {
		LOG("Unable to initialize server socket on port " + AsString(port));
		return;
	}
	client.GlobalTimeout(1000);
	while(!Thread::IsShutdownThreads()){
		LOG("Waiting for webServer...");
		if(client.Accept(server)){
			LOG("WebServer connected");
			int emptyData = 0;
			while(!client.IsError()){
				Upp::String data = client.GetLine();
				if(!client.IsTimeout()){
					Upp::String sendingCmd = "";
					LOG("Receiving : " + data + " from: " + client.GetPeerAddr());
					sendingCmd = ProcessCommandNetwork(data);
					LOG("Sending: " + sendingCmd + " To: " + client.GetPeerAddr());
					sendingCmd += '\n';
					sendingCmd.Shrink();
					client.Put(sendingCmd);
				}else{
					client.ClearError();
				}
			}
			LOG("WebServer error: " + client.GetErrorDesc());
			client.Close();
			client.Clear();
			LOG("WebServer disconnected");
		}
	}
	LOG("Connection with webServer ended");
}

Upp::String CV2DServer::ProcessCommandNetwork(const Upp::String& cmd){
	Value json = ParseJSON(cmd);
	if(!IsNull(json["commande"])){
		if((~json["command"]).IsEqual("getmap")){
			if(!IsNull(json["mapname"])){
				return GetMap(json["mapname"]);
			}
			return GetErrorJson("getmap","mapname field is missing");
		}else if((~json["command"]).IsEqual("keypressed")){
			
		}
	}
	if(cmd == "empty") return "";
	return "error";
}

Upp::String CV2DServer::ProcessCommandLine(const Upp::String& command){
	LOG("Command line : " + command);
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
	//Cout() << GetMap("test.json") <<"\n";
	CV2DServer server(LISTENING_PORT, TICK_RATE);
	serverPtr = &server;
	std::signal(SIGINT,static_cast<__p_sig_fn_t>([](int s)->void{serverPtr->StopServer();}));
	server.StartServer();
}