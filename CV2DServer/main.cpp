#include <Core/Core.h>
#include "Player.h"

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT | LOG_FILE | LOG_TIMESTAMP);
	
	Player p1("p1", 0.0f, 0.0f, 1.0f);
	p1.SetFacing(Player::Direction::DOWN);
	p1.SetMoving(true);
	for(int e = 0; e < 10; e++){
		p1.Update();
		LOG(AsString(p1.GetX()) + "," + AsString(p1.GetY()));
	}
	
}