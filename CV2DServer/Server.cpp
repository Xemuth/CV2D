#include "Server.h"

#define LLOG(x)   LOG(x)

namespace Upp{
	
Server::Server(const Upp::String& webServeurIp, int listeningPort) : d_webServeurIp(webServeurIp), d_port(listeningPort), d_ready(false){}

Server::~Server(){
	Stop();
}

void Server::SetCallbackClient(const Function<Upp::String (const TcpSocket& socket, const Upp::String&)>& callback){
	d_callbackClient = callback;
}

void Server::SetCallbackServer(const Function<Upp::String (const TcpSocket& socket, const Upp::String&)>& callback){
	d_callbackServer = callback;
}

bool Server::HaveCallbackServer()const{
	return d_callbackClient;
}
bool Server::HaveCallbackClient()const{
	return d_callbackClient;
}

void Server::RemoveCallbackClient(){
	d_callbackClient = Nuller();
}

void Server::RemoveCallbackServer(){
	d_callbackClient = Nuller();
}

void Server::Start(){
	LLOG("[Server][Start] Starting listener");
	d_stopThread = false;
	d_server.Run([&]{ Listener(); return;});
	WaitIsReady();
}

bool Server::IsReady(){
	return d_ready;
}

void Server::WaitIsReady(){
	while(!d_ready){
		Sleep(10); // I don't like that
	}
}

void Server::Stop(){
	bool showLog = false;
	d_stopThread = true;
	d_ready = false;
	if(d_server.IsOpen()){
		LLOG("[Server][Stop] Stoping server thread");
		d_socket.Close();
		d_socket.Clear();
		d_server.Wait();
		showLog = true;
	}
	if(d_sockets.GetCount() > 0){
		LLOG("[Server][Stop] Stoping all clients thread");
		for(TcpSocket& socket : d_sockets){
			socket.Close();
		}
		for(Thread& th : d_clients){
			th.Wait();
		}
		d_clients.Clear();
		showLog = true;
	}
	if(showLog) LLOG("[Server][Stop] have been stopped succesfully");
}

const TcpSocket& Server::ConnectNewClient(const Upp::String& addr, int port){
	int position = d_sockets.GetCount();
	TcpSocket& client = d_sockets.Create();
	LLOG("[Server][ConnectNewClient] Connection requestion on " + addr +":"+ AsString(port));
	if(client.Connect(addr, port)){
		d_clients.Create().Run([&]{Connection(client, position);});
		LLOG("[Server][ConnectNewClient] Connection thread for connection " + AsString(position) +" is started");
		return client;
	}else{
		LLOG("[Server][ConnectNewClient] Connection " + AsString(position) +" failled. no thread started");
		d_sockets.Remove(position);
	}
	throw Exc("Impossible to open socket on : " + addr + ":" + AsString(port));
}

void Server::Connection(TcpSocket& socket, int position){
	LLOG("[Server][Connection " + AsString(position) + "] Connection is started ");
	while(!socket.IsError() && !socket.IsEof()){
		Upp::String data = d_activeConnection.GetLine();
		if(!socket.IsTimeout() && data.GetCount() > 0){
			Upp::String sendingCmd = "";
			LLOG("[Server][Connection " + AsString(position) + "] Receiving from web server: " + data.Left(50));
			sendingCmd = d_callbackClient(socket, data);
			if(sendingCmd.GetCount() > 0){
				LLOG("[Server][Connection " + AsString(position) + "] Sending to web server: " + sendingCmd.Left(50));
				socket.Put(sendingCmd + "\n");
			}
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
	d_ready = true;
	while(!d_stopThread){
		if(!listening && !d_stopThread) {
			LLOG("[Server][Listener] Launching listener");
			if(!d_socket.Listen(d_port, 1)){
				LLOG("Unable to initialize server socket on port " + AsString(d_port));
				return;
			}
			listening = true;
		}
		if(!d_stopThread && d_activeConnection.Accept(d_socket)){
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
				if(!d_activeConnection.IsTimeout() && data.GetCount() > 0){
					Upp::String sendingCmd = "";
					LLOG("[Server][Listener] Receiving  from web server: " + data.Left(50));
					sendingCmd = d_callbackServer(d_socket, data);
					if(sendingCmd.GetCount() > 0){
						LLOG("[Server][Listener] Sending to web server: " + sendingCmd.Left(50));
						d_activeConnection.Put(sendingCmd + "\n");
					}
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
