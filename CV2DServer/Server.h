#ifndef _CV2DServer_Server_h_
#define _CV2DServer_Server_h_
#include <Core/Core.h>

namespace Upp{
	
class Server{
	public:
		Server(unsigned int listeningPort);
		~Server();
		
		bool AddAuthorizedIp(const Upp::String& ip);
		bool RemoveAuthorizedIp(const Upp::String& ip);
		void ChangePort(unsigned int port);
		
		void SetCallbackClient(const Function<Upp::String (const TcpSocket& socket, const Upp::String&)>& callback);
		void SetCallbackServer(const Function<Upp::String (const TcpSocket& socket, const Upp::String&)>& callback);
		void RemoveCallbackClient();
		void RemoveCallbackServer();
		
		bool HaveCallbackClient()const;
		bool HaveCallbackServer()const;
		
		unsigned int GetPort()const;
		const Vector<Upp::String>& GetAuthorizedIps()const;
		
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
		
		Vector<Upp::String> d_webServeurIps;
		unsigned int d_port;
};

}



#endif
