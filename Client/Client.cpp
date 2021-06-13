#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT | LOG_FILE | LOG_TIMESTAMP);
	TcpSocket socket;
	socket.Timeout(5000);
	for(int e = 0; e < 10; e++){
		if(socket.Connect("127.0.0.10", 64030)){
			socket.Linger(5000);
			for(int i = 0; i < 10; i++){
				Json json("command","getmap");
				JsonArray pn;
				pn << Json("name", "test");
				json("args", pn);
				
				socket.PutAll(~json +"\n");
				

				if(socket.Peek() != -1){
					Upp::String str;
					int read =  socket.Get();
						if(read == -1) break;
					str << char(read);
					do{
						read =  socket.Get();
						str << char(read);
					}while(socket.Peek() != -1);
					LOG(str.Left(10));
				}
				Sleep(1000);
			}
			socket.Close();
			Sleep(10000);
		}
	}
}
