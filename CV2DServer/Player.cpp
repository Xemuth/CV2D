#include "Player.h"

namespace Upp{
	
	static const float MOVING_FACTOR = 0.2f;
	
	Player::Player(const Upp::String& id, float x, float y, float speed)
	: d_x(x), d_y(y), d_speed(speed), d_id(id), d_facing(Player::Direction::DOWN){}
	
	Upp::String Player::GetId()const{return d_id;}
	float Player::GetSpeed()const{return d_speed;}
	float Player::GetX()const{return d_x;}
	float Player::GetY()const{return d_y;}
	byte Player::GetFacing()const{return d_facing;}
	bool Player::IsMoving()const{return d_moving;}
	
	void Player::SetFacing(byte facing){d_facing = facing;}
	void Player::SetMoving(bool b){d_moving = b;}
	
	void Player::Update(){
		if(d_moving){
			if(d_facing & Direction::UP)
				d_y += d_speed * MOVING_FACTOR;
			if(d_facing & Direction::DOWN)
				d_y -= d_speed * MOVING_FACTOR;
			if(d_facing & Direction::LEFT)
				d_x -= d_speed * MOVING_FACTOR;
			if(d_facing & Direction::RIGHT)
				d_y += d_speed * MOVING_FACTOR;
		}
	}
}
