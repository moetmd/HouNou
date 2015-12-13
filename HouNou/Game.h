#pragma once



extern const int SCREENW;
extern const int SCREENH;

extern const int MAX_STONE_NUM;

extern map<int, Sprite*> stones;
extern Monster monster_1;
extern Monster* monster;
extern map<int, Sprite*> players;


//sprite structure
struct SPRITE
{
	float x, y;
	int frame, columns;
	int width, height;
	float scaling, rotation;
	int startframe, endframe;
	int starttime, delay;
	int direction;
	float velx, vely;
	D3DCOLOR color;

	SPRITE()
	{
		frame = 0;
		columns = 1;
		width = height = 0;
		scaling = 1.0f;
		startframe = endframe = 0;
		direction = 1;
		starttime = 0;
		delay = 100;
		velx = vely = 0.0f;
		color = D3DCOLOR_XRGB(255, 255, 255);
	}
};

//sound
extern CDirectMusic g_sound_walking;
extern CDirectMusic g_sound_bgm;


//font
extern ID3DXFont* g_pFont;


//sprite object
extern LPD3DXSPRITE spriteobj;




//settings for the scoller
extern const int TILEWIDTH;  //块宽度
extern const int TILEHEIGHT; //块高度
extern const int MAPWIDTH;   //
extern const int MAPHEIGHT;

//scrolling window size 
extern const int WINDOWWIDTH;
extern const int WINDOWHEIGHT;

//entire game world dimensions
extern const int GAMEWORLDWIDTH;
extern const int GAMEWORLDHEIGHT;


extern float ScrollX, ScrollY;
extern float SpeedX, SpeedY;
extern long start;
extern LPDIRECT3DSURFACE9 gameworld;

//position caculation
extern float old_hero_x, old_hero_y;
extern float old_world_x, old_world_y;
extern bool hero_walking;

extern const int GAMEPANEL_WIDTH;
extern const int GAMEPANEL_HEIGHT;

//整个游戏地图数据
extern int MAPDATA[];

//游戏面板范围数据
extern int WALL[][16];

//游戏初始化
bool Game_Init();

//游戏画面更新
void Game_Update(HWND window);

//游戏画面渲染
void Game_Render(HWND hwnd);

//游戏资源清理
void Game_Clean();

//初始化石头
void Stones_Init();




