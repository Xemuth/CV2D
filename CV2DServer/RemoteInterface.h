#ifndef _CV2DServer_RemoteInterface_h_
#define _CV2DServer_RemoteInterface_h_

#include "Server.h"
#include "GameEngine.h"

namespace Upp{
	
	
class RemoteInterface{
	public:
		RemoteInterface(const Upp::String& webServeurIp, int listeningPort, int instanceTimeout, int mapLoadedTimeout, int playerTimeout, int tickRate);
		~RemoteInterface();
		
		void Start();
		void Stop();
		
		Upp::String HandleCommandLine(const Upp::String& str);
		
		bool IsStarted()const{return d_started;}
		
		typedef RemoteInterface CLASSNAME;
		
	private:
		Upp::String GetMap(const Upp::String& cmd,const ValueMap& args);
		
		
		enum Target: byte{COMMAND_LINE = 0x1, WEB_SERVER = 0x2, CLIENT = 0x4};
		Upp::String CommandClient(const TcpSocket& socket, const Upp::String& str);
		Upp::String CommandServer(const TcpSocket& socket, const Upp::String& str);
		
		Upp::String DispatchCommand(Target target , const TcpSocket& socket, const Upp::String& cmd,const ValueMap& args);
		Upp::String BuildResponse(const Upp::String& cmd, const ValueMap& args)const;
		
		void WaitIsReady();

		Thread d_thread;
		Upp::VectorMap<TcpSocket*, double> d_link;
		bool d_started;
		GameEngine d_gameEngine;
		Server d_server;
};
	
}


#endif
