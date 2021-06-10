#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	TcpSocket socket;
	if(socket.Connect("127.0.0.10", 9555)){
		socket.PutAll("Hello\n");
	}
	
	
}
