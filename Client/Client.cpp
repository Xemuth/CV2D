#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	TcpSocket socket;
	for(int e = 0; e < 10; e++){
		if(socket.Connect("127.0.0.10", 64030)){
			for(int i = 0; i < 10; i++){
				Json json("command","getmap");
				JsonArray pn;
				pn << Json("name", "test");
				json("args", pn);
				
				socket.PutAll(~json +"\n");
				Upp::String str = socket.GetLine();
				Cout() << str.Left(10) << "\n";
				Sleep(1000);
			}
			Sleep(10000);
			socket.Close();
		}
	}
	
	
	
}
