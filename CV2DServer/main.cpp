#include <Core/Core.h>
#include "GameEngine.h"

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT | LOG_FILE | LOG_TIMESTAMP);
	GameEngine ge(300,300,300, 60);
	try{
		double instanceId = ge.AddPlayer("p1", "C:\\Upp\\CV2D\\TiledMapReader\\test.json");
		ge.UpdatePlayer("p1", true, Player::Direction::DOWN);

		for(int e = 0; e < 20; e++){
			//ge.Update(); //ONLY FOR TEST PURPOSE, SHOULD BE PRIVATE AND TRIGGERED BY AN INTERNAL THREAD
			InstanceState is = ge.GetInstanceState(instanceId);
			LOG("instance Id : " + AsString(is.d_id));
			for(PlayerState& p : is.d_ps){
				
				LOG("Player ID: " + p.d_id);
				LOG("X: " + AsString(p.d_pos.x) +", Y: " + AsString(p.d_pos.y));
				LOG("facing: " +  Format64Hex(p.d_facing));
			}
			LOG("-----------------------------------------------");
		}
		ge.RemovePlayer("p1");
		Sleep(400000);
	}catch(Exc& exception){
		LOG("Exception : " + exception);
	}
}