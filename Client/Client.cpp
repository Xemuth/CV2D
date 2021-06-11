#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	TcpSocket socket;
	if(socket.Connect("127.0.0.10", 9555)){
		Json json("command","getmap");
		JsonArray pn;
		pn << Json("path", "test");
		json("args", pn);
		
		socket.PutAll(~json +"\n");
	}
	bool receive = false;
	Upp::String str ;
	while(!receive){
		str = socket.GetLine();
		if(str.GetCount() != 0) receive  = true;
	}
	Cout() << str;
	
	
}
