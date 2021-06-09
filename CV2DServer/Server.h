#ifndef _CV2DServer_Server_h_
#define _CV2DServer_Server_h_
#include <Core/Core.h>

namespace Upp{
	
class Server{
	public:
		Server(const Upp::String& webServeurIp, int listeningPort);
		~Server();
		
		void SetCallbackClient(Function<Upp::String (Upp::String)>& callback);
		void SetCallbackServer(Function<Upp::String (Upp::String)>& callback);
		
		bool HaveCallbackClient()const;
		bool HaveCallbackServer()const;
		
		void Run();
		void Stop();
		
		void ConnectNewClient(const Upp::String& addr, int port);

	private:
		void Connection(TcpSocket& socket, int position);
		void Listener();
		
		Function<Upp::String (Upp::String)> d_callbackClient;
		Function<Upp::String (Upp::String)> d_callbackServer;
		Array<Thread> d_clients;
		Array<TcpSocket> d_sockets;
		Thread d_server;
		TcpSocket d_socket;
		TcpSocket d_activeConnection;
		
		Upp::String d_webServeurIp;
		int d_port;
};

}



#endif
