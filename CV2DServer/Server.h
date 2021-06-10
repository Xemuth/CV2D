#ifndef _CV2DServer_Server_h_
#define _CV2DServer_Server_h_
#include <Core/Core.h>

namespace Upp{
	
class Server{
	public:
		Server(const Upp::String& webServeurIp, int listeningPort);
		~Server();
		
		void SetCallbackClient(const Function<Upp::String (const TcpSocket& socket, const Upp::String&)>& callback);
		void SetCallbackServer(const Function<Upp::String (const TcpSocket& socket, const Upp::String&)>& callback);
		void RemoveCallbackClient();
		void RemoveCallbackServer();
		
		bool HaveCallbackClient()const;
		bool HaveCallbackServer()const;
		
		void Start();
		void Stop();
		bool IsReady();
		
		const TcpSocket& ConnectNewClient(const Upp::String& addr, int port);

	private:
		void Connection(TcpSocket& socket, int position);
		void Listener();
		
		void WaitIsReady();
		
		bool d_ready;
		bool d_stopThread;
		
		Function<Upp::String (const TcpSocket& socket, const Upp::String&)> d_callbackClient;
		Function<Upp::String (const TcpSocket& socket, const Upp::String&)> d_callbackServer;
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
