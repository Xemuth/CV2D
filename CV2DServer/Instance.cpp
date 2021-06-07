#include "Instance.h"

namespace Upp{
	
Instance::Instance(TiledMapJson&& map, double id) : d_id(id), d_map(pick(map)){
	if(id == 0)
		d_id = GetHashValue(GetSysTime().Get());
}
	
bool Instance::AddPlayer(const Upp::String& id){
	for(const Player& p : d_players){
		if(p.GetId().IsEqual(id))
			return false;
	}
	d_players.Create(id, GetDefaultPos(), 1.0f);
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

InstanceState::InstanceState(const Instance& instance){
	for(const Player& p : instance.d_players){
		d_ps.Create(p);
	}
	d_id = instance.d_id;
}

InstanceState Instance::GetInstanceState()const{
	return InstanceState{*this};
}

bool Instance::IsOutOfBound(const Pointf& p){
	if(std::roundf(p.x) < 0) return true;
	if(std::roundf(p.y) > 0) return true;
	if(abs(p.x) > d_map.GetWidth()) return true;
	if(abs(p.y) > d_map.GetHeight()) return true;
	return false;
}

Pointf Instance::GetDefaultPos(){
	for(int e = 0; e > (-d_map.GetHeight()); e--){
		for(int i = 0; i < d_map.GetWidth(); i++){
			Pointf p(i,e);
			if(!CheckForColision(p))
				return p;
		}
	}
	throw Exc("No tile are possible");
}

bool Instance::CheckForColision(const Pointf& p){
	if(!IsOutOfBound(p)){
		for(const TiledLayer& t : d_map.GetLayers()){
			bool isSolide = false;
			for(int i = 0; i < t.GetProperties().GetCount(); i++){
				if(t.GetProperties()[0]["name"].ToString().IsEqual("solid")){
					isSolide = true;
					break;
				}
			}
			if(isSolide){
				//This both calcul determine x and y depending on tilewidth
				//float x = std::roundf(p.x) + std::lerp(0, d_map.GetTileWidth(),(p.x - std::roundf(p.x)));
				//float y = std::roundf(p.y) + std::lerp(0, d_map.GetTileHeight(),(p.y - std::roundf(p.y)));
				int tileToGet = int(abs<float>(p.x)) + (int(abs<float>(p.y)) * d_map.GetWidth());
				if(t.GetDatas()[tileToGet] != 0)
					return true;
				break;
			}
		}
		return false;
	}
	return true;
}

void Instance::Update(){
	for(Player& p : d_players){
		Pointf pos = p.CalculNewPosition();
		if(IsOutOfBound(pos))
			p.Update(GetDefaultPos());
		else if(!CheckForColision(pos))
			p.Update(pos);
	}
}

}