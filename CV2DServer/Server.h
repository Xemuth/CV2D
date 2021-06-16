#ifndef _CV2DServer_Server_h_
#define _CV2DServer_Server_h_
#include <Core/Core.h>

namespace Upp{
	
class Server{
	public:
		Server(const Upp::Vector<Upp::String>& authorizedIp, unsigned int listeningPort);
		~Server();
		
		bool AddAuthorizedIp(const Upp::String& ip);
		bool RemoveAuthorizedIp(const Upp::String& ip);
		void ChangePort(unsigned int port);
		
		void SetCallbackClientClose(const Function<void (const TcpSocket& socket)>& callback);
		void SetCallbackClientOpen(const Function<void (const TcpSocket& socket)>& callback);
		
		void SetCallbackServerClose(const Function<void (const TcpSocket& socket)>& callback);
		void SetCallbackServerOpen(const Function<void (const TcpSocket& socket)>& callback);
		
		void SetCallbackClientMessage(const Function<Upp::String (const TcpSocket& socket, const Upp::String&)>& callback);
		void SetCallbackServerMessage(const Function<Upp::String (const TcpSocket& socket, const Upp::String&)>& callback);
		
		
		void RemoveCallbackClientOpen();
		void RemoveCallbackServerOpen();
		void RemoveCallbackClientClose();
		void RemoveCallbackServerClose();
		void RemoveCallbackClientMessage();
		void RemoveCallbackServerMessage();
		
		bool HaveCallbackClientOpen()const;
		bool HaveCallbackClientClose()const;
		bool HaveCallbackServerOpen()const;
		bool HaveCallbackServerClose()const;
		bool HaveCallbackClientMessage()const;
		bool HaveCallbackServerMessage()const;
		
		unsigned int GetPort()const;
		const Vector<Upp::String>& GetAuthorizedIps()const;
		
		void Start();
		void Stop();
		bool IsReady();
		
		const TcpSocket& ConnectNewClient(const Upp::String& addr, int port);
		void CloseSocket(const TcpSocket* socket);
		const TcpSocket& GetWebServerSocket()const;
	private:
		void Connection(TcpSocket& socket, int position);
		void Listener();
		
		void WaitIsReady();
		
		bool d_ready;
		bool d_stopThread;
		
		Function<Upp::String (const TcpSocket& socket, const Upp::String&)> d_callbackClientMessage;
		Function<void (const TcpSocket& socket)> d_callbackClientClose;
		Function<void (const TcpSocket& socket)> d_callbackClientOpen;
		
		Function<Upp::String (const TcpSocket& socket, const Upp::String&)> d_callbackServerMessage;
		Function<void (const TcpSocket& socket)> d_callbackServerClose;
		Function<void (const TcpSocket& socket)> d_callbackServerOpen;
		
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
