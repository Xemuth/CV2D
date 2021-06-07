#ifndef _CV2DServer_Player_h_
#define _CV2DServer_Player_h_

#include <Core/Core.h>

namespace Upp{

class Player;

struct PlayerState : Upp::Moveable<PlayerState>{
	PlayerState(const Player& p);
	Upp::String d_id;
	Pointf d_pos;
	byte d_facing;
};

class Player : Upp::Moveable<Player>{
	public:
		Player(const Upp::String& id, Pointf pos, float speed);
	
		const Upp::String& GetId()const;
		float GetSpeed()const;
		float GetX()const;
		float GetY()const;
		byte GetFacing()const;
		bool IsMoving()const;

		void SetFacing(byte facing);
		void SetMoving(bool b);
	
		void Update(const Pointf& p);
		Pointf CalculNewPosition();
		
		enum Direction : byte{ UP = 0x1, DOWN = 0x2, LEFT = 0x4, RIGHT = 0x8};
		
	private:
		
		
		friend PlayerState;
		Upp::String d_id;
		bool d_moving;
		Pointf d_pos;
		float d_speed;
		byte d_facing;
};

}
#endif
