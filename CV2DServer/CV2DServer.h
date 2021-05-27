#ifndef _CV2DServer_CV2DServer_h_
#define _CV2DServer_CV2DServer_h_

#include <TiledMapReader/TiledMap.h>

namespace Upp{

enum class Facing:byte{Left = 1, Right = 2, Up = 4, Down = 8 };

Upp::String FacingToString(byte facing){
	Upp::String str;
	if(facing & (byte)Facing::Right)
		str = "right";
	else
		str = "left";
	
	if(facing & (byte)Facing:: Up)
		str = "up";
	else if(facing & (byte)Facing::Down)
		str = "down";
	return str;
}

// {Mapname, MapFilePath}
Upp::VectorMap<Upp::String, Upp::String> maps;

class Player : Upp::Moveable<Player>{
	public:
		Player(float _x, float _y, const Upp::String& _id) : x(_x), y(_y), id(_id), facing((byte)Facing::Down){}
	
		const Upp::String& GetId()const{return id;}
		float GetX()const{return x;}
		float GetY()const{return y;}
		byte GetFacing()const{return facing;}
		void UpdateFacing(byte direction){facing = direction;}
		void UpdateMove(bool pressed){move = pressed;}
		
		void Update(){
			if(move){
				if(facing & (byte)Facing::Left)
					x-=movespeed;
				if(facing & (byte)Facing::Right)
					x+=movespeed;
				if(facing & (byte)Facing::Up)
					y-=movespeed;
				if(facing & (byte)Facing::Down)
					y+=movespeed;
			}
		}
		
		void Jsonize(Json& json)const{
			json("id", id)
				("facing", FacingToString(facing))
				("x", x)
				("y", y);
		}

	private:
		bool move = false;
		byte facing;
		float movespeed = 0.16f;
		
		
		Upp::String id;
		
		float x;
		float y;
};

class Instance{
	public:
		Instance(const Upp::String& filePath, unsigned int _tickRate, Mutex& _mutex);
		
		Vector<Player> GetPlayers(){return players;}
		const Upp::String& GetID()const{return id;}
	private:
		void SendInstanceState();
		
		
		TiledMapJson tiledMap;
		Vector<Player> players;
		Upp::String id;
		Mutex& mutex;
		unsigned int tickRate;

		Thread stateSender;
};

class CV2DServer{
	public:
		CV2DServer(unsigned int _port, unsigned int _tickRate) : port(_port), tickRate(_tickRate){}
		~CV2DServer();
		void StartServer();
		void StopServer();
		
		Instance& CreateInstance(const Upp::String& mapName);
		
		/*
		void JoinInstance(const Upp::String& mapName);
		void RemoveInstance();
		
		void ProcessMove();
		void SendMap(const Upp::String& mapName){
			//LoadFile(f->FilePath);
		}
		*/
		
		typedef CV2DServer CLASSNAME;
	
	
	private:
		Upp::Array<Instance> instances;
		
		bool secureStop = false;
		
		Upp::String ProcessCommandNetwork(const Upp::String& networkCmd);
		Upp::String ProcessCommandLine(const Upp::String& command);
		void ServerRoutine();
		
		unsigned int port;
		unsigned int tickRate;
		
		Mutex mutex;
		
		Thread serverThread;
		TcpSocket server;
		TcpSocket client;
};

}
#endif
