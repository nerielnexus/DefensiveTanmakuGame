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
	// 일정주기마다 적 생성
	if ( counter % 30 == 0 )
		vEllipse.push_back( EllipseSpawner( getSpawnPosition( ), getTheta( ) ) );

	// 유저 이동처리
	hero->Movement( hWnd );

	// 적 피격처리, 이동처리
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

	// 적 탄막 지속적으로 생성
	if ( counter % 10 == 0 )
		for ( std::vector<EllipseSpawner>::iterator it = vEllipse.begin( ); it != vEllipse.end( ); it++ )
			vEnemys.push_back( Enemy( it->getXpos( ), it->getYpos( ) ) );

	// 적 탄막이 플레이어를 추적
	for ( std::vector<Enemy>::iterator eit = vEnemys.begin( ); eit != vEnemys.end( ); eit++ )
	{
		eit->getDisplacement( *hero );
		eit->tracemvnt( );
	}

	// 마우스 왼쪽 버튼으로 방어벽 생성
	if ( KEY_DOWN( VK_LBUTTON ) )
		vBullets.push_back( Bullet( *hero, BULLET_LIFE ) );

	// 마우스 오른쪽 버튼으로 폭탄 활성화/비활성화 설정
	if ( KEY_DOWN( VK_RBUTTON ) )
		for ( std::vector<Bomb>::iterator it = vBombs.begin( ); it != vBombs.end( ); it++ )
			it->isActive = true;
	else
		for ( std::vector<Bomb>::iterator it = vBombs.begin( ); it != vBombs.end( ); it++ )
		{
			it->isActive = false;
			//it->HP = 15;
		}
			

	// 폭탄 체력이 0이 되면 비활성화, 그리고 충전
	for ( std::vector<Bomb>::iterator it = vBombs.begin( ); it != vBombs.end( ); it++ )
	{
		if ( it->isActive && it->HP <= 0 )
			it->isActive = false;
	}
			

	// 폭탄 활성화중이면 회전이동하기
	for ( std::vector<Bomb>::iterator it = vBombs.begin( ); it != vBombs.end( ); it++ )
	{
		if ( it->isActive )
			it->escortmove( *hero );
	}
		

	// 폭탄과 적/탄막 충돌처리
	for ( std::vector<Bomb>::iterator it = vBombs.begin( ); it != vBombs.end( ); )
	{
		if ( it->HP <= 0 )
			it = vBombs.erase( it );
		else
		{
			// 탄막과 충돌처리
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

			// 적과 충돌처리
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

	// 방어벽과 탄막 충돌시 처리
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

	// 명을 다한 방어벽 삭제
	std::vector<Bullet>::iterator it_delete = vBullets.begin( );

	while ( it_delete != vBullets.end( ) )
	{
		if ( !it_delete->show( ) )
			it_delete = vBullets.erase( it_delete );
		else
			++it_delete;
	}

	// 명을 다한 적 탄막 삭제
	std::vector<Enemy>::iterator eit_delete = vEnemys.begin( );

	while ( eit_delete != vEnemys.end( ) )
	{
		if ( !eit_delete->boundCheck( ) )
			eit_delete = vEnemys.erase( eit_delete );
		else
			++eit_delete;
	}
}