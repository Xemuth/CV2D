#include <Core/Core.h>
#include "GameEngine.h"
using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT | LOG_FILE | LOG_TIMESTAMP);
	try{
		Instance instance(TiledMapJson("C:\\Upp\\CV2D\\TiledMapReader\\test.json"));
		LOG("ID : " + AsString(instance.GetId()));
		LOG("Map version : " + instance.GetMap().GetVersion());
		instance.AddPlayer("p1");
		instance.UpdatePlayer("p1", true, Player::Direction::UP);
		for(int e = 0; e < 10; e++){
			instance.Update();
			LOG(AsString(instance.GetPlayer("p1").GetX()) + "," + AsString(instance.GetPlayer("p1").GetY()));
		}
	}catch(Exc& exception){
		LOG("EXCEPTIOn : " + exception);
	}
}