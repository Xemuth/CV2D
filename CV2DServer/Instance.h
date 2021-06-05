#ifndef _CV2DServer_Instance_h_
#define _CV2DServer_Instance_h_

#include <Core/Core.h>
#include "Player.h"
#include <TiledMapReader/TiledMap.h>

namespace Upp{

class Instance{
	public:
		Instance(TiledMapJson&& map);
		
		bool AddPlayer(const Upp::String& id);
		bool RemovePlayer(const Upp::String& id);
		bool UpdatePlayer(const Upp::String& id, bool keyPressed, byte facing);
		
		double GetId()const;
		const Player& GetPlayer(const Upp::String& id)const noexcept(false);
		const TiledMapJson& GetMap()const;
		const Vector<Player>& GetPlayers()const;
		
		void Update();
	private:
		double d_id;
		TiledMapJson d_map;
		Upp::Vector<Player> d_players;
};
	
}

#endif
