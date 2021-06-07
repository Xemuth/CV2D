#ifndef _CV2DServer_Instance_h_
#define _CV2DServer_Instance_h_

#include <Core/Core.h>
#include "Player.h"
#include <TiledMapReader/TiledMap.h>

namespace Upp{

class Instance;

struct InstanceState{
	InstanceState(const Instance& instance);
	Vector<PlayerState> d_ps;
	double d_id;
};

class Instance{
	public:
		Instance(TiledMapJson&& map, double id = 0);
		
		bool AddPlayer(const Upp::String& id);
		bool RemovePlayer(const Upp::String& id);
		bool UpdatePlayer(const Upp::String& id, bool keyPressed, byte facing);
		
		double GetId()const;
		const Player& GetPlayer(const Upp::String& id)const noexcept(false);
		const TiledMapJson& GetMap()const;
		const Vector<Player>& GetPlayers()const;
		
		InstanceState GetInstanceState()const;
		
		void Update();
	private:
		bool CheckForColision(const Pointf& p);
		bool IsOutOfBound(const Pointf& p);
		Pointf GetDefaultPos();
		
		friend InstanceState;
		double d_id;
		TiledMapJson d_map;
		Upp::Vector<Player> d_players;
};
	
}

#endif
