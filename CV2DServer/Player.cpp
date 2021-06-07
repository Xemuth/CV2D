#include "Player.h"

namespace Upp{
	
	static const float MOVING_FACTOR = 0.2f;
	
	Player::Player(const Upp::String& id, Pointf pos, float speed)
	: d_pos(pos), d_speed(speed), d_id(id), d_facing(Player::Direction::DOWN){}
	
	const Upp::String& Player::GetId()const{return d_id;}
	float Player::GetSpeed()const{return d_speed;}
	float Player::GetX()const{return d_pos.x;}
	float Player::GetY()const{return d_pos.y;}
	byte Player::GetFacing()const{return d_facing;}
	bool Player::IsMoving()const{return d_moving;}
	
	void Player::SetFacing(byte facing){d_facing = facing;}
	void Player::SetMoving(bool b){d_moving = b;}
	
	void Player::Update(const Pointf& p){
		d_pos = p;
	}
	
	Pointf Player::CalculNewPosition(){
		Pointf p(d_pos);
		if(d_moving){
			if(d_facing & Direction::UP)
				p.y += d_speed * MOVING_FACTOR;
			if(d_facing & Direction::DOWN)
				p.y -= d_speed * MOVING_FACTOR;
			if(d_facing & Direction::LEFT)
				p.x -= d_speed * MOVING_FACTOR;
			if(d_facing & Direction::RIGHT)
				p.x += d_speed * MOVING_FACTOR;
		}
		return p;
	}
	
	PlayerState::PlayerState(const Player& p){
		d_id = p.d_id;
		d_pos = p.d_pos;
		d_facing = p.d_facing;
	}
}
