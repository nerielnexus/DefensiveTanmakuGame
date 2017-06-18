#pragma once
#include "GameComponent.h"

std::vector<Enemy> vEnemys;
std::vector<Bullet> vBullets;
std::vector<EllipseSpawner> vEllipse;
std::vector<Bomb> vBombs;
Hero* hero;

void init_game( void )
{
	hero = new Hero( 100.0f, 150.0f );

	for ( int i = 1; i <= 10; i++ )
		vBombs.push_back( Bomb( *hero, 72 * i ) );
}

void do_game_logic( HWND hWnd )
{
	// �����ֱ⸶�� �� ����
	if ( counter % 30 == 0 )
		vEllipse.push_back( EllipseSpawner( getSpawnPosition( ), getTheta( ) ) );

	// ���� �̵�ó��
	hero->Movement( hWnd );

	// �� �ǰ�ó��, �̵�ó��
	for ( std::vector<EllipseSpawner>::iterator it = vEllipse.begin( ); it != vEllipse.end( ); )
	{
		for ( std::vector<Enemy>::iterator eit = vEnemys.begin( ); eit != vEnemys.end( ); eit++ )
			if ( sphere_collision_check( it->getXpos( ), it->getYpos( ), 32, eit->getXpos( ), eit->getYpos( ), 32 ) )
				if ( eit->isReflected )
					it->life--;

		if ( !it->checkLife( ) )
		{
			it->increase_theta( );
			it->move( );
			++it;
		}
		else
			it = vEllipse.erase( it );
	}

	// �� ź�� ���������� ����
	if ( counter % 10 == 0 )
		for ( std::vector<EllipseSpawner>::iterator it = vEllipse.begin( ); it != vEllipse.end( ); it++ )
			vEnemys.push_back( Enemy( it->getXpos( ), it->getYpos( ) ) );

	// �� ź���� �÷��̾ ����
	for ( std::vector<Enemy>::iterator eit = vEnemys.begin( ); eit != vEnemys.end( ); eit++ )
	{
		eit->getDisplacement( *hero );
		eit->tracemvnt( );
	}

	// ���콺 ���� ��ư���� �� ����
	if ( KEY_DOWN( VK_LBUTTON ) )
		vBullets.push_back( Bullet( *hero, BULLET_LIFE ) );

	// ���콺 ������ ��ư���� ��ź Ȱ��ȭ/��Ȱ��ȭ ����
	if ( KEY_DOWN( VK_RBUTTON ) )
		for ( std::vector<Bomb>::iterator it = vBombs.begin( ); it != vBombs.end( ); it++ )
			it->isActive = true;
	else
		for ( std::vector<Bomb>::iterator it = vBombs.begin( ); it != vBombs.end( ); it++ )
		{
			it->isActive = false;
			//it->HP = 15;
		}
			

	// ��ź ü���� 0�� �Ǹ� ��Ȱ��ȭ, �׸��� ����
	for ( std::vector<Bomb>::iterator it = vBombs.begin( ); it != vBombs.end( ); it++ )
	{
		if ( it->isActive && it->HP <= 0 )
			it->isActive = false;
	}
			

	// ��ź Ȱ��ȭ���̸� ȸ���̵��ϱ�
	for ( std::vector<Bomb>::iterator it = vBombs.begin( ); it != vBombs.end( ); it++ )
	{
		if ( it->isActive )
			it->escortmove( *hero );
	}
		

	// ��ź�� ��/ź�� �浹ó��
	for ( std::vector<Bomb>::iterator it = vBombs.begin( ); it != vBombs.end( ); )
	{
		if ( it->HP <= 0 )
			it = vBombs.erase( it );
		else
		{
			// ź���� �浹ó��
			for ( std::vector<Enemy>::iterator eit = vEnemys.begin( ); eit != vEnemys.end( ); )
			{
				if ( sphere_collision_check(it->getXpos(),it->getYpos(),32,eit->getXpos(),eit->getYpos(),32) )
				{
					it->HP--;
					eit = vEnemys.erase( eit );
				}
				else
					++eit;
			}

			// ���� �浹ó��
			for ( std::vector<EllipseSpawner>::iterator eit = vEllipse.begin( ); eit != vEllipse.end( ); )
			{
				if ( sphere_collision_check(it->getXpos(),it->getYpos(),32,eit->getXpos(),eit->getYpos(),32) )
				{
					it->HP--;
					eit = vEllipse.erase( eit );
				}
				else
					++eit;
			}

			++it;
		}
	}

	// ���� ź�� �浹�� ó��
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

	// ���� ���� �� ����
	std::vector<Bullet>::iterator it_delete = vBullets.begin( );

	while ( it_delete != vBullets.end( ) )
	{
		if ( !it_delete->show( ) )
			it_delete = vBullets.erase( it_delete );
		else
			++it_delete;
	}

	// ���� ���� �� ź�� ����
	std::vector<Enemy>::iterator eit_delete = vEnemys.begin( );

	while ( eit_delete != vEnemys.end( ) )
	{
		if ( !eit_delete->boundCheck( ) )
			eit_delete = vEnemys.erase( eit_delete );
		else
			++eit_delete;
	}
}