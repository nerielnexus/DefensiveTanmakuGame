#pragma once

// for M_PI constant
#define _USE_MATH_DEFINES
#include <cmath>

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#include <functional>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define SCREEN_WIDTH    1280.0f
#define SCREEN_HEIGHT   960.0f
#define ENEMY_NUM       30
#define BULLET_LIFE     1000

enum { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };

POINT onScreenPos;

// global declarations
LPDIRECT3D9 d3d;                     // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;            // the pointer to the device class
LPD3DXSPRITE d3dspt;                 // the pointer to our Direct3D Sprite interface

static int counter = 0;

// sprite declarations
LPDIRECT3DTEXTURE9 sprite_hero;      // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_enemy;     // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_bullet;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_spawner;
LPDIRECT3DTEXTURE9 sprite_bomb;

// function prototypes
void initD3D( HWND hWnd );           // sets up and initializes Direct3D
void render_frame( void );           // renders a single frame
void cleanD3D( void );		         // closes Direct3D and releases memory
void init_game( void );
void do_game_logic( HWND hWnd );