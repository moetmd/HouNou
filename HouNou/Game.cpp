
#include "DirectX.h"

const int SCREENW = WINDOW_WIDTH;
const int SCREENH = WINDOW_HEIGHT;


LPD3DXSPRITE spriteobj = NULL;

//sprites
SPRITE player_1;

SPRITE item[];

//game state
bool						game_pause = true;         //游戏是否暂停
bool						game_over = true;			//游戏是否结束

//sound
CDirectMusic g_sound_walking;
CDirectMusic g_sound_bgm;

//textures
LPDIRECT3DTEXTURE9 imgHero = NULL;

//font
ID3DXFont* g_pFont = NULL;

//settings for the scoller
const int TILEWIDTH = 64;
const int TILEHEIGHT = 64;
const int MAPWIDTH = 18;
const int MAPHEIGHT = 15;

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
LPDIRECT3DSURFACE9 gameworld = NULL;

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


//physics functions 
//碰撞检测
int Collision(SPRITE sprite1, SPRITE sprite2);
bool CollisionD(SPRITE sprite1, SPRITE sprite2);


bool Game_Init()
{
	game_over = false;

	//create sprite object
	D3DXCreateSprite(g_pd3dDevice, &spriteobj);

	//load texture
	imgHero = LoadTexture(L"GameMedia\\027_00.png");
	if (!imgHero)
		return false;

	//set properties for sprites
	hero.x = 250;
	hero.y = 100;
	hero.width = hero.height = 96;
	hero.columns = 4;
	hero.startframe = 0;
	hero.endframe = 0;


	//create font
	if (FAILED(D3DXCreateFont(g_pd3dDevice, 36, 0, 0, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("微软雅黑"), &g_pFont)))
		return false;

	//create Sound
	if (!g_sound_walking.CreateSound("GameMedia\\walking.wav", 0))
		return false;
	if (!g_sound_bgm.CreateSound("GameMedia\\bgm.wav", UGP_INFINITE))
		return false;



	BuildGameWorld();

	old_hero_x = hero.x;
	old_hero_y = hero.y;
	old_world_x = ScrollX;
	old_world_y = ScrollY;

	g_sound_bgm.Play();


	start = GetTickCount();

	return true;
}


void Game_Update(HWND window) 
{
	//make sure the Direct3D device is valid
	if (!g_pd3dDevice)
		return;

	//如果是结束游戏，则重新初始化游戏
// 	if (game_over)
// 	{
// 		Game_Init();
// 		game_over = false;
// 	}

	//update input devices
	g_pDInput->GetInput();

	//font rect
	RECT fontRect;
	GetClientRect(window, &fontRect);

	//clear the scene
/*	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);*/

	//move with keys
	if (g_pDInput->IsKeyDown(DIK_W))
	{

		if (hero.y < SCREENH / 2)
		{
			ScrollY -= 0.1f;

			if (ScrollY <= 0)
			{
				hero.y -= 0.1f;
				if (hero.y < 0)
					hero.y = 0;
			}
		}
		else
		{
			hero.y -= 0.1f;
			if (hero.y < 0)
				hero.y = 0;
		}


		hero.startframe = 12;
		hero.endframe = hero.startframe + 3;
		Sprite_Animate(hero.frame, hero.startframe, hero.endframe,
			hero.direction, hero.starttime, hero.delay);


	}
	if (g_pDInput->IsKeyDown(DIK_S))
	{
		if (hero.y > SCREENH / 2)
		{
			ScrollY += 0.1f;

			if (ScrollY >= GAMEWORLDHEIGHT - SCREENH)
			{
				hero.y += 0.1f;
				if (hero.y > SCREENH - hero.height)
					hero.y = SCREENH - hero.height;
			}
		}
		else
		{
			hero.y += 0.1f;
			if (hero.y > SCREENH - hero.height)
				hero.y = 0;
		}

		hero.startframe = 0;
		hero.endframe = hero.startframe + 3;
		Sprite_Animate(hero.frame, hero.startframe, hero.endframe,
			hero.direction, hero.starttime, hero.delay);


	}
	if (g_pDInput->IsKeyDown(DIK_A))
	{
		if (hero.x < SCREENW / 2)
		{
			ScrollX -= 0.1f;

			if (ScrollX <= 0)
			{
				hero.x -= 0.1f;
				if (hero.x < 0)
					hero.x = 0;
			}
		}
		else
		{
			hero.x -= 0.1f;
			if (hero.x < 0)
				hero.x = 0;
		}

		hero.startframe = 4;
		hero.endframe = hero.startframe + 3;
		Sprite_Animate(hero.frame, hero.startframe, hero.endframe,
			hero.direction, hero.starttime, hero.delay);


	}
	if (g_pDInput->IsKeyDown(DIK_D))
	{
		if (hero.x > SCREENH / 2)
		{
			ScrollX += 0.1f;

			if (ScrollX >= GAMEWORLDWIDTH - SCREENW)
			{
				hero.x += 0.1f;
				if (hero.x > SCREENW - hero.width)
					hero.x = SCREENW - hero.width;
			}
		}
		else
		{
			hero.x += 0.1f;
			if (hero.x > SCREENW - hero.width)
				hero.x = 0;
		}

		hero.startframe = 8;
		hero.endframe = hero.startframe + 3;
		Sprite_Animate(hero.frame, hero.startframe, hero.endframe,
			hero.direction, hero.starttime, hero.delay);

	}

	//escape key exits
	if (g_pDInput->IsKeyDown(DIK_ESCAPE))
	{
		game_over = true;
		g_currentGUI = GUI_START_SCREEN;
		g_sound_bgm.Stop();
	}
}


void Game_Render()
{
	if (GetTickCount() - start >= 0)
	{
		start = GetTickCount();

		if (abs((long)(old_hero_x - hero.x)) > 30 || abs((long)(old_hero_y - hero.y)) > 30 ||
			abs((long)(old_world_x - ScrollX)) > 30 || abs((long)(old_world_y - ScrollY)) > 30
			)
		{
			g_sound_walking.Play();
			old_hero_x = hero.x;
			old_hero_y = hero.y;
			old_world_x = ScrollX;
			old_world_y = ScrollY;
			hero_walking = true;
		}

		ScrollScreen();

		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		Sprite_Transform_Draw(imgHero, hero.x, hero.y, hero.width,
			hero.height, hero.frame, hero.columns, 0, 1.0f, hero.color);

		spriteobj->End();

		if (hero_walking)
			g_sound_walking.Stop();
	}
}


void Game_Clean()
{
	//free memory
	if (gameworld)
		gameworld->Release();

	if (imgHero)
		imgHero->Release();

	g_sound_bgm.Shutdown();
	g_sound_walking.Shutdown();

}


void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source)
{
	//get width height from source surface
	D3DSURFACE_DESC desc;
	source->GetDesc(&desc);

	//create rects for drawing
	RECT source_rect = { 0, 0, (long)desc.Width, (long)desc.Height };
	RECT dest_rect = { (long)x, (long)y, (long)x + desc.Width, (long)y + desc.Height };

	//draw the source surface onto the dest
	g_pd3dDevice->StretchRect(source, &source_rect, dest, &dest_rect, D3DTEXF_NONE);
}

LPDIRECT3DSURFACE9 LoadSurface(LPCTSTR filename)
{
	LPDIRECT3DSURFACE9 image = NULL;

	//get width and height from bitmap file
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filename, &info);
	if (result != D3D_OK)
		return NULL;

	//create surface
	result = g_pd3dDevice->CreateOffscreenPlainSurface(
		info.Width, info.Height,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&image,
		NULL);
	if (result != D3D_OK)
		return NULL;

	//load surface from file into newly created surface
	result = D3DXLoadSurfaceFromFile(
		image,    //destination surface
		NULL,    //destination palette
		NULL,    //destination rectangle
		filename,
		NULL,    //source rectangle
		D3DX_DEFAULT, //controls how image is filtered
		D3DCOLOR_XRGB(0, 0, 0),
		NULL);    //source image info
	if (result != D3D_OK)
		return NULL;

	return image;
}

//DrawTile
void DrawTile(LPDIRECT3DSURFACE9 source, int tilenum, int width, int height,
	int columns, LPDIRECT3DSURFACE9 dest, int destx, int desty)
{
	RECT r1;
	r1.left = (tilenum % columns) * width;
	r1.top = (tilenum / columns) * height;//only one number can not figure two info , so we need % and / to calculate two result from a single number.
	r1.right = r1.left + width;
	r1.bottom = r1.top + height;

	RECT r2 = { destx, desty, destx + width, desty + height };

	g_pd3dDevice->StretchRect(source, &r1, dest, &r2, D3DTEXF_NONE);
}

//Build world
void BuildGameWorld()
{
	HRESULT result;
	int x, y;
	LPDIRECT3DSURFACE9 tiles;

	//load the bitmap image containing all the tiles
	tiles = LoadSurface(L"GameMedia\\groundtiles.bmp");

	//create the scrolling game world bitmap
	result = g_pd3dDevice->CreateOffscreenPlainSurface(
		GAMEWORLDWIDTH,
		GAMEWORLDHEIGHT,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&gameworld,
		NULL
		);

	if (result != D3D_OK)
	{
		MessageBox(NULL, L"Error creating working surface!", L"Error", 0);
		return;
	}

	for (y = 0; y < MAPHEIGHT; y++)
		for (x = 0; x < MAPWIDTH; x++)
			DrawTile(tiles, MAPDATA[y * MAPWIDTH + x], 64, 64, 16, gameworld,
				x * 64, y * 64);

	tiles->Release();
}

//Scroll Screen 
void  ScrollScreen()
{
	ScrollX += SpeedX;
	if (ScrollX < 0)
	{
		ScrollX = 0;
		SpeedX = 0;
	}
	else if (ScrollX > GAMEWORLDWIDTH - SCREENW)
	{
		ScrollX = GAMEWORLDWIDTH - SCREENW;
		SpeedX = 0;
	}

	ScrollY += SpeedY;
	if (ScrollY < 0)
	{
		ScrollY = 0;
		SpeedY = 0;
	}
	else if (ScrollY > GAMEWORLDHEIGHT - SCREENH)
	{
		ScrollY = GAMEWORLDHEIGHT - SCREENH;
		SpeedY = 0;
	}

	RECT r1 = { ScrollX, ScrollY, ScrollX + SCREENW - 1, ScrollY + SCREENH - 1 };
	RECT r2 = { 0, 0, SCREENW - 1, SCREENH - 1 };

	g_pd3dDevice->StretchRect(gameworld, &r1, backbuffer, &r2, D3DTEXF_NONE);
}

//load texture
LPDIRECT3DTEXTURE9 LoadTexture(LPCTSTR filename, D3DCOLOR transcolor)
{
	LPDIRECT3DTEXTURE9 texture = NULL;

	//get width and height from bitmap file
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filename, &info);
	if (result != D3D_OK)
		return NULL;

	//create the new texture by loading a bitmap image file
	result = D3DXCreateTextureFromFileEx(
		g_pd3dDevice, //Direct3D device object
		filename, //filename
		info.Width, //image width
		info.Height, //image height
		1, //mip-map levels (1 for no chain)
		D3DPOOL_DEFAULT, //the type of surface
		D3DFMT_UNKNOWN, //surface format
		D3DPOOL_DEFAULT, //memory class for the texture
		D3DX_DEFAULT, //image filter
		D3DX_DEFAULT, //mip filter
		transcolor, //color key for transparency
		&info, //bitmap file info
		NULL, //color palette
		&texture); //destination texture
	if (result != D3D_OK)
		return NULL;

	return texture;
}

D3DXVECTOR2 GetBitmapSize(LPCTSTR filename)
{
	D3DXIMAGE_INFO info;
	D3DXVECTOR2 size = D3DXVECTOR2(0.0f, 0.0f);
	HRESULT result = D3DXGetImageInfoFromFile(filename, &info);
	if (result == D3D_OK)
		size = D3DXVECTOR2((float)info.Width, (float)info.Height);

	return size;
}

//draw sprite frame
void Sprite_Draw_Frame(LPDIRECT3DTEXTURE9 texture, int destx, int desty,
	int framenum, int framew, int frameh, int columns)
{
	D3DXVECTOR3 position((float)destx, (float)desty, 0);
	D3DCOLOR white = D3DCOLOR_XRGB(255, 255, 255);

	RECT rect;
	rect.left = (framenum % columns) * framew;
	rect.top = (framenum / columns) * frameh;
	rect.right = rect.left + framew;
	rect.bottom = rect.top + frameh;

	spriteobj->Draw(texture, &rect, NULL, &position, white);
}

void Sprite_Animate(int &frame, int startframe, int endframe,
	int direction, int &starttime, int delay)
{
	if ((int)GetTickCount() > starttime + delay)
	{
		starttime = GetTickCount();

		frame += direction;
		if (frame > endframe)
			frame = startframe;
		if (frame < startframe)
			frame = endframe;
	}
}

void Sprite_Transform_Draw(LPDIRECT3DTEXTURE9 image, int x, int y, int width, int height, int frame, int columns, float rotation, float scaling, D3DCOLOR color)
{
	D3DXVECTOR2 scale(scaling, scaling);

	D3DXVECTOR2 trans(x, y);

	D3DXVECTOR2 center((float)(width*scaling) / 2, (float)(height*scaling) / 2);

	D3DXMATRIX mat;
	D3DXMatrixTransformation2D(&mat, NULL, 0, &scale, &center, rotation, &trans);

	spriteobj->SetTransform(&mat);

	int fx = (frame % columns) * width;
	int fy = (frame / columns) * height;

	RECT srcRect = { fx, fy, fx + width, fy + height };

	spriteobj->Draw(image, &srcRect, NULL, NULL, color);
}

//bounding box collision detection
int Collision(SPRITE sprite1, SPRITE sprite2)
{
	RECT rect1;
	rect1.left = (long)sprite1.x;
	rect1.top = (long)sprite1.y;
	rect1.right = (long)sprite1.x + sprite1.width * sprite1.scaling;
	rect1.bottom = (long)sprite1.y + sprite1.height * sprite1.scaling;

	RECT rect2;
	rect1.left = (long)sprite2.x;
	rect1.top = (long)sprite2.y;
	rect1.right = (long)sprite2.x + sprite2.width * sprite2.scaling;
	rect1.bottom = (long)sprite2.y + sprite2.height * sprite2.scaling;

	RECT dest;
	return IntersectRect(&dest, &rect1, &rect2);
}

bool CollisionD(SPRITE sprite1, SPRITE sprite2)
{
	double radius1, radius2;

	if (sprite1.width > sprite1.height)
		radius1 = (sprite1.width * sprite1.scaling) / 2.0;
	else
		radius1 = (sprite1.height * sprite1.scaling) / 2.0;

	double x1 = sprite1.x + radius1;
	double y1 = sprite1.y + radius1;
	D3DXVECTOR2 vector1(x1, y1);

	if (sprite2.width > sprite2.height)
		radius2 = (sprite2.width * sprite2.scaling) / 2.0;
	else
		radius2 = (sprite2.height * sprite2.scaling) / 2.0;

	double x2 = sprite2.x + radius2;
	double y2 = sprite2.y + radius2;
	D3DXVECTOR2 vector2(x2, y2);

	double deltax = vector1.x - vector2.x;
	double deltay = vector1.y - vector2.y;
	double dist = sqrt((deltax*deltax) + (deltay*deltay));

	return (dist < radius1 + radius2);
}
