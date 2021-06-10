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
		Upp::String CommandClient(const TcpSocket& socket, const Upp::String& str);
		Upp::String CommandServer(const TcpSocket& socket, const Upp::String& str);
		
		void WaitIsReady();

		Thread d_thread;
		Upp::VectorMap<TcpSocket*, double> d_link;
		bool d_started;
		GameEngine d_gameEngine;
		Server d_server;
};
	
}


#endif
