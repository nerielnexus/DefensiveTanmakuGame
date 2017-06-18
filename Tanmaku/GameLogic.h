#pragma once
#include "GameComponent.h"

std::vector<Enemy> vEnemys;
std::vector<Bullet> vBullets;
std::vector<EllipseSpawner> vEllipse;
Hero* hero;

void init_game(void)
{
	hero = new Hero( 100.0f, 150.0f );
}

void do_game_logic(HWND hWnd)
{
	if ( counter % 30 == 0 )
	{
		vEllipse.push_back( EllipseSpawner( getSpawnPosition( ) ) );
	}

	hero->Movement(hWnd);

	for ( std::vector<EllipseSpawner>::iterator it = vEllipse.begin( ); it != vEllipse.end( ); it++ )
	{
		it->increase_theta( getTheta() );
		it->move( );
	}
		

	for ( std::vector<Enemy>::iterator eit = vEnemys.begin( ); eit != vEnemys.end( ); eit++ )
	{
		eit->getDisplacement( *hero );
		eit->tracemvnt( );
	}

	if ( KEY_DOWN( VK_LBUTTON ) )
	{
		vBullets.push_back( Bullet( *hero, BULLET_LIFE ) );
	}

	for ( std::vector<Bullet>::iterator it = vBullets.begin( ); it != vBullets.end( ); )
	{
		if ( it->bLife < 0 )
			it = vBullets.erase( it );
		else
		{
			for ( std::vector<Enemy>::iterator eit = vEnemys.begin( ); eit != vEnemys.end( ); )
			{
				if ( it->check_collision( *eit ) )
					eit->isReflected = true;

				it->bLife--;
				++eit;
			}

			++it;
		}
	}

	std::vector<Bullet>::iterator it_delete = vBullets.begin( );

	while ( it_delete != vBullets.end( ) )
	{
		if ( !it_delete->show( ) )
			it_delete = vBullets.erase( it_delete );
		else
			++it_delete;

	}

	std::vector<Enemy>::iterator eit_delete = vEnemys.begin( );

	while ( eit_delete != vEnemys.end( ) )
	{
		if ( !eit_delete->boundCheck( ) )
			eit_delete = vEnemys.erase( eit_delete );
		else
			++eit_delete;
	}
}