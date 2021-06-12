#include "Server.h"

#define LLOG(x)   LOG(x)

namespace Upp{
	
Server::Server(unsigned int listeningPort) : d_port(listeningPort), d_ready(false){}

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

unsigned int Server::GetPort()const{
	return d_port;
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

bool Server::AddAuthorizedIp(const Upp::String& ip){
	for(const Upp::String& str : d_webServeurIps){
		if(str.IsEqual(ip)) return false;
	}
	d_webServeurIps.Add(ip);
	return true;
}

bool Server::RemoveAuthorizedIp(const Upp::String& ip){
	for(int i = 0; i < d_webServeurIps.GetCount(); i++){
		if(d_webServeurIps[i].IsEqual(ip)){
			d_webServeurIps.Remove(i);
			return true;
		}
	}
	return true;
}

void Server::ChangePort(unsigned int port){
	d_port = port;
}

const Vector<Upp::String>& Server::GetAuthorizedIps()const{
	return d_webServeurIps;
}

const TcpSocket& Server::ConnectNewClient(const Upp::String& addr, int port){
	int position = d_sockets.GetCount();
	TcpSocket& client = d_sockets.Create();
	client.GlobalTimeout(1000);
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
	
	while(!d_stopThread && !socket.IsError() && socket.IsOpen()){
		if(socket.Peek() != -1){
			Upp::String data;
			while(socket.Peek()!= -1){
				data << char(socket.Get());
			}
			if(socket.IsTimeout())
				socket.ClearError();
			
			if(data != String::GetVoid()){
				Upp::String sendingCmd = "";
				LLOG("[Server][Listener] Receiving  from web server: " + data.Left(20));
				sendingCmd = d_callbackServer(d_socket, data);
				if(sendingCmd.GetCount() > 0){
					LLOG("[Server][Listener] Sending to web server: " + sendingCmd.Left(20));
					socket.Put(sendingCmd + "\n\0");
				}
			}
		}else{
			Sleep(100);
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
				d_ready = false;
				return;
			}
			listening = true;
		}
		if(!d_stopThread && d_activeConnection.Accept(d_socket)){
			bool found = false;
			for(const Upp::String& str : d_webServeurIps){
				if(d_activeConnection.GetPeerAddr().IsEqual(str)){
					found = true;
					break;
				}
			}
			if(!found){
				LLOG("[Server][Listener] Invalid connection with unknow client have been blocked : " + d_activeConnection.GetPeerAddr());
				d_activeConnection.Close();
				continue;
			}
			LLOG("[Server][Listener] WebServer connected, closing listener");
			d_socket.Close();
			listening = false;
			timeval *tvalp = NULL;
			timeval tval;
			tval.tv_sec = 5;
			tval.tv_usec = 5000;
			tvalp = &tval;
			
			fd_set fdsetr[1];
			FD_ZERO(fdsetr);
			FD_SET(d_activeConnection.GetSOCKET(), fdsetr);
			
		//d_activeConnection.Timeout(5000);
		//	d_activeConnection.GlobalTimeout(5000);
			while(!d_stopThread && !d_activeConnection.IsError() && d_activeConnection.IsOpen()){
				LLOG("[Server][Listener] SOCKET: " + AsString(d_activeConnection.GetSOCKET()));
				FD_ZERO(fdsetr);
				FD_SET(d_activeConnection.GetSOCKET(), fdsetr);
				int value = select(0, fdsetr, nullptr, nullptr, tvalp);
				if(value > 0){
					Upp::String data;
					char d;
					recv(d_activeConnection.GetSOCKET(), &d, 1, 0);
					data << d;
					
					while(select(0, fdsetr, nullptr, nullptr, tvalp) > 0){
						recv(d_activeConnection.GetSOCKET(), &d, 1, 0);
						data << d;
					}
					
					if(data.GetCount() > 0){
						Upp::String sendingCmd = "";
						LLOG("[Server][Listener] Receiving  from web server: " + data.Left(20));
						sendingCmd = d_callbackServer(d_activeConnection, data);
						if(sendingCmd.GetCount() > 0){
							LLOG("[Server][Listener] Sending to web server: " + sendingCmd.Left(20));
							d_activeConnection.Put(sendingCmd + "\n\0");
							d_activeConnection.WaitWrite();
						}
					}
				}else if(value == 0){
					if(select(0, nullptr, nullptr, fdsetr, tvalp) == SOCKET_ERROR) break;
				}else{
					LOG("ERROR : " + AsString(WSAGetLastError()));
					break;
				}
			}
			if(d_activeConnection.IsError()) LLOG("[Server][Listener] WebServer error: " + d_activeConnection.GetErrorDesc());
			d_activeConnection.Clear();
			LLOG("[Server][Listener] WebServer disconnected");
		}
	}
	LLOG("[Server][Listener] Connection with webServer ended");
}

}
