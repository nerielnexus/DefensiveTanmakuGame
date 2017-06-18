#include "GameLogic.h"

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void CreateTexture(  LPCTSTR filename, LPDIRECT3DTEXTURE9* texture );
void RenderTexture( LPDIRECT3DTEXTURE9 texture, float xpos, float ypos, int size );

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int nCmdShow)
{
	AllocConsole( );
	freopen( "CONOUT$", "wt", stdout );


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
						  /* WS_EX_TOPMOST | WS_POPUP*/WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
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

		while ( (GetTickCount( ) - starting_point) < 16 );
		
		counter++;
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

	CreateTexture( L"Image/player.png", &sprite_hero );
	CreateTexture( L"Image/enemy.png", &sprite_enemy );
	CreateTexture( L"Image/bullet.png", &sprite_bullet );
	CreateTexture( L"Image/spawner.png", &sprite_spawner );

	return;
}

// this is the function used to render a single frame
void render_frame(void)
{
	// clear the window to a deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();    // begins the 3D scene

	d3dspt->Begin(D3DXSPRITE_ALPHABLEND);    // // begin sprite drawing with transparency

	/*
	UI 렌더링

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

	for ( std::vector<Bullet>::iterator it = vBullets.begin( ); it != vBullets.end( ); it++ )
		if ( it->show( ) )
			RenderTexture( sprite_bullet, it->x_pos, it->y_pos, 32 );

	for ( std::vector<Enemy>::iterator eit = vEnemys.begin( ); eit != vEnemys.end( ); eit++ )
		RenderTexture( sprite_enemy, eit->getXpos( ), eit->getYpos( ), 32 );

	for ( std::vector<EllipseSpawner>::const_iterator it = vEllipse.begin( ); it != vEllipse.end( ); it++ )
		RenderTexture( sprite_spawner, it->getXpos( ), it->getYpos( ), 64 );

	RenderTexture( sprite_hero, hero->getXpos( ), hero->getYpos( ), 64 );

	d3dspt->End();    // end sprite drawing
	d3ddev->EndScene();    // ends the 3D scene
	d3ddev->Present(NULL, NULL, NULL, NULL);

	return;
}

// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	d3ddev->Release();
	d3d->Release();

	// Release instances
	sprite_hero->Release();
	sprite_enemy->Release();
	sprite_bullet->Release();

	return;
}

void CreateTexture( LPCTSTR filename, LPDIRECT3DTEXTURE9* texture )
{
	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
								 filename,    // the file name
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
								 texture);    // load to sprite
}

void RenderTexture( LPDIRECT3DTEXTURE9 texture, float xpos, float ypos, int size )
{
	RECT part;
	SetRect( &part, 0, 0, size, size );
	D3DXVECTOR3 center( 0.0f, 0.0f, 0.0f );    // center at the upper-left corner
	D3DXVECTOR3 position(xpos, ypos, 0.0f );    // position at 50, 50 with no depth
	d3dspt->Draw( texture, &part, &center, &position, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
}