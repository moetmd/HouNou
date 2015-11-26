#pragma once

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

//physics functions 
//碰撞检测
int Collision(SPRITE sprite1, SPRITE sprite2);
bool CollisionD(SPRITE sprite1, SPRITE sprite2);