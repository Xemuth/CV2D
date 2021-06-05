#ifndef _CV2DServer_Player_h_
#define _CV2DServer_Player_h_

#include <Core/Core.h>

namespace Upp{

class Player : Upp::Moveable<Player>{
	public:
		Player(const Upp::String& id, float x, float y, float speed);
	
		const Upp::String& GetId()const;
		float GetSpeed()const;
		float GetX()const;
		float GetY()const;
		byte GetFacing()const;
		bool IsMoving()const;

		void SetFacing(byte facing);
		void SetMoving(bool b);
	
		void Update();
		
		enum Direction : byte{ UP = 0x1, DOWN = 0x2, LEFT = 0x4, RIGHT = 0x8};
		
	private:
		Upp::String d_id;
		bool d_moving;
		float d_x;
		float d_y;
		float d_speed;
		byte d_facing;
};

}
#endif
