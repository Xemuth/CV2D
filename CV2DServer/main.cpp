#include <Core/Core.h>
#include "RemoteInterface.h"
using namespace Upp;

String ReadCmd(int timeout)
{
	String r;
	int time = msecs();
	
	while(msecs(time) < timeout){
		int c = 0;
		int n = read(STDIN_FILENO, (char*) &c, 1);
		if(c == '\n')
			break;
		if(n > 0) {
			r.Cat(c);
			time = msecs();
		}
	}
	return r.GetCount() ? r : String::GetVoid();
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT | LOG_FILE | LOG_TIMESTAMP);
	try{
		RemoteInterface remote("127.0.0.1", 9555, 300, 300, 300, 60);
		remote.Start();
		Upp::String command;
		LOG("Command line interface ready");
		while(remote.IsStarted()){
			Cout() << "CV2D << ";
			command	= ToLower(ReadCmd(1000));
			command = remote.HandleCommandLine(command);
			Cout() << "CV2D >> " << command << "\n";
		}
	}catch(Exc& exception){
		LOG("Exception : " + exception);
	}
}