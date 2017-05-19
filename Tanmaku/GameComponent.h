#pragma once

#include "CommonDecl.h"

//기본 클래스 
class Entity
{
	public:
	float x_pos;
	float y_pos;
	int status;
	int HP;

	public:
	Entity( float x, float y )
	{
		this->x_pos = x;
		this->y_pos = y;
	}
};


bool sphere_collision_check( float x0, float y0, float size0, float x1, float y1, float size1 )
{

	if ( (x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1) < (size0 + size1) * (size0 + size1) )
		return true;
	else
		return false;

}



//주인공 클래스 
class Hero :public Entity
{
	public:
	Hero( float x, float y ) : Entity( x, y )
	{

	}

	public:
	void Movement( HWND hWnd );
};

void Hero::Movement( HWND hWnd )
{
	::GetCursorPos( &onScreenPos );
	::ScreenToClient( hWnd, &onScreenPos );

	this->x_pos = onScreenPos.x - 25;
	this->y_pos = onScreenPos.y - 25;
}




// 적 클래스 
class Enemy :public Entity
{
	public:
	void tracemove( const Hero & h );

	public:
	Enemy( float x, float y ) :Entity( x, y )
	{

	}
};

void Enemy::tracemove( const Hero & h )
{
	float dx = (h.x_pos - this->x_pos) / 20;
	float dy = (h.y_pos - this->y_pos) / 20;

	if ( dx <= 10.0f )
		dx *= 2.0f;

	if ( dy <= 10.0f )
		dy *= 2.0f;

	this->x_pos += dx;
	this->y_pos += dy;
}


class Bullet :public Entity
{
	public:
	bool bShow;
	int bLife;

	public:
	bool show( );
	void hide( );
	void active( );
	bool check_collision( const Enemy& e );

	public:
	Bullet( const Hero& h, int life ) : Entity( h.x_pos, h.y_pos )
	{
		this->active( );
		bLife = life;
	}
};


bool Bullet::check_collision( const Enemy& e )
{
	if ( sphere_collision_check( x_pos, y_pos, 32, e.x_pos, e.y_pos, 32 ) == true )
		return true;
	else
		return false;
}



bool Bullet::show( )
{
	return bShow;

}


void Bullet::active( )
{
	bShow = true;

}

void Bullet::hide( )
{
	bShow = false;

}