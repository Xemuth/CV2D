#ifndef _CV2DServer_RemoteInterface_h_
#define _CV2DServer_RemoteInterface_h_

#include "Server.h"
#include "GameEngine.h"

namespace Upp{
	
	
class RemoteInterface{
	public:
		RemoteInterface(const Upp::Vector<Upp::String>& authorizedIp, unsigned int listeningPort, int instanceTimeout, int mapLoadedTimeout, int playerTimeout, int tickRate);
		~RemoteInterface();
		
		void Start();
		void Stop();
		
		Upp::String HandleCommandLine(Upp::String& str);
		
		bool IsStarted()const{return d_started;}
		
		typedef RemoteInterface CLASSNAME;
		
	private:
		Upp::String GetMap(const Upp::String& cmd,const ValueMap& args);
			
		enum Target: byte{COMMAND_LINE = 0x1, WEB_SERVER = 0x2, CLIENT = 0x4};
		Upp::String CommandClient(const TcpSocket& socket, const Upp::String& str);
		Upp::String CommandServer(const TcpSocket& socket, const Upp::String& str);
		
		Upp::String ShowHelp(const Upp::String& cmd, const VectorMap<Upp::String, Upp::String>& args, const Vector<Upp::String>& examples = {});
		Upp::String DispatchCommand(Target target , const TcpSocket& socket, const Upp::String& cmd,const ValueMap& args);
		Upp::String BuildResponse(const Upp::String& cmd, const ValueMap& args)const;
		
		
		double AddPlayer(const Upp::String& playerId, const Upp::String& mapName) noexcept(false); //when a player want to be added to a map that isnt yet instantiate or loaded, then it try to load it and instantiate it before adding the player.
		bool UpdatePlayer(const Upp::String& playerId, bool keyPressed, byte facing); //if a player remain away from keyboard (no key pressed during more than #playerTimeout) then it remove player from the instance
		bool RemovePlayer(const Upp::String& playerId); //if player disconnect then it is possible to call this routin
		InstanceState GetInstanceState(double instanceId);
		
		void WaitIsReady();

		Thread d_thread;
		Upp::VectorMap<TcpSocket*, double> d_link;
		bool d_started;
		GameEngine d_gameEngine;
		Server d_server;
};
	
}


#endif
