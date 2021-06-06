#include "GameEngine.h"

namespace Upp{

GameEngine::GameEngine(int instanceTimeout, int mapLoadedTimeout, int playerTimeout) : d_timeout(instanceTimeout, mapLoadedTimeout, playerTimeout){}

#ifdef flagUSETUPLE
		
const Upp::String& GameEngine::LoadMapData(const Upp::String& filePath) noexcept(false){
	for(double instanceId : d_instances.GetKeys()){
		if(d_instances[instanceId].b.GetMap().GetPath().IsEqual(filePath))
			return d_instances[instanceId].b.GetMap().GetData();
	}
	for(const Upp::String& mapFilePath : d_maps.GetKeys()){
		if(mapFilePath.IsEqual(filePath)){
			d_maps.Get(mapFilePath).a = GetSysTime().Get(); //we refresh timer
			return d_maps.Get(mapFilePath).b.GetData();
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
	for(double instanceId : d_instances.GetKeys()){
		if(d_instances[instanceId].b.GetMap().GetPath().IsEqual(mapFilePath)){
			if(!d_instances[instanceId].b.AddPlayer(playerId))
				throw Upp::Exc("Player id \"" + playerId +"\" already exist");
			d_players.Add(playerId, MakeTuple<double,double>(GetSysTime().Get(),instanceId));
			return instanceId;
		}
	}
	for(int e = 0; e < 2; e++){
		int iterator = 0;
		for(const Upp::String& mapFilePath : d_maps.GetKeys()){
			if(mapFilePath.IsEqual(mapFilePath)){
				double id = GetHashValue(GetSysTime().Get());
				Tuple<double, Instance>& instance = d_instances.Add(id, pick( MakeTuple<double, Instance>(GetSysTime().Get(), pick(d_maps.Get(mapFilePath).b))));
				instance.b.AddPlayer(playerId);
				d_maps.Remove(iterator);
				d_players.Add(playerId, MakeTuple<double,double>(GetSysTime().Get(),id));
				return id;
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
		Tuple<double, TiledMapJson>& tuple = d_maps.Add(filepath, pick(MakeTuple<double, TiledMapJson>(GetSysTime().Get(),pick(map))));
		return tuple.b.GetData();
	}catch(Exc& exception){
		throw exception;
	}
}

bool GameEngine::UpdatePlayer(const Upp::String& playerId, bool keyPressed, byte facing){
	for(const Upp::String& pid : d_players.GetKeys()){
		if(pid.IsEqual(playerId)){
			if(d_instances[d_players.Get(pid).b].b.UpdatePlayer(playerId, keyPressed, facing)){
				d_players.Get(pid).a = GetSysTime().Get(); //we refresh timer
				return true;
			}
			LOG("ALERT ! UpdatePlayer, a good ID have been found on d_players, but instance don't contain this player");
			return false;
		}
	}
	return false;
}

bool GameEngine::RemovePlayer(const Upp::String& playerId){
	for(int e = 0; e < d_players.GetCount(); e++){
		if(d_players.GetKey(e).IsEqual(playerId)){
			for(int i = 0; i < d_instances.GetCount(); i++){
				if(d_instances.GetKey(i) ==  d_players.Get(d_players.GetKey(e)).b){
					if(!d_instances[d_instances.GetKey(i)].b.RemovePlayer(playerId))
						LOG("ALERT ! RemovePlayer, A player have been remove from d_players, but not from instance (because it has not been found)");
					d_instances[d_instances.GetKey(i)].a = GetSysTime().Get();
					d_players.Remove(e);
					return true;
				}
			}
		}
	}
	return false;
}

InstanceState GameEngine::GetInstanceState(double instanceId)const{
	for(double iid : d_instances.GetKeys()){
		if(iid == instanceId)
			return d_instances[iid].b.GetInstanceState();
	}
	throw Exc("No instance with id \""+ AsString(instanceId) + "\" has been found");
}

/*************************************************************************************************************/
/*************************************************************************************************************/
#else
/*************************************************************************************************************/
/*************************************************************************************************************/

const Upp::String& GameEngine::LoadMapData(const Upp::String& filePath) noexcept(false){
	for(double instanceId : d_instances.GetKeys()){
		if(d_instances[instanceId].d_instance.GetMap().GetPath().IsEqual(filePath))
			return d_instances[instanceId].d_instance.GetMap().GetData();
	}
	for(const Upp::String& mapFilePath : d_maps.GetKeys()){
		if(mapFilePath.IsEqual(filePath)){
			d_maps.Get(mapFilePath).d_timer = GetSysTime().Get(); //we refresh timer
			return d_maps.Get(mapFilePath).d_map.GetData();
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
	for(double instanceId : d_instances.GetKeys()){
		if(d_instances[instanceId].d_instance.GetMap().GetPath().IsEqual(mapFilePath)){
			if(!d_instances[instanceId].d_instance.AddPlayer(playerId))
				throw Upp::Exc("Player id \"" + playerId +"\" already exist");
			d_players.Add(playerId, {GetSysTime().Get(),instanceId});
			return instanceId;
		}
	}
	for(int e = 0; e < 2; e++){
		int iterator = 0;
		for(const Upp::String& mapFilePath : d_maps.GetKeys()){
			if(mapFilePath.IsEqual(mapFilePath)){
				double id = GetHashValue(GetSysTime().Get());
				TimeoutData& instance = d_instances.Add(id, {GetSysTime().Get(), pick(d_maps.Get(mapFilePath).d_map)});
				instance.d_instance.AddPlayer(playerId);
				d_maps.Remove(iterator);
				d_players.Add(playerId, {GetSysTime().Get(),id});
				return id;
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
		TimeoutData& tuple = d_maps.Add(filepath, {GetSysTime().Get(),pick(map), true});
		return tuple.d_map.GetData();
	}catch(Exc& exception){
		throw exception;
	}
}

bool GameEngine::UpdatePlayer(const Upp::String& playerId, bool keyPressed, byte facing){
	for(const Upp::String& pid : d_players.GetKeys()){
		if(pid.IsEqual(playerId)){
			if(d_instances.Get(d_players.Get(pid).d_instanceId).d_instance.UpdatePlayer(playerId, keyPressed, facing)){
				d_players.Get(pid).d_timer = GetSysTime().Get(); //we refresh timer
				return true;
			}
			LOG("ALERT ! UpdatePlayer, a good ID have been found on d_players, but instance don't contain this player");
			return false;
		}
	}
	return false;
}

bool GameEngine::RemovePlayer(const Upp::String& playerId){
	for(int e = 0; e < d_players.GetCount(); e++){
		if(d_players.GetKey(e).IsEqual(playerId)){
			for(int i = 0; i < d_instances.GetCount(); i++){
				if(d_instances.GetKey(i) ==  d_players.Get(d_players.GetKey(e)).d_instanceId){
					if(!d_instances.Get(d_instances.GetKey(i)).d_instance.RemovePlayer(playerId))
						LOG("ALERT ! RemovePlayer, A player have been remove from d_players, but not from instance (because it has not been found)");
					d_instances.Get(d_instances.GetKey(i)).d_timer = GetSysTime().Get();
					d_players.Remove(e);
					return true;
				}
			}
		}
	}
	return false;
}

InstanceState GameEngine::GetInstanceState(double instanceId)const{
	for(double iid : d_instances.GetKeys()){
		if(iid == instanceId)
			return d_instances[iid].d_instance.GetInstanceState();
	}
	throw Exc("No instance with id \""+ AsString(instanceId) + "\" has been found");
}

GameEngine::TimeoutData::TimeoutData(double timer, const Upp::String& filePath) : d_timer(timer), d_map(filePath){}
GameEngine::TimeoutData::TimeoutData(double timer, TiledMapJson&& map) : d_timer(timer), d_instance(pick(map)){}
GameEngine::TimeoutData::TimeoutData(double timer, TiledMapJson&& map, bool constuctMap) : d_timer(timer), d_map(pick(map)){}
GameEngine::TimeoutData::TimeoutData(double timer, double instanceId) : d_timer(timer), d_instanceId(instanceId){}

#endif
			
GameEngine::Timeout::Timeout(int instanceTm,  int mapTm, int playerTm) : d_instance(instanceTm), d_loadedMap(mapTm), d_player(playerTm){}






}
