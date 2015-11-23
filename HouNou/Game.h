#pragma once


extern const int SCREENW;
extern const int SCREENH;






//textures
extern LPDIRECT3DTEXTURE9 imgHero;

//sound
extern CDirectMusic g_sound_walking;
extern CDirectMusic g_sound_bgm;


//font
extern ID3DXFont* g_pFont;




//settings for the scoller
extern const int TILEWIDTH;
extern const int TILEHEIGHT;
extern const int MAPWIDTH;
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


extern int MAPDATA[];

//游戏初始化
bool Game_Init();

//游戏画面更新
void Game_Update(HWND window);

//游戏画面渲染
void Game_Render();

//游戏资源清理
void Game_Clean();

//构建地图
void BuildGameWorld();

//绘制块
void DrawTile(LPDIRECT3DSURFACE9 source, int tilenum, int width, int height,
	int columns, LPDIRECT3DSURFACE9 dest, int destx, int desty);

//加载表面
LPDIRECT3DSURFACE9 LoadSurface(LPCTSTR filename);

//绘制表面
void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source);

//地图滚动
void  ScrollScreen();

//取得位图大小
D3DXVECTOR2 GetBitmapSize(LPCTSTR filename);

//加载材质
LPDIRECT3DTEXTURE9 LoadTexture(LPCTSTR filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(0, 0, 0));

//draw functions
//绘制Sprite帧
void Sprite_Draw_Frame(LPDIRECT3DTEXTURE9 texture, int destx, int desty,
	int framenum, int framew, int frameh, int columns);

//Sprite帧动画操作
void Sprite_Animate(int &frame, int startframe, int endframe,
	int direction, int &starttime, int delay);

//Sprite变形动画操作
void Sprite_Transform_Draw(LPDIRECT3DTEXTURE9 image, int x, int y, int width,
	int height, int frame, int columns, float rotation, float scaling, D3DCOLOR color);


