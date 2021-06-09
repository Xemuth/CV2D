#include "Server.h"

#define LLOG(x)   LOG(x)

namespace Upp{
	
Server::Server(const Upp::String& webServeurIp, int listeningPort) : d_webServeurIp(webServeurIp), d_port(listeningPort){}

Server::~Server(){
	Stop();
}

void Server::SetCallbackClient(Function<Upp::String (Upp::String)>& callback){
	d_callbackClient = callback;
}

void Server::SetCallbackServer(Function<Upp::String (Upp::String)>& callback){
	d_callbackServer = callback;
}

bool Server::HaveCallbackServer()const{
	return d_callbackClient;
}
bool Server::HaveCallbackClient()const{
	return d_callbackClient;
}

void Server::Run(){
	d_server.Run([&]{Listener();});
}

void Server::Stop(){
	if(d_server.IsOpen()){
		d_server.ShutdownThreads();
		d_socket.Close();
		d_socket.Clear();
	}
	for(Thread& th : d_clients){
		th.ShutdownThreads();
	}
	for(Thread& th : d_clients){
		th.Wait();
	}
	d_server.Wait();
	d_clients.Clear();
}

void Server::ConnectNewClient(const Upp::String& addr, int port){
	int position = d_sockets.GetCount();
	TcpSocket& client = d_sockets.Create();
	LLOG("[Server][ConnectNewClient] Connection requestion on " + addr +":"+ AsString(port));
	if(client.Connect(addr, port)){
		d_clients.Create().Run([&]{Connection(client, position);});
		LLOG("[Server][ConnectNewClient] Connection thread for connection " + AsString(position) +" is started");
	}else{
		LLOG("[Server][ConnectNewClient] Connection " + AsString(position) +" failled. no thread started");
		d_sockets.Remove(position);
	}
}

void Server::Connection(TcpSocket& socket, int position){
	LLOG("[Server][Connection " + AsString(position) + "] Connection is started ");
	while(!socket.IsError() && !socket.IsEof()){
		Upp::String data = d_activeConnection.GetLine();
		if(!socket.IsTimeout()){
			Upp::String sendingCmd = "";
			LLOG("[Server][Connection " + AsString(position) + "] Receiving from web server: " + data);
			sendingCmd = d_callbackClient(data);
			LLOG("[Server][Connection " + AsString(position) + "] Sending to web server: " + sendingCmd);
			socket.Put(sendingCmd);
		}else{
			socket.ClearError();
		}
	}
	socket.Close();
	d_sockets.Remove(position);
	LLOG("[Server][Connection " + AsString(position) + "] Connection closed");
}

void Server::Listener(){
	bool listening = false;
	LLOG("[Server][Listener] Waiting for webServer...");
	while(!Thread::IsShutdownThreads()){
		if(!listening) {
			if(!d_socket.Listen(d_port, 1)){
				LLOG("Unable to initialize server socket on port " + AsString(d_port));
				return;
			}
			listening = true;
		}
		if(d_activeConnection.Accept(d_socket)){
			if(!d_activeConnection.GetPeerAddr().IsEqual(d_webServeurIp)){
				LLOG("[Server][Listener] Invalid connection with unknow client have been blocked : " + d_activeConnection.GetPeerAddr());
				d_activeConnection.Close();
				continue;
			}
			LLOG("[Server][Listener] WebServer connected, closing listener");
			d_socket.Close();
			listening = false;
			while(!d_activeConnection.IsError() && !d_activeConnection.IsEof()){
				Upp::String data = d_activeConnection.GetLine();
				if(!d_activeConnection.IsTimeout()){
					Upp::String sendingCmd = "";
					LLOG("[Server][Listener] Receiving  from web server: " + data);
					sendingCmd = d_callbackServer(data);
					LLOG("[Server][Listener] Sending to web server: " + sendingCmd);
					d_activeConnection.Put(sendingCmd);
				}else{
					d_activeConnection.ClearError();
				}
			}
			LLOG("[Server][Listener] WebServer error: " + d_socket.GetErrorDesc());
			d_activeConnection.Close();
			d_activeConnection.Clear();
			LLOG("[Server][Listener] WebServer disconnected");
		}
	}
	LLOG("[Server][Listener] Connection with webServer ended");
}

}
