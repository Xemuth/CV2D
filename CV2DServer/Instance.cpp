#include "Instance.h"

namespace Upp{
	
Instance::Instance(TiledMapJson&& map) : d_map(pick(map)){
	d_id = GetHashValue(GetSysTime().Get());
}
	
bool Instance::AddPlayer(const Upp::String& id){
	for(const Player& p : d_players){
		if(p.GetId().IsEqual(id))
			return false;
	}
	d_players.Create(id, 0.0f, 0.0f, 1.0f);
	return true;
}
bool Instance::RemovePlayer(const Upp::String& id){
	for(int e = 0; e < d_players.GetCount(); e++){
		if(d_players[e].GetId().IsEqual(id)){
			d_players.Remove(e);
			return true;
		}
	}
	return false;
}
bool Instance::UpdatePlayer(const Upp::String& id, bool keyPressed, byte facing){
	for(Player& p : d_players){
		if(p.GetId().IsEqual(id)){
			p.SetMoving(keyPressed);
			p.SetFacing(facing);
			return true;
		}
	}
	return false;
}

double Instance::GetId()const{return d_id;}

const Player& Instance::GetPlayer(const Upp::String& id)const noexcept(false){
	for(const Player& p : d_players){
		if(p.GetId().IsEqual(id))
			return p;
	}
	throw Exc("No player exist with id: "+ id);
}

const TiledMapJson& Instance::GetMap()const{
	return d_map;
}

const Vector<Player>& Instance::GetPlayers()const{
	return d_players;
}


void Instance::Update(){
	for(Player& p : d_players){
		p.Update();
	}
}

}