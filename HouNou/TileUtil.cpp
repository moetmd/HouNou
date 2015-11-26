#include "DirectX.h"




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