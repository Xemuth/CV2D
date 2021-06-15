#include <Core/Core.h>

using namespace Upp;

void ThreadAccept(TcpSocket& server);

double instanceID;

CONSOLE_APP_MAIN
{
	instanceID = -1;
	StdLogSetup(LOG_COUT | LOG_FILE | LOG_TIMESTAMP);
	
	TcpSocket server;
	server.Listen(64030,2, false, true, nullptr);
	Thread th;
	th.Run([&]{ThreadAccept(server);});
	
	
	TcpSocket socket;
	socket.Timeout(5000);
	if(socket.Connect("127.0.0.10", 64030)){
		
		Json json("command","addplayer");
		JsonArray pn;
		pn << Json("player_id", "test");
		pn << Json("name", "test");
		json("args", pn);
		
		socket.PutAll(~json);
	
		Sleep(300);
		if(socket.Peek() != -1){
			Upp::String str;
			int read =  socket.Get();
				if(read == -1) return;
			str << char(read);
			do{
				read =  socket.Get();
				str << char(read);
			}while(socket.Peek() != -1);
			Value json = ParseJSON(str);
			instanceID = stou64(~(json["args"][0]["instance_id"]));
			LOG(instanceID);
		}
		th.ShutdownThreads();
		th.Wait();
	}
}


void ThreadAccept(TcpSocket& server){
	while(! Thread::IsShutdownThreads()){
		TcpSocket socket;
		if(socket.Accept(server)){
			Cout() << "CONNECTED" << EOL;
			while(instanceID == -1){}
			Json json("command","authenticate");
			JsonArray pn;
			pn << Json("instance_id", instanceID);
			json("args", pn);
	
			socket.Put(~json);
			Sleep(300);
			if(socket.Peek() != -1){
				Upp::String str;
				int read =  socket.Get();
					if(read == -1) break;
				str << char(read);
				do{
					read =  socket.Get();
					str << char(read);
				}while(socket.Peek() != -1);
				LOG(str);
			}
			return;
		}
	}
}