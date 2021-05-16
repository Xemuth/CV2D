#ifndef _CV2DServer_CV2DServer_h_
#define _CV2DServer_CV2DServer_h_

#include <TiledMapReader/TiledMap.h>

namespace Upp{

enum class Facing{Left = 1, Right = 2 , Up = 4, Down = 8 };

Upp::String FacingToString(byte facing){
	Upp::String str;
	if(facing & Right)
		str = "right";
	else if
		str = "left";
	
	if(facing & Up)
		str = "up";
	else if(facing & Facing::Down)
		str = "down";
	return str;
}

// {Mapname, MapFilePath}
Upp::VectorMap<Upp::String, Upp::String> maps;

class Player{
	public:
		Player(float _x, float _y, const Upp::String& _id) x(_x), y(_y), id(_id), facing(Facing::Down){}
	
		int GetId()const{return id;}
		float GetX()const{return x;}
		float GetY()const{return y;}
		byte GetFacing()const{return facing;}
		void UpdateFacing(byte direction){facing = direction;}
		void UpdateMove(bool pressed){move = pressed;}
		
		void Update(){
			if(move){
				if(facing & Facing::Left)
					x-=movespeed;
				if(facing & Facing::Right)
					x+=movespeed;
				if(facing & Facing::Up)
					y-=movespeed;
				if(facing & Facing::Down)
					y+=movespeed;
			}
		}
		
		void Jsonize(JsonIO& json)const{
			json("id", id)
				("facing", FacingToString(facing))
				("x", x)
				("y", y);
		}

	private:
		bool move = false;
		byte facing;
		float movespeed 0.16f
		
		
		Upp::String id;
		
		float x;
		float y;
};

class Instance{
	public:
		Instance(const Upp::String& filePath, TcpSocket clientSocket) : tiledMap(filePath){}
		
	private:
		void SendInstanceState();
		
		TiledMapJson tiledMap;
		Vector<Player> players;
		
		Thread stateSender;
};

class CV2DServer{
	public:
		CV2DServer();
		
		void ProcessCommand(const Upp::String& cmd);
		
		void CreateInstance(const Upp::String& mapName);
		void JoinInstance(const Upp::String& mapName);
		void RemoveInstance();
		
		void ProcessMove();
		void SendMap(const Upp::String& mapName){
			LoadFile(f->FilePath);
		}
		
		void StartServer();
	
	private:
		TcpSocket server;
		TcpSocket client;
};

}
#endif
