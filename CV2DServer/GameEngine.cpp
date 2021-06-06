#include "GameEngine.h"

namespace Upp{

GameEngine::GameEngine(int instanceTimeout, int mapLoadedTimeout, int playerTimeout) : d_timeout(instanceTimeout, mapLoadedTimeout, playerTimeout){}

const Upp::String& GameEngine::LoadMapData(const Upp::String& filePath) noexcept(false){
	for(Instance& instance : d_instances){
		if(instance.GetMap().GetPath().IsEqual(filePath))
			return instance.GetMap().GetData();
	}
	for(int e = 0; e < d_maps.GetCount(); e++){
		if(d_maps[e].GetPath().IsEqual(filePath)){
			d_timeout.d_mapsTimeout[e] = GetSysTime().Get(); //We refresh timer
			return d_maps[e].GetData();
		}
	}
	//This map has not been found, we need to load it, and ensure it's correct. if it's
	//incorrect then it throw exception
	try{
		return LoadAMap(filePath);
	}catch(Exc& exception){
		throw exception;
	}
}

double GameEngine::AddPlayer(const Upp::String& playerId, const Upp::String& mapFilePath) noexcept(false){
	if(d_players.Find(playerId) != -1)
		throw Upp::Exc("Player id \"" + playerId +"\" already exist");
	for(Instance& ins : d_instances){
		if(ins.GetMap().GetPath().IsEqual(mapFilePath)){
			if(!ins.AddPlayer(playerId))
				throw Upp::Exc("Player id \"" + playerId +"\" already exist");
			d_players.Add(playerId, ins.GetId());
			d_timeout.d_playersTimeout.Add(GetSysTime().Get());
			return ins.GetId();
		}
	}
	for(int e = 0; e < 2; e++){
		int iterator = 0;
		for(TiledMapJson& map : d_maps){
			if(map.GetPath().IsEqual(mapFilePath)){
				Instance& instance = d_instances.Create(pick(map));
				d_timeout.d_instancesTimeout.Add(GetSysTime().Get());
				
				d_maps.Remove(iterator);
				d_timeout.d_mapsTimeout.Remove(iterator);
				
				instance.AddPlayer(playerId);
				d_players.Add(playerId, instance.GetId());
				return instance.GetId();
			}
			iterator++;
		}
		try{
			LoadAMap(mapFilePath);
		}catch(Exc& exception){
			throw exception;
		}
	}
	throw Upp::Exc("ALERT ! AddPlayer have loaded multiple dummy map and created multiple instance of the same map");
}

const Upp::String& GameEngine::LoadAMap(const Upp::String& filepath) noexcept(false){
	try{
		TiledMapJson map(filepath);
		TiledMapJson& instertedMap = d_maps.Add(pick(map));
		d_timeout.d_mapsTimeout.Add(GetSysTime().Get());
		return instertedMap.GetData();
	}catch(Exc& exception){
		throw exception;
	}
}

bool GameEngine::UpdatePlayer(const Upp::String& playerId, bool keyPressed, byte facing){
	for(int e = 0; e < d_players.GetCount(); e++){
		const Upp::String& pid = d_players.GetKey(e);
		if(pid.IsEqual(playerId)){
			for(int i = 0; i < d_instances.GetCount(); i++){
				Instance& ins = d_instances[i];
				if(ins.GetId() == d_players.Get(pid)){
					if(ins.UpdatePlayer(playerId, keyPressed, facing)){
						Upp::int64 ti = GetSysTime().Get();
						d_timeout.d_playersTimeout[e] = ti;
						d_timeout.d_instancesTimeout[i] = ti;
						return true;
					}
					LOG("ALERT ! UpdatePlayer, a good ID have been found on d_players, but instance don't contain this player");
					return false;
				}
			}
			LOG("ALERT ! UpdatePlayer, Player exist in d_players but the instance linked to it don't exist");
			return false;
		}
	}
	return false;
}

bool GameEngine::RemovePlayer(const Upp::String& playerId){
	for(int e = 0; e < d_players.GetCount(); e++){
		if(d_players.GetKey(e).IsEqual(playerId)){
			double instanceId = d_players.Get(d_players.GetKey(e));
			d_players.Remove(e);
			d_timeout.d_playersTimeout.Remove(e);
			for(int i = 0; i < d_instances.GetCount(); i++){
				if(d_instances[i].GetId() == instanceId){
					if(!d_instances[i].RemovePlayer(playerId))
						LOG("ALERT ! RemovePlayer, A player have been remove from d_players, but not from instance (because it has not been found)");
					d_timeout.d_instancesTimeout[i] = GetSysTime().Get();
					return true;
				}
			}
			LOG("ALERT ! RemovePlayer, The instance of the player to be remove have not been found");
			return false;
		}
	}
	return false;
}

InstanceState GameEngine::GetInstanceState(double instanceId)const{
	for(const Instance& inst : d_instances){
		if(inst.GetId() == instanceId)
			return inst.GetInstanceState();
	}
	throw Exc("No instance with id \""+ AsString(instanceId) + "\" has been found");
}
			
GameEngine::Timeout::Timeout(int instanceTm,  int mapTm, int playerTm) : d_instance(instanceTm), d_loadedMap(mapTm), d_player(playerTm){}






}
