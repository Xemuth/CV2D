#ifndef _CV2DServer_CV2DServer_h_
#define _CV2DServer_CV2DServer_h_

#include <TiledMapReader/TiledMap.h>

namespace Upp{

enum class Facing{Left, Right, Up, Down};
Upp::String FacingToString(Facing facing){
	switch(facing){
		case Facing::Left: return "left";
		case Facing::Right: return "right";
		case Facing::Up: return "up";
		case Facing::Down: return "down";
	}
	return "";
}

// {Mapname, MapFilePath}
Upp::VectorMap<Upp::String, Upp::String> maps;

class Player{
	public:
		Player(int _x, int _y, int _id) x(_x), y(_y), id(_id), facing(Facing::Up){}
	
		int GetId()const{return id;}
		int GetX()const{return x;}
		int GetY()const{return y;}
		Facing GetFacing()const{return facing;}
		
		void Move(Facing _facing, unsigned int value = 1){
			facing = _facing;
			switch(_facing){
				case Facing::Left: x -= value; break;
				case Facing::Right: x += value;	break;
				case Facing::Up: y += value; break;
				case Facing::Down: y -= value; break;
			};
		}

		void Jsonize(JsonIO& json)const{
			json("id", id)
				("facing", FacingToString(facing))
				("x", x)
				("y", y);
		}

	private:
		int id;
		Facing facing;
		int x;
		int y;
};

class Instance{
	public:
		Instance(const Upp::String& filePath) : tiledMap(filePath){}
		
	private:
		TiledMapJson tiledMap;
		Vector<Player> players;
};

class CV2DServer : SkylarkApp{
	public:
		
		void CreateInstance(const Upp::String& mapName);
		void JoinInstance(const Upp::String& mapName);
		void RemoveInstance();
		
		void ProcessMove();
		void SendMap(const Upp::String& mapName);
	
	private:

		
};

}
#endif
