#ifndef _CV2DServer_GameEngine_h_
#define _CV2DServer_GameEngine_h_
#include <Core/Core.h>
#include "Instance.h"

namespace Upp{

class GameEngine{
	public:
		GameEngine(int instanceTimeout, int mapLoadedTimeout, int playerTimeout);
		
		const Upp::String& LoadMapData(const Upp::String& filePath) noexcept(false); //The loaded will remain #mapLoadedTimeout time loaded. It is refreshed when someone ask for it. if an instance is created with a map, then this one unload from the loaded map vector
		
		double AddPlayer(const Upp::String& playerId, const Upp::String& mapFilePath) noexcept(false); //when a player want to be added to a map that isnt yet instantiate or loaded, then it try to load it and instantiate it before adding the player.
		bool UpdatePlayer(const Upp::String& playerId, bool keyPressed, byte facing); //if a player remain away from keyboard (no key pressed during more than #playerTimeout) then it remove player from the instance
		bool RemovePlayer(const Upp::String& playerId); //if player disconnect then it is possible to call this routin
		
		InstanceState GetInstanceState(double instanceId)const; //if instance state is not call for a period higher than #instanceTimeout then this instance is removed from game engine
		
	private:
		const Upp::String& LoadAMap(const Upp::String& filepath) noexcept(false);
		
		struct Timeout{
			Timeout(int instanceTm,  int mapTm, int playerTm);
			int d_instance;
			Upp::Vector<double> d_instancesTimeout;
			int d_loadedMap;
			Upp::Vector<double> d_mapsTimeout;
			int d_player;
			Upp::Vector<double> d_playersTimeout;
		}d_timeout;

		Upp::Array<Instance> d_instances;
		Upp::Array<TiledMapJson> d_maps;
		Upp::VectorMap<Upp::String, double> d_players; // <playerId, instanceId>
	
		Thread d_thread;
};

}

#endif
