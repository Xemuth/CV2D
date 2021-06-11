#include "RemoteInterface.h"

#define LLOG(x)   LOG(x)

namespace Upp{

RemoteInterface::RemoteInterface(const Upp::String& webServeurIp, int listeningPort, int instanceTimeout, int mapLoadedTimeout, int playerTimeout, int tickRate) :
	d_gameEngine(instanceTimeout, mapLoadedTimeout, playerTimeout, tickRate), d_server(webServeurIp, listeningPort) {
		//d_server.SetCallbackClient(THISBACK(CommandClient));
		d_server.SetCallbackClient([&](const TcpSocket& socket, const Upp::String& str) -> Upp::String{ return CommandServer(socket, str); });
		//d_server.SetCallbackServer(THISBACK(CommandServer));
		d_server.SetCallbackServer([&](const TcpSocket& socket, const Upp::String& str) -> Upp::String{ return CommandServer(socket, str); });
		d_started = false;
	}
	
RemoteInterface::~RemoteInterface(){
	d_started = false;
	Stop();
}

void RemoteInterface::Start(){
	d_thread.Run([&]{
		d_started = true;
		d_server.Start();
		d_gameEngine.Start();
		while(!d_thread.IsShutdownThreads()){
			Sleep(100);
		}
		d_server.Stop();
		d_gameEngine.Stop();
		d_started = false;
	});
	WaitIsReady();
}

void RemoteInterface::WaitIsReady(){
	while(!d_gameEngine.IsReady()){
		Sleep(10); // I don't like that
	}
	while(!d_server.IsReady()){
		Sleep(10); // I don't like that
	}
}

void RemoteInterface::Stop(){
	d_thread.ShutdownThreads();
	d_started = false;
	d_thread.Wait();
}

Upp::String RemoteInterface::HandleCommandLine(const Upp::String& str){
	LLOG("[RemoteInterface][HandleCommandLine] Cmd: " + str);
	return DispatchCommand(Target::COMMAND_LINE, TcpSocket(), TrimBoth(ToLower(str)),ValueMap{Value{"TODO"}});
}

Upp::String RemoteInterface::CommandClient(const TcpSocket& socket, const Upp::String& str){
	LLOG("[RemoteInterface][CommandClient] Socket 0x" + Format64Hex((uint64)(&socket))  + " Cmd: " + str);
	Value json = ParseJSON(str);
	if(!IsNull(json["command"])) return "Invalide command\n";
	return DispatchCommand(Target::CLIENT, socket, TrimBoth(ToLower(json["command"].ToString())), json["args"]);
}

Upp::String RemoteInterface::CommandServer(const TcpSocket& socket, const Upp::String& str){
	LLOG("[RemoteInterface][CommandServer] Socket 0x" + Format64Hex((uint64)(&socket))  + " Cmd: " + str);
	Value json = ParseJSON(str);
	if(IsNull(json["command"])) return "Invalide command\n";
	return DispatchCommand(Target::WEB_SERVER, socket, TrimBoth(ToLower(json["command"].ToString())), json["args"]);
}

Upp::String RemoteInterface::BuildResponse(const Upp::String& cmd, const ValueMap& args)const{
	Json json;
	json("command", cmd);
	JsonArray array;
	array << AsJSON(Value(args));
	json("args", array);
	return json.ToString();
}

Upp::String RemoteInterface::DispatchCommand(Target target , const TcpSocket& socket, const Upp::String& cmd,const ValueMap& args){
	if((target & Target::WEB_SERVER) && cmd.IsEqual("getmap")){
		try{
			return GetMap(cmd, args);
		}catch(Exc& exception){
			return BuildResponse("getmap", ValueMap{{"error", exception}});
		}
	}else if((target & Target::COMMAND_LINE) && cmd.IsEqual("stop")){
		Stop();
		return "System have been stopped";
	}
	else{
		return "Invalid command";
	}
}

Upp::String RemoteInterface::GetMap(const Upp::String& cmd,const ValueMap& args){
	bool found = false;
	int iterator;
	for(iterator = 0; iterator < args.GetCount(); iterator++){
		if(!IsNull(args[iterator]["path"])){
			found = true;
			break;
		}
	}
	if(!found) throw Exc("no path have been provided");
	try{
		const TiledMapJson& tiledMap = d_gameEngine.LoadMapData(args[iterator]["path"].ToString());
		return BuildResponse("getmap", ValueMap{
			{"data", Base64Encode(tiledMap.GetData())},
		//	{"image", Base64Encode(LoadFile( GetFileFolder(tiledMap.GetPath()) + "\\" + Replace(tiledMap.GetTilesSets()[0].GetSource(),{"tsx"},{"png"})))}
		});
	}catch(Exc& exception){
		return exception;
	}
}

}