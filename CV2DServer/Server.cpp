#include "Server.h"

#define LLOG(x)   LOG(x)

namespace Upp{
	
Server::Server(const Upp::Vector<Upp::String>& authorizedIp, unsigned int listeningPort) : d_webServeurIps(clone(authorizedIp)), d_port(listeningPort), d_ready(false){}

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
	client.Timeout(500);
	client.Linger(200);
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
	SocketWaitEvent swe;
	swe.Add(socket, 0x7); // 0x7 is WAIT_ALL
	while(!d_stopThread && !socket.IsError() && socket.IsOpen()){
		swe.Wait(500);
		if(swe[0] & WAIT_WRITE){
			if((swe[0] & WAIT_READ)){
				Upp::String data;
				
				int read =  socket.Get();
				if(read == -1) break;
				data << char(read);
				do{ data << char(socket.Get()); }while(socket.Peek() != -1);
									
				if(data.GetCount() > 0){
					Upp::String sendingCmd = "";
					LLOG("[Server][Listener] Receiving  from web server: " + data.Left(20));
					sendingCmd = d_callbackClient(socket, data);
					if(sendingCmd.GetCount() > 0){
						LLOG("[Server][Listener] Sending to web server: " + sendingCmd.Left(20));
						socket.Put(sendingCmd);
					}
				}
			}
		}else{
			break;
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
			
			d_activeConnection.Timeout(500);
			d_activeConnection.Linger(200);
			SocketWaitEvent swe;
			swe.Add(d_activeConnection, 0x7); // 0x7 is WAIT_ALL
			while(!d_stopThread && !d_activeConnection.IsError() && d_activeConnection.IsOpen()){
				swe.Wait(500);
				if(swe[0] & WAIT_WRITE){
					if((swe[0] & WAIT_READ)){
						Upp::String data;
						
						int read =  d_activeConnection.Get();
						if(read == -1) break;
						data << char(read);
						do{ data << char(d_activeConnection.Get()); }while(d_activeConnection.Peek() != -1);
											
						if(data.GetCount() > 0){
							Upp::String sendingCmd = "";
							LLOG("[Server][Listener] Receiving  from web server: " + data.Left(20));
							sendingCmd = d_callbackServer(d_activeConnection, data);
							if(sendingCmd.GetCount() > 0){
								LLOG("[Server][Listener] Sending to web server: " + sendingCmd.Left(20));
								d_activeConnection.Put(sendingCmd);
							}
						}
					}
				}else{
					break;
				}
			}
			if(d_activeConnection.IsError()) LLOG("[Server][Listener] WebServer error: " + d_activeConnection.GetErrorDesc());
			d_activeConnection.Close();
			LLOG("[Server][Listener] WebServer disconnected");
		}
	}
	LLOG("[Server][Listener] Listener thread stopping");
}

}
