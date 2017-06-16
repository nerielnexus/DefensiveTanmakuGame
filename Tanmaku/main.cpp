#include "CommonDecl.h"
#include "GameComponent.h"

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

std::vector<Enemy> vEnemys;
std::vector<Bullet> vBullets;
Hero* hero;



// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL, L"WindowClass", L"Our Direct3D Program",
						   WS_EX_TOPMOST | WS_POPUP, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
						   NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	initD3D(hWnd);


	//게임 오브젝트 초기화 
	init_game();

	// enter the main loop:

	MSG msg;

	while (TRUE)
	{
		DWORD starting_point = GetTickCount();

		for(int i=0; i<3; i++)
			vEnemys.push_back( Enemy( rand( ) % SCREEN_WIDTH, rand( ) % SCREEN_HEIGHT ) );

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		do_game_logic(hWnd);

		render_frame();

		// check the 'escape' key
		if (KEY_DOWN(VK_ESCAPE))
			PostMessage(hWnd, WM_DESTROY, 0, 0);




		while ((GetTickCount() - starting_point) < 13);
	}

	// clean up DirectX and COM
	cleanD3D();

	return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;


	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
					   D3DDEVTYPE_HAL,
					   hWnd,
					   D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					   &d3dpp,
					   &d3ddev);

	D3DXCreateSprite(d3ddev, &d3dspt);    // create the Direct3D Sprite object

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
								 L"Panel3.png",    // the file name
								 D3DX_DEFAULT,    // default width
								 D3DX_DEFAULT,    // default height
								 D3DX_DEFAULT,    // no mip mapping
								 NULL,    // regular usage
								 D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
								 D3DPOOL_MANAGED,    // typical memory handling
								 D3DX_DEFAULT,    // no filtering
								 D3DX_DEFAULT,    // no mip filtering
								 D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
								 NULL,    // no image info struct
								 NULL,    // not using 256 colors
								 &sprite);    // load to sprite


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
								 L"hero.png",    // the file name
								 D3DX_DEFAULT,    // default width
								 D3DX_DEFAULT,    // default height
								 D3DX_DEFAULT,    // no mip mapping
								 NULL,    // regular usage
								 D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
								 D3DPOOL_MANAGED,    // typical memory handling
								 D3DX_DEFAULT,    // no filtering
								 D3DX_DEFAULT,    // no mip filtering
								 D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
								 NULL,    // no image info struct
								 NULL,    // not using 256 colors
								 &sprite_hero);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
								 L"enemy.png",    // the file name
								 D3DX_DEFAULT,    // default width
								 D3DX_DEFAULT,    // default height
								 D3DX_DEFAULT,    // no mip mapping
								 NULL,    // regular usage
								 D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
								 D3DPOOL_MANAGED,    // typical memory handling
								 D3DX_DEFAULT,    // no filtering
								 D3DX_DEFAULT,    // no mip filtering
								 D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
								 NULL,    // no image info struct
								 NULL,    // not using 256 colors
								 &sprite_enemy);    // load to sprite


	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
								 L"bullet.png",    // the file name
								 D3DX_DEFAULT,    // default width
								 D3DX_DEFAULT,    // default height
								 D3DX_DEFAULT,    // no mip mapping
								 NULL,    // regular usage
								 D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
								 D3DPOOL_MANAGED,    // typical memory handling
								 D3DX_DEFAULT,    // no filtering
								 D3DX_DEFAULT,    // no mip filtering
								 D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
								 NULL,    // no image info struct
								 NULL,    // not using 256 colors
								 &sprite_bullet);    // load to sprite





	return;
}


void init_game(void)
{
	hero = new Hero( 100.0f, 150.0f );

	for ( int i = 0; i < ENEMY_NUM; i++ )
	{
		vEnemys.push_back( Enemy( rand( ) % SCREEN_WIDTH, rand( ) % SCREEN_HEIGHT ) );
	}
		
}

void do_game_logic(HWND hWnd)
{

	hero->Movement(hWnd);

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

// this is the function used to render a single frame
void render_frame(void)
{
	// clear the window to a deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();    // begins the 3D scene

	d3dspt->Begin(D3DXSPRITE_ALPHABLEND);    // // begin sprite drawing with transparency

											 //UI 창 렌더링 


											 /*
											 static int frame = 21;    // start the program on the final frame
											 if(KEY_DOWN(VK_SPACE)) frame=0;     // when the space key is pressed, start at frame 0
											 if(frame < 21) frame++;     // if we aren't on the last frame, go to the next frame

											 // calculate the x-position
											 int xpos = frame * 182 + 1;

											 RECT part;
											 SetRect(&part, xpos, 0, xpos + 181, 128);
											 D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
											 D3DXVECTOR3 position(150.0f, 50.0f, 0.0f);    // position at 50, 50 with no depth
											 d3dspt->Draw(sprite, &part, &center, &position, D3DCOLOR_ARGB(127, 255, 255, 255));
											 */

											 //주인공 



	for ( std::vector<Bullet>::iterator it = vBullets.begin( ); it != vBullets.end( ); it++ )
	{
		if ( it->show( ) )
		{
			RECT part1;
			SetRect( &part1, 0, 0, 64, 64 );
			D3DXVECTOR3 center1( 0.0f, 0.0f, 0.0f );    // center at the upper-left corner
			D3DXVECTOR3 position1( it->x_pos, it->y_pos, 0.0f );    // position at 50, 50 with no depth
			d3dspt->Draw( sprite_bullet, &part1, &center1, &position1, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
	}

	for ( std::vector<Enemy>::iterator eit = vEnemys.begin( ); eit != vEnemys.end( ); eit++ )
	{
		RECT part2;
		SetRect( &part2, 0, 0, 64, 64 );
		D3DXVECTOR3 center2( 0.0f, 0.0f, 0.0f );    // center at the upper-left corner
		D3DXVECTOR3 position2( eit->x_pos, eit->y_pos, 0.0f );    // position at 50, 50 with no depth
		d3dspt->Draw( sprite_enemy, &part2, &center2, &position2, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}

	RECT part;
	SetRect( &part, 0, 0, 64, 64 );
	D3DXVECTOR3 center( 0.0f, 0.0f, 0.0f );    // center at the upper-left corner
	D3DXVECTOR3 position( hero->x_pos, hero->y_pos, 0.0f );    // position at 50, 50 with no depth
	d3dspt->Draw( sprite_hero, &part, &center, &position, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );



	d3dspt->End();    // end sprite drawing

	d3ddev->EndScene();    // ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL);

	return;
}


// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	sprite->Release();
	d3ddev->Release();
	d3d->Release();

	// Release instances
	sprite_hero->Release();
	sprite_enemy->Release();
	sprite_bullet->Release();

	return;
}
