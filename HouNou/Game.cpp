
#include "DirectX.h"

const int SCREENW = WINDOW_WIDTH;
const int SCREENH = WINDOW_HEIGHT;


LPD3DXSPRITE spriteobj = NULL;

//sprites
Sprite player_1;



//game state
bool						game_pause = true;         //ÓÎÏ·ÊÇ·ñÔÝÍ£
bool						game_over = true;			//ÓÎÏ·ÊÇ·ñ½áÊø

//sound
CDirectMusic g_sound_walking;
CDirectMusic g_sound_bgm;

//textures
LPDIRECT3DTEXTURE9 imgHero = NULL;

//font
ID3DXFont* g_pFont = NULL;

LPDIRECT3DSURFACE9 gameworld = NULL;

//settings for the scoller
const int TILEWIDTH = 64;
const int TILEHEIGHT = 64;
const int MAPWIDTH = 18;
const int MAPHEIGHT = 15;

const int GAMEPANEL_WIDTH = 16;
const int GAMEPANEL_HEIGHT = 11;

//scrolling window size 
const int WINDOWWIDTH = (SCREENW / TILEWIDTH) * TILEWIDTH;
const int WINDOWHEIGHT = (SCREENH / TILEHEIGHT) * TILEHEIGHT;

//entire game world dimensions
const int GAMEWORLDWIDTH = TILEWIDTH * MAPWIDTH;
const int GAMEWORLDHEIGHT = TILEHEIGHT * MAPHEIGHT;

//gameworld position scrolling
float ScrollX = 0;
float ScrollY = 0;
float SpeedX = 0;
float SpeedY = 0;
long start = 0;



//position caculation
float old_hero_x = 0;
float old_hero_y = 0;
float old_world_x = 0;
float old_world_y = 0;
bool hero_walking = false;


int MAPDATA[MAPWIDTH*MAPHEIGHT] = {
	41, 6, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
	41, 41,41, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 50, 2, 41, 2, 79,
	80, 41,41, 50, 14, 15, 15, 15, 16, 2, 2, 2, 2, 2, 2, 2, 41, 89,
	90, 41,41, 2, 24, 25, 35, 25, 26, 2, 2, 44, 49, 2, 2, 2, 2, 41,
	2, 41,41, 2, 24, 26, 2, 24, 26, 2, 2, 64, 69, 2, 2, 2, 2, 2,
	41, 41,41, 2, 24, 25, 15, 25, 26, 2, 2, 50, 2, 2, 2, 2, 2, 2,
	2, 41,41, 2, 34, 35, 35, 35, 36, 2, 2, 2, 2, 11, 12, 13, 2, 2,
	2, 41,41, 2, 50, 2, 2, 2, 2, 2, 2, 2, 2, 21, 3, 23, 2, 2,
	2, 41,41, 41, 2, 2, 2, 2, 2, 2, 50, 2, 2, 31, 32, 33, 2, 2,
	2, 41,41, 2, 41, 2, 2, 55, 55, 55, 55, 2, 2, 2, 2, 2, 50, 2,
	2, 41,41, 79, 80, 41, 2, 2, 2, 2, 2, 2, 2, 14, 15, 15, 15, 16,
	2, 41,41, 89, 90, 2, 41, 2, 2, 2, 2, 2, 50, 34, 35, 35, 35, 36,
	2, 41,41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
	41, 41,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0
};

int WALL[GAMEPANEL_HEIGHT][GAMEPANEL_WIDTH] = {
	{ 0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0, 0,-1,-1,-1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0, 0, 0,-1,-1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0, 0, 0, 0,-1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0, 0, 0, 0, 0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0, 0, 0, 0, 0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0, 0, 0, 0, 0 },
	{ -1, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ -1,-1, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ -1,-1,-1, 0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ -1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0 }
};





bool Game_Init()
{
	game_over = false;

	//create sprite object
	D3DXCreateSprite(g_pd3dDevice, &spriteobj);

	//load texture
// 	imgHero = LoadTexture(L"GameMedia\\027_00.png");
// 	if (!imgHero)
// 		return false;

	/*Player player_1;*/
	
	player_1.Set_img(L"GameMedia\\027_00.png");

	//set properties for sprites
	player_1.world_X = 2;
	player_1.world_Y = 2;
	player_1.width = player_1.height = 96;
	player_1.columns = 4;
	player_1.startframe = 0;
	player_1.endframe = 0;
	player_1.foot = 20;


	//create font
	if (FAILED(D3DXCreateFont(g_pd3dDevice, 36, 0, 0, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Î¢ÈíÑÅºÚ"), &g_pFont)))
		return false;

	//create Sound
	if (!g_sound_walking.CreateSound("GameMedia\\walking.wav", 0))
		return false;
	if (!g_sound_bgm.CreateSound("GameMedia\\bgm.wav", UGP_INFINITE))
		return false;



	BuildGameWorld();

	old_hero_x = player_1.world_X;
	old_hero_y = player_1.world_Y;
	old_world_x = ScrollX;
	old_world_y = ScrollY;

	g_sound_bgm.Play();


	start = timeGetTime();

	return true;
}


void Game_Update(HWND window) 
{

	//make sure the Direct3D device is valid
	if (!g_pd3dDevice)
		return;

	//escape key exits
	if (g_pDInput->IsKeyDown(DIK_ESCAPE))
	{
		game_over = true;
		g_currentGUI = GUI_START_SCREEN;
		g_sound_bgm.Stop();
	}


		//update input devices
		g_pDInput->GetInput();

	if (timeGetTime() - start >= 80)
	{
		start = timeGetTime();
// 		//font rect
// 				RECT fontRect;
// 				GetClientRect(window, &fontRect);


		//move with keys
		if (g_pDInput->IsKeyDown(DIK_UPARROW))
		{

			player_1.Move_Up();
			if (player_1.world_Y < 0)
			{
				player_1.world_Y = 0;
			}
			if (WALL[player_1.world_Y][player_1.world_X] == -1)
			{
				player_1.Move_Down();
			}

		}
		if (g_pDInput->IsKeyDown(DIK_DOWNARROW))
		{
			player_1.Move_Down();
			if (player_1.world_Y > GAMEPANEL_HEIGHT - 1)
			{
				player_1.world_Y = GAMEPANEL_HEIGHT - 1;
			}
			if (WALL[player_1.world_Y][player_1.world_X] == -1)
			{
				player_1.Move_Up();
			}
		}

		if (g_pDInput->IsKeyDown(DIK_LEFTARROW))
		{
			player_1.Move_left();
			if (player_1.world_X < 0)
			{
				player_1.world_X = 0;
			}
			if (WALL[player_1.world_Y][player_1.world_X] == -1)
			{
				player_1.Move_Right();
			}
		}

		if (g_pDInput->IsKeyDown(DIK_RIGHTARROW))
		{
			player_1.Move_Right();
			if (player_1.world_X > GAMEPANEL_WIDTH - 1)
			{
				player_1.world_X = GAMEPANEL_WIDTH - 1;
			}
			if (WALL[player_1.world_Y][player_1.world_X] == -1)
			{
				player_1.Move_left();
			}
		}

	}
}


void Game_Render()
{	

		ScrollScreen();

		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		player_1.Draw();

		spriteobj->End();

		if (hero_walking)
			g_sound_walking.Stop();

}


void Game_Clean()
{
	//free memory
	if (gameworld)
		gameworld->Release();

	

	g_sound_bgm.Shutdown();
	g_sound_walking.Shutdown();

}



