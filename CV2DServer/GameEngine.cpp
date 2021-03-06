#include "GameEngine.h"

#define LLOG(x)   LOG(x)

#ifdef flagWIN32
	static const Upp::String GLOBAL_FILEPATH = "C:\\Upp\\CV2D\\TiledMapReader\\";
#else
	static const Upp::String GLOBAL_FILEPATH = "/home/xemuth/Upp/CV2D/TiledMapReader/";
#endif

namespace Upp{

GameEngine::GameEngine(int instanceTimeout, int mapLoadedTimeout, int playerTimeout, int tickRate) : d_timeout(instanceTimeout, mapLoadedTimeout, playerTimeout), d_tickRate(tickRate), d_ready(false){}
GameEngine::~GameEngine(){ Stop();}

void GameEngine::Start(){
	d_stopThread = false;
	d_threadUpdater.Run(THISBACK(Updater));
	d_threadJanitor.Run(THISBACK(Janitor));
	WaitIsReady();
}

bool GameEngine::IsReady(){
	return d_ready;
}

void GameEngine::Stop(){
	d_stopThread = true;
	d_ready = false;
	d_threadUpdater.Wait();
	d_threadJanitor.Wait();
}

void GameEngine::SetCallbackInstanceTimeout(const Function<void (double)>& callback){
	d_callbackInstanceTimeout = callback;
}
void GameEngine::SetCallbackInstanceClose(const Function<void (double)>& callback){
	d_callbackInstanceClose = callback;
}
void GameEngine::SetCallbackInstanceCreate(const Function<void (double)>& callback){
	d_callbackInstanceCreate = callback;
}

bool GameEngine::RemoveInstance(double instanceId){
	for(Instance& i : d_instances){
		if(i.GetId() == instanceId){
			RemoveInstanceAdvance(instanceId);
			return true;
		}
	}
	return false;
}

const TiledMapJson& GameEngine::LoadMapData(const Upp::String& mapName) noexcept(false){
	LLOG("[GameEngine][LoadMapData] Loading map data \"" +  mapName + "\"");
	for(Instance& instance : d_instances){
		if(instance.GetMap().GetName().IsEqual(mapName)){
			LLOG("[GameEngine][LoadMapData] map found in instance id \"" +  AsString(instance.GetId()) + "\"");
			return instance.GetMap();
		}
	}
	for(int e = 0; e < d_maps.GetCount(); e++){
		if(d_maps[e].GetName().IsEqual(mapName)){
			d_timeout.d_mapsTimeout[e] = GetSysTime().Get(); //We refresh timer
			LLOG("[GameEngine][LoadMapData] map is already loaded, returning data");
			return d_maps[e];
		}
	}
	//This map has not been found, we need to load it, and ensure it's correct. if it's
	//incorrect then it throw exception
	try{
		LLOG("[GameEngine][LoadMapData] map is not already load or used in an instance. Loading in progress...");
		return LoadAMap(mapName);
	}catch(Exc& exception){
		LLOG("[GameEngine][LoadMapData] LoadAMap function have throw an exception. This filepath is invalid");
		throw exception;
	}
}

double GameEngine::AddPlayer(const Upp::String& playerId, const Upp::String& mapName) noexcept(false){
	LLOG("[GameEngine][AddPlayer] Adding a player with id \"" + playerId + "\" in progress...");
	if(d_players.Find(playerId) != -1){
		LLOG("[GameEngine][AddPlayer] Player id \"" + playerId +"\" already exist. Exception throw");
		throw Upp::Exc("Player id \"" + playerId +"\" already exist");
	}
	for(Instance& ins : d_instances){
		if(ins.GetMap().GetName().IsEqual(mapName)){
			if(!ins.AddPlayer(playerId)){
				LLOG("[GameEngine][AddPlayer] Player id \"" + playerId +"\" already exist in instance id \""+ ins.GetId() +"\". Exception throw");
				throw Upp::Exc("Player id \"" + playerId +"\" already exist in instance id \""+ ins.GetId() +"\"");
			}
			d_players.Add(playerId, ins.GetId());
			d_timeout.d_playersTimeout.Add(GetSysTime().Get());
			LLOG("[GameEngine][AddPlayer] Player id \"" + playerId +"\" have been add to instance \""+ ins.GetId() +"\". Timeout entry have been created");
			return ins.GetId();
		}
	}
	LLOG("[GameEngine][AddPlayer] No instance have been found with map name \"" + mapName + "\". Creating instance in progress...");
	for(int e = 0; e < 2; e++){
		int iterator = 0;
		for(TiledMapJson& map : d_maps){
			if(map.GetName().IsEqual(mapName)){
				LLOG("[GameEngine][AddPlayer] Map have been found, instance creation in progress...");
				Instance& instance = d_instances.Create(pick(map));
				d_timeout.d_instancesTimeout.Add(GetSysTime().Get());
				
				d_maps.Remove(iterator);
				d_timeout.d_mapsTimeout.Remove(iterator);
				
				instance.AddPlayer(playerId);
				d_players.Add(playerId, instance.GetId());
				d_timeout.d_playersTimeout.Add(GetSysTime().Get());
				LLOG("[GameEngine][AddPlayer] Instance created ( Id: \""+ AsString(instance.GetId()) +"\") and player have been added to it");
				d_callbackInstanceCreate(instance.GetId());
				return instance.GetId();
			}
			iterator++;
		}
		try{
			LLOG("[GameEngine][AddPlayer] map is not already load or used in an instance. Loading in progress...");
			LoadAMap(mapName);
		}catch(Exc& exception){
			throw exception;
		}
	}
	throw Upp::Exc("ALERT ! AddPlayer have loaded multiple dummy map and created multiple instance of the same map");
}

const TiledMapJson& GameEngine::LoadAMap(const Upp::String& mapName) noexcept(false){
	try{
		LLOG("[GameEngine][LoadAMap] Attempting to load a map \""+ mapName +"\" ");
		TiledMapJson map(GLOBAL_FILEPATH + mapName + ((mapName.Find(".json") != -1)? "":".json"));
		TiledMapJson& instertedMap = d_maps.Add(pick(map));
		d_timeout.d_mapsTimeout.Add(GetSysTime().Get());
		LLOG("[GameEngine][LoadAMap] Map have been loaded with success, timeout entry have been created");
		return instertedMap;
	}catch(Exc& exception){
		LLOG("[GameEngine][LoadAMap] Exception during load of map file \""+ mapName +"\" ("+ exception +")");
		throw exception;
	}
}

bool GameEngine::UpdatePlayer(const Upp::String& playerId, bool keyPressed, byte facing){
	LLOG("[GameEngine][UpdatePlayer] Trying to update player id \"" + playerId +"\". KeyPressed: " + AsString(keyPressed) + " Facing: " + Format64Hex(facing));
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
						LLOG("[GameEngine::UpdatePlayer]Player id \"" + playerId +"\" have been updated succesfully");
						return true;
					}
					LLOG("[GameEngine][UpdatePlayer][ALERT] Player id \"" + playerId +"\" have been found, The instance stick to it have been found. But this instance don't contain this player");
					return false;
				}
			}
			LLOG("[GameEngine][UpdatePlayer][ALERT] Player id \"" + playerId +"\" have been found, but the instance stick to it is invalid");
			return false;
		}
	}
	LLOG("[GameEngine][UpdatePlayer] Player id \"" + playerId +"\" can't be found in current loaded player");
	return false;
}

bool GameEngine::RemovePlayer(const Upp::String& playerId){
	LLOG("[GameEngine][RemovePlayer] Trying to remove player with id \"" + playerId +"\" ");
	for(int e = 0; e < d_players.GetCount(); e++){
		if(d_players.GetKey(e).IsEqual(playerId)){
			double instanceId = d_players.Get(d_players.GetKey(e));
			d_players.Remove(e);
			d_timeout.d_playersTimeout.Remove(e);
			LLOG("[GameEngine][RemovePlayer] Player id \"" + playerId +"\" have been removed");
			for(int i = 0; i < d_instances.GetCount(); i++){
				if(d_instances[i].GetId() == instanceId){
					if(!d_instances[i].RemovePlayer(playerId))
						LLOG("[GameEngine][RemovePlayer][ALERT] Player id \"" + playerId +"\" have been found but the instance stick to it don't carry this player");
					d_timeout.d_instancesTimeout[i] = GetSysTime().Get();
					LLOG("[GameEngine][RemovePlayer] The instance have been updated succesfully");
					return true;
				}
			}
			LLOG("[GameEngine][RemovePlayer][ALERT] Player id \"" + playerId +"\" have been found but the instance stick to it is invalid");
			return false;
		}
	}
	LLOG("[GameEngine::RemovePlayer] Player id \"" + playerId +"\" can't be found ");
	return false;
}

InstanceState GameEngine::GetInstanceState(double instanceId){
	LLOG("[GameEngine][GetInstanceState] Instance state have been requested for id \""+ AsString(instanceId) +"\"");
	for(int e = 0; e < d_instances.GetCount(); e++){
		const Instance& inst = d_instances[e];
		if(inst.GetId() == instanceId){
			LLOG("[GameEngine][GetInstanceState] Instance have been found, timeout refreshed");
			d_timeout.d_instancesTimeout[e] = GetSysTime().Get();
			return inst.GetInstanceState();
		}
	}
	LLOG("[GameEngine][GetInstanceState][ALERT] Instance id \""+ AsString(instanceId) +"\" don't exist. Exception have been throw");
	throw Exc("No instance with id \""+ AsString(instanceId) + "\" has been found");
}

void GameEngine::Updater(){
	LLOG("[GameEngine][Updater] Updater thread started");
	while(!d_stopThread){
		Sleep(1000/d_tickRate);
		for(Instance& inst : d_instances){
			inst.Update();
		}
	}
	LLOG("[GameEngine][Updater] Updater thread stopped");
}

void GameEngine::RemovePlayerAdvance(const Upp::String& id){
	for(int e = 0; e < d_players.GetCount(); e++){
		if(d_players.GetKey(e).IsEqual(id)){
			for(int i = 0; i < d_instances.GetCount(); i++){
				if(d_instances[i].GetId() == d_players.Get(d_players.GetKey(e))){
					d_instances[i].RemovePlayer(id);
					d_players.Remove(e);
					d_timeout.d_playersTimeout.Remove(e);
					return;
				}
			}
			break;
		}
	}
}
void GameEngine::RemoveInstanceAdvance(double id){
	for(int e = 0; e < d_instances.GetCount(); e++){
		Instance& inst = d_instances[e];
		if(inst.GetId() == id){
			for(const Player& p : inst.GetPlayers()){
				RemovePlayer(p.GetId());
			}
			d_instances.Remove(e);
			d_timeout.d_instancesTimeout.Remove(e);
			break;
		}
	}
}

void GameEngine::WaitIsReady(){
	while(!d_ready){
		Sleep(10); // I don't like that
	}
}


void GameEngine::Janitor(){
	LLOG("[GameEngine][Janitor] Janitor thread starting");
	d_ready = true;
	while(!d_stopThread){
		for(int e = 0; e < 60; e++){
			if(d_stopThread)
				break;
			Sleep(1000);
		}
		for(int e = 0; e < d_timeout.d_playersTimeout.GetCount(); e++){
			if((GetSysTime().Get() - d_timeout.d_playersTimeout[e]) > d_timeout.d_player){
				LLOG("[GameEngine][Janitor] Player " + d_players.GetKey(e) + " have timeout");
				RemovePlayerAdvance(d_players.GetKey(e));
				e--;
			}
		}
		for(int e = 0; e < d_timeout.d_mapsTimeout.GetCount(); e++){
			if((GetSysTime().Get() - d_timeout.d_mapsTimeout[e]) > d_timeout.d_loadedMap){
				LLOG("[GameEngine][Janitor] Map " + d_maps[e].GetPath() + " have timeout");
				d_maps.Remove(e);
				d_timeout.d_mapsTimeout.Remove(e);
				e--;
			}
		}
		for(int e = 0; e < d_timeout.d_instancesTimeout.GetCount(); e++){
			if((GetSysTime().Get() - d_timeout.d_instancesTimeout[e]) > d_timeout.d_instance){
				LLOG("[GameEngine][Janitor] Instance " + AsString(d_instances[e].GetId()) + " have timeout");
				d_callbackInstanceTimeout(d_instances[e].GetId());
				RemoveInstanceAdvance(d_instances[e].GetId());
				e--;
			}
		}
	}
	LLOG("[GameEngine][Janitor] Janitor thread stoping");
}

			
GameEngine::Timeout::Timeout(int instanceTm,  int mapTm, int playerTm) : d_instance(instanceTm), d_loadedMap(mapTm), d_player(playerTm){}






}
