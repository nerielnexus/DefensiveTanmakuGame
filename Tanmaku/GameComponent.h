#pragma once
#include "CommonDecl.h"

struct SPoint
{
	float x;
	float y;

	SPoint( float _x, float _y ) :x( _x ), y( _y )
	{

	}
	SPoint( )
	{
	}
};

bool sphere_collision_check( float x0, float y0, float size0, float x1, float y1, float size1 )
{

	if ( (x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1) < (size0 + size1) * (size0 + size1) )
		return true;
	else
		return false;
}

float getDistance( float x0, float y0, float x1, float y1 )
{
	return sqrt( pow( x0 - x1, 2 ) + pow( y0 - y1, 2 ) );
}

SPoint getLeftPosition( )
{
	std::tr1::mt19937 rand_engine( ( unsigned int ) time( NULL ) );
	std::tr1::uniform_int<int> spawn_area_x_left( 50, 425 );
	std::tr1::uniform_int<int> spawn_area_y_all( 0, SCREEN_HEIGHT );

	SPoint temp;
	temp.x = std::bind( spawn_area_x_left, rand_engine )();
	temp.y = std::bind( spawn_area_y_all, rand_engine )();

	return temp;
}

SPoint getRightPosition( )
{
	std::tr1::mt19937 rand_engine( ( unsigned int ) time( NULL ) );
	std::tr1::uniform_int<int> spawn_area_x_right( 835, 1230 );
	std::tr1::uniform_int<int> spawn_area_y_all( 0, SCREEN_HEIGHT );

	SPoint temp;
	temp.x = std::bind( spawn_area_x_right, rand_engine )();
	temp.y = std::bind( spawn_area_y_all, rand_engine )();

	return temp;
}

SPoint getUpPosition( )
{
	std::tr1::mt19937 rand_engine( ( unsigned int ) time( NULL ) );
	std::tr1::uniform_int<int> spawn_area_y_up( 50, 320 );
	std::tr1::uniform_int<int> spawn_area_x_all( 0, SCREEN_WIDTH );

	SPoint temp;
	temp.x = std::bind( spawn_area_x_all, rand_engine )();
	temp.y = std::bind( spawn_area_y_up, rand_engine )();

	return temp;
}

SPoint getDownPosition( )
{
	std::tr1::mt19937 rand_engine( ( unsigned int ) time( NULL ) );
	std::tr1::uniform_int<int> spawn_area_y_down( 640, 910 );
	std::tr1::uniform_int<int> spawn_area_x_all( 0, SCREEN_WIDTH );

	SPoint temp;
	temp.x = std::bind( spawn_area_x_all, rand_engine )();
	temp.y = std::bind( spawn_area_y_down, rand_engine )();

	return temp;
}

SPoint getSpawnPosition( )
{
	std::tr1::mt19937 rand_engine( ( unsigned int ) time( NULL ) );
	std::tr1::uniform_int<int> select_area( 1, 4 );

	int area = std::bind( select_area, rand_engine )();

	SPoint result;

	switch ( area )
	{
		case 1:
			result = getLeftPosition( );
			break;

		case 2:
			result = getRightPosition( );
			break;

		case 3:
			result = getUpPosition( );
			break;

		case 4:
			result = getDownPosition( );
			break;
	}

	return result;
}

float getTheta( )
{
	std::tr1::mt19937 rand_engine( ( unsigned int ) time( NULL ) );
	std::tr1::uniform_real<float> createTheta( -10.0f, 10.0f );

	return std::bind( createTheta, rand_engine )();
}


class Hero;
class Enemy;
class Bullet;

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

	float getXpos( ) const
	{
		return x_pos;
	}

	float getYpos( ) const
	{
		return y_pos;
	}
};


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

class Spawner :public Entity
{
	protected:
	float theta;
	int life;

	public:
	Spawner( SPoint pos )
		:Entity(pos.x,pos.y)
	{

	}

	protected:
	virtual void move( ) = 0;
	virtual void setposition( ) = 0;

	public:
	void increase_theta( float _t )
	{
		theta += _t;
	}
};

class EllipseSpawner :public Spawner
{
	private:
	SPoint radius;

	public:
	EllipseSpawner( SPoint pos ) :Spawner(pos)
	{
		setposition( );
	}

	public:
	virtual void setposition( );
	virtual void move( );
};

void EllipseSpawner::setposition( )
{
	float dist = getDistance( this->x_pos, this->y_pos, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 );

	radius.x = dist*static_cast<float>(SCREEN_WIDTH / SCREEN_HEIGHT);
	radius.y = dist*static_cast<float>(SCREEN_HEIGHT / SCREEN_WIDTH);

	theta = atan2( this->x_pos - SCREEN_WIDTH / 2, this->y_pos - SCREEN_HEIGHT / 2 ) * 360 / M_PI;
}

void EllipseSpawner::move( )
{
	x_pos = radius.x * cos( M_PI * theta / 360 ) + SCREEN_WIDTH / 2;
	y_pos = radius.y * sin( M_PI * theta / 360 ) + SCREEN_HEIGHT / 2;
}


// 적 클래스 
class Enemy :public Entity
{
	private:
	float dx;
	float dy;

    public:
    bool isReflected;

	public:
	void getDisplacement( const Hero& h );
	void tracemvnt( );
	bool boundCheck( );

	public:
	Enemy( float x, float y ) :Entity( x, y )
	{
        this->isReflected = false;
        this->status = true;        // Entity::status -> set bullet live
		dx = dy = 0.0f;
	}
};

void Enemy::getDisplacement( const Hero& h )
{
	dx = (h.x_pos - this->x_pos) / 200;
	dy = (h.y_pos - this->y_pos) / 200;

	if ( dx <= 10.0f )
		dx *= 5.0f;

	if ( dy <= 10.0f )
		dy *= 5.0f;
}

void Enemy::tracemvnt( )
{
	if ( !isReflected )
	{
		this->x_pos += dx;
		this->y_pos += dy;
	}
	else
	{
		this->x_pos += -dx *5;
		this->y_pos += -dy *5;
	}
}

bool Enemy::boundCheck( )
{
	if ( this->x_pos < 0 )
		return false;

	if ( this->x_pos > SCREEN_WIDTH )
		return false;

	if ( this->y_pos < 0 )
		return false;

	if ( this->y_pos > SCREEN_HEIGHT )
		return false;

	return true;
}


// 유저 방어벽
class Bullet :public Entity
{
	public:
	bool bShow;
	int bLife;

	public:
	bool show( );
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