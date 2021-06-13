#include <Core/Core.h>
#include <csignal>
#include "RemoteInterface.h"
using namespace Upp;

RemoteInterface* ptr;

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
		RemoteInterface remote({{"127.0.0.1"},{"37.170.105.247"}},64030, 300, 300, 300, 60);
		ptr = &remote;
		std::signal(SIGINT,static_cast<__p_sig_fn_t>([](int s)->void{ptr->Stop();}));
		remote.Start();
		Upp::String command;
		LOG("CV2D >> Command line interface ready");
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