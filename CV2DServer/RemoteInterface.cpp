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
	Upp::String returnCmd;
	if(TrimBoth(ToLower(str)).IsEqual("stop")){
		Stop();
		returnCmd = " System have been stopped";
	}
	return returnCmd;
}

Upp::String RemoteInterface::CommandClient(const TcpSocket& socket, const Upp::String& str){
	LLOG("[RemoteInterface][CommandClient] Socket 0x" + Format64Hex((uint64)(&socket))  + " Cmd: " + str);
	return "\n";
}

Upp::String RemoteInterface::CommandServer(const TcpSocket& socket, const Upp::String& str){
	LLOG("[RemoteInterface][CommandServer] Socket 0x" + Format64Hex((uint64)(&socket))  + " Cmd: " + str);
	return "\n";
}

}